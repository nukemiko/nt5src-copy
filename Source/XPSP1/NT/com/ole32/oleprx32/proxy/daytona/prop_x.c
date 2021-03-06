
//+===================================================================
//
//   This file is a wrapper for prop_p.c (which is generated by MIDL
//   from ..\..\idl\prop.idl).
//
//   Prop_p.c makes references to wire-marshal routines that are
//   provided by oleaut32.dll.  But we can't (for performance
//   reasons) link ole32.dll directly to oleaut32, we do it
//   lazily.  So in this file, we use #defines to map
//   the wire-marshal routines called by prop_p.c so that
//   they are actually calls to "Load*" wrappers.  These
//   wrappers load oleaut32.dll, do a GetProcAddr, and
//   then make the actual call.
//
//+===================================================================

#include "prop.h"
#include <privoa.h>

#define BSTR_UserSize        LoadBSTR_UserSize
#define BSTR_UserMarshal     LoadBSTR_UserMarshal
#define BSTR_UserUnmarshal   LoadBSTR_UserUnmarshal
#define BSTR_UserFree        LoadBSTR_UserFree

#define LPSAFEARRAY_UserSize        LoadLPSAFEARRAY_UserSize
#define LPSAFEARRAY_UserMarshal     LoadLPSAFEARRAY_UserMarshal
#define LPSAFEARRAY_UserUnmarshal   LoadLPSAFEARRAY_UserUnmarshal
#define LPSAFEARRAY_UserFree        LoadLPSAFEARRAY_UserFree

#if defined(_WIN64)
#define BSTR_UserSize64        LoadBSTR_UserSize
#define BSTR_UserMarshal64     LoadBSTR_UserMarshal
#define BSTR_UserUnmarshal64   LoadBSTR_UserUnmarshal
#define BSTR_UserFree64        LoadBSTR_UserFree

#define LPSAFEARRAY_UserSize64        LoadLPSAFEARRAY_UserSize
#define LPSAFEARRAY_UserMarshal64     LoadLPSAFEARRAY_UserMarshal
#define LPSAFEARRAY_UserUnmarshal64   LoadLPSAFEARRAY_UserUnmarshal
#define LPSAFEARRAY_UserFree64        LoadLPSAFEARRAY_UserFree
#endif // defined(_WIN64)

#include "prop_p.c"
