/////////////////////////////////////////////////////////////////////////////
//
//	Copyright (c) 1996-1998 Microsoft Corporation
//
//	Module Name:
//		AdmNetUtils.cpp
//
//	Abstract:
//		Declaration of network utility functions.
//
//	Implementation File:
//		AdmNetUtils.cpp
//
//	Author:
//		David Potter (davidp)	February 19, 1998
//
//	Revision History:
//
//	Notes:
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __ADMNETUTILS_H_
#define __ADMNETUTILS_H_

/////////////////////////////////////////////////////////////////////////////
// Include Files
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Forward Class Declarations
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// External Class Declarations
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Global Function Prototypes
/////////////////////////////////////////////////////////////////////////////

BOOL BIsValidIpAddress(IN LPCWSTR pszAddress);
BOOL BIsValidSubnetMask(IN LPCWSTR pszMask);
BOOL BIsValidIpAddressAndSubnetMask(IN LPCWSTR pszAddress, IN LPCWSTR pszMask);
BOOL BIsIpAddressInUse(IN LPCWSTR pszAddress);

/////////////////////////////////////////////////////////////////////////////

#endif // __ADMNETUTILS_H_
