/******************************************************************************

   Copyright (C) Microsoft Corporation 1985-1995. All rights reserved.

   Title:   aviplay.c - Code for actually playing AVI files, part of
	       AVI's background task.

*****************************************************************************/

#include "graphic.h"

#define AVIREADMANY             // read more than one record at a time

#ifdef _WIN32
//#define AVIREAD               // multi-threaded async read of file
#else
#undef AVIREAD
#endif

#ifdef AVIREAD
#include "aviread.h"
#endif

#define BOUND(x, low, high)     max(min(x, high), low)

#define ALIGNULONG(i)     ((i+3)&(~3))                  /* ULONG aligned ! */

#ifdef INTERVAL_TIMES
BOOL   fOneIntervalPerLine=FALSE;
#endif
//
// redefine StreamFromFOURCC to only handle 0-9 streams!
//
#undef StreamFromFOURCC
#define StreamFromFOURCC(fcc) (UINT)(HIBYTE(LOWORD(fcc)) - (BYTE)'0')

#ifdef DEBUG
static char szBadFrame[] = "Bad frame number";
static char szBadPos[]   = "Bad stream position";
#define AssertFrame(i)   AssertSz((long)(i) <= npMCI->lFrames && (long)(i) >= -(long)npMCI->wEarlyRecords, szBadFrame)
#define AssertPos(psi,i) AssertSz((long)(i) <= psi->lEnd && (long)(i) >= psi->lStart, szBadPos)
#else
#define AssertFrame(i)
#define AssertPos(psi,i)
#endif

#define WIDTHBYTES(i)     ((unsigned)((i+31)&(~31))/8)  /* ULONG aligned ! */
#define DIBWIDTHBYTES(bi) (DWORD)WIDTHBYTES((int)(bi).biWidth * (int)(bi).biBitCount)

LONG NEAR PASCAL WhatFrameIsItTimeFor(NPMCIGRAPHIC npMCI);
LONG NEAR PASCAL HowLongTill(NPMCIGRAPHIC npMCI);
BOOL NEAR PASCAL TimeToQuit(NPMCIGRAPHIC npMCI);
BOOL NEAR PASCAL WaitTillNextFrame(NPMCIGRAPHIC npMCI);

void NEAR PASCAL FindKeyFrame(NPMCIGRAPHIC npMCI, STREAMINFO *psi, LONG lPos);
LONG NEAR PASCAL FindPrevKeyFrame(NPMCIGRAPHIC npMCI, STREAMINFO *psi, LONG lPos);
LONG NEAR PASCAL FindNextKeyFrame(NPMCIGRAPHIC npMCI, STREAMINFO *psi, LONG lPos);

BOOL NEAR PASCAL CalculateTargetFrame(NPMCIGRAPHIC npMCI);
DWORD NEAR PASCAL CalculatePosition(NPMCIGRAPHIC npMCI);
BOOL NEAR PASCAL ReadRecord(NPMCIGRAPHIC npMCI);
BOOL NEAR PASCAL ReadNextVideoFrame(NPMCIGRAPHIC npMCI, STREAMINFO *psi);
STATICFN INLINE DWORD NEAR PASCAL ReadNextChunk(NPMCIGRAPHIC npMCI);

BOOL NEAR PASCAL ReadBuffer(NPMCIGRAPHIC npMCI, LONG off, LONG len);
BOOL NEAR PASCAL AllocateReadBuffer(NPMCIGRAPHIC npMCI);
BOOL NEAR PASCAL ResizeReadBuffer(NPMCIGRAPHIC npMCI, DWORD dwNewSize);
void NEAR PASCAL ReleaseReadBuffer(NPMCIGRAPHIC npMCI);

BOOL NEAR PASCAL ProcessPaletteChanges(NPMCIGRAPHIC npMCI, LONG lFrame);

STATICFN INLINE void DealWithOtherStreams(NPMCIGRAPHIC npMCI, LONG lFrame);
STATICFN INLINE BOOL NEAR PASCAL StreamRead(NPMCIGRAPHIC npMCI, STREAMINFO *psi, LONG lPos);

#ifdef _WIN32
#define AllocMem(dw) GlobalLock(GlobalAlloc(GMEM_MOVEABLE|GMEM_SHARE, (dw)))
#define FreeMem(lp) GlobalFreePtr(lp)
#else
static LPVOID AllocMem(DWORD dw);
#define FreeMem(lp) GlobalFree((HGLOBAL)SELECTOROF(lp))
#endif

INT     gwSkipTolerance = 4;
INT     gwHurryTolerance = 2;
INT     gwMaxSkipEver = 60;
#define YIELDEVERY      8

#ifdef DEBUG
#define WAITHISTOGRAM           /* Extra debugging information */
#define SHOWSKIPPED
//#define BEHINDHIST
#define DRAWTIMEHIST
#define READTIMEHIST
#define TIMEPLAY
#endif

#ifdef WAITHISTOGRAM
    UINT        wHist[100];
#endif

#define HIGHRESTIMER

#ifdef SHOWSKIPPED
#define NUMSKIPSSHOWN   25
    LONG        lSkipped[NUMSKIPSSHOWN];
    UINT        wSkipped = 0;
#endif
#ifdef BEHINDHIST
#define NUMBEHIND       50
#define BEHINDOFFSET    10
    WORD        wBehind[NUMBEHIND];
#endif
#ifdef DRAWTIMEHIST
#define NUMDRAWN        100
    DWORD       dwDrawTime[NUMDRAWN];
    UINT        wDrawn;
#endif
#ifdef READTIMEHIST
#define NUMREAD        100
    DWORD       dwReadTime[NUMREAD];
    UINT        wRead;
#endif

// If available, use a correctly functioning waveOutGetPosition
BOOL	gfUseGetPosition;
LONG	giGetPositionAdjust;

#ifdef AVIREAD
/*
 * the aviread object creates a worker thread to read the file
 * asynchronously. That thread calls this callback function
 * to actually read a buffer from the file. The 'instance data' DWORD in
 * this case is npMCI. see aviread.h for outline.
 */
BOOL mciaviReadBuffer(PBYTE pData, DWORD_PTR dwInstanceData, long lSize, long * lpNextSize)
{
    NPMCIGRAPHIC npMCI = (NPMCIGRAPHIC) dwInstanceData;
    DWORD size;
    DWORD UNALIGNED * lp;

    if(mmioRead(npMCI->hmmio, pData, lSize) != lSize) {
	return(FALSE);
    }

    /* we've read in the complete chunk, plus the FOURCC, size and formtype of
     * the next chunk. So the size of the next chunk is the last but one
     * DWORD in this buffer
     */
    lp = (DWORD UNALIGNED *) (pData + lSize - 2 * sizeof(DWORD));
    size = *lp;

    /* don't forget to add on the FOURCC and size dwords */
    *lpNextSize = size + 2 * sizeof(DWORD);

    return(TRUE);
}
#endif

//
// call this to find out the current position. This function
// should be safe to call from the user thread as well as from the
// worker thread

DWORD InternalGetPosition(NPMCIGRAPHIC npMCI, LPLONG lpl)
{
    LONG    l;

    l = npMCI->lCurrentFrame - npMCI->dwBufferedVideo;


    if ((npMCI->wTaskState == TASKCUEING) &&
       !(npMCI->dwFlags & MCIAVI_SEEKING) &&
       l < npMCI->lRealStart)
	l = npMCI->lRealStart;

    if (l < 0)
	l = 0;

    *lpl = l;

    return 0L;
}




STATICFN DWORD NEAR PASCAL PrepareToPlay(NPMCIGRAPHIC npMCI);
STATICFN DWORD NEAR PASCAL PrepareToPlay2(NPMCIGRAPHIC npMCI);
void  NEAR PASCAL CleanUpPlay(NPMCIGRAPHIC npMCI);
void  NEAR PASCAL CheckSignals(NPMCIGRAPHIC npMCI, LONG lFrame);

BOOL  NEAR PASCAL PlayNonInterleaved(NPMCIGRAPHIC npMCI);
BOOL  NEAR PASCAL PlayInterleaved(NPMCIGRAPHIC npMCI);
BOOL  NEAR PASCAL PlayAudioOnly(NPMCIGRAPHIC npMCI);
BOOL  NEAR PASCAL PlayNonIntFromCD(NPMCIGRAPHIC npMCI);
/***************************************************************************
 *
 * @doc INTERNAL MCIAVI
 *
 * @api UINT | mciaviPlayFile | Play an AVI file.
 *
 * @parm NPMCIGRAPHIC | npMCI | Pointer to instance data.
 *
 * @rdesc Notification code that should be returned.
 *
 ***************************************************************************/

UINT NEAR PASCAL mciaviPlayFile (NPMCIGRAPHIC npMCI, BOOL bSetEvent)
{
    BOOL        fContinue;
    DWORD       dwFlags = npMCI->dwFlags;

    BOOL (NEAR PASCAL *Play)(NPMCIGRAPHIC npMCI);

#ifdef WAITHISTOGRAM
    UINT        w;
#endif
#ifdef SHOWSKIPPED
    wSkipped = 0;
#endif
#ifdef WAITHISTOGRAM
    //for (w = 0; (int)w < (sizeof(wHist)/sizeof(wHist[0])); w++)
	//wHist[w] = 0;
    ZeroMemory(wHist, sizeof(wHist));
#endif
#ifdef BEHINDHIST
    //for (w = 0; w < NUMBEHIND; w++)
	//wBehind[w] = 0;
    ZeroMemory(wBehind, sizeof(wBehind));
#endif
#ifdef DRAWTIMEHIST
    wDrawn = 0;
    //for (w = 0; w < NUMDRAWN; w++)
	//dwDrawTime[w] = 0;
    ZeroMemory(dwDrawTime, sizeof(dwDrawTime));
#endif
#ifdef READTIMEHIST
    wRead = 0;
    //for (w = 0; w < NUMREAD; w++)
	//dwReadTime[w] = 0;
    ZeroMemory(dwReadTime, sizeof(dwReadTime));
#endif


#ifdef HIGHRESTIMER
    /* Figure out how much time each frame takes */
    /* Then set a high resolution timer, unless  */
    /* we are in the special "play every frame". */

    if (npMCI->dwSpeedFactor) {

        // Only if we are worried about timing each frame...
	// Set a timer resolution for a fraction of the frame rate
	// Initially we set the timer to 4ms.  This initial guess can
	// be overridden by a registry setting.
	// Note: there is no UI (yet?) for writing a value to the registry

        if ((npMCI->msPeriodResolution = mmGetProfileInt(szIni, TEXT("TimerResolution"), 4))
           && (0 == timeBeginPeriod(npMCI->msPeriodResolution))) {
	    // Timer successfully set
            DPF1(("Set timer resolution to %d milliseconds\n", npMCI->msPeriodResolution));
        } else {
            TIMECAPS tc;
            if ((timeGetDevCaps(&tc, sizeof(tc)) == 0)  &&
                (0 == timeBeginPeriod(tc.wPeriodMin))) {
                    npMCI->msPeriodResolution = tc.wPeriodMin;
                    DPF1(("Set timer resolution to the minimum of %d milliseconds\n", npMCI->msPeriodResolution));
            } else {
                // Reset, so we do not try and call timeEndPeriod
                // (note: the value may have been set by mmGetProfileInt)
    	        npMCI->msPeriodResolution = 0;
                DPF1(("NO high resolution timer set\n"));
            }
        }
    }
#endif


Repeat:


    // internal task state (used only by worker thread to
    // distinguish cueing/seeking/playing)
    npMCI->wTaskState = TASKSTARTING;


    TIMEZERO(timePlay);
    TIMEZERO(timePrepare);
    TIMEZERO(timeCleanup);
    TIMEZERO(timePaused);
    TIMEZERO(timeRead);
    TIMEZERO(timeWait);
    TIMEZERO(timeYield);
    TIMEZERO(timeVideo);
    TIMEZERO(timeOther);
    TIMEZERO(timeAudio);
    TIMEZERO(timeDraw);
    TIMEZERO(timeDecompress);

    TIMESTART(timePrepare);
    npMCI->dwTaskError = PrepareToPlay(npMCI);

    TIMEEND(timePrepare);

#ifdef INTERVAL_TIMES
    // interframe timing
    npMCI->nFrames = 0;
    npMCI->msFrameTotal = 0;
    npMCI->msSquares = 0;
    npMCI->msFrameMax = 0;
    npMCI->msFrameMin = 9999;
    npMCI->msReadMaxBytesPer = 0;
    npMCI->msReadMax = 0;
    npMCI->nReads = 0;
    npMCI->msReadTotal = 0;
    npMCI->msReadTimeuS = 0;

    {
        int i;
	PLONG pL;
        //for (i = 0; i < NBUCKETS; i++) {
        //    npMCI->buckets[i] = 0;
        //}
	ZeroMemory(npMCI->buckets, NBUCKETS*sizeof(LONG));
	pL = npMCI->paIntervals;
	if (pL) {
	    ZeroMemory(pL, (npMCI->cIntervals)*sizeof(LONG));
	}
    }
#endif


    //
    //  pick a play function.
    //
    switch (npMCI->wPlaybackAlg) {
	case MCIAVI_ALG_INTERLEAVED:
	    Play = PlayInterleaved;
	    break;

#ifdef USENONINTFROMCD	
	case MCIAVI_ALG_CDROM:
	    Play = PlayNonIntFromCD;
	    break;
#endif

	case MCIAVI_ALG_HARDDISK:
	    Play = PlayNonInterleaved;
	    break;

	case MCIAVI_ALG_AUDIOONLY:
	    Play = PlayAudioOnly;
	    break;

	default:
	    Assert(0);
	    return MCI_NOTIFY_ABORTED; //???
    }

    // bias lTo by dwBufferedVideo so we play to the right place
    npMCI->lTo += npMCI->dwBufferedVideo;

    npMCI->lFramePlayStart = npMCI->lRealStart;

    DPF(("Playing from %ld to %ld, starting at %ld.  Flags=%0.8x\n", npMCI->lFrom, npMCI->lTo, npMCI->lCurrentFrame, npMCI->dwFlags));

    // at this point we have detected any errors that we are going to detect
    // during startup. we can complete the synchronous portion of the command
    // if requested to do so
    if (bSetEvent) {
	bSetEvent = FALSE;
	TaskReturns(npMCI, npMCI->dwTaskError);
    }

    if (npMCI->dwTaskError != 0L)
	goto SKIP_PLAYING;



    /* We're done initializing; now we're warming up to play. */
    npMCI->wTaskState = TASKCUEING;

    TIMESTART(timePlay);

  try {
  // If the avi file is corrupt we want to try and catch errors without
  // aborting the application.  Hence we need this try/except clause at
  // this very high level.  It PROBABLY costs too much to set up the
  // exception handler every time we want to draw a single frame.

    /* Loop until things are done */
    while (1) {

	if (npMCI->dwFlags & MCIAVI_REVERSE) {
	    if (npMCI->lCurrentFrame < npMCI->lTo)
		break;
	} else {
	    if (npMCI->lCurrentFrame > npMCI->lTo)
		break;
	}

	if ((npMCI->wTaskState != TASKPLAYING) &&
		!(npMCI->dwFlags & MCIAVI_UPDATING)) {

	    TIMESTART(timeYield);
	    aviTaskCheckRequests(npMCI);
	    TIMEEND(timeYield);
	}

	fContinue = Play(npMCI);

	if (fContinue) fContinue = !TimeToQuit(npMCI);

	if (!fContinue)
	    break;

	//
	//  while playing we may need to update
	//
	//  always mark the movie as clean, even if a stream fails to update
	//  otherwise we will need to stop play and restart.
	//
	if (!(npMCI->dwFlags & MCIAVI_SEEKING) &&
	     (npMCI->dwFlags & MCIAVI_NEEDUPDATE)) {
	    DoStreamUpdate(npMCI, FALSE);

	    if (npMCI->dwFlags & MCIAVI_NEEDUPDATE) {
		DOUT("Update failed while playing, I dont care!\n");
		npMCI->dwFlags &= ~MCIAVI_NEEDUPDATE;   //!!! I dont care if it failed
	    }
	}

	/* Increment the frame number.  If we're done, don't increment
	** it an extra time, but just get out.
	*/
	if (npMCI->dwFlags & MCIAVI_REVERSE) {
	    if (npMCI->lCurrentFrame > npMCI->lTo)
		--npMCI->lCurrentFrame;
	    else
		break;
	} else {
	    if (npMCI->lCurrentFrame < npMCI->lTo)
		++npMCI->lCurrentFrame;
	    else {

		// need to ensure that we wait for audio to complete on the
		// last frame played
		if ((npMCI->lFrom != npMCI->lTo) &&
		    (npMCI->wTaskState == TASKPLAYING)) {

		    npMCI->lCurrentFrame++;
		    WaitTillNextFrame(npMCI);
		    npMCI->lCurrentFrame--;
		}

		break;
	    }
	}
    }
  } except (EXCEPTION_EXECUTE_HANDLER) {
      npMCI->dwTaskError = MCIERR_DRIVER_INTERNAL;
  }

    TIMEEND(timePlay);

    if (npMCI->lCurrentFrame != npMCI->lTo) {

	DPF(("Ended at %ld, not %ld (drawn = %ld).\n", npMCI->lCurrentFrame, npMCI->lTo, npMCI->lFrameDrawn));

	//
	// if we ended early lets set lCurrentFrame to the last frame
	// drawn to guarantee we can re-paint the frame, we dont
	// want to do this when we play to end because after playing
	// from A to B the current position *must* be B or preston will
	// enter a bug.
	//
	// but only set this if lFrameDraw is valid
	//

	if (npMCI->lFrameDrawn > (-(LONG)npMCI->wEarlyRecords))
	    npMCI->lCurrentFrame = npMCI->lFrameDrawn;
    }

SKIP_PLAYING:
    /* Flush any extra changes out to screen */

    DPF2(("Updating unfinished changes....\n"));

    // Make sure we really draw.... !!!do we need this?
    // npMCI->lRealStart = npMCI->lCurrentFrame;

    if (npMCI->hdc)
	DoStreamUpdate(npMCI, FALSE);

    npMCI->lTo -= npMCI->dwBufferedVideo;
    npMCI->lCurrentFrame -= npMCI->dwBufferedVideo;
    npMCI->dwBufferedVideo = 0;

    if (npMCI->lCurrentFrame < 0) {
	DPF2(("Adjusting position to be >= 0.\n"));
	npMCI->lCurrentFrame = 0;
    }

    if (npMCI->lTo < 0)
	npMCI->lTo = 0;

    /* Adjust position to be > start? */
    /* Adjust position to be > where it was when we began? */

    npMCI->dwTotalMSec += Now() - npMCI->dwMSecPlayStart;

    TIMESTART(timeCleanup);
    DPF(("Cleaning up the play\n"));
    CleanUpPlay(npMCI);
    TIMEEND(timeCleanup);

#ifdef AVIREAD
    /* shut down async reader */
    if (npMCI->hAviRd) {
	avird_endread(npMCI->hAviRd);
	npMCI->hAviRd = NULL;
    }
#endif

    /* If we're repeating, do it.  It sure would be nice if we could repeat
    ** without de-allocating and then re-allocating all of our buffers....
    */
    if (npMCI->dwTaskError == 0 && (!(npMCI->dwFlags & MCIAVI_STOP)) &&
		(npMCI->dwFlags & MCIAVI_REPEATING)) {
	npMCI->lFrom = npMCI->lRepeatFrom;

	//
	//  DrawEnd() likes to clear this flag so make sure it gets set
	//  in the repeat case.
	//
	if (dwFlags & MCIAVI_FULLSCREEN)
	    npMCI->dwFlags |= MCIAVI_FULLSCREEN;

	//
	// make sure we set the task state back before we repeat.
	// otherwise our code will think we are playing, for example.
	// if the audio code thinks we are playing and see's the wave buffers
	// are empty it will reset the wave device then restart it when
	// they get full again, this is bad if we are pre-rolling audio.
	//
	npMCI->wTaskState = TASKSTARTING;

	DPF((".........repeating\n"));
	goto Repeat;
    }

    /* Turn off flags only used during play. */
    npMCI->dwFlags &= ~(MCIAVI_STOP | MCIAVI_PAUSE | MCIAVI_SEEKING |
	    MCIAVI_REPEATING | MCIAVI_FULLSCREEN);

    if (npMCI->wTaskState == TASKPLAYING) {
	DWORD   dwCorrectTime;
	DWORD   dwFramesPlayed;

	dwFramesPlayed = (npMCI->dwFlags & MCIAVI_REVERSE) ?
	    npMCI->lFramePlayStart - npMCI->lCurrentFrame :
	    npMCI->lCurrentFrame - npMCI->lFramePlayStart;

	dwCorrectTime = muldiv32(dwFramesPlayed,
				muldiv32(npMCI->dwMicroSecPerFrame,
					1000L,
					(npMCI->dwSpeedFactor == 0 ?
						1000 : npMCI->dwSpeedFactor)),
				1000);

	if (dwCorrectTime != 0 && npMCI->dwTotalMSec != 0)
	    npMCI->dwSpeedPercentage = muldiv32(dwCorrectTime, 100,
						npMCI->dwTotalMSec);
	else
	    npMCI->dwSpeedPercentage = 100;

	if (dwFramesPlayed > 15) {
	    npMCI->lFramesPlayed      = (LONG)dwFramesPlayed;
	    npMCI->lFramesSeekedPast  = (LONG)npMCI->dwFramesSeekedPast;
	    npMCI->lSkippedFrames     = (LONG)npMCI->dwSkippedFrames;
	    npMCI->lAudioBreaks       = (LONG)npMCI->dwAudioBreaks;
	}

#ifdef DEBUG
	if (npMCI->dwFramesSeekedPast) {
	    DPF(("Didn't even read %ld frames.\n", npMCI->dwFramesSeekedPast));
	}
	if (npMCI->dwSkippedFrames && dwFramesPlayed > 0) {
	    DPF(("Skipped %ld of %ld frames. (%ld%%)\n",
			npMCI->dwSkippedFrames, dwFramesPlayed,
			npMCI->dwSkippedFrames*100/dwFramesPlayed));
	}
	if (npMCI->dwAudioBreaks) {
	    DPF(("Audio broke up %lu times.\n", npMCI->dwAudioBreaks));
	}
#ifndef TIMEPLAY
	DPF(("Played at %lu%% of correct speed.\n", npMCI->dwSpeedPercentage));
	DPF(("Correct time = %lu ms, Actual = %lu ms.\n",
				    dwCorrectTime, npMCI->dwTotalMSec));
#endif
#endif

    // don't print pages of stuff when we've just stopped temporarily,
    // it gets on my nerves.
    if (! (npMCI->dwFlags & MCIAVI_UPDATING)) {
#ifdef DEBUG
        extern int giDebugLevel, giTimingLevel;
        int oldDebugLevel = giDebugLevel;
        giDebugLevel = max(giTimingLevel, oldDebugLevel);
#endif

#ifdef SHOWSKIPPED
	if (wSkipped) {
	    DPF(("Skipped: "));
	    for (w = 0; w < wSkipped; w++) {
		DPF(("!%ld ", lSkipped[w]));
	    }
	    DPF(("!\n"));
	}
#endif

#ifdef WAITHISTOGRAM
	if (npMCI->wPlaybackAlg == MCIAVI_ALG_INTERLEAVED) {
	    DPF(("Wait histogram: "));
	    for (w = 0; (int)w <= (int)npMCI->wABs; w++) {
		if (wHist[w]) {
		    DPF(("![%d]: %d  ",w,wHist[w]));
		}
	    }
	    DPF(("!\n"));
	}
#endif
#ifdef BEHINDHIST
	DPF(("Behind histogram: "));
	for (w = 0; w <= NUMBEHIND; w++) {
	    if (wBehind[w]) {
		DPF(("![%d]: %d  ",w - BEHINDOFFSET,wBehind[w]));
	    }
	}
	DPF(("!\n"));
#endif
#ifdef DRAWTIMEHIST
	DPF(("Draw times: "));
	for (w = 0; w < wDrawn; w++) {
	    DPF(("!%lu ", dwDrawTime[w]));
	}
	DPF(("!\n"));
#endif

#ifdef READTIMEHIST
	DPF(("Read times: "));
	for (w = 0; w < wRead; w++) {
	    DPF(("!%lu ", dwReadTime[w]));
	}
	DPF(("!\n"));
#endif

#ifdef TIMEPLAY
	#define SEC(time)    (UINT)(npMCI->time / 1000l) , (UINT)(npMCI->time % 1000l)
	#define SECX(time,t) SEC(time) , (npMCI->t ? (UINT)(npMCI->time * 100l / npMCI->t) : 0)

	DPF(("***********************************************************\r\n"));
	DPF(("    timePlay:         %3d.%03dsec\r\n",SEC(timePlay)));
	DPF(("     timeRead:        %3d.%03dsec (%d%%)\r\n",SECX(timeRead, timePlay)));
	DPF(("     timeWait:        %3d.%03dsec (%d%%)\r\n",SECX(timeWait, timePlay)));
	DPF(("     timeYield:       %3d.%03dsec (%d%%)\r\n",SECX(timeYield, timePlay)));
	DPF(("     timeVideo:       %3d.%03dsec (%d%%)\r\n",SECX(timeVideo, timePlay)));
	DPF(("      timeDraw:       %3d.%03dsec (%d%%)\r\n",SECX(timeDraw, timeVideo)));
	DPF(("      timeDecompress: %3d.%03dsec (%d%%)\r\n",SECX(timeDecompress, timeVideo)));
	DPF(("     timeAudio:       %3d.%03dsec (%d%%)\r\n",SECX(timeAudio, timePlay)));
	DPF(("     timeOther:       %3d.%03dsec (%d%%)\r\n",SECX(timeOther, timePlay)));
	DPF(("    timePaused:       %3d.%03dsec\r\n",SEC(timePaused)));
	DPF(("    timePrepare:      %3d.%03dsec\r\n",SEC(timePrepare)));
	DPF(("    timeCleanup:      %3d.%03dsec\r\n",SEC(timeCleanup)));
	DPF(("***********************************************************\r\n"));
#endif

#ifdef INTERVAL_TIMES
// frame interval timing

        if (npMCI->nFrames > 2) {
            int i;

            DPF(("-- %ld frames, ave interval %ld ms\r\n", npMCI->nFrames,
                npMCI->msFrameTotal/(npMCI->nFrames-1)));
            DPF(("-- min %ld ms, max %ld ms\r\n", npMCI->msFrameMin, npMCI->msFrameMax));
            DPF(("-- sd = sqrt(%ld)\r\n",
                        (npMCI->msSquares -
                          MulDiv(npMCI->msFrameTotal,
                                 npMCI->msFrameTotal,
                                 npMCI->nFrames-1)
                        ) / (npMCI->nFrames-2)
               ));
            for (i = 3; i < NBUCKETS-3; i++) {
                DPF(("%d ms:  %d\r\n", i * 10, npMCI->buckets[i]));
            }

	    DPF(("Actual intervals:\r\n"));
	    for (i = 1; i < min(npMCI->cIntervals,npMCI->nFrames); i++) {
		DPF(("!%3ld ", *(npMCI->paIntervals+i)));
                if ((fOneIntervalPerLine) || ((i % 20) == 0))
		    DPF(("!\n"));
	    }
	    DPF(("!\r\n"));
	}

	if (npMCI->nReads > 0) {
	    DPF(("-- %ld disk reads, ave %ld ms, max %ld ms\r\n",
		 npMCI->nReads,
		 npMCI->msReadTotal/(npMCI->nReads),
		 npMCI->msReadMax));
	}
#ifdef DEBUG
	giDebugLevel = oldDebugLevel;
#endif
#endif
    }


    }

#ifdef HIGHRESTIMER

    /* If we set a high resolution timer earlier... */
    if (npMCI->msPeriodResolution) {
	// Clear the timer resolution
	timeEndPeriod(npMCI->msPeriodResolution);
        DPF1(("Cleared the timer resolution from %d milliseconds\n", npMCI->msPeriodResolution));
        npMCI->msPeriodResolution = 0;
    }
#endif

    // if we are not stopping temporarily, then set task state to idle
    if (! (npMCI->dwFlags & MCIAVI_UPDATING)) {
	npMCI->wTaskState = TASKIDLE;
    }


    DPF(("mciaviPlayFile ending, dwTaskError==%d\n",npMCI->dwTaskError));
    if (npMCI->dwTaskError)
	return MCI_NOTIFY_FAILURE;

    if (npMCI->dwFlags & MCIAVI_REVERSE) {
	if (npMCI->lCurrentFrame <= npMCI->lTo)
	    return MCI_NOTIFY_SUCCESSFUL;
    } else {
	if (npMCI->lCurrentFrame >= npMCI->lTo)
	    return MCI_NOTIFY_SUCCESSFUL;
    }

    return MCI_NOTIFY_ABORTED;
}

static BOOL NEAR PASCAL RestartAVI(NPMCIGRAPHIC npMCI);
static BOOL NEAR PASCAL PauseAVI(NPMCIGRAPHIC npMCI);
static BOOL NEAR PASCAL BePaused(NPMCIGRAPHIC npMCI);

/******************************************************************************
 *****************************************************************************/

#ifdef DEBUG

INLINE void FillR(HDC hdc, LPRECT prc, DWORD rgb)
{
    SetBkColor(hdc,rgb);
    ExtTextOut(hdc,0,0,ETO_OPAQUE,prc,NULL,0,NULL);
}

void StatusBar(NPMCIGRAPHIC npMCI, int n, int dx, int max, int cur)
{
    HDC hdc;
    RECT rc;

    if (npMCI->dwFlags & MCIAVI_FULLSCREEN)
	return;

    if (cur > max)
	cur = max+1;

    if (cur < 0)
	cur = 0;

    /*
     * If the window is iconic, or there is no title bar, return
     * without painting the status bars.
     */

    if (!IsWindow(npMCI->hwndPlayback) || IsIconic(npMCI->hwndPlayback)) {
        return;
    }
    if (!(GetWindowLong((npMCI->hwndPlayback), GWL_STYLE) & WS_CAPTION)) {
        return;
    }

    hdc = GetWindowDC(npMCI->hwndPlayback);

    //
    //  show the amount of audio and how far behind we are
    //
    rc.left = 32;
    rc.top  = 4 + n*5;
    rc.bottom = rc.top + 4;

    rc.right = rc.left + cur * dx;

    FillR(hdc, &rc, RGB(255,255,0));

    rc.left = rc.right;
    rc.right = rc.left + (max - cur) * dx;

    FillR(hdc, &rc, RGB(255,0,0));

    ReleaseDC(npMCI->hwndPlayback, hdc);
}

#else

#define StatusBar(p,a,b,c,d)

#endif

/******************************************************************************
 *****************************************************************************/

BOOL NEAR PASCAL PlayInterleaved(NPMCIGRAPHIC npMCI)
{
    LONG iFrame;
    LONG iKey;
    LONG iNextKey;
    LONG iPrevKey;
    BOOL fHurryUp=FALSE;
    int  iHurryUp=0;
    BOOL fPlayedAudio = FALSE;
    BOOL f;

    DPF2(("PlayInterleaved, npMCI=%8x\n",npMCI));

    /* If lCurrentFrame == lFrames, we're really at the end of
    ** the file, so there isn't another record to read.
    */
    if (npMCI->lCurrentFrame < npMCI->lFrames) {
	/* Read new record into buffer */

	DPF2(("Reading", iFrame = (LONG)timeGetTime()));
	TIMESTART(timeRead);
	f = ReadRecord(npMCI);
	TIMEEND(timeRead);
	DPF2((".done %ldms\n", (LONG)timeGetTime() - iFrame));

	if (!f) {
	    npMCI->dwTaskError = MCIERR_INVALID_FILE;
	    DPF(("Error reading frame #%ld\n", npMCI->lCurrentFrame));
	    return FALSE;
	}


	if (npMCI->hWave && npMCI->lCurrentFrame >= npMCI->lAudioStart) {
	    TIMESTART(timeAudio);
	    if (!PlayRecordAudio(npMCI, &fHurryUp, &fPlayedAudio)) {
		DPF(("Error playing frame #%ld audio\n", npMCI->lCurrentFrame));
		return FALSE;
	    }
	    TIMEEND(timeAudio);
	}
    }

    /* If we're at the right frame, and we haven't started yet,
    ** then begin play and start timing.
    */

    if ((npMCI->lCurrentFrame > npMCI->lRealStart + (LONG) npMCI->dwBufferedVideo) &&
			(npMCI->wTaskState != TASKPLAYING)) {

	if (!(npMCI->dwFlags & MCIAVI_PAUSE)) {
	    goto RestartPlay0;
	} else {
	    // We were paused already, and now we're restarting or pausing
	    // again... so we've already done the KeepFilling stuff to pre-fill
	    // our buffers. If we don't skip over the KeepFilling stuff we'll
	    // roll ahead.
	    PauseAVI(npMCI);
	    goto BePaused0;
        }
    }

    if (npMCI->wTaskState == TASKPLAYING) {
	if (npMCI->dwFlags & MCIAVI_PAUSE) {

	    PauseAVI(npMCI);

#ifndef _WIN32
// no way do we want to do this on NT.  If you get a slow disk, you will
// never get to pause because we can't get the stuff in fast enough to keep up
// !!! The above is not necessarily true

	    /* The line below says that if we're trying to pause,
	    ** but we're behind on our audio, we should keep playing
	    ** for a little bit so that our audio buffers get full.
	    ** This way we'll be all cued up when we RESUME and be less
	    ** likely to fall behind.
	    ** The jerky playback fix makes us use big buffers now so
	    ** the last buffer can never be totally filled, so it's OK
	    ** to stop after filling up (total - 1) buffers.
	    */
	    if (fPlayedAudio && npMCI->wABFull < npMCI->wABs - 1)
		goto KeepFilling;
#endif


BePaused0:
	    BePaused(npMCI);

RestartPlay0:
	    if (npMCI->dwFlags & MCIAVI_STOP)
		return FALSE;

	    if (TimeToQuit(npMCI))
		return FALSE;

	    RestartAVI(npMCI);
	}
    }

#ifndef _WIN32
KeepFilling:
#endif
    if (npMCI->lCurrentFrame > npMCI->lVideoStart &&
	npMCI->lCurrentFrame < npMCI->lFrames &&
	npMCI->wTaskState == TASKPLAYING) {

	iFrame = WhatFrameIsItTimeFor(npMCI);

	if (iFrame >= npMCI->lFrames)
	    goto dontskip;

	iHurryUp = (int)(iFrame - npMCI->lCurrentFrame);
	fHurryUp = iHurryUp > gwHurryTolerance;


	if (iHurryUp > 1 && npMCI->hpFrameIndex && (npMCI->dwOptionFlags & MCIAVIO_SKIPFRAMES)) {

	    //
	    //  WE ARE BEHIND!!! by one or more frames.
	    //
	    //  if we are late we can do one of the following:
	    //
	    //      dont draw frames but keep reading/decompressing them
	    //      (ie set fHurryUp)
	    //
	    //      skip ahead to a key frame.
	    //
	    // !!! If we're very close to the next key frame, be more
	    // willing to skip ahead....
	    //

	    if (iHurryUp > gwSkipTolerance) {

		iNextKey = FrameNextKey(iFrame);
		iPrevKey = FramePrevKey(iFrame);

		if (iPrevKey > npMCI->lCurrentFrame &&
		    iFrame - iPrevKey < gwHurryTolerance &&
		    iNextKey - iFrame > gwSkipTolerance) {

		    DPF2(("Skipping from %ld to PREV KEY %ld (time for %ld next key=%ld).\n", npMCI->lCurrentFrame, iPrevKey, iFrame, iNextKey));
		    iKey = iPrevKey;
		}
		// !!! We'll only skip if the key frame is at most as far
		// ahead as we are behind.....
		else if (iNextKey > npMCI->lCurrentFrame &&
		    iNextKey <= iFrame + gwSkipTolerance /*gwMaxSkipEver*/) {
		    DPF2(("Skipping from %ld to NEXT KEY %ld (time for %ld prev key=%ld).\n", npMCI->lCurrentFrame, iNextKey, iFrame, iPrevKey));
		    iKey = iNextKey;
		} else {
		    DPF2(("WANTED to skip from %ld to %ld (time for %ld)!\n", npMCI->lCurrentFrame,iNextKey,iFrame));
		    goto dontskip;
		}

		npMCI->lVideoStart = iKey;
		npMCI->dwSkippedFrames += iKey - npMCI->lCurrentFrame;
dontskip:
		fHurryUp = TRUE;
	    }
	    else {
		iKey = FrameNextKey(npMCI->lCurrentFrame);

		if (iKey - npMCI->lCurrentFrame > 0 &&
		    iKey - npMCI->lCurrentFrame <= gwHurryTolerance) {
		    DPF2(("Skipping from %ld to next key frame %ld (time for %ld).\n", npMCI->lCurrentFrame, iKey, iFrame));

		    npMCI->dwSkippedFrames += iKey - npMCI->lCurrentFrame;
		    npMCI->lVideoStart = iKey;
		    fHurryUp = TRUE;
		}
	    }
	}
    }

    if (npMCI->dwFlags & MCIAVI_WAVEPAUSED)
	fHurryUp = TRUE;

    /* If we've actually started timing:
    **  Check if we should send a signal.
    **  Check to see if we should break out of the loop.
    **  Wait until it's time for the next frame.
    */
    if (npMCI->wTaskState == TASKPLAYING &&
	npMCI->lCurrentFrame >= npMCI->lVideoStart) {

	if (npMCI->dwSignals)
	    CheckSignals(npMCI, npMCI->lCurrentFrame - npMCI->dwBufferedVideo);

#ifdef WAITHISTOGRAM
	/* Adjust to achieve proper tension. */
	if (fPlayedAudio) {
	    /* If we're playing, keep statistics about how we're doing. */
	    ++wHist[npMCI->wABFull];
	}
#endif

	if (!WaitTillNextFrame(npMCI))
	    return FALSE;
    }

    if (npMCI->lCurrentFrame >= npMCI->lVideoStart &&
	npMCI->lCurrentFrame < npMCI->lFrames) {

#ifdef SHOWSKIPPED
	if (fHurryUp && wSkipped < NUMSKIPSSHOWN) {
	    lSkipped[wSkipped++] = npMCI->lCurrentFrame;
	}
#endif
	/* hold critsec round all worker thread drawing */
	EnterHDCCrit(npMCI);
	TIMESTART(timeVideo);
	if (!DisplayVideoFrame(npMCI, fHurryUp)) {
	    npMCI->dwTaskError = MCIERR_AVI_DISPLAYERROR;
	    TIMEZERO(timeVideo);
	    LeaveHDCCrit(npMCI);
	    return FALSE;
	}

	TIMEEND(timeVideo);
	LeaveHDCCrit(npMCI);

#ifdef DRAWTIMEHIST
	if (!fHurryUp && (wDrawn < NUMDRAWN)) {
	    dwDrawTime[wDrawn++] = npMCI->dwLastDrawTime;
	}
#endif
    }

    StatusBar(npMCI, 0, 4, npMCI->wABs, npMCI->wABFull);
    StatusBar(npMCI, 1, 4, npMCI->wABs, npMCI->wABs - iHurryUp);

#ifdef AVIREAD
    if ((npMCI->hAviRd) && (npMCI->lpBuffer != NULL)) {
	/* finished with this buffer - put back on queue */
	avird_emptybuffer(npMCI->hAviRd, npMCI->lpBuffer);
	npMCI->lpBuffer = NULL;
    }
#endif

    DPF2(("PlayInterleaved...ENDING, npMCI=%8x, TaskState=%d\n", npMCI, npMCI->wTaskState));
    return TRUE;
}

/******************************************************************************
 *****************************************************************************/

BOOL NEAR PASCAL PlayNonInterleaved(NPMCIGRAPHIC npMCI)
{
    BOOL fHurryUp = FALSE;
    int  iHurryUp;
    LONG iFrame;
    LONG iKey;
    LONG iNextKey;
    LONG iPrevKey;

    DPF2(("PlayNonInterleaved, npMCI=%8x\n",npMCI));
    if (npMCI->hWave) {
	TIMESTART(timeAudio);
	KeepPlayingAudio(npMCI);
	TIMEEND(timeAudio);
    }

    if (npMCI->wTaskState == TASKPLAYING) {

	iFrame = WhatFrameIsItTimeFor(npMCI);

	if (iFrame >= npMCI->lFrames)
	    goto dontskip;

	if (npMCI->dwFlags & MCIAVI_REVERSE) {

	    /* Since we're going backwards, always skip to key frame. */
	    DPF3(("  Current = %ld, time for %ld.\n", npMCI->lCurrentFrame, iFrame));

	    iFrame = FramePrevKey(iFrame);

	    // !!! Send signals for skipped frames?
	    npMCI->dwFramesSeekedPast += npMCI->lCurrentFrame - iFrame;
	    npMCI->dwSkippedFrames += npMCI->lCurrentFrame - iFrame;
	    npMCI->lCurrentFrame = iFrame;
	} else if (npMCI->lCurrentFrame < npMCI->lFrames) {

#ifdef BEHINDHIST
	    {
	    int      iDelta;
	    iDelta = iFrame - npMCI->lCurrentFrame + BEHINDOFFSET;
	    iDelta = min(NUMBEHIND, max(0, iDelta));
	    wBehind[iDelta]++;
	    }
#endif
	    iHurryUp = (int)(iFrame - npMCI->lCurrentFrame);
	    fHurryUp = iHurryUp > gwHurryTolerance;

	    if (iHurryUp > 1 && npMCI->hpFrameIndex && (npMCI->dwOptionFlags & MCIAVIO_SKIPFRAMES)) {

		//
		//  WE ARE BEHIND!!! by one or more frames.
		//
		//  if we are late we can do one of the following:
		//
		//      dont draw frames but keep reading/decompressing them
		//      (ie set fHurryUp)
		//
		//      skip ahead to a key frame.
		//
		// !!! If we're very close to the next key frame, be more
		// willing to skip ahead....
		//

		if (iHurryUp > gwSkipTolerance) {

		    iNextKey = FrameNextKey(iFrame);
		    iPrevKey = FramePrevKey(iFrame);

		    if (iPrevKey > npMCI->lCurrentFrame &&
			iFrame - iPrevKey < gwHurryTolerance &&
			iNextKey - iFrame > gwSkipTolerance) {

			DPF2(("Skipping from %ld to PREV KEY %ld (time for %ld next key=%ld).\n", npMCI->lCurrentFrame, iPrevKey, iFrame, iNextKey));

			iKey = iPrevKey;
			fHurryUp = TRUE;
		    }
		    // !!! We'll only skip if the key frame is at most as far
		    // ahead as we are behind.....
		    else if (iNextKey > npMCI->lCurrentFrame &&
			iNextKey <= iFrame + gwSkipTolerance /*gwMaxSkipEver*/) {
			DPF2(("Skipping from %ld to NEXT KEY %ld (time for %ld prev key=%ld).\n", npMCI->lCurrentFrame, iNextKey, iFrame, iPrevKey));
			iKey = iNextKey;        // assume next key
			fHurryUp = FALSE;
		    } else {
			DPF2(("WANTED to skip from %ld to %ld (time for %ld)!\n", npMCI->lCurrentFrame,iNextKey,iFrame));
			goto dontskip;
		    }

		    npMCI->dwFramesSeekedPast += iKey - npMCI->lCurrentFrame;
		    npMCI->dwSkippedFrames += iKey - npMCI->lCurrentFrame;
		    npMCI->lCurrentFrame = iKey;
dontskip:
		    ;
		}
		else if (FramePrevKey(iFrame) == iFrame) {
		    DPF2(("Skipping from %ld to %ld (time for key frame).\n", npMCI->lCurrentFrame, iFrame));

		    iKey = iFrame;
		    npMCI->dwFramesSeekedPast += iKey - npMCI->lCurrentFrame;
		    npMCI->dwSkippedFrames += iKey - npMCI->lCurrentFrame;
		    npMCI->lCurrentFrame = iKey;

		    fHurryUp = FALSE;
		}
		else {
		    iKey = FrameNextKey(npMCI->lCurrentFrame);

		    if (iKey > npMCI->lCurrentFrame &&
			iKey - npMCI->lCurrentFrame <= gwHurryTolerance) {
			DPF2(("Skipping from %ld to next key frame %ld (time for %ld).\n", npMCI->lCurrentFrame, iKey, iFrame));

			npMCI->dwFramesSeekedPast += iKey - npMCI->lCurrentFrame;
			npMCI->dwSkippedFrames += iKey - npMCI->lCurrentFrame;
			npMCI->lCurrentFrame = iKey;

			fHurryUp = ((iKey - iFrame) > gwHurryTolerance);
		    }
		}
	    }

	    StatusBar(npMCI, 0, 4, npMCI->wABs, npMCI->wABFull);
	    StatusBar(npMCI, 1, 4, npMCI->wABs, npMCI->wABs - iHurryUp);
	}
    }

    // !!! Somewhere in here, read other streams.
    // Should this be before, or after, video?

    /* If lCurrentFrame == lFrames, we're really at the end of
    ** the file, so there isn't another record to read.
    */
    if (npMCI->lCurrentFrame < npMCI->lFrames) {
	/* Read new record into buffer */

	npMCI->dwLastReadTime = (DWORD)(-(LONG)timeGetTime());
	TIMESTART(timeRead);
	if (!ReadNextVideoFrame(npMCI, NULL)) {
	    npMCI->dwTaskError = MCIERR_INVALID_FILE;
	    DPF2(("Error reading frame #%ld\n", npMCI->lCurrentFrame));
	    return FALSE;
	}
	TIMEEND(timeRead);

	npMCI->dwLastReadTime += timeGetTime();
	npMCI->lLastRead = npMCI->lCurrentFrame;

#ifdef READTIMEHIST
	if (wRead < NUMREAD) {
	    dwReadTime[wRead++] = npMCI->dwLastReadTime;
    }
#endif
    }

    /* If we're at the right frame, and we haven't started yet,
    ** then begin play and start timing.
    */
    if ((((npMCI->lCurrentFrame > (npMCI->lRealStart +
					(LONG) npMCI->dwBufferedVideo)) &&
			(npMCI->lCurrentFrame < (npMCI->lTo))) ||
			(npMCI->dwFlags & MCIAVI_REVERSE)) &&
			(npMCI->wTaskState != TASKPLAYING) &&
			!(npMCI->dwFlags & MCIAVI_SEEKING)) {
	if (!(npMCI->dwFlags & MCIAVI_PAUSE)) {
	    goto RestartPlay;
	} else
	    goto PauseNow;
    }

    /* If we've actually started timing:
    **  Check if we should send a signal.
    **  Check to see if we should return FALSE out of the loop.
    **  Wait until it's time for the next frame.
    */
    if (npMCI->wTaskState == TASKPLAYING) {
	if (npMCI->dwFlags & MCIAVI_PAUSE) {
PauseNow:
	    PauseAVI(npMCI);

	    BePaused(npMCI);

RestartPlay:
	    if (TimeToQuit(npMCI))
		return FALSE;

	    RestartAVI(npMCI);
	}

	if (npMCI->dwSignals)
	    CheckSignals(npMCI, npMCI->lCurrentFrame - npMCI->dwBufferedVideo);

	if (npMCI->lCurrentFrame < npMCI->lFrames + (LONG) npMCI->dwBufferedVideo) {
	    while (1) {
		iFrame = WhatFrameIsItTimeFor(npMCI);

		TIMESTART(timeYield);
		aviTaskCheckRequests(npMCI);
		TIMEEND(timeYield);

		if (npMCI->dwFlags & MCIAVI_REVERSE) {
		    if (iFrame <= npMCI->lCurrentFrame)
			break;

		    if (npMCI->lCurrentFrame < npMCI->lTo)
			break;
		} else {
		    if (iFrame >= npMCI->lCurrentFrame)
			break;

		    if (npMCI->lCurrentFrame > npMCI->lTo)
			break;
		}

		if (npMCI->hWave) {
		    TIMESTART(timeAudio);
		    KeepPlayingAudio(npMCI);
		    TIMEEND(timeAudio);
		}

		DPF3(("Waiting:  Current = %ld, time for %ld.\n", npMCI->lCurrentFrame, iFrame));
		if (!(npMCI->dwFlags & MCIAVI_REVERSE)) {
		    WaitTillNextFrame(npMCI);
		}

		if (TimeToQuit(npMCI))
		    return FALSE;
	    }
	}

	if (TimeToQuit(npMCI))
	    return FALSE;
    }

    if (((npMCI->lCurrentFrame >= npMCI->lVideoStart) &&
	    (npMCI->lCurrentFrame < npMCI->lFrames)) ||
	(npMCI->dwFlags & MCIAVI_REVERSE)) {

	// Quick exit if we are being told to stop
	if (npMCI->dwFlags & MCIAVI_STOP) {
	    return(FALSE);
	}

	EnterHDCCrit(npMCI);
	TIMESTART(timeVideo);
	if (!DisplayVideoFrame(npMCI, fHurryUp)) {
	    npMCI->dwTaskError = MCIERR_AVI_DISPLAYERROR;
	    TIMEZERO(timeVideo);
	    LeaveHDCCrit(npMCI);
	    return FALSE;
	}
	
	TIMEEND(timeVideo);
	LeaveHDCCrit(npMCI);
#ifdef DRAWTIMEHIST
	if (!fHurryUp && (wDrawn < NUMDRAWN)) {
	    dwDrawTime[wDrawn++] = npMCI->dwLastDrawTime;
	}
#endif
    }

    //
    // now is a good time to deal with other streams
    //
    if (npMCI->nOtherStreams > 0 || npMCI->nVideoStreams > 1) {

	if (npMCI->wTaskState != TASKPLAYING)
	    iFrame = npMCI->lCurrentFrame;

	TIMESTART(timeOther);
	DealWithOtherStreams(npMCI, iFrame);
	TIMEEND(timeOther);
    }

    DPF2(("PlayNONInterleaved...ENDING, npMCI=%8x, TaskState=%d\n", npMCI, npMCI->wTaskState));
    return TRUE;
}

/******************************************************************************
 *****************************************************************************/

BOOL NEAR PASCAL PlayAudioOnly(NPMCIGRAPHIC npMCI)
{
    DPF2(("PlayAudioOnly, npMCI=%8x\n",npMCI));
    npMCI->lFrameDrawn = npMCI->lCurrentFrame;

    if (npMCI->hWave) {
	TIMESTART(timeAudio);
	KeepPlayingAudio(npMCI);
	TIMEEND(timeAudio);
    }

    /* If we're at the right frame, and we haven't started yet,
    ** then begin play and start timing.
    */
    if ((npMCI->wTaskState != TASKPLAYING) &&
			!(npMCI->dwFlags & MCIAVI_SEEKING)) {
	if (!(npMCI->dwFlags & MCIAVI_PAUSE)) {
	    goto RestartPlay;
	} else
	    goto PauseNow;
    }

    /* If we've actually started timing:
    **  Check if we should send a signal.
    **  Check to see if we should return FALSE out of the loop.
    **  Wait until it's time for the next frame.
    */
    if (npMCI->wTaskState == TASKPLAYING) {

	npMCI->lCurrentFrame = WhatFrameIsItTimeFor(npMCI);

	if (npMCI->dwFlags & MCIAVI_PAUSE) {
PauseNow:
	    PauseAVI(npMCI);

	    BePaused(npMCI);

RestartPlay:
	    if (TimeToQuit(npMCI))
		return FALSE;

	    RestartAVI(npMCI);
	}

	if (npMCI->dwSignals)
	    CheckSignals(npMCI, npMCI->lCurrentFrame - npMCI->dwBufferedVideo);

	//
	// dont yield if updating
	//
	if (!(npMCI->dwFlags & MCIAVI_UPDATING)) {
	    TIMESTART(timeYield);
	    aviTaskCheckRequests(npMCI);
	    TIMEEND(timeYield);
	}

	if (TimeToQuit(npMCI))
	    return FALSE;
    }

    return TRUE;
}

/******************************************************************************
 *****************************************************************************/

#ifdef USENONINTFROMCD	
#pragma message("PlayNonIntFromCD needs fixed?")

BOOL NEAR PASCAL PlayNonIntFromCD(NPMCIGRAPHIC npMCI)
{
    BOOL fHurryUp = FALSE;
    LONG lNewFrame;
    DWORD ckid;
    UINT wStream;

    DPF2(("PlayNonIntFromCD, npMCI=%8x\n",npMCI));
AnotherChunk:
    /* If lCurrentFrame == lFrames, we're really at the end of
    ** the file, so there isn't another record to read.
    */
    if (npMCI->lCurrentFrame < npMCI->lFrames) {
	/* Read new record into buffer */

	TIMESTART(timeRead);
	ckid = ReadNextChunk(npMCI);
	TIMEEND(timeRead);

	if (ckid == 0) {
	    npMCI->dwTaskError = MCIERR_INVALID_FILE;
	    DPF(("Error reading frame #%ld\n", npMCI->lCurrentFrame));
	    return FALSE;
	}

	npMCI->lLastRead = npMCI->lCurrentFrame;
	wStream = StreamFromFOURCC(ckid);

	if (wStream == (UINT) npMCI->nVideoStream) {
	    if (TWOCCFromFOURCC(ckid) == cktypePALchange) {
		npMCI->lp += 2 * sizeof(DWORD);
		ProcessPaletteChange(npMCI, npMCI->dwThisRecordSize -
							    2 * sizeof(DWORD));
		npMCI->lLastPaletteChange = npMCI->lCurrentFrame;
		goto AnotherChunk;
	    }
	} else if (wStream == (UINT) npMCI->nAudioStream) {
	    TIMESTART(timeAudio);
	    if (npMCI->hWave)
		HandleAudioChunk(npMCI);
	    TIMEEND(timeAudio);
	    goto AnotherChunk;
	} else {
	    goto AnotherChunk;
	}
    }

    if (npMCI->wTaskState == TASKPLAYING) {
	lNewFrame = WhatFrameIsItTimeFor(npMCI);

	DPF3(("  Current = %ld, time for %ld.\n", npMCI->lCurrentFrame, lNewFrame));
	if (npMCI->lCurrentFrame < lNewFrame) {
	    fHurryUp = TRUE;
	}
    }

    /* If we're at the right frame, and we haven't started yet,
    ** then begin play and start timing.
    */
    if ((npMCI->lCurrentFrame > npMCI->lRealStart + (LONG) npMCI->dwBufferedVideo) &&
			(npMCI->lCurrentFrame < npMCI->lTo) &&
			(npMCI->wTaskState != TASKPLAYING)) {

	if (!(npMCI->dwFlags & MCIAVI_PAUSE)) {
	    goto RestartPlay;
	} else
	    goto PauseNow;
    }

    /* If we've actually started timing:
    **  Check if we should send a signal.
    **  Check to see if we should return FALSE out of the loop.
    **  Wait until it's time for the next frame.
    */
    if (npMCI->wTaskState == TASKPLAYING) {
	if (npMCI->dwFlags & MCIAVI_PAUSE) {
PauseNow:
	    PauseAVI(npMCI);

	    BePaused(npMCI);

RestartPlay:
	    if (TimeToQuit(npMCI))
		return FALSE;

	    RestartAVI(npMCI);
	}

	if (npMCI->dwSignals)
	    CheckSignals(npMCI, npMCI->lCurrentFrame - npMCI->dwBufferedVideo);

WaitMore:              ///////WAITING//////////
	lNewFrame = WhatFrameIsItTimeFor(npMCI);

	TIMESTART(timeYield);
	aviTaskCheckRequests(npMCI);
	TIMEEND(timeYield);

	if (lNewFrame < npMCI->lCurrentFrame) {
	    DPF3(("Waiting:  Current = %ld, time for %ld.\n", npMCI->lCurrentFrame, lNewFrame));
	    WaitTillNextFrame(npMCI);
	    if (TimeToQuit(npMCI))
		return FALSE;
	    else
		goto WaitMore;
	}
    }

    if (npMCI->lCurrentFrame >= npMCI->lVideoStart) {
	TIMESTART(timeVideo);
	EnterHDCCrit(npMCI);
	if (!DisplayVideoFrame(npMCI, fHurryUp)) {
	    npMCI->dwTaskError = MCIERR_AVI_DISPLAYERROR;
	    TIMEZERO(timeVideo);
	    LeaveHDCCrit(npMCI);
	    return FALSE;
	}
	TIMEEND(timeVideo);
	LeaveHDCCrit(npMCI);
    }

    return TRUE;
}
#endif


/******************************************************************************
 *****************************************************************************/

STATICFN INLINE LONG waveTime(NPMCIGRAPHIC npMCI, LONG lTime)
{
	if (gfUseGetPosition && npMCI->wABFull > 0) {
	    MMTIME	mmtime;
            LONG        lTimeInc;
	
            mmtime.wType = TIME_SAMPLES;
	
            waveOutGetPosition(npMCI->hWave, &mmtime, sizeof(mmtime));
            if (mmtime.wType == TIME_SAMPLES)
		lTimeInc = muldiv32(mmtime.u.sample,
				  1000L, npMCI->pWF->nSamplesPerSec);
	    else if (mmtime.wType == TIME_BYTES)
		lTimeInc = muldiv32(mmtime.u.cb,
		                  1000L, npMCI->pWF->nAvgBytesPerSec);
	    else
		goto ack2;

            //DPF0(("lTime: %3d,  LastDraw: %4d,  lTimeInc: %5d ",
            //       lTime,        npMCI->dwLastDrawTime,        lTimeInc));
            lTime = lTimeInc + npMCI->dwLastDrawTime;
	    // !!! this is too accurate: adjust by 100ms to match old SB...
	    lTime = max(0, lTime - giGetPositionAdjust);
	} else {
ack2:
	lTime += muldiv32(npMCI->dwAudioPlayed,
			   1000L, npMCI->pWF->nAvgBytesPerSec);
        }
	return(lTime);
}


INLINE LONG GetVideoTime(NPMCIGRAPHIC npMCI)
{
    //
    //  NOTE we must grab dwTimingStart *before* calling
    //  timeGetTime() because dwTimingStart is changed in the wave
    //  callback and we dont want to have time go backward.
    //
    LONG lTime = npMCI->dwTimingStart;

    lTime = (LONG)timeGetTime() - lTime
	+ npMCI->dwLastDrawTime
//      + npMCI->dwLastReadTime
	;

    Assert(lTime >= 0);

    if (npMCI->hWave) {

	if (npMCI->dwFlags & MCIAVI_WAVEPAUSED)	{
	    lTime = 0;
    }

	lTime = waveTime(npMCI, lTime);
    }
    return(lTime);
}

/* This function returns what frame we should be on. */
LONG NEAR PASCAL WhatFrameIsItTimeFor(NPMCIGRAPHIC npMCI)
{
    LONG        lTime;
    LONG        lFrame;

    // If timing is off, it's always just time to play the current frame.
    if (npMCI->dwPlayMicroSecPerFrame == 0)
	return npMCI->lCurrentFrame;

    //
    // if we have not started playing npMCI->dwTimingStart is bogus
    //
    Assert(npMCI->wTaskState == TASKPLAYING);
    AssertFrame(npMCI->lCurrentFrame - (LONG)npMCI->dwBufferedVideo);

    lTime = GetVideoTime(npMCI);

    /* Convert from MS to frames.... */
    //force round down - subtract half a frame
    lTime -= (npMCI->dwPlayMicroSecPerFrame / 2000);
    if (lTime < 0) {
	lTime = 0;
	lFrame = 0;
    } else {
	lFrame = muldiv32(lTime, 1000, npMCI->dwPlayMicroSecPerFrame);
    }

    if (npMCI->dwFlags & MCIAVI_REVERSE) {

	lFrame = npMCI->lFramePlayStart - lFrame;

	if (lFrame < npMCI->lTo)
	    lFrame = npMCI->lTo;
    }
    else {
	lFrame = lFrame + npMCI->lFramePlayStart + npMCI->dwBufferedVideo;

	if ((lFrame > npMCI->lTo) && (lFrame > npMCI->lCurrentFrame))
	    lFrame = npMCI->lTo;
    }




    if (lFrame > npMCI->lFrames + (LONG)npMCI->dwBufferedVideo || lFrame < 0) {
	DPF(("WhatFrameIsItTimeFor: bad frame %ld\n", lFrame));
	AssertSz(0, "bad frame in WhatFrameIsItTimeFor");
	lFrame = npMCI->lCurrentFrame;
    }

    return lFrame;
}

/******************************************************************************
 *****************************************************************************/

/* This function returns time to go until the target frame */
LONG NEAR PASCAL HowLongTill(NPMCIGRAPHIC npMCI)
{
    LONG        lTime;
    LONG        lTimeTarget;
    LONG        lFrameTarget = npMCI->lCurrentFrame;

    // If timing is off, it's always just time to play the current frame.
    if (npMCI->dwPlayMicroSecPerFrame == 0)
	return 0;

    //
    // if we have not started playing npMCI->dwTimingStart is bogus
    //
    Assert(npMCI->wTaskState == TASKPLAYING);

    // no longer valid because of last-frame-audio fix
    //AssertFrame(npMCI->lCurrentFrame - (LONG)npMCI->dwBufferedVideo);

    lTime = GetVideoTime(npMCI);

    if (npMCI->dwFlags & MCIAVI_REVERSE)
	lFrameTarget = npMCI->lFramePlayStart - lFrameTarget;
    else
	lFrameTarget -= npMCI->lFramePlayStart + npMCI->dwBufferedVideo;

    lTimeTarget = muldiv32(lFrameTarget, npMCI->dwPlayMicroSecPerFrame, 1000);

    //DPF0(("! >> %5d\n", lTimeTarget - lTime));
    return lTimeTarget - lTime;
}

/******************************************************************************
 *****************************************************************************/

static BOOL NEAR PASCAL PauseAVI(NPMCIGRAPHIC npMCI)
{
    DPF2(("PauseAVI\n"));
    if (npMCI->wTaskState == TASKPLAYING) {
	int stream;

	if (npMCI->hWave)
	    waveOutPause(npMCI->hWave);

	if (npMCI->hicDraw)
	    ICDrawStop(npMCI->hicDraw);

	for (stream = 0; stream < npMCI->streams; stream++) {
	    if (SI(stream)->hicDraw)
		ICDrawStop(SI(stream)->hicDraw);
	}

	npMCI->dwPauseTime = Now();
	npMCI->dwTotalMSec += npMCI->dwPauseTime - npMCI->dwMSecPlayStart;
    }

    if (npMCI->dwFlags & MCIAVI_WAITING) {
	// waiting for completion of a pause or cue request
	DPF3(("Releasing UI waiter\n"));
	SetEvent(npMCI->hEventAllDone);
	npMCI->dwFlags &= ~MCIAVI_WAITING;
    }

    // this flag is set to indicate that notify should be issued
    // when we reach a paused state (eg on Cue).
    if (npMCI->dwFlags & MCIAVI_CUEING) {
	/* If we're cueing, report that it was successful. */
	npMCI->dwFlags &= ~(MCIAVI_CUEING);
	GraphicDelayedNotify(npMCI, MCI_NOTIFY_SUCCESSFUL);
    }


    DPF2(("Pausing npMCI==%8x\n",npMCI));
    npMCI->wTaskState = TASKPAUSED;

    return TRUE;
}

/******************************************************************************
 *****************************************************************************/

static BOOL NEAR PASCAL BePaused(NPMCIGRAPHIC npMCI)
{
    DWORD dwObject;

    TIMEEND(timePlay);
    TIMESTART(timePaused);

    while (npMCI->dwFlags & MCIAVI_PAUSE) {

	if (npMCI->dwFlags & MCIAVI_STOP)
	    return FALSE;

	if (npMCI->dwFlags & MCIAVI_NEEDUPDATE) {
	    /* Since we're paused and we have nothing better
	    ** to do, update the screen.
	    */
	    DoStreamUpdate(npMCI, FALSE);
	}

	// block until told to do something else

	// need to handle send-messages to the ole windows -see mciaviTask()

    	do {
	    dwObject = MsgWaitForMultipleObjects(2, &npMCI->hEventSend,
			FALSE, INFINITE, QS_SENDMESSAGE);
	    if (dwObject == WAIT_OBJECT_0 + 2) {
		MSG msg;

		// just a single peekmessage with NOREMOVE will
		// process the inter-thread send and not affect the queue
		PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
	    }
	} while (dwObject == WAIT_OBJECT_0 + 2);

    	// find out what needed doing
	aviTaskCheckRequests(npMCI);

        if (npMCI->dwFlags & MCIAVI_WAITING) {
	    // waiting for completion of a pause or cue request
	    SetEvent(npMCI->hEventAllDone);
	    npMCI->dwFlags &= ~MCIAVI_WAITING;
	}

    }

    TIMEEND(timePaused);
    TIMESTART(timePlay);

    return TRUE;
}


/******************************************************************************
 *****************************************************************************/

static BOOL NEAR PASCAL RestartAVI(NPMCIGRAPHIC npMCI)
{
    int stream;

    Assert(npMCI->wTaskState != TASKPLAYING);

    /* Mark that play has actually begun */
    npMCI->wTaskState = TASKPLAYING;
    DPF(("Restart AVI, TaskState now TASKPLAYING, npMCI=%8x\n", npMCI));

#ifndef _WIN32
    TIMESTART(timeYield);
    aviTaskYield();
    aviTaskYield();
    aviTaskYield();
    TIMEEND(timeYield);
    DPF2(("Starting (done yielding)\n"));
#endif


    /* Reset clock and restart */

    if (npMCI->dwPauseTime == 0) {
	Assert(npMCI->dwTimingStart == 0);
    }

    npMCI->dwMSecPlayStart = Now(); // get the time we started playing

    //
    // if we were paused subtract off the time we spent paused from
    // the timing start
    //
    if (npMCI->dwPauseTime == 0)
	npMCI->dwTimingStart = npMCI->dwMSecPlayStart;
    else
	npMCI->dwTimingStart += (npMCI->dwMSecPlayStart - npMCI->dwPauseTime);

    if (npMCI->hWave)
	waveOutRestart(npMCI->hWave);

    if (npMCI->hicDraw)
	ICDrawStart(npMCI->hicDraw);

    for (stream = 0; stream < npMCI->streams; stream++) {
	if (SI(stream)->hicDraw)
	    ICDrawStart(SI(stream)->hicDraw);
    }

    DPF(("Returning from RestartAVI\n"));
    return TRUE;
}

/* This function sets up things that will be needed to play.
**
** Returns zero if no error, otherwise an MCI error code.
**
** Note: Even if this function returns an error, CleanUpPlay()
** will still be called, so we don't have to cleanup here.
*/
STATICFN DWORD NEAR PASCAL PrepareToPlay(NPMCIGRAPHIC npMCI)
{
    BOOL        fCDFile;
    BOOL        fNetFile;
    BOOL        fHardFile;

    Assert(npMCI->wTaskState != TASKPLAYING);

    //
    // lets choose the play back method:
    //
    //      playing reverse: (random access!)
    //          use MCIAVI_ALG_HARDDISK always (random access mode)
    //
    //      audio is preloaded: (will never happen?)
    //          on a CD-ROM   use MCIAVI_ALG_INTERLEAVED
    //          on a HARDDISK use MCIAVI_ALG_HARDDISK
    //          on a NET      use MCIAVI_ALG_HARDDISK
    //
    //      file is interleaved:
    //          on a CD-ROM   use MCIAVI_ALG_INTERLEAVED
    //          on a HARDDISK use MCIAVI_ALG_HARDDISK
    //          on a NET      use MCIAVI_ALG_HARDDISK
    //
    //      file is not interleaved:
    //          on a CD-ROM   use MCIAVI_ALG_CDROM
    //          on a HARDDISK use MCIAVI_ALG_HARDDISK
    //          on a NET      use MCIAVI_ALG_HARDDISK
    //

    fCDFile   = npMCI->uDriveType == DRIVE_CDROM;
    fNetFile  = npMCI->uDriveType == DRIVE_REMOTE;
    fHardFile = !fCDFile && !fNetFile;

    if (npMCI->nVideoStreams == 0 && npMCI->nOtherStreams == 0) {
	npMCI->wPlaybackAlg = MCIAVI_ALG_AUDIOONLY;
    } else if (npMCI->dwFlags & MCIAVI_REVERSE || npMCI->pf) {
        // avifile-handled files are always played as non-interleaved
        // though audio buffering might be different
	npMCI->wPlaybackAlg = MCIAVI_ALG_HARDDISK;
    }
    else if (npMCI->dwFlags & MCIAVI_NOTINTERLEAVED) {
#if 0
	if (fCDFile)
	    npMCI->wPlaybackAlg = MCIAVI_ALG_CDROM;
	else
#endif
	if (fNetFile)
	    npMCI->wPlaybackAlg = MCIAVI_ALG_HARDDISK;
	else
	    npMCI->wPlaybackAlg = MCIAVI_ALG_HARDDISK;
    }
    else {
	if (fCDFile)
	    npMCI->wPlaybackAlg = MCIAVI_ALG_INTERLEAVED;
#if 0
	else if (fNetFile)
	    npMCI->wPlaybackAlg = MCIAVI_ALG_HARDDISK;
	else
	    npMCI->wPlaybackAlg = MCIAVI_ALG_HARDDISK;
#else
	else if (fNetFile)
	    npMCI->wPlaybackAlg = MCIAVI_ALG_INTERLEAVED;
	else
	    npMCI->wPlaybackAlg = MCIAVI_ALG_INTERLEAVED;
#endif
    }

    // Interleaved playback doesn't work well at very low speeds!
    if ((npMCI->dwSpeedFactor < 100) &&
	(npMCI->wPlaybackAlg != MCIAVI_ALG_HARDDISK) &&
	(npMCI->wPlaybackAlg != MCIAVI_ALG_AUDIOONLY)) {
	DPF(("Was going to play interleaved, but speed < 10%% of normal...\n"));
	npMCI->wPlaybackAlg = MCIAVI_ALG_HARDDISK;
    }

#if 0
//
// sigh! we need to always have the index read now, so we do it in
// aviopen
//
    /* Be sure the index has been read, if we need it. */

    if (npMCI->hpFrameIndex == NULL)
	if (npMCI->wPlaybackAlg != MCIAVI_ALG_INTERLEAVED || npMCI->lFrom > 0)
	ReadIndex(npMCI);
#endif

#ifdef DEBUG
    switch (npMCI->wPlaybackAlg) {
	case MCIAVI_ALG_INTERLEAVED:
	    Assert(!(npMCI->dwFlags & MCIAVI_NOTINTERLEAVED));
	    DPF(("playing a interleaved file\n"));
	    break;

	case MCIAVI_ALG_CDROM:
	    Assert(npMCI->dwFlags & MCIAVI_NOTINTERLEAVED);
	    DPF(("playing a non interleaved file from CD-ROM\n"));
	    break;

	case MCIAVI_ALG_HARDDISK:
	    if (npMCI->dwFlags & MCIAVI_NOTINTERLEAVED)
		DPF(("random access play (non-interleaved file)\n"));
	    else
		DPF(("random access play (interleaved file)\n"));
	    break;

	case MCIAVI_ALG_AUDIOONLY:
	    Assert(npMCI->nAudioStreams);
	    DPF(("audio-only!\n"));
	    break;

	default:
	    Assert(0);
	    break;
    }
#endif

#if 0
    //
    // set a MMIO buffer if we are playing interleaved off a non cd-rom
    //
    if (npMCI->hmmio && fNetFile && npMCI->wPlaybackAlg == MCIAVI_ALG_INTERLEAVED) {

	#define BUFFER_SIZE (32l*1024)

	if (npMCI->lpMMIOBuffer == NULL) {

	    DPF(("Using %u byte MMIO buffer...\n", BUFFER_SIZE));

	    npMCI->lpMMIOBuffer = AllocMem(BUFFER_SIZE);

	    mmioSetBuffer(npMCI->hmmio, npMCI->lpMMIOBuffer, BUFFER_SIZE, 0);
	}

	//!!! should we do this for a seek?
	//!!! should we free this in CleanUpPlay?
    }
    else {
	if (npMCI->lpMMIOBuffer != NULL)
	    FreeMem(npMCI->lpMMIOBuffer);

	npMCI->lpMMIOBuffer = NULL;

	if (npMCI->hmmio)
	    mmioSetBuffer(npMCI->hmmio, NULL, 0, 0);
    }
#endif

    // !!!!
#ifdef DEBUG
    gwHurryTolerance = mmGetProfileInt(szIni, TEXT("Hurry"), 2);
    gwSkipTolerance = mmGetProfileInt(szIni, TEXT("Skip"), gwHurryTolerance * 2);
    gwMaxSkipEver = mmGetProfileInt(szIni, TEXT("MaxSkip"), max(60, gwSkipTolerance * 10));

#endif

    gfUseGetPosition = DEFAULTUSEGETPOSITION;	// !!! Detect WSS 1.0, 2.0?
    gfUseGetPosition = mmGetProfileInt(szIni, TEXT("GetPosition"), gfUseGetPosition);
    giGetPositionAdjust = mmGetProfileInt(szIni, TEXT("GetPositionAdjust"), 100);

#ifdef DEBUG
    if (gfUseGetPosition) {
	DPF2(("Using waveOutGetPosition adjusted by %dms instead of done bits...\n", giGetPositionAdjust));
    } else {
	DPF2(("NOT using waveOutGetPosition\n"));
    }
#endif

    Assert(npMCI->lTo <= npMCI->lFrames);
    Assert(npMCI->lFrom >= 0);

    /* Clear out variables, so we'll know what needs to be released. */
    /* Access to these variables should only ever be on the task thread */
    npMCI->hWave = NULL;
    npMCI->lpAudio = NULL;
    npMCI->lpBuffer = NULL;
    npMCI->dwBufferSize = 0L;
    npMCI->wABFull = 0;

    npMCI->dwSkippedFrames = 0L;
    npMCI->dwFramesSeekedPast = 0L;
    npMCI->dwAudioBreaks = 0L;

    npMCI->dwTotalMSec = 0;
    npMCI->dwLastDrawTime = 0;
    npMCI->dwLastReadTime = 0;
    npMCI->dwBufferedVideo = 0;
    npMCI->dwPauseTime = 0;
    npMCI->dwTimingStart = 0;

    /* Figure out how fast we're playing.... */
    if (npMCI->dwSpeedFactor)
	npMCI->dwPlayMicroSecPerFrame = muldiv32(npMCI->dwMicroSecPerFrame,
						 1000L,
						 npMCI->dwSpeedFactor);
    else
	npMCI->dwPlayMicroSecPerFrame = 0; // Special "play every frame" mode

    /* If we're already at the end, and we're going to repeat from the
    ** start of the file, just repeat now.
    */
    if ((npMCI->lFrom == npMCI->lTo) &&
		(npMCI->dwFlags & MCIAVI_REPEATING) &&
		(npMCI->lFrom != npMCI->lRepeatFrom)) {
	DPF(("Repeating from beginning before we've even started....\n"));
	npMCI->lFrom = npMCI->lRepeatFrom;
    }

    if (npMCI->lFrom == npMCI->lTo) {
	npMCI->dwFlags |= MCIAVI_SEEKING;
	npMCI->dwFlags &= ~(MCIAVI_REVERSE | MCIAVI_REPEATING);
    }

    if (npMCI->dwFlags & MCIAVI_SEEKING)
	goto PlayWithoutWave;

    if (npMCI->hicDraw) {
	ICGetBuffersWanted(npMCI->hicDraw, &npMCI->dwBufferedVideo);
    }
#ifdef DEBUG
    npMCI->dwBufferedVideo = mmGetProfileInt(szIni, TEXT("Buffer"), (int) npMCI->dwBufferedVideo);
#endif
    if (npMCI->dwFlags & MCIAVI_REVERSE) {
	npMCI->dwBufferedVideo = 0;
    }

    if (npMCI->dwBufferedVideo) {
	DPF(("Buffering %lu frames of video ahead....\n", npMCI->dwBufferedVideo));
    }

    //
    //  now initialize the audio stream
    //


    /* Open up our wave output device, if appropriate.
     * Appropriate means that there are audio streams,
     * 		that we are not muted,
     *		that the user has not turned sound off
     *		that we have not had the wave device stolen
     *		and that the wave stream is ok
     */
    if ((npMCI->nAudioStreams > 0)
      	&& (npMCI->dwFlags & MCIAVI_PLAYAUDIO)
      	&& !(npMCI->dwFlags & MCIAVI_LOSEAUDIO)
      	&& !(npMCI->dwOptionFlags & MCIAVIO_NOSOUND)
      	&& (npMCI->dwPlayMicroSecPerFrame != 0)) {

	npMCI->dwTaskError = SetUpAudio(npMCI, TRUE);

	if ((npMCI->dwTaskError == MCIERR_OUT_OF_MEMORY) &&
	    (npMCI->wPlaybackAlg != MCIAVI_ALG_AUDIOONLY)) {
	    DPF(("Not enough memory to play audio; continuing onward....\n"));
	    CleanUpAudio(npMCI);
	    npMCI->dwTaskError = 0;
	}

	if (npMCI->dwTaskError == MCIERR_WAVE_OUTPUTSINUSE) {
#ifdef STEALWAVE
	    //
	    //  we did not get a wave device, time to go steal one.
	    //
	    //  only do this if we got a real play command
	    //  from the user, and not a internal play command
	    //  (like when repeating or restarting)
	    //
	    //  MCIAVI_NEEDTOSHOW is set when the play command
	    //  came in through graphic.c (ie from the outside world)
	    //
	    if (npMCI->dwFlags & MCIAVI_NEEDTOSHOW) {
		if (StealWaveDevice(npMCI))
		    // Some other AVI task was prepared to release the
		    // wave device that they were holding.  This gives us
		    // a second chance to set up for playing audio.
		    npMCI->dwTaskError = SetUpAudio(npMCI, TRUE);
	    }
#endif // STEALWAVE

	    if (npMCI->dwTaskError == MCIERR_WAVE_OUTPUTSINUSE) {
		//
		// even though we did not steal the wave device we still
		// want it if it becomes available
		//
		npMCI->dwFlags |= MCIAVI_LOSTAUDIO;     // we want it
	    }
	}

	/*
	 * We will fail to play even without a wave device available
	 * in the following circumstances:
	 *
	 * 1: Playback algorithm is audio only
	 * 2: Wave failure is not ...OUTPUTSINUSE or ...OUTPUTSUNSUITABLE
	 *
	 * In all other cases we continue and play without wave
	 */

	if (npMCI->dwTaskError) {
	    if ( ((npMCI->dwTaskError != MCIERR_WAVE_OUTPUTSINUSE) &&
		    (npMCI->dwTaskError != MCIERR_WAVE_OUTPUTSUNSUITABLE))
               || (npMCI->wPlaybackAlg == MCIAVI_ALG_AUDIOONLY)
	       )
	    {
		// Must call CleanUpAudio(npMCI) to release buffers
		return npMCI->dwTaskError;
	    }

	    // Reset the error and continue
	    npMCI->dwTaskError = 0;
	}

    } else {
	DPF2(("Playing silently, nAudioStreams=%d, PlayAudio=%x\n",
		npMCI->nAudioStreams, npMCI->dwFlags & MCIAVI_PLAYAUDIO));
	// Was someone stealing our wave device?
        npMCI->dwFlags &= ~MCIAVI_LOSEAUDIO;  // OK - reset the flag
    }

PlayWithoutWave:
    return(PrepareToPlay2(npMCI));
}

INLINE STATICFN DWORD NEAR PASCAL PrepareToPlay2(NPMCIGRAPHIC npMCI)
{
    int         stream;
    UINT        w;
    DWORD       dwPosition;

    if (npMCI->dwFlags & MCIAVI_NEEDTOSHOW) {
	ShowStage(npMCI);
    }
    /* Get and prepare the DC we're going to be playing into */

    // must hold the critsec when getting dc to avoid
    // interaction with window thread calling DeviceRealize

    EnterHDCCrit(npMCI);

    if (npMCI->hdc == NULL) {
	npMCI->hdc = GetDC(npMCI->hwndPlayback);    // Shouldn't use cached DC!

	if (npMCI->hdc == NULL) {
	    LeaveHDCCrit(npMCI);
	    return MCIERR_DRIVER_INTERNAL;
	}

	npMCI->dwFlags |= MCIAVI_RELEASEDC;
    }

    if (npMCI->dwFlags & MCIAVI_SEEKING) {
	//
	// audio only
	//
	if (npMCI->nVideoStreams == 0 && npMCI->nOtherStreams == 0) {
	    npMCI->lCurrentFrame = npMCI->lFrom;
	    LeaveHDCCrit(npMCI);
	    return 0;
	}
    }

    /* Start up the external decompressor, if any */
    /* !!!We should check these for errors */

    if (!DrawBegin(npMCI, NULL)) {
	LeaveHDCCrit(npMCI);
	return npMCI->dwTaskError ? npMCI->dwTaskError : MCIERR_DRIVER_INTERNAL;
    }


    if (!(npMCI->dwFlags & MCIAVI_SEEKING)) {
	PrepareDC(npMCI);
    }
    // critsec just held around getting and preparing dc - look at
    // InternalRealize to see the function we are protecting against.
    LeaveHDCCrit(npMCI);

    /*
    **  what if selecting the palette causes palette changes? we should
    **  yield and let the palette changes happen.
    */

    if (npMCI->hicDraw && !(npMCI->dwFlags & MCIAVI_SEEKING) &&
		(npMCI->dwBufferedVideo > 0)) {
	ICDrawFlush(npMCI->hicDraw);
	npMCI->lFrameDrawn = (- (LONG) npMCI->wEarlyRecords) - 1;
    }

    if (npMCI->dwFlags & MCIAVI_FULLSCREEN) {
	/* Clear out key state flags:
	** We watch for escape, space, and the left button.
	** Unfortunately, we must look for LBUTTON and RBUTTON in case
	** the user has switched mouse buttons.  In that instance, the
	** UI might believe that the Left mouse button is physically the
	** right-hand one, but GetAsyncKeyState looks at the physical
	** left-hand mouse button.
	*/
	GetAsyncKeyState(VK_ESCAPE);
	GetAsyncKeyState(VK_SPACE);
	GetAsyncKeyState(VK_LBUTTON);
	GetAsyncKeyState(VK_RBUTTON);
    }

    /* Figure out where in the file to start playing from */
    CalculateTargetFrame(npMCI);

    // !!! ACK: We're starting from after where we planned to finish....
    if ((npMCI->dwFlags & MCIAVI_REVERSE) &&
	(npMCI->lCurrentFrame <= npMCI->lTo)) {
	npMCI->dwFlags |= MCIAVI_SEEKING;
    }

    // !!! This should be in CalcTarget
    if (npMCI->dwFlags & MCIAVI_SEEKING)
	npMCI->lTo = npMCI->lRealStart;

    //
    // start all the streams
    //
    for (stream = 0; stream < npMCI->streams; stream++) {

	STREAMINFO *psi = SI(stream);

#ifdef USEAVIFILE
	if (!(npMCI->dwFlags & MCIAVI_SEEKING)) {
	    if (SI(stream)->ps) {
		AVIStreamBeginStreaming(SI(stream)->ps,
			MovieToStream(SI(stream), npMCI->lFrom),
			MovieToStream(SI(stream), npMCI->lTo),
			npMCI->dwPlayMicroSecPerFrame); // !!!
	    }
	}
#endif

	//
	// NOTE DrawBegin() handled the default draw guy
	//
	if (psi->hicDraw && psi->hicDraw != npMCI->hicDraw) {

	    DWORD   dw;

	    dw = ICDrawBegin(psi->hicDraw,
		(npMCI->dwFlags & MCIAVI_FULLSCREEN) ?
			ICDRAW_FULLSCREEN : ICDRAW_HDC,

		npMCI->hpal,           // palette to draw with
		npMCI->hwndPlayback,   // window to draw to
		npMCI->hdc,            // HDC to draw to

		RCX(psi->rcDest),
		RCY(psi->rcDest),
		RCW(psi->rcDest),
		RCH(psi->rcDest),

		SI(stream)->lpFormat,

		RCX(psi->rcSource),
		RCY(psi->rcSource),
		RCW(psi->rcSource),
		RCH(psi->rcSource),

		muldiv32(psi->sh.dwRate, npMCI->dwSpeedFactor, 1000),
		psi->sh.dwScale);

	    if ((LONG)dw < 0) {
		// !!! Error checking?
		DPF(("Draw handler failed ICDrawBegin() (err = %ld)\n", dw));
	    }

	    //
	    // tell the draw handler the play range
	    //
	    ICDrawStartPlay(psi->hicDraw,psi->lPlayFrom, psi->lPlayTo);
	}
    }

    //
    // tell the draw handler the play range
    //
    if (npMCI->hicDraw) {
	ICDrawStartPlay(npMCI->hicDraw,npMCI->lRealStart,npMCI->lTo);
    }

    //
    //  seek to the right place in the file.
    //
    dwPosition = CalculatePosition(npMCI);

    if (dwPosition == 0) {
	return MCIERR_DRIVER_INTERNAL;
    }

#ifdef AVIREADMANY
    //
    //  see if we want to try to read two records at a shot, this
    //  should cut down the time spent in DOS doing reads.
    //
    //  we only can do this if we have a index, and the buffer
    //  sizes are "small enough"
    //
    //  if reading 2 buffers works good how about 3? 4?
    //
    //  this helps on CD's and Networks but makes things slower
    //  on KenO's hard disk, so dont do hard disks.
    //
    //  default is read many when coming from a Network, this is
    //  better than the old mmioSetBuffer() we used to do.
    //
    if (npMCI->uDriveType == DRIVE_REMOTE)
	npMCI->fReadMany = TRUE;
    else
	npMCI->fReadMany = FALSE;

    if (npMCI->wPlaybackAlg == MCIAVI_ALG_INTERLEAVED &&
	npMCI->dwSuggestedBufferSize <= 30*1024 &&
	mmGetProfileInt(szIni, TEXT("ReadMany"), npMCI->fReadMany) &&
	npMCI->hpFrameIndex) {

	npMCI->dwBufferSize = npMCI->dwSuggestedBufferSize * 2;
	npMCI->fReadMany = TRUE;
    }
    else {
	npMCI->fReadMany = FALSE;
    }

    if (npMCI->fReadMany) {
	DPF(("MCIAVI: reading two records at once (%ld bytes).\n", npMCI->dwBufferSize));
	npMCI->lLastRead = npMCI->lCurrentFrame - 2;
    }
#endif

    AllocateReadBuffer(npMCI);

    // look for palette changes between the last place we read and where
    // we're starting....
    ProcessPaletteChanges(npMCI, npMCI->lVideoStart);

    if (npMCI->hmmio) {
	/* Seek to the start of frame we're playing from */
	mmioSeek(npMCI->hmmio, dwPosition, SEEK_SET);
    }

#ifdef AVIREAD
    /* start the async read object if we are using interleaved
     * and therefore consecutive reads
     */
    if (npMCI->wPlaybackAlg == MCIAVI_ALG_INTERLEAVED) {

	/* start async reader  - allocates itself new buffers */
	npMCI->hAviRd = avird_startread(mciaviReadBuffer, (DWORD_PTR) npMCI,
					npMCI->dwNextRecordSize,
					npMCI->lCurrentFrame,
					min(npMCI->lTo+1, npMCI->lFrames));

	if (!npMCI->hAviRd) {
	    DPF(("async read failed - reading synchronously\n"));
	    ResizeReadBuffer(npMCI, npMCI->dwNextRecordSize);
	}

    } else {
	npMCI->hAviRd = NULL;
    }

    if (!npMCI->hAviRd)
#endif
    {
	if (!npMCI->lpBuffer) {
	    return MCIERR_OUT_OF_MEMORY;
	}
    }

    if (npMCI->hWave) {
	TIMESTART(timeAudio);
	if (npMCI->wPlaybackAlg == MCIAVI_ALG_HARDDISK ||
	    npMCI->wPlaybackAlg == MCIAVI_ALG_AUDIOONLY) {
	    /* Load audio into our buffers */
	    for (w = 0; w < npMCI->wABs; w++)
		KeepPlayingAudio(npMCI);
	} else if (npMCI->wPlaybackAlg == MCIAVI_ALG_CDROM) {
	    //!!!!
	    npMCI->wPlaybackAlg = MCIAVI_ALG_HARDDISK;
	}
	TIMEEND(timeAudio);
    }

    return 0L;          /* Success! */
}

/******************************************************************************
 *****************************************************************************/

void NEAR PASCAL CleanUpPlay(NPMCIGRAPHIC npMCI)
{
    int stream;

    if (npMCI->wTaskState == TASKPLAYING) {
	if (npMCI->hicDraw) {
	    ICDrawStop(npMCI->hicDraw);
	    ICDrawStopPlay(npMCI->hicDraw);
	}

	for (stream = 0; stream < npMCI->streams; stream++) {
	    if (SI(stream)->hicDraw) {
		ICDrawStop(SI(stream)->hicDraw);
		ICDrawStopPlay(SI(stream)->hicDraw);
	    }
	}

	if (npMCI->hWave) {
	    waveOutRestart(npMCI->hWave); // some wave devices need this
	    waveOutReset(npMCI->hWave);
	}
    } else if (npMCI->wTaskState == TASKCUEING) {
	if (npMCI->hicDraw) {
	    /* Kick the device in the head to make sure it draws when we seek. */
	    ICDrawRenderBuffer(npMCI->hicDraw);
	}
    }

    if (!(npMCI->dwFlags & MCIAVI_SEEKING) &&
		(npMCI->dwBufferedVideo > 0)) {
	ICDrawFlush(npMCI->hicDraw);
	npMCI->lFrameDrawn = (- (LONG) npMCI->wEarlyRecords) - 1;
    }

    /* end drawing this will leave fullscreen mode etc. */
    DrawEnd(npMCI);

    for (stream = 0; stream < npMCI->streams; stream++) {
	if (SI(stream)->hicDraw) {
	    LRESULT   dw;
	    dw = ICDrawEnd(SI(stream)->hicDraw);
	    // !!! Error checking?
	}
#ifdef USEAVIFILE
	if (!(npMCI->dwFlags & MCIAVI_SEEKING)) {
	    if (SI(stream)->ps) {
		AVIStreamEndStreaming(SI(stream)->ps);
	    }
	}
#endif
    }

    /* Clean up and close our wave output device. */
    if (npMCI->hWave) {

	Assert(!(npMCI->dwFlags & MCIAVI_LOSTAUDIO));
	// We should never hold the
	// wave device AND have MCIAVI_LOSTAUDIO turned on.

	CleanUpAudio(npMCI);

#ifdef STEALWAVE
	//
	// if we are not being forced to give up the audio try to
	// give it to someone.	Unless we are repeating.  In which
	// case someone might steal it from us, but we do not want
	// to waste time looking for another user.  We have it; we
	// will keep it until forced to release it.
	//
	if (!(npMCI->dwFlags & MCIAVI_NEEDTOSHOW) &&
	    !(npMCI->dwFlags & MCIAVI_REPEATING)  &&
	    !(npMCI->dwFlags & MCIAVI_UPDATING))
	    GiveWaveDevice(npMCI);
	else {
	    DPF2(("Not giving the wave device away, flags=%x\n",npMCI->dwFlags));
	}
#endif
    } else {
	//
	//  done playing, we dont want a wave device any more
	//  LATER: do we really want to turn this flag off if we are
	//  repeating?  Today it is benign as the flag will be turned
	//  back on again when the video restarts.  It will then try and
	//  open the audio, fail because it is in use, and turn on LOSTAUDIO.
	//  It would be more efficient to rely on the wave device being
	//  returned to us and not try to reopen it.
	//
	npMCI->dwFlags &= ~MCIAVI_LOSTAUDIO;
    }

    /* Release the DC we played into. */

    // worker thread must hold critsec round all access to hdc
    // (can be used by DeviceRealize on winproc thread)
    EnterHDCCrit(npMCI);

    if (npMCI->hdc) {
	//
	// we MUST call this otherwise our palette will stay selected
	// as the foreground palette and it may get deleted (ie by
	// DrawDibBegin) while still the foreground palette and GDI
	// get's real pissed about this.
	//
	UnprepareDC(npMCI);
#if 0
	if (npMCI->dwFlags & MCIAVI_ANIMATEPALETTE)
	    RealizePalette(npMCI->hdc);
#endif
	if (npMCI->dwFlags & MCIAVI_RELEASEDC) {
	    ReleaseDC(npMCI->hwndPlayback, npMCI->hdc);
	    HDCCritCheckIn(npMCI);
	    npMCI->hdc = NULL;
	    npMCI->dwFlags &= ~MCIAVI_RELEASEDC;
	}
    }
    LeaveHDCCrit(npMCI);

#ifdef AVIREAD
    /* shut down async reader */
    if (npMCI->hAviRd) {
	avird_endread(npMCI->hAviRd);
	npMCI->hAviRd = NULL;
    } else
#endif
    {
	/* we weren't using async reader - so release the buffer we
	 * allocated
	 */
	ReleaseReadBuffer(npMCI);
    }

}

/******************************************************************************
 *****************************************************************************/

// !!! Should this take a "how many frames to check for" parameter,
// in case we need to check for signals on several frames at once?

void NEAR PASCAL CheckSignals(NPMCIGRAPHIC npMCI, LONG lFrame)
{
    LONG        lTemp;

    lTemp = npMCI->signal.dwPeriod == 0 ? lFrame :
			(((lFrame - npMCI->signal.dwPosition) %
					    npMCI->signal.dwPeriod) +
				npMCI->signal.dwPosition);

    if ((DWORD) lTemp == npMCI->signal.dwPosition) {
	/* Send the signal in the right time format */
	SEND_DGVSIGNAL(npMCI->dwSignalFlags,
			    npMCI->signal.dwCallback,
			    0,
			    (HANDLE) npMCI->wDevID,
			    npMCI->signal.dwUserParm,
			    ConvertFromFrames(npMCI, lFrame));
	// !!! Needs to use time format at time of signal command!
    }
}

/******************************************************************************
 *****************************************************************************/

BOOL NEAR PASCAL WaitTillNextFrame(NPMCIGRAPHIC npMCI)
{
#ifdef _WIN32
    LONG WaitForFrame;
#endif
    LONG  lMaxWait;
#ifdef DEBUG
    int iWait = 0;
    StatusBar(npMCI,2,1,4,iWait); // we should not wait more than 4 times...
#endif

    /* Here we wait for a while if we're ahead
     *   of schedule (so that we can yield nicely instead of blocking
     *   in the driver, for instance, and also so that we'll work off
     *   faster devices.)
     */

    /* Always yield at least once in a while  (every 8 frames ~ 1/2 sec)*/
    if ((npMCI->lCurrentFrame % YIELDEVERY) == 0) {
	TIMESTART(timeYield);
	aviTaskCheckRequests(npMCI);
	TIMEEND(timeYield);
    }

    if (npMCI->dwFlags & MCIAVI_WAVEPAUSED)
	return TRUE;

    if (TimeToQuit(npMCI))
	return FALSE;

    Assert(npMCI->wTaskState == TASKPLAYING);

    // with the change to play the last frame of audio, these two asserts
    // are no longer valid. We will wait until it's time for frame lTo+1
    // and stop then (before attempting to read or draw it).
    //AssertFrame(npMCI->lCurrentFrame - (LONG)npMCI->dwBufferedVideo);
    //Assert(npMCI->lCurrentFrame <= npMCI->lTo);

    Assert(!(npMCI->dwFlags & MCIAVI_REVERSE));

    /* The maximum wait time is 95% of the correct frame rate, or 100ms
     * (to cope with a very slow frame rate)
     */
    lMaxWait = min(100, muldiv32(npMCI->dwMicroSecPerFrame,
			950L,
			(npMCI->dwSpeedFactor == 0 ?
				1000 : npMCI->dwSpeedFactor)));

    if (HowLongTill(npMCI) > 0) {

	while ((WaitForFrame=HowLongTill(npMCI)) > 0) {
		     ///////WAITING//////////
	    StatusBar(npMCI,2,1,4,++iWait);

	    // use sleep regardless of accuracy as polling hurts on NT
	    if (npMCI->msPeriodResolution > 0) {

		// NOTE:  There are no fudge factors in here.  This code
		// needs to be tuned to allow for the overhead of calculating
		// the wait time, for the overhead of the timer, etc.

		// Don't wait for more than one frame time at a time....
		if (WaitForFrame > lMaxWait) {
		    WaitForFrame = lMaxWait;
		}

		DPF2(("Sleeping for %d milliseconds\n", WaitForFrame));
		TIMESTART(timeWait);
		Sleep(WaitForFrame);
		TIMEEND(timeWait);
	    } else {
		Sleep(0);
	    }

	    // check if anything interesting has happened
	    TIMESTART(timeYield);
	    aviTaskCheckRequests(npMCI);
	    TIMEEND(timeYield);
	    if (TimeToQuit(npMCI)) {
		return FALSE;
	    }
	}
    } else {
	// force some cpu idle time at least every 1/2 second, to ensure
	// that other processes do get some time (eg for 16-bit hook procs).
	/* Always yield at least once in a while  (every 8 frames ~ 1/2 sec)*/
	if ((npMCI->lCurrentFrame % YIELDEVERY) == 0) {

	    TIMESTART(timeWait);
	    Sleep(1);
	    TIMEEND(timeWait);

	    TIMESTART(timeYield);
	    aviTaskCheckRequests(npMCI);
	    TIMEEND(timeYield);

	    if (TimeToQuit(npMCI)) {
		return FALSE;
	    }
	}
    }

    return TRUE;
}

/* Idea: this should go from the current frame to the frame
** we actually have to be at to start playing from.
**
** If fPlaying is set, that means we're really going to play.
**
** When this finishes:
**      lAudioStart is set to the first frame with meaningful audio info
**      lVideoStart is the first frame with meaningful video info
**      lRealStart is the first frame that's 'real', namely
**              the original value of lCurrentFrame.  If the
**              SEEK EXACT flag is not set, then lRealStart may
**              actually not be what lCurrentFrame was, indicating
**              that play may start from somewhere else.
**      lCurrentFrame gets set to the first frame we have to read from.
**
** !!! This also needs to look for "palette key frames" or something.
*/
BOOL NEAR PASCAL CalculateTargetFrame(NPMCIGRAPHIC npMCI)
{
    int         i;
    LONG        lVideoPlace;
    BOOL        fForceBeginning = FALSE;
    int		lMovieStart=0xffffffff;	    // Max UINT (or -1 when signed)
    int		lStreamStart;

    npMCI->lCurrentFrame = npMCI->lFrom;
    npMCI->lRealStart = npMCI->lFrom;

    //
    //  walk all streams and figure out where to start
    //
    for (i=0; i<npMCI->streams; i++) {

	STREAMINFO *psi = SI(i);

	if (!(psi->dwFlags & STREAM_ENABLED))
	    continue;

	if (psi->dwFlags & STREAM_ERROR)
	    continue;

	if (psi->dwFlags & STREAM_AUDIO)
	    continue;

	//
	// map from movie time to stream time.
	//
	psi->lPlayFrom = MovieToStream(psi, npMCI->lFrom);
	psi->lPlayTo   = MovieToStream(psi, npMCI->lTo);

	psi->dwFlags &= ~STREAM_ACTIVE;

	//
	// is this stream part of play?
	//
	if (psi->lPlayFrom < psi->lStart && psi->lPlayTo < psi->lStart)
	    continue;

	if (psi->lPlayFrom >= psi->lEnd && psi->lPlayTo >= psi->lEnd)
	    continue;

	psi->dwFlags |= STREAM_ACTIVE;

	psi->lPlayFrom  = BOUND(psi->lPlayFrom,psi->lStart,psi->lEnd);
	psi->lPlayTo    = BOUND(psi->lPlayTo,  psi->lStart,psi->lEnd);
	psi->lPlayStart = FindPrevKeyFrame(npMCI,psi,psi->lPlayFrom);

	//
	// if the main frame is invalid invalidate the stream too.
	//
	if (npMCI->lFrameDrawn <= (-(LONG)npMCI->wEarlyRecords)) {
	    psi->lFrameDrawn = -4242;
	}

	//
	//  if we have a drawn frame use it!
	//
	if ((psi->lFrameDrawn  > psi->lPlayStart) &&
	    (psi->lFrameDrawn <= psi->lPlayFrom))
	    psi->lPlayStart = npMCI->lFrameDrawn + 1;

	lStreamStart = StreamToMovie(psi, (DWORD)psi->lPlayStart);
	if ((DWORD)lMovieStart > (DWORD)lStreamStart) {
	    (DWORD)lMovieStart = (DWORD)lStreamStart;
	}

	//
	//  if seek exactly is off start play at the key frame
	//
	if (!(npMCI->dwOptionFlags & MCIAVIO_SEEKEXACT)) {

	    if (psi->lPlayFrom == psi->lPlayTo)
		 psi->lPlayTo = psi->lPlayStart;

	    psi->lPlayFrom = psi->lPlayStart;

	    //!!! is this right for reverse?

	    if (StreamToMovie(psi, psi->lPlayFrom) < npMCI->lFrom) {
//              npMCI->lRealStart = StreamToMovie(psi, psi->lPlayFrom);
//              npMCI->lFrom = npMCI->lRealStart;
	    }
	}

//      if (StreamToMovie(psi, psi->lPlayStart) < npMCI->lCurrentFrame)
//          npMCI->lCurrentFrame = StreamToMovie(psi, psi->lPlayStart);

	DPF(("CalculateTargetFrame:  Stream #%d: from:%ld, to:%ld, start:%ld\n", i, psi->lPlayFrom, psi->lPlayTo, psi->lPlayStart));
    }

    //
    // we are done with now special case the video and audio streams.
    // note: if lMovieStart has NOT been altered above then it will be -1
    //
    if (npMCI->lFrom < lMovieStart) {
	npMCI->lFrom = lMovieStart;
    }

    /* If we're starting from the beginning, don't force the index
    ** to be read, but use it if we've already read it.
    */
    if (npMCI->lFrom == 0 && npMCI->hpFrameIndex == NULL)
	goto ForceBeginning;

    if (!npMCI->pbiFormat) {
	npMCI->lVideoStart = npMCI->lFrom;

	if (npMCI->lVideoStart >= npMCI->lFrames)
	    npMCI->lVideoStart = npMCI->lFrames - 1;

	lVideoPlace = npMCI->lVideoStart;
    } else
    if (npMCI->dwFlags & MCIAVI_HASINDEX) {

	if (npMCI->hpFrameIndex == NULL)
	    goto ForceBeginning;

	//
	// get nearest key frame
	//
	npMCI->lVideoStart = FramePrevKey(npMCI->lFrom);

	if (npMCI->lVideoStart) {
	    lVideoPlace = npMCI->lVideoStart;
	} else {
	    /* Didn't find a key frame--retreat to the beginning. */
	    npMCI->lVideoStart = -(LONG)npMCI->wEarlyVideo;
	    lVideoPlace = 0;
	}

	if ((npMCI->lFrameDrawn > npMCI->lVideoStart) &&
		(npMCI->lFrameDrawn <= npMCI->lFrom)) {
	    npMCI->lVideoStart = npMCI->lFrameDrawn + 1;
	    if (npMCI->lVideoStart >= npMCI->lFrames)
		npMCI->lVideoStart = npMCI->lFrames - 1;
	    lVideoPlace = npMCI->lFrameDrawn;
	}
    } else {
	/* Always go back to frame 0 */
ForceBeginning:
	npMCI->lVideoStart = - (LONG) npMCI->wEarlyVideo;
	lVideoPlace = 0;
	fForceBeginning = TRUE;
    }

    if (!(npMCI->dwOptionFlags & MCIAVIO_SEEKEXACT)) {
	npMCI->lRealStart = lVideoPlace;
    }

    if (npMCI->hWave) {
	npMCI->lAudioStart = npMCI->lRealStart - (LONG) npMCI->wEarlyAudio;
    }

    if (npMCI->hWave && (npMCI->wPlaybackAlg == MCIAVI_ALG_INTERLEAVED))
	npMCI->lCurrentFrame = min(npMCI->lAudioStart, npMCI->lVideoStart);
    else
	npMCI->lCurrentFrame = npMCI->lVideoStart;

    if (npMCI->lRealStart < npMCI->lCurrentFrame)
	npMCI->lCurrentFrame = npMCI->lRealStart;

    if (fForceBeginning) {
	if (npMCI->wPlaybackAlg == MCIAVI_ALG_INTERLEAVED)
	    npMCI->lCurrentFrame = - (LONG) npMCI->wEarlyRecords;
	else
	    npMCI->lCurrentFrame = - (LONG) npMCI->wEarlyVideo;
    }

    if (npMCI->hWave) {
	LONG l;

	/* Figure out what sample of audio we should be starting at */

	//
	//  convert frame number to block
	//
	npMCI->dwAudioPos = MovieToStream(npMCI->psiAudio, npMCI->lRealStart);

	//
	// now convert block to byte position
	//
	npMCI->dwAudioPos = npMCI->dwAudioPos * npMCI->pWF->nBlockAlign;

	Assert(npMCI->dwAudioPos % npMCI->pWF->nBlockAlign == 0);

	if (npMCI->dwAudioPos > npMCI->dwAudioLength)
	    npMCI->dwAudioPos = npMCI->dwAudioLength;

	npMCI->dwAudioPlayed = 0L;

	//
	//  convert the audio start back to a frame number.
	//  and posibly readjust the video start time.
	//
	l = npMCI->lRealStart - StreamToMovie(npMCI->psiAudio,
		npMCI->dwAudioPos/npMCI->pWF->nBlockAlign);

	if (l < 0)
	    DPF(("Audio will be ahead of the video by %ld frames\n", -l));
	else if (l > 0)
	    DPF(("Audio will be behind the video by %ld frames\n", l));
    }

#ifdef DEBUG
    Assert(npMCI->lCurrentFrame < npMCI->lFrames);
    if (npMCI->wPlaybackAlg == MCIAVI_ALG_INTERLEAVED) {
	Assert(npMCI->lCurrentFrame >= - (LONG) npMCI->wEarlyRecords);
    }
    if (npMCI->hWave) {
	Assert(npMCI->lAudioStart <= npMCI->lFrames);
    }
    Assert(npMCI->lVideoStart < npMCI->lFrames);
#endif

    return TRUE;
}


/******************************************************************************
 *****************************************************************************/

void ReturnToOriginalPalette(NPMCIGRAPHIC npMCI)
{
    if (npMCI->bih.biClrUsed) {
	hmemcpy(npMCI->argb, npMCI->argbOriginal,
		npMCI->bih.biClrUsed * sizeof(RGBQUAD));

	if (npMCI->pbiFormat->biBitCount == 8) {
	    hmemcpy((LPBYTE) npMCI->pbiFormat + npMCI->pbiFormat->biSize,
		    (LPBYTE) npMCI->argb,
		    sizeof(RGBQUAD) * npMCI->pbiFormat->biClrUsed);
	}

	npMCI->dwFlags |= MCIAVI_PALCHANGED;
	npMCI->lLastPaletteChange = 0;
    }
}


/* Returns the position in the file where the frame referenced
** by lCurrentFrame is.
**
**  input   npMCI->lCurrentFrame
**
**  output  npMCI->dwNextRecordSize set correctly
**          npMCI->lLastRead set correctly
**          returns offset to read from
**
** If there's an error, returns zero.
*/
DWORD NEAR PASCAL CalculatePosition(NPMCIGRAPHIC npMCI)
{
    DWORD       dwPosition;

    AssertFrame(npMCI->lCurrentFrame);

    if (npMCI->pf || npMCI->nVideoStreams == 0)
	return 1;

    if (npMCI->lCurrentFrame + npMCI->wEarlyRecords == 0) {
ForceBeginning:
	npMCI->lCurrentFrame = - (LONG)npMCI->wEarlyRecords;
//!!!BeforeBeginning:
	dwPosition = npMCI->dwFirstRecordPosition;
	npMCI->dwNextRecordSize = npMCI->dwFirstRecordSize;
	npMCI->dwNextRecordType = npMCI->dwFirstRecordType;
    } else if (npMCI->dwFlags & MCIAVI_HASINDEX) {
	if (npMCI->hpFrameIndex == NULL)
	    goto ForceBeginning;

	dwPosition = FrameOffset(npMCI->lCurrentFrame);
	npMCI->dwNextRecordSize = FrameLength(npMCI->lCurrentFrame) + 8;
	npMCI->dwNextRecordType = 0;
    } else {
	goto ForceBeginning;
    }

    npMCI->lLastRead = npMCI->lCurrentFrame - 1;

    DPF3(("Frame %ld: Seeking to position %lX\n", npMCI->lCurrentFrame, dwPosition));

    DPF3(("CalculatePosition: next record = %lu bytes.\n", npMCI->dwNextRecordSize));

    mmioSeek(npMCI->hmmio, dwPosition, SEEK_SET);

    return dwPosition;
}

/***************************************************************************
 *
 ***************************************************************************/

BOOL NEAR PASCAL ReadIndexChunk(NPMCIGRAPHIC npMCI, LONG iIndex)
{
    Assert(iIndex >= 0 && iIndex < (LONG)npMCI->macIndex);

    return ReadBuffer(npMCI, (LONG)IndexOffset(iIndex), (LONG)IndexLength(iIndex) + 8);
}

/***************************************************************************
 *
 * @doc INTERNAL MCIAVI
 *
 * @api void | DealWithOtherStreams | does what is says
 *
 *  this function is called inside of the non-interlaved play loop.
 *  it's mission is to catch the "other" streams up to the current time.
 *
 *  right now all we do is go to key frames, we should fix this
 *
 * @parm NPMCIGRAPHIC | npMCI | pointer to instance data block.
 *
 ***************************************************************************/

STATICFN INLINE void DealWithOtherStreams(NPMCIGRAPHIC npMCI, LONG lFrame)
{
    int i;
    STREAMINFO *psi;
    LONG lPos;
    LONG err;

    for (i=0; i<npMCI->streams; i++) {

	// If this is the active audio or video stream, then ignore it
	if ((i == npMCI->nVideoStream)
	    || (i == npMCI->nAudioStream))
	    continue;

	psi = SI(i);

	if (!(psi->dwFlags & STREAM_ENABLED))
	    continue;

	if (psi->hicDraw == NULL)
	    continue;

	lPos = MovieToStream(psi, lFrame);

	if (lPos < psi->lPlayStart || lPos > psi->lPlayTo) {
	    DPF2(("OtherStream(%d): out of range lPos = %ld [%ld, %ld]\n", i, lPos, psi->lPlayStart, psi->lPlayTo));
	    continue;
	}

	//
	// we have the right thing drawn now
	//
	// !!!we should not always go to a key frame.
	//
	//
	if (psi->lFrameDrawn >= psi->lLastKey &&
	    psi->lFrameDrawn <= lPos &&
	    lPos < psi->lNextKey) {
	    DPF2(("OtherStream(%d) lPos = %ld, lFrameDrawn=%ld, NextKey=%ld\n", i, lPos, psi->lFrameDrawn, psi->lNextKey));
	    continue;
	}

	FindKeyFrame(npMCI, psi, lPos);

	DPF2(("OtherStream(%d): pos=%ld (prev key=%ld, next key=%ld)\n",i,lPos,psi->lLastKey,psi->lNextKey));

	lPos = psi->lLastKey;

	if (!StreamRead(npMCI, psi, lPos)) {
	    DPF2(("StreamRead failed\n"));
	    continue;
	} else {
	    DPF2(("Read stream, ThisRecordSize==%d, biSizeImage==%d\n",
		npMCI->dwThisRecordSize, 0));
	}

	//
	//  now draw the data.
	//
	err = (LONG)ICDraw(psi->hicDraw, 0L, psi->lpFormat,
		npMCI->lpBuffer,npMCI->dwThisRecordSize,
		psi->lLastKey - psi->lPlayFrom);

	if (err >= 0) {
	    psi->dwFlags &= ~STREAM_NEEDUPDATE;
	    psi->lFrameDrawn = lPos;
	}
	else {
	    DPF2(("Draw failed!\n"));
	}
    }
}

/***************************************************************************
 *
 * FindKeyFrame
 *
 *  given a stream position, find the previous and next key frame
 *  cacheing the last ones found to make it sort of fast.
 *
 ***************************************************************************/

void NEAR PASCAL FindKeyFrame(NPMCIGRAPHIC npMCI, STREAMINFO *psi, LONG lPos)
{
    if (psi == NULL)
	psi = npMCI->psiVideo;

    Assert(psi);
//  AssertPos(psi, lPos);

    //
    //  if we are in the current key range return it.
    //
    if (psi->lLastKey <= lPos && lPos < psi->lNextKey)
	return;

    if (lPos < psi->lStart || lPos >= psi->lEnd)
	return;

    //
    //  otherwise query from the stream
    //
#ifdef USEAVIFILE
    if (psi->ps) {
	if (lPos == psi->lNextKey)
	    psi->lLastKey = psi->lNextKey;
	else
	    psi->lLastKey = AVIStreamFindSample(psi->ps, lPos, FIND_KEY|FIND_PREV);

	psi->lNextKey = AVIStreamFindSample(psi->ps, lPos+1, FIND_KEY|FIND_NEXT);

	if (psi->lLastKey == -1)
	    ; // psi->lLastKey = psi->lStart;

	if (psi->lNextKey == -1)
	    psi->lNextKey = psi->lEnd+1;
    }
#endif
    else if (psi->dwFlags & STREAM_VIDEO) {
	//
	// for a video stream either read our index or assume no key frames.
	//
	if (npMCI->hpFrameIndex && psi == npMCI->psiVideo) {
	    psi->lLastKey = FramePrevKey(lPos);
	    psi->lNextKey = FrameNextKey(lPos);
	}
	else {
	    psi->lLastKey = psi->lStart;
	    psi->lNextKey = psi->lEnd+1;
	}
    }
    else {
	//
	// for a non-video stream assume all key frames
	//
	psi->lLastKey = lPos;
	psi->lNextKey = lPos+1;
    }

    return;
}

/***************************************************************************
 ***************************************************************************/

LONG NEAR PASCAL FindPrevKeyFrame(NPMCIGRAPHIC npMCI, STREAMINFO *psi, LONG lPos)
{
    FindKeyFrame(npMCI, psi, lPos);
    return psi->lLastKey;
}

/***************************************************************************
 ***************************************************************************/

LONG NEAR PASCAL FindNextKeyFrame(NPMCIGRAPHIC npMCI, STREAMINFO *psi, LONG lPos)
{
    FindKeyFrame(npMCI, psi, lPos);
    return psi->lNextKey;
}

/***************************************************************************
 ***************************************************************************/

BOOL NEAR PASCAL ProcessPaletteChanges(NPMCIGRAPHIC npMCI, LONG lFrame)
{
    LONG        iPalette;
    LONG        iFrame;
    STREAMINFO *psi;

    if (!(npMCI->dwFlags & MCIAVI_ANIMATEPALETTE))
	return TRUE;

    psi = npMCI->psiVideo;
    Assert(psi);

#ifdef USEAVIFILE
    if (psi->ps) {

	DWORD       dw;
	//
	//  we are in the palette range nothing to do.
	//
	if (npMCI->lLastPaletteChange <= lFrame &&
	    npMCI->lNextPaletteChange >  lFrame) {

	    return TRUE;
	}

	dw = psi->cbFormat;

	//!!! should be psi->lpFormat
	if (AVIStreamReadFormat(psi->ps, lFrame, npMCI->pbiFormat, &dw) != 0) {
	    DOUT("Unable to read Stream format\n");
	    return FALSE;
	}

	npMCI->lLastPaletteChange = lFrame;
	npMCI->lNextPaletteChange = AVIStreamFindSample(psi->ps, lFrame+1, FIND_NEXT|FIND_FORMAT);

	if (npMCI->lNextPaletteChange == -1)
	    npMCI->lNextPaletteChange = npMCI->lFrames+2;

	npMCI->dwFlags |= MCIAVI_PALCHANGED;
	return TRUE;
    }
#endif

    DPF2(("Looking for palette changes at %ld, last=%ld\n", lFrame, npMCI->lLastPaletteChange));

    if (lFrame < npMCI->lLastPaletteChange) {
	ReturnToOriginalPalette(npMCI);
    }

    /* If there's no index, assume we're starting from the beginning
    ** and thus we don't have to worry about palette changes.
    */
    if (npMCI->hpFrameIndex == NULL)
	return TRUE;

    //
    // walk from the last palette change to the current frame, and apply any
    // palette changes we find.
    //
    for (iFrame = npMCI->lLastPaletteChange,
	 iPalette = FramePalette(iFrame);
	 iFrame <= lFrame;
	 iFrame++) {

	if (iPalette != FramePalette(iFrame)) {

	    iPalette = FramePalette(iFrame);

	    /* We've found a palette change we need to deal with */
	    DPF2(("Processing palette change at frame %ld.\n", iFrame));

	    Assert(iPalette >= 0 && iPalette < (LONG)npMCI->macIndex);

	    if (!ReadIndexChunk(npMCI, iPalette))
		    return FALSE;

	    npMCI->lp += 2 * sizeof(DWORD);
	    ProcessPaletteChange(npMCI, IndexLength(iPalette));

	    npMCI->lLastPaletteChange = iFrame;
	}
    }

    return TRUE;
}

BOOL NEAR PASCAL ReadRecord(NPMCIGRAPHIC npMCI)
{
    DWORD UNALIGNED FAR * pdw;

    AssertFrame(npMCI->lCurrentFrame);

#ifdef AVIREADMANY
    if (npMCI->fReadMany) {
	//
	//  either read two records or return the one we read last time.
	//
	Assert(npMCI->hpFrameIndex);
	Assert(npMCI->lCurrentFrame - npMCI->lLastRead > 0);
	Assert(npMCI->lCurrentFrame - npMCI->lLastRead <= 2);

	if (npMCI->lLastRead == npMCI->lCurrentFrame-1) {
	    //
	    //  return the second half of the buffer.
	    //
	    npMCI->lp = npMCI->lpBuffer + (UINT)npMCI->dwThisRecordSize;
	    npMCI->dwThisRecordSize = npMCI->dwNextRecordSize;
	}
	else {
	    //
	    //  read in two buffers, and return the first one
	    //
	    //  figure out how much to read by looking at the index
	    //  we dont have to worry about the last frame because
	    //  the dummy index entry on the end is 0 in length.
	    //
	    npMCI->dwThisRecordSize = FrameLength(npMCI->lCurrentFrame) + 8;
	    npMCI->dwNextRecordSize = FrameLength(npMCI->lCurrentFrame+1) + 8;

	    if (!ReadBuffer(npMCI, -1,
		npMCI->dwThisRecordSize + npMCI->dwNextRecordSize))
		return FALSE;

	    npMCI->lLastRead = npMCI->lCurrentFrame;
	    npMCI->lp = npMCI->lpBuffer;
	    npMCI->dwThisRecordSize -= npMCI->dwNextRecordSize;
	}

#ifdef DEBUG
	pdw = (LPDWORD)(npMCI->lp + npMCI->dwThisRecordSize - 3 * sizeof(DWORD));

	if (npMCI->lCurrentFrame < npMCI->lFrames - 1) {
	    Assert(pdw[0] == FOURCC_LIST);
	    Assert(pdw[2] == listtypeAVIRECORD);
	}
#endif
	return TRUE;
    }
    else
#endif

#ifdef AVIREAD
    if (npMCI->hAviRd) {
	/* async reader is going - get the next buffer from him */
	npMCI->lpBuffer = avird_getnextbuffer(npMCI->hAviRd, &dwThisBuffer);
	npMCI->dwThisRecordSize = npMCI->dwNextRecordSize;

	if ((dwThisBuffer == 0) || (npMCI->lpBuffer == NULL)) {
	    npMCI->dwTaskError = MCIERR_FILE_READ;
	    return FALSE;
	}

    } else
#endif
    {
	if (!ReadBuffer(npMCI, -1, (LONG)npMCI->dwNextRecordSize))
	    return FALSE;
    }

    pdw = (DWORD UNALIGNED FAR *)(npMCI->lp + npMCI->dwThisRecordSize - 3 * sizeof(DWORD));

    npMCI->dwNextRecordType = pdw[0];
    npMCI->dwNextRecordSize = pdw[1] + 2 * sizeof(DWORD);

#ifdef DEBUG
    if (npMCI->lCurrentFrame < npMCI->lFrames - 1) {
	Assert(pdw[0] == FOURCC_LIST);
	Assert(pdw[2] == listtypeAVIRECORD);
    }
#endif

    return TRUE;
}

STATICFN INLINE DWORD NEAR PASCAL ReadNextChunk(NPMCIGRAPHIC npMCI)
{
    LPDWORD pdw;
    DWORD dw;

ReadAgain:
    dw = npMCI->dwNextRecordType;

    if (!ReadBuffer(npMCI, -1, (LONG)npMCI->dwNextRecordSize))
	return 0;

    pdw = (LPDWORD)(npMCI->lp + npMCI->dwNextRecordSize - 2 * sizeof(DWORD));

    if (dw == FOURCC_LIST)
	pdw--;

    npMCI->dwNextRecordType = pdw[0];
    npMCI->dwNextRecordSize = pdw[1] + 2 * sizeof(DWORD);

    if (dw == ckidAVIPADDING)
	goto ReadAgain;

    return dw;
}

STATICFN INLINE BOOL NEAR PASCAL StreamRead(NPMCIGRAPHIC npMCI, STREAMINFO *psi, LONG lPos)
{
    LONG  lSize;

    Assert(psi);
#ifdef USEAVIFILE
    Assert(psi->ps);
#endif

    //
    // if we are before the start or after the end, read nothing.
    //
    if (lPos < psi->lStart || lPos >= psi->lEnd) {
	lSize = 0;
	goto done;
    }

#ifdef USEAVIFILE
    if (AVIStreamRead(psi->ps, lPos, 1,
	(LPSTR)npMCI->lpBuffer,npMCI->dwBufferSize,&lSize, NULL) != 0) {

	//
	// the read failed try incressing the buffer size
	//
	AVIStreamRead(psi->ps, lPos, 1, NULL, 0, &lSize, NULL);

	if (lSize > (LONG) (npMCI->dwBufferSize)) {

	    DPF2(("ReadStream: Enlarging buffer....\n"));

	    if (!ResizeReadBuffer(npMCI, lSize)) {
		DPF(("Failed to increase buffer size!\n"));
		npMCI->dwTaskError = MCIERR_OUT_OF_MEMORY;
		return FALSE;
	    }
	}

	if (AVIStreamRead(psi->ps, lPos, 1,
		(LPSTR)npMCI->lpBuffer,npMCI->dwBufferSize,&lSize,NULL) != 0) {
	    npMCI->dwTaskError = MCIERR_FILE_READ;
	    return FALSE;
	}
    }
#endif

done:
    npMCI->lp = npMCI->lpBuffer;
    npMCI->dwThisRecordSize = lSize;
    return TRUE;
}

BOOL NEAR PASCAL ReadNextVideoFrame(NPMCIGRAPHIC npMCI, STREAMINFO *psi)
{
    MMCKINFO    ck;

    if (psi == NULL)
	psi = npMCI->psiVideo;

    Assert(psi);
    AssertFrame(npMCI->lCurrentFrame);

#ifdef USEAVIFILE
    if (psi->ps) {
	LONG        lSize;
	LONG        lPos;

	//
	// map from movie time into this stream.
	//
	lPos = MovieToStream(psi, npMCI->lCurrentFrame);

	//
	// if we are before the start or after the end, read nothing.
	//
	if (lPos <  (LONG)psi->sh.dwStart ||
	    lPos >= (LONG)psi->sh.dwStart+(LONG)psi->sh.dwLength) {
	    lSize = 0;
	    goto done;
	}

	//
	// if this frame has a new palette then deal with it
	//
	if (npMCI->dwFlags & MCIAVI_ANIMATEPALETTE) {
	    ProcessPaletteChanges(npMCI, lPos);
	}

	if (AVIStreamRead(psi->ps, lPos, 1,
		      (LPSTR) npMCI->lpBuffer + 2 * sizeof(DWORD),
		      npMCI->dwBufferSize - 2 * sizeof(DWORD),
		      &lSize, NULL) != 0) {
	    //
	    // the read failed try incressing the buffer size
	    //
	    AVIStreamRead(psi->ps, lPos, 1, NULL, 0, &lSize, NULL);

	    if (lSize > (LONG) (npMCI->dwBufferSize - 2 * sizeof(DWORD))) {

		DPF2(("ReadNextVideoFrame: Enlarging buffer....\n"));

		if (!ResizeReadBuffer(npMCI, lSize + 2 * sizeof(DWORD))) {
		    DPF(("Failed to increase buffer size!\n"));
		    npMCI->dwTaskError = MCIERR_OUT_OF_MEMORY;
		    return FALSE;
		}
	    }

	    if (AVIStreamRead(psi->ps, lPos, 1,
		      (LPSTR) npMCI->lpBuffer + 2 * sizeof(DWORD),
		      npMCI->dwBufferSize - 2 * sizeof(DWORD),
		      &lSize, NULL) != 0) {
		return FALSE;
	    }
	}

done:
	((DWORD FAR *)npMCI->lpBuffer)[0] = MAKEAVICKID(cktypeDIBbits,
							npMCI->nVideoStream);
	((DWORD FAR *)npMCI->lpBuffer)[1] = lSize;

	npMCI->lp = npMCI->lpBuffer;

	npMCI->dwThisRecordSize = lSize + 2 * sizeof(DWORD);

	return TRUE;
    }
#endif
    //
    // if we are not reading the "next" frame then figure out where it is.
    //
    if (npMCI->lLastRead != npMCI->lCurrentFrame-1)
	CalculatePosition(npMCI);

    //
    // dwNextRecordSize is the size to read
    // and we are seeked to the right place.
    //
    if (npMCI->hpFrameIndex) {

	//
	// if this frame has a new palette then deal with it
	//
	if (npMCI->dwFlags & MCIAVI_ANIMATEPALETTE) {
	     if (FramePalette(npMCI->lCurrentFrame) !=
		 FramePalette(npMCI->lLastPaletteChange))

		ProcessPaletteChanges(npMCI, npMCI->lCurrentFrame);
	}

	//
	// now just go read the frame from the disk.
	//
	// if interleaved add 8 to skip the 'REC'!!!!
	//
	return ReadBuffer(npMCI,
	    (LONG)FrameOffset(npMCI->lCurrentFrame),
	    (LONG)FrameLength(npMCI->lCurrentFrame) + 8);
    } else {
ReadAgainNoIndex:
	for (;;) {
	    if (mmioDescend(npMCI->hmmio, &ck, NULL, 0) != 0) {
		DPF(("Unable to descend!\n"));
		npMCI->dwTaskError = MCIERR_INVALID_FILE;
		return FALSE;
	    }

	    /* If it's a list, stay descended in it. */
	    /* Hack: we never ascend.            */
	    if (ck.ckid == FOURCC_LIST)
		continue;

#ifdef ALPHAFILES
	    /* Skip wave bytes, since they've been preloaded. */
	    if (npMCI->dwFlags & MCIAVI_USINGALPHAFORMAT) {
		if ((ck.ckid != ckidAVIPADDING) &&
			(ck.ckid != ckidOLDPADDING) &&
			(ck.ckid != ckidWAVEbytes))
		    break;
	    } else
#endif
	    {
		if (StreamFromFOURCC(ck.ckid) == (WORD)npMCI->nVideoStream)
		    break;
	    }

	    mmioAscend(npMCI->hmmio, &ck, 0);
	}

	if (ck.cksize + 2 * sizeof(DWORD) > npMCI->dwBufferSize) {
	    if (!ResizeReadBuffer(npMCI, ck.cksize + 2 * sizeof(DWORD))) {
		DPF(("ReadNextVideoFrame: Failed to increase buffer size!\n"));
		npMCI->dwTaskError = MCIERR_OUT_OF_MEMORY;
		return FALSE;
	    }
	}

	*((LPMMCKINFO) npMCI->lpBuffer) = ck;
	if (mmioRead(npMCI->hmmio, npMCI->lpBuffer + 2 * sizeof(DWORD),
			    ck.cksize) != (LONG) ck.cksize) {
	    npMCI->dwTaskError = MCIERR_INVALID_FILE;
	    return FALSE;
	}

	mmioAscend(npMCI->hmmio, &ck, 0);
	npMCI->lp = npMCI->lpBuffer;

	npMCI->dwThisRecordSize = ck.cksize + 2 * sizeof(DWORD);

	if (TWOCCFromFOURCC(ck.ckid) == cktypePALchange) {
	    npMCI->lp += 2 * sizeof(DWORD);
	    ProcessPaletteChange(npMCI, ck.cksize);
	    npMCI->lLastPaletteChange = npMCI->lCurrentFrame;
	    goto ReadAgainNoIndex;
	}
    }

    return TRUE;
}

BOOL NEAR PASCAL TimeToQuit(NPMCIGRAPHIC npMCI)
{
    /* If we're using DisplayDib, give the user a chance to break. */

    if ((npMCI->dwFlags & MCIAVI_FULLSCREEN) &&
		!(npMCI->dwFlags & MCIAVI_NOBREAK) &&
		(npMCI->wTaskState == TASKPLAYING)) {

	// Check each of the "stop" events
	if (1 & GetAsyncKeyState(VK_LBUTTON)) {
	    npMCI->dwFlags |= MCIAVI_STOP;
	}
	else if
	      (1 & GetAsyncKeyState(VK_RBUTTON)) {
	    npMCI->dwFlags |= MCIAVI_STOP;
	}
	else if
	      (1 & GetAsyncKeyState(VK_ESCAPE)) {
	    npMCI->dwFlags |= MCIAVI_STOP;
	}
	else if
	      (1 & GetAsyncKeyState(VK_SPACE)) {
	    npMCI->dwFlags |= MCIAVI_STOP;
	}
    }

    // this will be set by aviTaskCheckRequests if there is
    // a request that we need to stop to handle
    if (npMCI->dwFlags & MCIAVI_STOP)
	return TRUE;

#ifdef _WIN32
    if (TestNTFlags(npMCI, NTF_RETRYAUDIO)) {
	ResetNTFlags(npMCI, NTF_RETRYAUDIO);
	/*
	 * IF we get access to the wave device, set the flag that
	 * will cause the play to be restarted, then abort this play.
	 */
	SetUpAudio(npMCI, TRUE);
	if (npMCI->hWave) {
	     SetNTFlags(npMCI, NTF_RESTARTFORAUDIO);
	     return(TRUE);
	}
    }
#endif
    return FALSE;
}

/***************************************************************************
 *
 * @doc INTERNAL MCIAVI
 *
 * @api BOOL | AllocateReadBuffer | Allocates buffers needed to read
 *      disk information in to.  The amount of memory to allocate
 *      is in npMCI->dwBufferSize.
 *
 * @parm NPMCIGRAPHIC | npMCI | pointer to instance data block.
 *
 * @rdesc TRUE means OK, otherwise unable to allocate memory.
 *
 ***************************************************************************/
BOOL NEAR PASCAL AllocateReadBuffer(NPMCIGRAPHIC npMCI)
{
    if (npMCI->dwBufferSize == 0)
	npMCI->dwBufferSize = npMCI->dwSuggestedBufferSize;

    if (npMCI->dwBufferSize <= 8 * sizeof(DWORD))
    {
	if (npMCI->dwBytesPerSec > 0 &&
	    npMCI->dwBytesPerSec < 600l*1024 &&
	    npMCI->dwMicroSecPerFrame > 0)

	    npMCI->dwBufferSize = (muldiv32(npMCI->dwBytesPerSec,
			npMCI->dwMicroSecPerFrame,1000000L) + 2047) & ~2047;
	else
	    npMCI->dwBufferSize = 10*1024;

	npMCI->dwSuggestedBufferSize = npMCI->dwBufferSize;
    }

    DPF3(("allocating %lu byte read buffer.\n", npMCI->dwBufferSize));

    if (npMCI->lpBuffer) {
	DPF(("Already have buffer in AllocateReadBuffer!\n"));
	return ResizeReadBuffer(npMCI, npMCI->dwBufferSize);
    }

    //!!! we dont need DOS memory when we have a MMIO buffer!
    //!!! we dont need DOS memory when we are using AVIFile???

    if (npMCI->lpMMIOBuffer != NULL || npMCI->pf)
	npMCI->lpBuffer = GlobalAllocPtr(GHND | GMEM_SHARE, npMCI->dwBufferSize);
    else
	npMCI->lpBuffer = AllocMem(npMCI->dwBufferSize);

    return npMCI->lpBuffer != NULL;
}

/***************************************************************************
 *
 * @doc INTERNAL MCIAVI
 *
 * @api BOOL | ResizeReadBuffer | Enlarges buffer needed to read
 *      disk information in to.
 *
 * @parm NPMCIGRAPHIC | npMCI | pointer to instance data block.
 *
 * @parm DWORD | dwNewSize | new amount of memory to allocate
 *
 * @rdesc TRUE means OK, otherwise unable to allocate memory.
 *
 ***************************************************************************/
BOOL NEAR PASCAL ResizeReadBuffer(NPMCIGRAPHIC npMCI, DWORD dwNewSize)
{
    if (dwNewSize > npMCI->dwSuggestedBufferSize && !npMCI->fReadMany)
	npMCI->dwSuggestedBufferSize = dwNewSize;

    if (dwNewSize <= npMCI->dwBufferSize)
	return TRUE;

    DPF(("Increasing buffer size to %ld (was %ld).\n", dwNewSize, npMCI->dwBufferSize));

    ReleaseReadBuffer(npMCI);
    npMCI->dwBufferSize = dwNewSize;
    return AllocateReadBuffer(npMCI);
}

/***************************************************************************
 *
 * @doc INTERNAL MCIAVI
 *
 * @api void | ReleaseReadBuffer | Releases read buffer.
 *
 * @parm NPMCIGRAPHIC | npMCI | pointer to instance data block.
 *
 ***************************************************************************/
void NEAR PASCAL ReleaseReadBuffer(NPMCIGRAPHIC npMCI)
{
    if (npMCI->lpBuffer) {
	DPF3(("Releasing read buffer.\n"));

	GlobalFreePtr(npMCI->lpBuffer);

	npMCI->lpBuffer = NULL;
	npMCI->dwBufferSize = 0L;
	npMCI->fReadMany = FALSE;
    }
}

/***************************************************************************
 *
 * @doc INTERNAL MCIAVI
 *
 * @api BOOL | ReadBuffer
 *
 ***************************************************************************/

BOOL NEAR PASCAL ReadBuffer(NPMCIGRAPHIC npMCI, LONG off, LONG len)
{
    npMCI->lp = npMCI->lpBuffer;
    npMCI->dwThisRecordSize = len;

    if (len == 0) {
	((DWORD FAR *)npMCI->lpBuffer)[0] = 0; //!!!lpIndexEntry->ckid;
	((DWORD FAR *)npMCI->lpBuffer)[1] = 0;
	npMCI->dwThisRecordSize = 8;
	return TRUE;
    }

    if (len > (LONG)npMCI->dwBufferSize) {
	if (!ResizeReadBuffer(npMCI, len)) {
	    DPF(("Failed to increase buffer size!\n"));
	    npMCI->dwTaskError = MCIERR_OUT_OF_MEMORY;
	    return FALSE;
	}

	npMCI->lp = npMCI->lpBuffer;
    }

    if (off >= 0)
	DPF2(("ReadBuffer %ld bytes at %ld\n", len, off));
    else
	DPF2(("ReadBuffer %ld bytes\n", len));

    if (off >= 0)
	mmioSeek(npMCI->hmmio, off, SEEK_SET);

#ifdef INTERVAL_TIMES
{
    LONG    lReadStart = -(LONG)timeGetTime();
#endif

    if (mmioRead(npMCI->hmmio, npMCI->lp, len) != len) {
	npMCI->dwTaskError = MCIERR_FILE_READ;
	return FALSE;
    }

#ifdef INTERVAL_TIMES
    lReadStart += timeGetTime();
    npMCI->nReads++;
    npMCI->msReadTotal += lReadStart;
    if (lReadStart > npMCI->msReadMax) {
	npMCI->msReadMax = lReadStart;
    }
}
#endif
    return TRUE;
}

/***************************************************************************
 *
 * @doc INTERNAL MCIAVI
 *
 * @api LPVOID | AllocMem | try to allocate DOS memory (< 1Mb)
 *
 * @parm DWORD | dw | size in bytes
 *
 ***************************************************************************/

#ifndef _WIN32
static LPVOID AllocMem(DWORD dw)
{
    /* Memory allocation internal routines */

    extern DWORD FAR PASCAL GlobalDosAlloc(DWORD);

    LPVOID p;

    if (p = (LPVOID)MAKELONG(0, LOWORD(GlobalDosAlloc(dw))))
	{
	DPF(("Got %ld bytes DOS memory\n", dw));
	GlobalReAlloc((HANDLE)HIWORD((DWORD)p), 0, GMEM_MODIFY|GMEM_SHARE);
	return p;
	}
    else
    {
	DPF(("unable to get %ld bytes of DOS memory\n", dw));
	return GlobalLock(GlobalAlloc(GMEM_MOVEABLE|GMEM_SHARE, dw));
    }
}
#endif


