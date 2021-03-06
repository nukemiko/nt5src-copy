#ifndef UNIMCRO
#define UNIMCRO

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "wiamicro.h"
#include <sti.h>
#include <math.h>
#include <usbscan.h>
//#include <winioctl.h>

#ifdef DEBUG
#include <stdio.h>
#endif

HRESULT GetOLESTRResourceString(LONG lResourceID,LPOLESTR *ppsz,BOOL bLocal);
HRESULT ParseGSD(LPSTR szGSDName, void *pIOBlock);

#define INITGUID

//
// Button GUIDS
//

DEFINE_GUID( guidScanButton, 0xa6c5a715, 0x8c6e, 0x11d2, 0x97, 0x7a, 0x0, 0x0, 0xf8, 0x7a, 0x92, 0x6f);
DEFINE_GUID( guidCopyButton, 0xb38e1061, 0x65ab, 0x11d1, 0xa4, 0x3b, 0x8, 0x0, 0x9,  0xee, 0xbd, 0xf6);
DEFINE_GUID( guidEMailButton,0x8efdf2c0, 0x7260, 0x11d2, 0xa7, 0xc5, 0x8, 0x0, 0x9,  0xc0, 0x94, 0x24);
DEFINE_GUID( guidFaxButton,  0x8efdf2c1, 0x7260, 0x11d2, 0xa7, 0xc5, 0x8, 0x0, 0x9,  0xc0, 0x94, 0x24);
DEFINE_GUID( guidDocMgmtButton, 0x8efdf2c2, 0x7260, 0x11d2, 0xa7, 0xc5, 0x8, 0x0, 0x9,  0xc0, 0x94, 0x24);

#undef INITGUID

#endif
