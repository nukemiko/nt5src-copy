;/* messages.asm
; *
; * Microsoft Confidential
; * Copyright (C) Microsoft Corporation 1988-1991
; * All Rights Reserved.
; *
; * Modification History
; *
; * Sudeepb 14-May-1991 Ported for NT XMS support
; */


	page    95,160
	title   himem3 - Initialization messages

	.xlist
	include himem.inc
	.list

;*----------------------------------------------------------------------*
;*      DRIVER MESSAGES                                                 *
;*----------------------------------------------------------------------*

	public  SignOnMsg
	public  ROMDisabledMsg
	public  UnsupportedROMMsg
	public  ROMHookedMsg
	public  BadDOSMsg
	public  NowInMsg
	public  On8086Msg
	public  NoExtMemMsg
	public  FlushMsg
	public  StartMsg
	public  HandlesMsg
	public  HMAMINMsg
	public  KMsg
	public  NoHMAMsg
	public  A20OnMsg
	public  HMAOKMsg
	public  InsA20Msg
	public  InsA20EndMsg
	public  InsExtA20msg
	public  NoA20HandlerMsg
	public  VDISKInMsg
	public  BadArgMsg
	public  EndText

; Start of text subject to translation
;  Material appearing in single quotation marks should be translated.


SignOnMsg db    13,10,'HIMEM: šæšŁŁ Šă©Ș   «Š DOS XMS, ëĄŠ© '
	db      '0' + (HimemVersion shr 8),'.'
	db      '0' + ((HimemVersion and 0ffh) / 16)
	db      '0' + ((HimemVersion and 0ffh) mod 16)
	db      ' - '
	db      DATE_String
	db      13,10,'šŠ š­ă XMS ëĄŠ© 2.0'
	db      13,10,'€ŹŁ« Ąá  Ą éŁ« 1988-1991 Microsoft Corp.'
	db      13,10,'$'

ROMDisabledMsg    db    13,10,  ' ©Ą éȘ RAM ć€  §€šŠ§Š Łâ€.$'
UnsupportedROMMsg db    13,10,':  §€šŠ§Šć© «Ș ©Ą éȘ RAM '
		  db            '€ Ź§Š©«šć«  © Ź«æ «Š ©ç©«Ł.$'
ROMHookedMsg      db    13,10,':  ©Ą éȘ RAM źš© ŁŠ§Š ć«  Ą  € Ł§Ššć '
		  db            '€ §€šŠ§Š ć.$'

BadDOSMsg       db      13,10,': Ź«æ «Š HIMEM.SYS ć€    Windows NT.$'
NowInMsg        db      13,10,': ë€ §šæšŁŁ  źćš ©Ș Ł€ăŁȘ Extended âź  ă Ą«©«ć.$'
On8086Msg       db      13,10,': Š HIMEM.SYS § «ć Ł  80x86 Łź€ă.$'
NoExtMemMsg     db      13,10,': € šâĄ  â© Ł Ł€ăŁ extended.$'
NoA20HandlerMsg db      13,10,': € ć€  Ź€«æȘ Š âąźŠȘ «Ș šŁŁăȘ A20 !$'
VDISKInMsg      db      13,10,': Š §šæšŁŁ Ąźéš©Ș Ł€ăŁȘ VDISK âź  ă Ą«©«ć.$'
FlushMsg        db      13,10,7,'      Š §šæšŁŁ Šă©Ș   «Š XMS € Ą«©«áĄ.',13,10,13,10,'$'

StartMsg        db      13,10,'$'
HandlesMsg      db      ' Š  ćĄ«Ș ź š ©ŁŠç «Ș Ł€ăŁȘ extended ć€   â© ŁŠ .$'
HMAMINMsg       db      13,10,'Š ŁâŠȘ «Ș §š ŠźăȘ Ł€ăŁȘ High (HMA) Ššć©Ą © $'
KMsg            db      'K.$'
InsA20Msg       db      13,10,' Ą«©«áĄ ćĄ«Ș ź š ©ŁŠç A20 $'
InsA20EndMsg    db      '.$'
InsExtA20Msg    db      13,10,'Ą«©«áĄ „à«š ĄæȘ ćĄ«Ș ź š ©ŁŠç A20.$'

NoHMAMsg        db      13,10,':  §š Šźă Ł€ăŁȘ High € ć€   â© Ł.'
		db      13,10,'$'
A20OnMsg        db      13,10,':  šŁŁă A20 ă«€ ă €šŠ§Š Łâ€.'
		db      13,10,'$'

BadArgMsg       db      13,10,':   Ł âĄŹš §šáŁ«šŠȘ §šąâ­Ą: $'

HMAOKMsg        db      13,10,' 64K §š Šźă Ł€ăŁȘ High ć€   â© Ł.'
		db      13,10,13,10,'$'

		db      'Ź«æ «Š §šæšŁŁ €ăĄ  ©« Microsoft Corporation.'

; end of material subject to translation


EndText         label   byte
_text   ends
	end
