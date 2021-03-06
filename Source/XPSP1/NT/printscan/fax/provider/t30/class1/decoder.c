/***************************************************************************
 Name     :     DECODER.C
 Comment  :     HDLC decoding routines
 Functions:     (see Prototypes just below)

        Copyright (c) Microsoft Corp. 1991 1992 1993

 Revision Log
 Date     Name  Description
 -------- ----- ---------------------------------------------------------

***************************************************************************/


#include "prep.h"


#include "decoder.h"
#include "debug.h"


#include "glbproto.h"



#define FILEID                  FILEID_DECODER

extern  BYTE CODEBASED DecLen[8][256], CODEBASED DecOut[8][256], CODEBASED DecWidth[8][256];





USHORT SWECMEXP HDLC_Decode(PThrdGlbl pTG, LPBYTE lpbSrc, USHORT cbSrc, LPBYTE lpbDst, USHORT far* lpcbDst, LPDECODESTATE lpState)
{

   BYTE FlagAbort = lpState->flagabort != FALSE;

#ifdef PORTABLE_CODE
   BYTE StateLen = lpState->len;
   BYTE StateWidth = lpState->dec_width;
   USHORT SrcSize;
   BYTE InByte;
   LPBYTE CurrentSrc = lpbSrc;
   LPBYTE CurrentDst = lpbDst;
   USHORT ProcessWord = (USHORT) lpState->carry;      //carry-in at the start of each loop iteration
   BYTE TmpLen;
   USHORT ExitLoop = FALSE;

#ifdef CLASS1_TEST_HOOKS
   BYTE TstlpbDst[512];
   DECODESTATE TstState;
   LPDECODESTATE TstlpState = &TstState;
   USHORT TstcbSrc;
   USHORT TstcbDst;
   USHORT *TstlpcbDst = &TstcbDst;
   USHORT i;
   USHORT BadFlag = FALSE;
   CurrentDst = TstlpbDst;
#endif //CLASS1_TEST_HOOKS

        // Processes input bytes until flag, abort or idle or cbSrc
        // Returns number of bytes written to lpbDst in *lpcbDst
        // Return value is number of bytes read
        // Returns reason in lpState->flagabort


   for (SrcSize = cbSrc;((SrcSize > 0) && !ExitLoop); SrcSize--) {
      InByte = *CurrentSrc++;
      ProcessWord |=   (((USHORT) DecOut[StateLen][InByte]) << StateWidth);  //Processed Inbyte Leftshift width or carryin
      StateWidth += DecWidth[StateLen][InByte];
      TmpLen = DecLen[StateLen][InByte];
      if ( TmpLen < 8)  {                 //Normal Case
         StateLen = TmpLen;
         if (StateWidth >= 8) {
            *CurrentDst++ = LOBYTE(ProcessWord);
            ProcessWord >>= 8;
            FlagAbort = 0;
            StateWidth -= 8;
         }
      }
      else {                                     //GotFlagAbort
         ProcessWord = (USHORT) DecOut[StateLen][InByte];
         StateWidth = DecWidth[StateLen][InByte];
         StateLen = TmpLen & 0x3f;
         switch (TmpLen & 0xc0) {
         case 0x40:
            if (FlagAbort != ABORT) {
               FlagAbort = ABORT;
               ExitLoop = TRUE;
            }
            break;
         case 0x80:
            if (StateWidth > 8) {
               DebugBreak();
               // ERRMSG((SZMOD "<<ERROR>> AssertFailed in decoder.c\r\n"));
                    BG_CHK(FALSE);
            }
            else {
               if (FlagAbort != FLAG) {
                  FlagAbort = FLAG;
                  ExitLoop = TRUE;
               }
            }
            break;
         default:
                // only time 0x40 and 0x80 are together is when inlen=7 and inbyte=0x7E.
            DebugBreak();
            // ERRMSG((SZMOD "<<ERROR>> AssertFailed in decoder.c\r\n"));
                 BG_CHK(FALSE);
            break;
         }
      }
   }

#ifdef CLASS1_TEST_HOOKS
   TstlpState->carry = LOBYTE(ProcessWord);
   TstlpState->dec_width = StateWidth;
   TstlpState->len = StateLen;
   TstlpState->flagabort = FlagAbort;
   *TstlpcbDst = CurrentDst - TstlpbDst;
   TstcbSrc = CurrentSrc - lpbSrc;
#else
   lpState->carry = LOBYTE(ProcessWord);
   lpState->dec_width = StateWidth;
   lpState->len = StateLen;
   lpState->flagabort = FlagAbort;
   *lpcbDst = (USHORT)(CurrentDst - lpbDst);
   cbSrc = (USHORT)(CurrentSrc - lpbSrc);
#endif
#endif //PORTABLE_CODE


#ifdef  CLASS1_TEST_HOOKS
for (i = 0; (i < *lpcbDst) && !BadFlag; i++) {
   if (lpbDst[i] != TstlpbDst[i]) {
      BadFlag = TRUE;
   }
}

if (BadFlag || (cbSrc != TstcbSrc) || (*TstlpcbDst != *lpcbDst) || (lpState->carry != TstlpState->carry) || (lpState->len != TstlpState->len) ||
   (lpState->dec_width != TstlpState->dec_width) || lpState->flagabort != TstlpState->flagabort ){
   ERRORMSG(("HDLC_Decode: Assembly Decode != C Decode\n"));
   DebugBreak();
}
#endif //CLASS1_TEST_HOOKS

        return cbSrc;

#if !defined(PORTABLE_CODE) || defined(CLASS1_TEST_HOOKS)
AssertFail:
#ifndef WIN32
        _asm    pop             ds
#endif
        _asm int 3
        // ERRMSG((SZMOD "<<ERROR>> AssertFailed in decoder.c\r\n"));
        BG_CHK(FALSE);
#endif // !defined(PORTABLE_CODE) || defined(CLASS1_TEST_HOOKS)
}


BYTE CODEBASED DecLen[8][256] = {
// Input Len = 0
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 64,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 129,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 7, 71 },
// Input Len = 1
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 64,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 64,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 129,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 65,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 130,
 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 7, 71 },
// Input Len = 2
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 64,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 64,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 64,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 64,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 129, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 65,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 129, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 65,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 130, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 66,
 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 131, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 7, 71 },
// Input Len = 3
{0, 0, 0, 0, 0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 0, 64,
 0, 0, 0, 0, 0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 0, 64,
 0, 0, 0, 0, 0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 0, 64,
 0, 0, 0, 0, 0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 128, 64,
 1, 1, 1, 1, 1, 1, 1, 129, 1, 1, 1, 1, 1, 1, 1, 65, 1, 1, 1, 1, 1, 1, 1, 129, 1, 1, 1, 1, 1, 1, 1, 65,
 1, 1, 1, 1, 1, 1, 1, 129, 1, 1, 1, 1, 1, 1, 1, 65, 1, 1, 1, 1, 1, 1, 1, 129, 1, 1, 1, 1, 1, 1, 1, 65,
 2, 2, 2, 2, 2, 2, 2, 130, 2, 2, 2, 2, 2, 2, 2, 66, 2, 2, 2, 2, 2, 2, 2, 130, 2, 2, 2, 2, 2, 2, 2, 66,
 3, 3, 3, 3, 3, 3, 3, 131, 3, 3, 3, 3, 3, 3, 3, 67, 4, 4, 4, 4, 4, 4, 4, 132, 5, 5, 5, 5, 6, 6, 7, 71 },
// Input Len = 4
{0, 0, 0, 128, 0, 0, 0, 64, 0, 0, 0, 128, 0, 0, 0, 64, 0, 0, 0, 128, 0, 0, 0, 64, 0, 0, 0, 128, 0, 0, 0, 64,
 0, 0, 0, 128, 0, 0, 0, 64, 0, 0, 0, 128, 0, 0, 0, 64, 0, 0, 0, 128, 0, 0, 0, 64, 0, 0, 0, 128, 0, 0, 0, 64,
 0, 0, 0, 128, 0, 0, 0, 64, 0, 0, 0, 128, 0, 0, 0, 64, 0, 0, 0, 128, 0, 0, 0, 64, 0, 0, 0, 128, 0, 0, 0, 64,
 0, 0, 0, 128, 0, 0, 0, 64, 0, 0, 0, 128, 0, 0, 0, 64, 0, 0, 0, 128, 0, 0, 0, 64, 0, 0, 0, 128, 0, 0, 128, 64,
 1, 1, 1, 129, 1, 1, 1, 65, 1, 1, 1, 129, 1, 1, 1, 65, 1, 1, 1, 129, 1, 1, 1, 65, 1, 1, 1, 129, 1, 1, 1, 65,
 1, 1, 1, 129, 1, 1, 1, 65, 1, 1, 1, 129, 1, 1, 1, 65, 1, 1, 1, 129, 1, 1, 1, 65, 1, 1, 1, 129, 1, 1, 1, 65,
 2, 2, 2, 130, 2, 2, 2, 66, 2, 2, 2, 130, 2, 2, 2, 66, 2, 2, 2, 130, 2, 2, 2, 66, 2, 2, 2, 130, 2, 2, 2, 66,
 3, 3, 3, 131, 3, 3, 3, 67, 3, 3, 3, 131, 3, 3, 3, 67, 4, 4, 4, 132, 4, 4, 4, 68, 5, 5, 5, 133, 6, 6, 7, 71 },
// Input Len = 5
{0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 0, 64,
 0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 0, 64,
 0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 0, 64,
 0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 0, 64, 0, 128, 128, 64,
 1, 129, 1, 65, 1, 129, 1, 65, 1, 129, 1, 65, 1, 129, 1, 65, 1, 129, 1, 65, 1, 129, 1, 65, 1, 129, 1, 65, 1, 129, 1, 65,
 1, 129, 1, 65, 1, 129, 1, 65, 1, 129, 1, 65, 1, 129, 1, 65, 1, 129, 1, 65, 1, 129, 1, 65, 1, 129, 1, 65, 1, 129, 1, 65,
 2, 130, 2, 66, 2, 130, 2, 66, 2, 130, 2, 66, 2, 130, 2, 66, 2, 130, 2, 66, 2, 130, 2, 66, 2, 130, 2, 66, 2, 130, 2, 66,
 3, 131, 3, 67, 3, 131, 3, 67, 3, 131, 3, 67, 3, 131, 3, 67, 4, 132, 4, 68, 4, 132, 4, 68, 5, 133, 5, 69, 6, 134, 7, 71 },
// Input Len = 6
{128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64,
 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64,
 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64,
 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64, 128, 64,
 129, 65, 129, 65, 129, 65, 129, 65, 129, 65, 129, 65, 129, 65, 129, 65, 129, 65, 129, 65, 129, 65, 129, 65, 129, 65, 129, 65, 129, 65, 129, 65,
 129, 65, 129, 65, 129, 65, 129, 65, 129, 65, 129, 65, 129, 65, 129, 65, 129, 65, 129, 65, 129, 65, 129, 65, 129, 65, 129, 65, 129, 65, 129, 65,
 130, 66, 130, 66, 130, 66, 130, 66, 130, 66, 130, 66, 130, 66, 130, 66, 130, 66, 130, 66, 130, 66, 130, 66, 130, 66, 130, 66, 130, 66, 130, 66,
 131, 67, 131, 67, 131, 67, 131, 67, 131, 67, 131, 67, 131, 67, 131, 67, 132, 68, 132, 68, 132, 68, 132, 68, 133, 69, 133, 69, 134, 70, 135, 71 },
// Input Len = 7
{64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65,
 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66,
 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 68, 68, 68, 68, 68, 68, 68, 68, 69, 69, 69, 69, 70, 70, 71, 71 }
}; // Len


BYTE CODEBASED DecOut[8][256] = {
// Input Len = 0
{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x00,
 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x3f,
 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x00, 0x00,
 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x5f,
 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0x7e, 0x01,
 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0x7f,
 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0x00 },
// Input Len = 1
{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x00,
 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x1f,
 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x00,
 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x2f,
 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x01,
 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x3f,
 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x00, 0x00,
 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x4f,
 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x02,
 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0x5f,
 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0x7e, 0x02,
 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0x6f,
 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0x03,
 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0x7f,
 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0x00 },
// Input Len = 2
{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x00,
 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x0f, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x00,
 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x17, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x01,
 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x1f, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x00,
 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x27, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x02,
 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x2f, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x02,
 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x37, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x03,
 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x3f, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x00, 0x00,
 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x47, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x04,
 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x4f, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x04,
 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0x57, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0x05,
 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0x5f, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0x7e, 0x02,
 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0x67, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0x06,
 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0x6f, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0x06,
 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0x77, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0x07,
 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0x7f, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0x00 },
// Input Len = 3
{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x00, 0x08, 0x09, 0x0a, 0x07, 0x0c, 0x0d, 0x0e, 0x00,
 0x10, 0x11, 0x12, 0x0b, 0x14, 0x15, 0x16, 0x01, 0x18, 0x19, 0x1a, 0x0f, 0x1c, 0x1d, 0x1e, 0x00,
 0x20, 0x21, 0x22, 0x13, 0x24, 0x25, 0x26, 0x02, 0x28, 0x29, 0x2a, 0x17, 0x2c, 0x2d, 0x2e, 0x02,
 0x30, 0x31, 0x32, 0x1b, 0x34, 0x35, 0x36, 0x03, 0x38, 0x39, 0x3a, 0x1f, 0x3c, 0x3d, 0x3e, 0x00,
 0x40, 0x41, 0x42, 0x23, 0x44, 0x45, 0x46, 0x04, 0x48, 0x49, 0x4a, 0x27, 0x4c, 0x4d, 0x4e, 0x04,
 0x50, 0x51, 0x52, 0x2b, 0x54, 0x55, 0x56, 0x05, 0x58, 0x59, 0x5a, 0x2f, 0x5c, 0x5d, 0x5e, 0x02,
 0x60, 0x61, 0x62, 0x33, 0x64, 0x65, 0x66, 0x06, 0x68, 0x69, 0x6a, 0x37, 0x6c, 0x6d, 0x6e, 0x06,
 0x70, 0x71, 0x72, 0x3b, 0x74, 0x75, 0x76, 0x07, 0x78, 0x79, 0x7a, 0x3f, 0x7c, 0x7d, 0x00, 0x00,
 0x80, 0x81, 0x82, 0x43, 0x84, 0x85, 0x86, 0x08, 0x88, 0x89, 0x8a, 0x47, 0x8c, 0x8d, 0x8e, 0x08,
 0x90, 0x91, 0x92, 0x4b, 0x94, 0x95, 0x96, 0x09, 0x98, 0x99, 0x9a, 0x4f, 0x9c, 0x9d, 0x9e, 0x04,
 0xa0, 0xa1, 0xa2, 0x53, 0xa4, 0xa5, 0xa6, 0x0a, 0xa8, 0xa9, 0xaa, 0x57, 0xac, 0xad, 0xae, 0x0a,
 0xb0, 0xb1, 0xb2, 0x5b, 0xb4, 0xb5, 0xb6, 0x0b, 0xb8, 0xb9, 0xba, 0x5f, 0xbc, 0xbd, 0x7e, 0x02,
 0xc0, 0xc1, 0xc2, 0x63, 0xc4, 0xc5, 0xc6, 0x0c, 0xc8, 0xc9, 0xca, 0x67, 0xcc, 0xcd, 0xce, 0x0c,
 0xd0, 0xd1, 0xd2, 0x6b, 0xd4, 0xd5, 0xd6, 0x0d, 0xd8, 0xd9, 0xda, 0x6f, 0xdc, 0xdd, 0xde, 0x06,
 0xe0, 0xe1, 0xe2, 0x73, 0xe4, 0xe5, 0xe6, 0x0e, 0xe8, 0xe9, 0xea, 0x77, 0xec, 0xed, 0xee, 0x0e,
 0xf0, 0xf1, 0xf2, 0x7b, 0xf4, 0xf5, 0xf6, 0x0f, 0xf8, 0xf9, 0xfa, 0x7f, 0xfc, 0xfd, 0xfe, 0x00 },
// Input Len = 4
{0x00, 0x01, 0x02, 0x00, 0x04, 0x03, 0x06, 0x00, 0x08, 0x05, 0x0a, 0x01, 0x0c, 0x07, 0x0e, 0x00,
 0x10, 0x09, 0x12, 0x02, 0x14, 0x0b, 0x16, 0x02, 0x18, 0x0d, 0x1a, 0x03, 0x1c, 0x0f, 0x1e, 0x00,
 0x20, 0x11, 0x22, 0x04, 0x24, 0x13, 0x26, 0x04, 0x28, 0x15, 0x2a, 0x05, 0x2c, 0x17, 0x2e, 0x02,
 0x30, 0x19, 0x32, 0x06, 0x34, 0x1b, 0x36, 0x06, 0x38, 0x1d, 0x3a, 0x07, 0x3c, 0x1f, 0x3e, 0x00,
 0x40, 0x21, 0x42, 0x08, 0x44, 0x23, 0x46, 0x08, 0x48, 0x25, 0x4a, 0x09, 0x4c, 0x27, 0x4e, 0x04,
 0x50, 0x29, 0x52, 0x0a, 0x54, 0x2b, 0x56, 0x0a, 0x58, 0x2d, 0x5a, 0x0b, 0x5c, 0x2f, 0x5e, 0x02,
 0x60, 0x31, 0x62, 0x0c, 0x64, 0x33, 0x66, 0x0c, 0x68, 0x35, 0x6a, 0x0d, 0x6c, 0x37, 0x6e, 0x06,
 0x70, 0x39, 0x72, 0x0e, 0x74, 0x3b, 0x76, 0x0e, 0x78, 0x3d, 0x7a, 0x0f, 0x7c, 0x3f, 0x00, 0x00,
 0x80, 0x41, 0x82, 0x10, 0x84, 0x43, 0x86, 0x10, 0x88, 0x45, 0x8a, 0x11, 0x8c, 0x47, 0x8e, 0x08,
 0x90, 0x49, 0x92, 0x12, 0x94, 0x4b, 0x96, 0x12, 0x98, 0x4d, 0x9a, 0x13, 0x9c, 0x4f, 0x9e, 0x04,
 0xa0, 0x51, 0xa2, 0x14, 0xa4, 0x53, 0xa6, 0x14, 0xa8, 0x55, 0xaa, 0x15, 0xac, 0x57, 0xae, 0x0a,
 0xb0, 0x59, 0xb2, 0x16, 0xb4, 0x5b, 0xb6, 0x16, 0xb8, 0x5d, 0xba, 0x17, 0xbc, 0x5f, 0x7e, 0x02,
 0xc0, 0x61, 0xc2, 0x18, 0xc4, 0x63, 0xc6, 0x18, 0xc8, 0x65, 0xca, 0x19, 0xcc, 0x67, 0xce, 0x0c,
 0xd0, 0x69, 0xd2, 0x1a, 0xd4, 0x6b, 0xd6, 0x1a, 0xd8, 0x6d, 0xda, 0x1b, 0xdc, 0x6f, 0xde, 0x06,
 0xe0, 0x71, 0xe2, 0x1c, 0xe4, 0x73, 0xe6, 0x1c, 0xe8, 0x75, 0xea, 0x1d, 0xec, 0x77, 0xee, 0x0e,
 0xf0, 0x79, 0xf2, 0x1e, 0xf4, 0x7b, 0xf6, 0x1e, 0xf8, 0x7d, 0xfa, 0x1f, 0xfc, 0x7f, 0xfe, 0x00 },
// Input Len = 5
{0x00, 0x00, 0x01, 0x00, 0x02, 0x01, 0x03, 0x00, 0x04, 0x02, 0x05, 0x02, 0x06, 0x03, 0x07, 0x00,
 0x08, 0x04, 0x09, 0x04, 0x0a, 0x05, 0x0b, 0x02, 0x0c, 0x06, 0x0d, 0x06, 0x0e, 0x07, 0x0f, 0x00,
 0x10, 0x08, 0x11, 0x08, 0x12, 0x09, 0x13, 0x04, 0x14, 0x0a, 0x15, 0x0a, 0x16, 0x0b, 0x17, 0x02,
 0x18, 0x0c, 0x19, 0x0c, 0x1a, 0x0d, 0x1b, 0x06, 0x1c, 0x0e, 0x1d, 0x0e, 0x1e, 0x0f, 0x1f, 0x00,
 0x20, 0x10, 0x21, 0x10, 0x22, 0x11, 0x23, 0x08, 0x24, 0x12, 0x25, 0x12, 0x26, 0x13, 0x27, 0x04,
 0x28, 0x14, 0x29, 0x14, 0x2a, 0x15, 0x2b, 0x0a, 0x2c, 0x16, 0x2d, 0x16, 0x2e, 0x17, 0x2f, 0x02,
 0x30, 0x18, 0x31, 0x18, 0x32, 0x19, 0x33, 0x0c, 0x34, 0x1a, 0x35, 0x1a, 0x36, 0x1b, 0x37, 0x06,
 0x38, 0x1c, 0x39, 0x1c, 0x3a, 0x1d, 0x3b, 0x0e, 0x3c, 0x1e, 0x3d, 0x1e, 0x3e, 0x1f, 0x00, 0x00,
 0x40, 0x20, 0x41, 0x20, 0x42, 0x21, 0x43, 0x10, 0x44, 0x22, 0x45, 0x22, 0x46, 0x23, 0x47, 0x08,
 0x48, 0x24, 0x49, 0x24, 0x4a, 0x25, 0x4b, 0x12, 0x4c, 0x26, 0x4d, 0x26, 0x4e, 0x27, 0x4f, 0x04,
 0x50, 0x28, 0x51, 0x28, 0x52, 0x29, 0x53, 0x14, 0x54, 0x2a, 0x55, 0x2a, 0x56, 0x2b, 0x57, 0x0a,
 0x58, 0x2c, 0x59, 0x2c, 0x5a, 0x2d, 0x5b, 0x16, 0x5c, 0x2e, 0x5d, 0x2e, 0x5e, 0x2f, 0x3f, 0x02,
 0x60, 0x30, 0x61, 0x30, 0x62, 0x31, 0x63, 0x18, 0x64, 0x32, 0x65, 0x32, 0x66, 0x33, 0x67, 0x0c,
 0x68, 0x34, 0x69, 0x34, 0x6a, 0x35, 0x6b, 0x1a, 0x6c, 0x36, 0x6d, 0x36, 0x6e, 0x37, 0x6f, 0x06,
 0x70, 0x38, 0x71, 0x38, 0x72, 0x39, 0x73, 0x1c, 0x74, 0x3a, 0x75, 0x3a, 0x76, 0x3b, 0x77, 0x0e,
 0x78, 0x3c, 0x79, 0x3c, 0x7a, 0x3d, 0x7b, 0x1e, 0x7c, 0x3e, 0x7d, 0x3e, 0x7e, 0x3f, 0x7f, 0x00 },
// Input Len = 6
{0x00, 0x00, 0x01, 0x00, 0x02, 0x02, 0x03, 0x00, 0x04, 0x04, 0x05, 0x02, 0x06, 0x06, 0x07, 0x00,
 0x08, 0x08, 0x09, 0x04, 0x0a, 0x0a, 0x0b, 0x02, 0x0c, 0x0c, 0x0d, 0x06, 0x0e, 0x0e, 0x0f, 0x00,
 0x10, 0x10, 0x11, 0x08, 0x12, 0x12, 0x13, 0x04, 0x14, 0x14, 0x15, 0x0a, 0x16, 0x16, 0x17, 0x02,
 0x18, 0x18, 0x19, 0x0c, 0x1a, 0x1a, 0x1b, 0x06, 0x1c, 0x1c, 0x1d, 0x0e, 0x1e, 0x1e, 0x1f, 0x00,
 0x20, 0x20, 0x21, 0x10, 0x22, 0x22, 0x23, 0x08, 0x24, 0x24, 0x25, 0x12, 0x26, 0x26, 0x27, 0x04,
 0x28, 0x28, 0x29, 0x14, 0x2a, 0x2a, 0x2b, 0x0a, 0x2c, 0x2c, 0x2d, 0x16, 0x2e, 0x2e, 0x2f, 0x02,
 0x30, 0x30, 0x31, 0x18, 0x32, 0x32, 0x33, 0x0c, 0x34, 0x34, 0x35, 0x1a, 0x36, 0x36, 0x37, 0x06,
 0x38, 0x38, 0x39, 0x1c, 0x3a, 0x3a, 0x3b, 0x0e, 0x3c, 0x3c, 0x3d, 0x1e, 0x3e, 0x3e, 0x00, 0x00,
 0x40, 0x40, 0x41, 0x20, 0x42, 0x42, 0x43, 0x10, 0x44, 0x44, 0x45, 0x22, 0x46, 0x46, 0x47, 0x08,
 0x48, 0x48, 0x49, 0x24, 0x4a, 0x4a, 0x4b, 0x12, 0x4c, 0x4c, 0x4d, 0x26, 0x4e, 0x4e, 0x4f, 0x04,
 0x50, 0x50, 0x51, 0x28, 0x52, 0x52, 0x53, 0x14, 0x54, 0x54, 0x55, 0x2a, 0x56, 0x56, 0x57, 0x0a,
 0x58, 0x58, 0x59, 0x2c, 0x5a, 0x5a, 0x5b, 0x16, 0x5c, 0x5c, 0x5d, 0x2e, 0x5e, 0x5e, 0x3f, 0x02,
 0x60, 0x60, 0x61, 0x30, 0x62, 0x62, 0x63, 0x18, 0x64, 0x64, 0x65, 0x32, 0x66, 0x66, 0x67, 0x0c,
 0x68, 0x68, 0x69, 0x34, 0x6a, 0x6a, 0x6b, 0x1a, 0x6c, 0x6c, 0x6d, 0x36, 0x6e, 0x6e, 0x6f, 0x06,
 0x70, 0x70, 0x71, 0x38, 0x72, 0x72, 0x73, 0x1c, 0x74, 0x74, 0x75, 0x3a, 0x76, 0x76, 0x77, 0x0e,
 0x78, 0x78, 0x79, 0x3c, 0x7a, 0x7a, 0x7b, 0x1e, 0x7c, 0x7c, 0x7d, 0x3e, 0x7e, 0x7e, 0x7f, 0x00 },
// Input Len = 7
{0x00, 0x00, 0x02, 0x00, 0x04, 0x02, 0x06, 0x00, 0x08, 0x04, 0x0a, 0x02, 0x0c, 0x06, 0x0e, 0x00,
 0x10, 0x08, 0x12, 0x04, 0x14, 0x0a, 0x16, 0x02, 0x18, 0x0c, 0x1a, 0x06, 0x1c, 0x0e, 0x1e, 0x00,
 0x20, 0x10, 0x22, 0x08, 0x24, 0x12, 0x26, 0x04, 0x28, 0x14, 0x2a, 0x0a, 0x2c, 0x16, 0x2e, 0x02,
 0x30, 0x18, 0x32, 0x0c, 0x34, 0x1a, 0x36, 0x06, 0x38, 0x1c, 0x3a, 0x0e, 0x3c, 0x1e, 0x3e, 0x00,
 0x40, 0x20, 0x42, 0x10, 0x44, 0x22, 0x46, 0x08, 0x48, 0x24, 0x4a, 0x12, 0x4c, 0x26, 0x4e, 0x04,
 0x50, 0x28, 0x52, 0x14, 0x54, 0x2a, 0x56, 0x0a, 0x58, 0x2c, 0x5a, 0x16, 0x5c, 0x2e, 0x5e, 0x02,
 0x60, 0x30, 0x62, 0x18, 0x64, 0x32, 0x66, 0x0c, 0x68, 0x34, 0x6a, 0x1a, 0x6c, 0x36, 0x6e, 0x06,
 0x70, 0x38, 0x72, 0x1c, 0x74, 0x3a, 0x76, 0x0e, 0x78, 0x3c, 0x7a, 0x1e, 0x7c, 0x3e, 0x00, 0x00,
 0x80, 0x40, 0x82, 0x20, 0x84, 0x42, 0x86, 0x10, 0x88, 0x44, 0x8a, 0x22, 0x8c, 0x46, 0x8e, 0x08,
 0x90, 0x48, 0x92, 0x24, 0x94, 0x4a, 0x96, 0x12, 0x98, 0x4c, 0x9a, 0x26, 0x9c, 0x4e, 0x9e, 0x04,
 0xa0, 0x50, 0xa2, 0x28, 0xa4, 0x52, 0xa6, 0x14, 0xa8, 0x54, 0xaa, 0x2a, 0xac, 0x56, 0xae, 0x0a,
 0xb0, 0x58, 0xb2, 0x2c, 0xb4, 0x5a, 0xb6, 0x16, 0xb8, 0x5c, 0xba, 0x2e, 0xbc, 0x5e, 0x7e, 0x02,
 0xc0, 0x60, 0xc2, 0x30, 0xc4, 0x62, 0xc6, 0x18, 0xc8, 0x64, 0xca, 0x32, 0xcc, 0x66, 0xce, 0x0c,
 0xd0, 0x68, 0xd2, 0x34, 0xd4, 0x6a, 0xd6, 0x1a, 0xd8, 0x6c, 0xda, 0x36, 0xdc, 0x6e, 0xde, 0x06,
 0xe0, 0x70, 0xe2, 0x38, 0xe4, 0x72, 0xe6, 0x1c, 0xe8, 0x74, 0xea, 0x3a, 0xec, 0x76, 0xee, 0x0e,
 0xf0, 0x78, 0xf2, 0x3c, 0xf4, 0x7a, 0xf6, 0x1e, 0xf8, 0x7c, 0xfa, 0x3e, 0xfc, 0x7e, 0xfe, 0x00 }
}; // End of Squeeze


BYTE CODEBASED DecWidth[8][256] = {
// Input Len = 0
{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7,
 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 1,
 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7,
 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 0, 1,
 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7,
 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 1,
 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7,
 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0 },
// Input Len = 1
{8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 2,
 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 2,
 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 2,
 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 0, 1,
 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 2,
 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 2,
 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 2,
 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0 },
// Input Len = 2
{8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 3, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 3,
 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 3, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 7, 2,
 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 3, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 3,
 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 3, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 7, 7, 0, 1,
 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 3, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 3,
 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 3, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 7, 2,
 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 3, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 3,
 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 3, 8, 8, 8, 8, 8, 8, 8, 7, 8, 8, 8, 8, 8, 8, 8, 0 },
// Input Len = 3
{8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 8, 8, 8, 3,
 8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 8, 8, 7, 2,
 8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 8, 8, 8, 3,
 8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 7, 7, 0, 1,
 8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 8, 8, 8, 3,
 8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 8, 8, 7, 2,
 8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 8, 8, 8, 3,
 8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 8, 8, 8, 4, 8, 8, 8, 7, 8, 8, 8, 0 },
// Input Len = 4
{8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 4, 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 3,
 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 4, 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 7, 2,
 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 4, 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 3,
 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 4, 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 5, 7, 6, 0, 1,
 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 4, 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 3,
 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 4, 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 7, 2,
 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 4, 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 3,
 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 4, 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 5, 8, 7, 8, 0 },
// Input Len = 5
{7, 6, 7, 6, 7, 6, 7, 5, 7, 6, 7, 6, 7, 6, 7, 4, 7, 6, 7, 6, 7, 6, 7, 5, 7, 6, 7, 6, 7, 6, 7, 3,
 7, 6, 7, 6, 7, 6, 7, 5, 7, 6, 7, 6, 7, 6, 7, 4, 7, 6, 7, 6, 7, 6, 7, 5, 7, 6, 7, 6, 7, 6, 6, 2,
 7, 6, 7, 6, 7, 6, 7, 5, 7, 6, 7, 6, 7, 6, 7, 4, 7, 6, 7, 6, 7, 6, 7, 5, 7, 6, 7, 6, 7, 6, 7, 3,
 7, 6, 7, 6, 7, 6, 7, 5, 7, 6, 7, 6, 7, 6, 7, 4, 7, 6, 7, 6, 7, 6, 7, 5, 7, 6, 7, 6, 6, 5, 0, 1,
 7, 6, 7, 6, 7, 6, 7, 5, 7, 6, 7, 6, 7, 6, 7, 4, 7, 6, 7, 6, 7, 6, 7, 5, 7, 6, 7, 6, 7, 6, 7, 3,
 7, 6, 7, 6, 7, 6, 7, 5, 7, 6, 7, 6, 7, 6, 7, 4, 7, 6, 7, 6, 7, 6, 7, 5, 7, 6, 7, 6, 7, 6, 6, 2,
 7, 6, 7, 6, 7, 6, 7, 5, 7, 6, 7, 6, 7, 6, 7, 4, 7, 6, 7, 6, 7, 6, 7, 5, 7, 6, 7, 6, 7, 6, 7, 3,
 7, 6, 7, 6, 7, 6, 7, 5, 7, 6, 7, 6, 7, 6, 7, 4, 7, 6, 7, 6, 7, 6, 7, 5, 7, 6, 7, 6, 7, 6, 7, 0 },
// Input Len = 6
{7, 7, 7, 6, 7, 7, 7, 5, 7, 7, 7, 6, 7, 7, 7, 4, 7, 7, 7, 6, 7, 7, 7, 5, 7, 7, 7, 6, 7, 7, 7, 3,
 7, 7, 7, 6, 7, 7, 7, 5, 7, 7, 7, 6, 7, 7, 7, 4, 7, 7, 7, 6, 7, 7, 7, 5, 7, 7, 7, 6, 7, 7, 6, 2,
 7, 7, 7, 6, 7, 7, 7, 5, 7, 7, 7, 6, 7, 7, 7, 4, 7, 7, 7, 6, 7, 7, 7, 5, 7, 7, 7, 6, 7, 7, 7, 3,
 7, 7, 7, 6, 7, 7, 7, 5, 7, 7, 7, 6, 7, 7, 7, 4, 7, 7, 7, 6, 7, 7, 7, 5, 7, 7, 7, 6, 6, 6, 0, 1,
 7, 7, 7, 6, 7, 7, 7, 5, 7, 7, 7, 6, 7, 7, 7, 4, 7, 7, 7, 6, 7, 7, 7, 5, 7, 7, 7, 6, 7, 7, 7, 3,
 7, 7, 7, 6, 7, 7, 7, 5, 7, 7, 7, 6, 7, 7, 7, 4, 7, 7, 7, 6, 7, 7, 7, 5, 7, 7, 7, 6, 7, 7, 6, 2,
 7, 7, 7, 6, 7, 7, 7, 5, 7, 7, 7, 6, 7, 7, 7, 4, 7, 7, 7, 6, 7, 7, 7, 5, 7, 7, 7, 6, 7, 7, 7, 3,
 7, 7, 7, 6, 7, 7, 7, 5, 7, 7, 7, 6, 7, 7, 7, 4, 7, 7, 7, 6, 7, 7, 7, 5, 7, 7, 7, 6, 7, 7, 7, 0 },
// Input Len = 7
{8, 7, 8, 6, 8, 7, 8, 5, 8, 7, 8, 6, 8, 7, 8, 4, 8, 7, 8, 6, 8, 7, 8, 5, 8, 7, 8, 6, 8, 7, 8, 3,
 8, 7, 8, 6, 8, 7, 8, 5, 8, 7, 8, 6, 8, 7, 8, 4, 8, 7, 8, 6, 8, 7, 8, 5, 8, 7, 8, 6, 8, 7, 7, 2,
 8, 7, 8, 6, 8, 7, 8, 5, 8, 7, 8, 6, 8, 7, 8, 4, 8, 7, 8, 6, 8, 7, 8, 5, 8, 7, 8, 6, 8, 7, 8, 3,
 8, 7, 8, 6, 8, 7, 8, 5, 8, 7, 8, 6, 8, 7, 8, 4, 8, 7, 8, 6, 8, 7, 8, 5, 8, 7, 8, 6, 7, 6, 0, 1,
 8, 7, 8, 6, 8, 7, 8, 5, 8, 7, 8, 6, 8, 7, 8, 4, 8, 7, 8, 6, 8, 7, 8, 5, 8, 7, 8, 6, 8, 7, 8, 3,
 8, 7, 8, 6, 8, 7, 8, 5, 8, 7, 8, 6, 8, 7, 8, 4, 8, 7, 8, 6, 8, 7, 8, 5, 8, 7, 8, 6, 8, 7, 7, 2,
 8, 7, 8, 6, 8, 7, 8, 5, 8, 7, 8, 6, 8, 7, 8, 4, 8, 7, 8, 6, 8, 7, 8, 5, 8, 7, 8, 6, 8, 7, 8, 3,
 8, 7, 8, 6, 8, 7, 8, 5, 8, 7, 8, 6, 8, 7, 8, 4, 8, 7, 8, 6, 8, 7, 8, 5, 8, 7, 8, 6, 8, 7, 8, 0 }
}; // width


