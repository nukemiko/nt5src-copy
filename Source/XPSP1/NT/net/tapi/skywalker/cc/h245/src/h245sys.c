/******************************************************************************
 *
 *   INTEL Corporation Proprietary Information
 *   Copyright (c) 1994, 1995, 1996 Intel Corporation.
 *
 *   This listing is supplied under the terms of a license agreement
 *   with INTEL Corporation and may not be used, copied, nor disclosed
 *   except in accordance with the terms of that agreement.
 *
 *****************************************************************************/

/******************************************************************************
 *
 *  $Workfile:   h245sys.c  $
 *  $Revision:   1.8  $
 *  $Modtime:   Mar 04 1997 17:38:08  $
 *  $Log:   S:/STURGEON/SRC/H245/SRC/VCS/h245sys.c_v  $
 * 
 *    Rev 1.8   Mar 04 1997 17:51:56   tomitowx
 * process detach fix
 * 
 *    Rev 1.7   24 Jan 1997 19:40:48   SBELL1
 * upgraded to oss 4.2
 * 
 *    Rev 1.6   21 Jun 1996 18:53:22   unknown
 * Changed InstUnlock() to fix shutdown re-entrancy bug.
 * 
 *    Rev 1.5   10 Jun 1996 16:59:34   EHOWARDX
 * Moved init/shutdown of submodules to CreateInstance/InstanceUnlock.
 * 
 *    Rev 1.4   07 Jun 1996 17:38:22   EHOWARDX
 * 
 * Changed H245_malloc, H245_realloc, and H245_free to macros.
 * Added _H245_malloc, _H245_realloc, and _H245_free for use by OSS ASN.1.
 * 
 *    Rev 1.3   04 Jun 1996 13:57:02   EHOWARDX
 * Fixed Release build warnings.
 * 
 *    Rev 1.2   28 May 1996 14:25:38   EHOWARDX
 * Tel Aviv update.
 * 
 *    Rev 1.1   16 May 1996 13:51:26   EHOWARDX
 * Fixed minor timer/lock count interaction bugs.
 * 
 *    Rev 1.0   09 May 1996 21:06:28   EHOWARDX
 * Initial revision.
 * 
 *    Rev 1.25   09 May 1996 19:39:04   EHOWARDX
 * Changed includes.
 * 
 *    Rev 1.24   29 Apr 1996 12:56:58   EHOWARDX
 * Made timers more accurate & made them use fewer resources.
 * Note: This is windows-specific.
 * 
 *    Rev 1.18.1.2   15 Apr 1996 15:12:34   EHOWARDX
 * Kludge to not call H245DeInitTimer() when last timer is stopped.
 * This will have to be revisited later.
 * 
 *    Rev 1.18.1.1   02 Apr 1996 22:06:22   EHOWARDX
 * No change.
 * 
 *    Rev 1.18.1.0   27 Mar 1996 16:44:18   EHOWARDX
 * Changed timer code; decided it would work better if H245InitTimer()
 * actually got called.
 * 
 *    Rev 1.18   26 Mar 1996 09:46:08   cjutzi
 * 
 * - ok.. Added Enter&Leave&Init&Delete Critical Sections for Ring 0
 * 
 *    Rev 1.17   25 Mar 1996 18:30:14   helgebax
 * - removed H245ASSERT .
 * .
 * 
 * 
 *    Rev 1.16   25 Mar 1996 18:10:48   cjutzi
 * 
 * - well .. I broke the build.. had to put back what I did.. 
 * 
 * 
 *    Rev 1.15   25 Mar 1996 17:50:02   cjutzi
 * 
 * - removed critical section.. back step
 * 
 *    Rev 1.14   25 Mar 1996 17:20:34   cjutzi
 * 
 * - added Remesh's EnterCritical section definitions.. to use
 *   oil layer. 
 * 
 *    Rev 1.13   18 Mar 1996 12:44:40   cjutzi
 * 
 * - put NULL as callback at shutdown
 * 
 *    Rev 1.12   18 Mar 1996 12:41:32   cjutzi
 * - added timer code for multiple timers queue.. 
 * 
 *    Rev 1.11   12 Mar 1996 15:48:46   cjutzi
 * 
 * - added InstanceTbl Lock
 * 
 *    Rev 1.10   07 Mar 1996 22:47:34   dabrown1
 * 
 * Modifications required for ring0/ring3 compatibilty
 * 
 *    Rev 1.9   02 Mar 1996 22:14:52   DABROWN1
 * 
 * removed h245_bzero and h245_bcopy (use memset and memcpy instead)
 * put guardbands in h245_malloc with check in h245_free for _DEBUG
 * changed # parameters passed to h245_free to 1 only (ptr)
 * redefined h245_realloc
 * 
 *    Rev 1.8   26 Feb 1996 17:10:56   cjutzi
 * 
 * - removed h245sys.h
 * 
 *    Rev 1.7   26 Feb 1996 12:42:26   cjutzi
 * 
 * - added bcopy 
 * 
 *    Rev 1.6   21 Feb 1996 16:23:38   DABROWN1
 * 
 * removed h245_ASN1free
 * modified malloc and free to save buffer size internally
 * 
 *    Rev 1.5   13 Feb 1996 14:54:12   DABROWN1
 * 
 * removed trace/debug files to new debug directory
 * 
 *    Rev 1.4   09 Feb 1996 15:45:08   cjutzi
 * - added h245trace
 * - added h245Assert
 *
 *****************************************************************************/
#define STRICT

#include <nt.h>
#include <ntrtl.h>
#include <nturtl.h>

/***********************/
/* HTF SYSTEM INCLUDES */
/***********************/

#if defined(_IA_SPOX_)
# include <oil.x>
#else
# pragma warning( disable : 4115 4201 4214 4514 )
# include <windows.h>
#endif

#include "h245api.h"
#include "h245com.h"
#include "h245sys.x"

#if defined(_IA_SPOX_)
#include <h223api.h>

/************************************/
/* MALLOC ABSTRACTION
/************************************/
typedef struct MEMALLOC {
  DWORD     dwSize;
  void     *pBuffer;
} MEMALLOC, *PMEMALLOC;

#endif

/*****************************************************************************
 *
 * TYPE:        Global System
 *
 * PROCEDURE:   H245_malloc
 *
 * DESCRIPTION:
 *
 * RETURN:
 *
 *****************************************************************************/
void * 
DLL_ENTRY_FDEF
_H245_malloc ( size_t dwSize )
{
#if defined(_IA_SPOX_)

  PMEMALLOC p_mem;
  DWORD     dwBufferSize;
#ifdef DBG
  DWORD    *testptr;
#endif

  // embed the size of the memory in the block so we can pull it
  // out when we free it.
#ifdef DBG
  dwBufferSize = dwSize + sizeof(DWORD) + sizeof(DWORD);
#else
  dwBufferSize = dwSize + sizeof(DWORD);
#endif // DBG

  OIL_Alloc (dwBufferSize, &p_mem);

  if (p_mem) {

   /* Save the size so we know how big it is when we need to free it */
   p_mem->dwSize = dwBufferSize;

#ifdef DBG
   // Have guardbands for debugging purposes
   testptr = (DWORD *)((char *)p_mem + dwBufferSize - sizeof(DWORD));
   *testptr = 0x12345678;
#endif //DBG

   return &(p_mem->pBuffer);
  }
  else {
   return NULL;
  }

#else

# if defined(_MALLOC_DBG)
  return _malloc_dbg(dwSize, _NORMAL_BLOCK, __FILE__, __LINE__);
# else
  return malloc(dwSize);
# endif

#endif  // (_IA_SPOX_)

}


/*****************************************************************************
 *
 * TYPE:        Global System
 *
 * PROCEDURE:   H245_free
 *
 * DESCRIPTION:
 *
 * RETURN:
 *
 *****************************************************************************/

void
DLL_ENTRY_FDEF
_H245_free( void *pBuf )
{
#if defined(_IA_SPOX_)

  PMEMALLOC p_mem;
#ifdef DBG
  DWORD    *testptr;
#endif // DBG

  /* Adjust pointer to account for size field */
  p_mem = (PMEMALLOC)((char *)pBuf - sizeof(DWORD));

#ifdef DBG
  testptr = (DWORD *)((char *)p_mem + p_mem->dwSize - sizeof(DWORD));
  H245ASSERT(*testptr == 0x12345678);
#endif // DBG

  OIL_Free (p_mem, p_mem->dwSize);

#else

# if defined(_MALLOC_DBG)
  _free_dbg(pBuf, _NORMAL_BLOCK);
# else
  free (pBuf);
# endif

#endif  // (_IA_SPOX_)
}


/*****************************************************************************
 *
 * TYPE:        Global System
 *
 * PROCEDURE:   H245_realloc
 *
 * DESCRIPTION:
 *
 * RETURN:
 *
 *****************************************************************************/

void *
DLL_ENTRY_FDEF
_H245_realloc( void *pBuf, size_t dwSize )
{
#if defined(_IA_SPOX_)

  void     *pNewBuf;
  PMEMALLOC p_MemOld;

  // If user supplied ubffer is null, equivalent to malloc
  if (pBuf == NULL) {
    return H245_malloc(dwSize);
  }

  // If size is 0, same as free with a NULL return (we know
  // there is a buffer due to test (pBuf == NULL), above
  if (dwSize == 0) {
    H245_free( pBuf );
    return NULL;
  }

  // Obtain the buffer size requested
  pNewBuf = H245_malloc(dwSize);

  // If new malloc failed, return NULL
  if (pNewBuf) {

    // A buffer was submitted.  Check if newly requested
    // size is smaller or larger than the original
    p_MemOld = (PMEMALLOC)((char *)pBuf - sizeof(DWORD));

    // Copy over as much data from the old buffer as we can
    memcpy(pNewBuf,
           pBuf,
           H245_min(p_MemOld->dwSize, dwSize));

    // After data is copied, return the old buffer
    H245_free( pBuf );
  }

  return pNewBuf;

#else

# if defined(_MALLOC_DBG)
  return _realloc_dbg(pBuf, dwSize, _NORMAL_BLOCK, __FILE__, __LINE__);
# else
  return realloc (pBuf, dwSize);
# endif

#endif  // (_IA_SPOX_)
}



/**************************************************************************
 *
 * Instance Table/Instance Lock implementation
 *
 **************************************************************************/

CRITICAL_SECTION        InstanceCreateLock     = {0};
CRITICAL_SECTION        InstanceLocks[MAXINST] = {0};
struct InstanceStruct * InstanceTable[MAXINST] = {0};

struct InstanceStruct *InstanceCreate(DWORD dwPhysId, H245_CONFIG_T Configuration)
{
  register struct InstanceStruct *pInstance;
  register unsigned int           uIndex;
  unsigned int                    uFirst = (dwPhysId - 1) % MAXINST;
  HRESULT                         lError;

  // Allocate new instance
  pInstance = (struct InstanceStruct *)H245_malloc(sizeof(*pInstance));
  if (pInstance == NULL)
  {
    H245TRACE(dwPhysId,1,"InstanceCreate -> Instance malloc failed");
    return NULL;
  }

  // Make sure no one is trying to simultaneously add same physical Id
  // (I know this is a stretch...)
  EnterCriticalSection(&InstanceCreateLock);

  // Check if instance corresponding to dwPhysId already exists
  uIndex = uFirst;                      // Hash start index into table
  do
  {
    // Avoid entering critical section for unused instances
    if (InstanceTable[uIndex])
    {
      EnterCriticalSection(&InstanceLocks[uIndex]);
      if (InstanceTable[uIndex] && InstanceTable[uIndex]->dwPhysId == dwPhysId)
      {
        LeaveCriticalSection(&InstanceLocks[uIndex]);
        LeaveCriticalSection(&InstanceCreateLock);
        H245_free(pInstance);
        H245TRACE(dwPhysId,1,"InstanceCreate -> Physical Id already in use");
        return NULL;
      }
      LeaveCriticalSection(&InstanceLocks[uIndex]);
    }
    uIndex = (uIndex + 1) % MAXINST;
  } while (uIndex != uFirst);

  // Find empty slot for new instance
  uIndex = uFirst;                      // Hash start index into table
  do
  {
    // Avoid entering critical section for used instances
    if (InstanceTable[uIndex] == NULL)
    {
      EnterCriticalSection(&InstanceLocks[uIndex]);
      if (InstanceTable[uIndex] == NULL)
      {
        // Initialize new instance
        // Once the new instance is added to the instance table,
        // we can relinquish the CreateInstanceLock
        InstanceTable[uIndex] = pInstance;
        LeaveCriticalSection(&InstanceCreateLock);
        memset(pInstance, 0, sizeof(*pInstance));
        pInstance->dwPhysId  = dwPhysId;
        pInstance->dwInst    = uIndex + 1;
        pInstance->LockCount = 1;

        /* API Subsystem Initialization */
        pInstance->Configuration = Configuration;
        lError = api_init(pInstance);
        if (lError != H245_ERROR_OK)
        {
          H245_free(pInstance);
          H245TRACE(dwPhysId,1,"InstanceCreate -> api_init failed");
          return NULL;
        }

        /* Send Receive Subsystem Initialization */
        lError = sendRcvInit(pInstance);
        if (lError != H245_ERROR_OK)
        {
          api_deinit(pInstance);
          H245_free(pInstance);
          H245TRACE(dwPhysId,1,"InstanceCreate -> sendRcvInit failed");
          return NULL;
        }

        /* State Machine Subsystem Initialization */
        lError = Fsm_init(pInstance);
        if (lError != H245_ERROR_OK)
        {
          sendRcvShutdown(pInstance);
          api_deinit(pInstance);
          H245_free(pInstance);
          H245TRACE(dwPhysId,1,"InstanceCreate -> Fsm_init failed");
          return NULL;
        }

        H245TRACE(pInstance->dwInst,9,"InstanceCreate: ++LockCount=%d", pInstance->LockCount);
        return pInstance;                 // Return locked instance
      }
      LeaveCriticalSection(&InstanceLocks[uIndex]);
    }
    uIndex = (uIndex + 1) % MAXINST;
  } while (uIndex != uFirst);

  LeaveCriticalSection(&InstanceCreateLock);
  H245_free(pInstance);
  H245TRACE(dwPhysId,1,"InstanceCreate -> Too many instances");
  return NULL;
} // InstanceCreate()

struct InstanceStruct *InstanceFind(DWORD dwPhysId)
{
  register unsigned int           uIndex;
  unsigned int                    uFirst = (dwPhysId - 1) % MAXINST;

  // Check if instance corresponding to dwPhysId exists
  uIndex = uFirst;                      // Hash start index into table
  do
  {
    // Avoid entering critical section for unused instances
    if (InstanceTable[uIndex])
    {
      EnterCriticalSection(&InstanceLocks[uIndex]);
      if (InstanceTable[uIndex] && InstanceTable[uIndex]->dwPhysId == dwPhysId)
      {
        InstanceTable[uIndex]->LockCount++;
        H245TRACE(InstanceTable[uIndex]->dwInst,9,"InstanceFind: ++LockCount=%d", InstanceTable[uIndex]->LockCount);
        return InstanceTable[uIndex];     // Return locked instance
      }
      LeaveCriticalSection(&InstanceLocks[uIndex]);
    }
    uIndex = (uIndex + 1) % MAXINST;
  } while (uIndex != uFirst);

  H245TRACE(dwPhysId,1,"InstanceFind -> Instance not found");
  return NULL;                          // Instance not found
} // InstanceFind()

struct InstanceStruct *InstanceLock(register H245_INST_T dwInst)
{
  if (--dwInst >= MAXINST)
  {
    H245TRACE(dwInst+1,1,"InstanceLock -> Invalid instance");
    return NULL;
  }

  // Lock instance, then see if it exists
  EnterCriticalSection(&InstanceLocks[dwInst]);
  if (InstanceTable[dwInst])
  {
    InstanceTable[dwInst]->LockCount++;
    H245TRACE(dwInst+1,9,"InstanceLock: ++LockCount=%d", InstanceTable[dwInst]->LockCount);
    return InstanceTable[dwInst];      // Return locked instance
  }
  LeaveCriticalSection(&InstanceLocks[dwInst]);
  H245TRACE(dwInst+1,1,"InstanceLock -> Invalid instance");
  return NULL;
} // InstanceLock()

int InstanceUnlock(struct InstanceStruct *pInstance)
{
  register H245_INST_T dwInst = pInstance->dwInst - 1;
  if (dwInst >= MAXINST || InstanceTable[dwInst] != pInstance)
  {
    H245TRACE(pInstance->dwInst,1,"InstanceUnlock -> Invalid instance");
    return -1;
  }
  if (pInstance->fDelete && pInstance->LockCount == 1)
  {
    H245TRACE(pInstance->dwInst,9,"InstanceUnlock: deleting instance");
    pInstance->fDelete = FALSE; // InstanceUnlock will be re-entered from H245WS callback!
    Fsm_shutdown(pInstance);
    sendRcvShutdown(pInstance);
    api_deinit(pInstance);
    InstanceTable[dwInst] = NULL;
    LeaveCriticalSection(&InstanceLocks[dwInst]);
    while (pInstance->pTimerList)
    {
      register TimerList_T *pTimer = pInstance->pTimerList;
      pInstance->pTimerList = pTimer->pNext;
      H245TRACE(pInstance->dwInst,1,"InstanceUnlock: deleting timer");
      H245_free(pTimer);
    }
    H245_free(pInstance);
  }
  else
  {
    pInstance->LockCount--;
    H245TRACE(pInstance->dwInst,9,"InstanceUnlock: --LockCount=%d", pInstance->LockCount);
    LeaveCriticalSection(&InstanceLocks[dwInst]);
  }
  return 0;
} // InstanceUnlock()

int InstanceDelete(struct InstanceStruct *pInstance)
{
  H245TRACE(pInstance->dwInst,9,"InstanceDelete");
  pInstance->fDelete = TRUE;
  return InstanceUnlock(pInstance);
} // InstanceDelete()



int InstanceUnlock_ProcessDetach(struct InstanceStruct *pInstance, BOOL fProcessDetach)
{
  register H245_INST_T dwInst = pInstance->dwInst - 1;
  if (dwInst >= MAXINST || InstanceTable[dwInst] != pInstance)
  {
    H245TRACE(pInstance->dwInst,1,"InstanceUnlock -> Invalid instance");
    return -1;
  }
  if (pInstance->fDelete && pInstance->LockCount == 1)
  {
    H245TRACE(pInstance->dwInst,9,"InstanceUnlock: deleting instance");
    pInstance->fDelete = FALSE; // InstanceUnlock will be re-entered from H245WS callback!

    Fsm_shutdown(pInstance);
    //sendRcvShutdown(pInstance);
    sendRcvShutdown_ProcessDetach(pInstance,fProcessDetach);

    api_deinit(pInstance);
    InstanceTable[dwInst] = NULL;
    LeaveCriticalSection(&InstanceLocks[dwInst]);
    while (pInstance->pTimerList)
    {
      register TimerList_T *pTimer = pInstance->pTimerList;
      pInstance->pTimerList = pTimer->pNext;
      H245TRACE(pInstance->dwInst,1,"InstanceUnlock: deleting timer");
      H245_free(pTimer);
    }
    H245_free(pInstance);
  }
  else
  {
    pInstance->LockCount--;
    H245TRACE(pInstance->dwInst,9,"InstanceUnlock: --LockCount=%d", pInstance->LockCount);
    LeaveCriticalSection(&InstanceLocks[dwInst]);
  }
  return 0;
} // InstanceUnlock_ProcessDetach()




/*****************************************************************************
 *
 * TYPE:  TIMER STUFF
 *
 *****************************************************************************/

CRITICAL_SECTION        TimerLock = {0};
static int              TimerInited = 0;
#ifndef _IA_SPOX_
static UINT_PTR         H245TimerId;
#endif

/*****************************************************************************
 *
 * TYPE:  Global System
 *
 * PROCEDURE:   H245TimerTick - ticks every 1000ms
 *
 * DESCRIPTION:
 *
 * RETURN:
 *
 *****************************************************************************/
void H245TimerTick (void)
{
  DWORD                           dwTickCount = GetTickCount();
  unsigned int                    uIndex;
  register struct InstanceStruct *pInstance;
  register TimerList_T           *pTimer;

  H245ASSERT(TimerInited != 0);
  H245TRACE(0,9,"H245TimerTick <-");

  for (uIndex = 0; uIndex < MAXINST; ++uIndex)
  {
    // Avoid entering critical section for unused instances
    if (InstanceTable[uIndex])
    {
      pInstance = InstanceLock(uIndex + 1);
      if (pInstance)
      {
        while (pInstance->pTimerList && (pInstance->pTimerList->dwAlarm - dwTickCount) >= 0x80000000)
        {
          pTimer = pInstance->pTimerList;
          pInstance->pTimerList = pTimer->pNext;
          EnterCriticalSection(&TimerLock);
          if (--TimerInited == 0)
          {
#ifdef _IA_SPOX_
            H223_RegisterTimerCallBack((H223TIMERCB)NULL);
#else
            KillTimer (NULL, H245TimerId);
#endif
          }
          LeaveCriticalSection(&TimerLock);
          if (pTimer->pfnCallBack)
          {
            // TBD - what if pContext is no longer valid?
            (pTimer->pfnCallBack)(pInstance, PtrToUlong(pTimer), pTimer->pContext);
          }
          H245_free (pTimer);
        } // while
        InstanceUnlock(pInstance);
      } // if
    } // if
  } // for
  H245TRACE(0,9,"H245TimerTick ->");
} // TimerTick()

/*****************************************************************************
 *
 * TYPE:  Global System
 *
 * PROCEDURE:   H245TimerProc - FOR WINDOWS Ring 3 only
 *
 * DESCRIPTION:
 *
 * RETURN:
 *
 *****************************************************************************/
#ifndef _IA_SPOX_
#pragma warning ( disable : 4100 )
void CALLBACK  H245TimerProc(HWND  hwHwnd,
                             UINT  uiMessg,
                             UINT_PTR  idTimer,
                             DWORD dwTime)
{
  if (idTimer == H245TimerId)
    H245TimerTick ();
}
#pragma warning ( default : 4100 )
#endif

/*****************************************************************************
 *
 * TYPE:        Global System
 *
 * PROCEDURE:   H245StartTimer
 *
 * DESCRIPTION:
 *
 * RETURN:
 *
 *****************************************************************************/
DWORD H245StartTimer (struct InstanceStruct *   pInstance,
                      void                  *   pContext,
                      H245TIMERCALLBACK         pfnCallBack,
                      DWORD                     dwTicks)
{
  TimerList_T   *pNew;
  TimerList_T   *pLook;
  TimerList_T   *pList;

  if (!pfnCallBack)
  {
    H245TRACE(pInstance->dwInst,1,"H245StartTimer: pfnCallBack == NULL");
    return 0;
  }

  pNew = (TimerList_T *)H245_malloc(sizeof(TimerList_T));
  if (pNew == NULL)
  {
    H245TRACE(pInstance->dwInst,1,"H245StartTimer: memory allocation failed");
    return 0;
  }

  pNew->pNext       = NULL;
  pNew->pContext    = pContext;
  pNew->pfnCallBack = pfnCallBack;
  pNew->dwAlarm     = GetTickCount() + dwTicks;

  EnterCriticalSection(&TimerLock);
  if (++TimerInited == 1)
  {
#ifdef _IA_SPOX_
    H223_RegisterTimerCallBack((H223TIMERCB)H245TimerTick);
#else
    H245TimerId = SetTimer ((HWND)NULL, (UINT_PTR)0, (UINT)1000, (TIMERPROC)H245TimerProc);
#endif
  }
  LeaveCriticalSection(&TimerLock);

  /* as you traverse the list.. subtract the delta off the new one */
  /* and link it in.. this list is a list of delta's off the time  */
  /* out that is linked in front of it.. so subtract as you go     */
  for (pList = NULL, pLook = pInstance->pTimerList;
       pLook && (pLook->dwAlarm - pNew->dwAlarm) >= 0x80000000;
       pList = pLook, pLook = pLook->pNext);

  /* link it in the list */
  pNew->pNext = pLook;
  if (pList)
  {
    // Insert new timer after pList and before pLook
    pList->pNext = pNew;
  }
  else
  {
    // Insert new timer at front of list
    pInstance->pTimerList = pNew;
  }
  return PtrToUlong(pNew);
}


/*****************************************************************************
 *
 * TYPE:        Global System
 *
 * PROCEDURE:   H245StopTimer
 *
 * DESCRIPTION:
 *
 * RETURN:
 *
 *****************************************************************************/
DWORD H245StopTimer(struct InstanceStruct *pInstance, DWORD dwTimerId)
{
  TimerList_T   *pTimer = (TimerList_T *)dwTimerId;
  TimerList_T   *pLook;
  TimerList_T   *pList;

  H245ASSERT(TimerInited != 0);

  EnterCriticalSection(&TimerLock);
  if (--TimerInited == 0)
  {
#ifdef _IA_SPOX_
    H223_RegisterTimerCallBack((H223TIMERCB)NULL);
#else
    KillTimer (NULL, H245TimerId);
#endif
  }
  LeaveCriticalSection(&TimerLock);

  if (pInstance->pTimerList == NULL)
  {
    H245TRACE(pInstance->dwInst,1,"H245StopTimer: timer list NULL");
    return TRUE;
  }

  if (pTimer == pInstance->pTimerList)
  {
    pInstance->pTimerList = pTimer->pNext;
    H245_free (pTimer);
    return FALSE;
  }

  pList = pInstance->pTimerList;
  pLook = pList->pNext;
  while (pLook && pLook != pTimer)
  {
     pList = pLook;
     pLook = pLook->pNext;
  }

  /* if the timer exists.. */
  if (pLook == NULL)
  {
    H245TRACE(pInstance->dwInst,1,"H245StopTimer: pTimer not in timer list");
    return TRUE;
  }

  pList->pNext = pTimer->pNext;
  H245_free (pTimer);
  return FALSE;
}

#ifdef _IA_SPOX_
PUBLIC RESULT InitializeCriticalSection(CRITICAL_SECTION * phLock)

{
  return OIL_CreateLock(phLock);
}

PUBLIC RESULT EnterCriticalSection(CRITICAL_SECTION * phLock)
{
  return OIL_AcquireLock(*phLock);
}

PUBLIC RESULT LeaveCriticalSection(CRITICAL_SECTION * phLock)
{
  return OIL_ReleaseLock(*phLock);
}

PUBLIC RESULT DeleteCriticalSection(CRITICAL_SECTION * phLock)
{
  return OIL_DeleteLock(*phLock);
}
#endif
