/*++ 

Copyright (c) 1992  Microsoft Corporation

Module Name:

    perfutil.c

Abstract:

    This file implements the utility routines used to construct the
	common parts of a PERF_INSTANCE_DEFINITION (see winperf.h) and
    perform event logging functions.

--*/
//
//  include files
//

#define UNICODE 1
#define _UNICODE 1

#include <windows.h>
#include <string.h>
#include <winperf.h>
#include "perferr.h"	 // error message definition
#include "perfmsg.h"
#include "perfutil.h"

#define INITIAL_SIZE     1024L
#define EXTEND_SIZE      1024L

//
// Global data definitions.
//

ULONG                   ulInfoBufferSize = 0;

HANDLE hEventLog = NULL;      // event log handle for reporting events
                              // initialized in Open... routines
DWORD  dwLogUsers = 0;        // count of functions using event log
extern TCHAR szBuf[];

DWORD MESSAGE_LEVEL = 0;

WCHAR GLOBAL_STRING[] = L"Global";
WCHAR FOREIGN_STRING[] = L"Foreign";
WCHAR COSTLY_STRING[] = L"Costly";

WCHAR NULL_STRING[] = L"\0";    // pointer to null string

// test for delimiter, end of line and non-digit characters
// used by IsNumberInUnicodeList routine
//
#define DIGIT       1
#define DELIMITER   2
#define INVALID     3

#define EvalThisChar(c,d) ( \
     (c == d) ? DELIMITER : \
     (c == 0) ? DELIMITER : \
     (c < (WCHAR)'0') ? INVALID : \
     (c > (WCHAR)'9') ? INVALID : \
     DIGIT)


HANDLE
MonOpenEventLog (
)
/*++

Routine Description:

    Reads the level of event logging from the registry and opens the
        channel to the event logger for subsequent event log entries.

Arguments:

      None

Return Value:

    Handle to the event log for reporting events.
    NULL if open not successful.

--*/
{
    HKEY hAppKey;
    TCHAR LogLevelKeyName[] = L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Perflib";
    TCHAR LogLevelValueName[] = L"EventLogLevel";
	TCHAR EventLogRegistryPath[] = L"System\\CurrentControlSet\\Services\\EventLog\\Application\\PerfAPI";
    LONG lStatus;
    DWORD dwLogLevel;
    DWORD dwValueType;
    DWORD dwValueSize;
	DWORD result;
	HKEY  hPerfEventLogKey;

    // if global value of the logging level not initialized or is disabled,
    //  check the registry to see if it should be updated.

    if (!MESSAGE_LEVEL) {

       lStatus = RegOpenKeyEx (HKEY_LOCAL_MACHINE,
                               LogLevelKeyName,
                               0,
                               KEY_READ,
                               &hAppKey);

       dwValueSize = sizeof (dwLogLevel);

       if (lStatus == ERROR_SUCCESS) {
            lStatus = RegQueryValueEx (hAppKey,
                               LogLevelValueName,
                               (LPDWORD)NULL,
                               &dwValueType,
                               (LPBYTE)&dwLogLevel,
                               &dwValueSize);

            if (lStatus == ERROR_SUCCESS) {
               MESSAGE_LEVEL = dwLogLevel;
            } else {
               MESSAGE_LEVEL = MESSAGE_LEVEL_DEFAULT;
            }
            RegCloseKey (hAppKey);
       } else {
         MESSAGE_LEVEL = MESSAGE_LEVEL_DEFAULT;
       }
    }

    if (hEventLog == NULL){
		lStatus = RegCreateKeyEx (HKEY_LOCAL_MACHINE, (LPCTSTR) EventLogRegistryPath, 0, NULL, 
    					 		 REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hPerfEventLogKey, &result);
		if (lStatus == ERROR_SUCCESS) {
			result = SearchPath(NULL, L"PerfApi", L".dll", 1024, szBuf, NULL);
			if ( result && result < 1024 ) {
		   		RegSetValueEx(hPerfEventLogKey, L"EventMessageFile", 0, REG_EXPAND_SZ, (CONST BYTE *)szBuf, (wcslen(szBuf)+1)*sizeof(TCHAR) ) ;
				result = 7;
				RegSetValueEx(hPerfEventLogKey, L"TypesSupported", 0, REG_DWORD, (CONST BYTE *) &result, sizeof(DWORD));
			}
		}
        hEventLog = RegisterEventSource (
            (LPTSTR)NULL,            // Use Local Machine
            TEXT(APP_NAME));               // event log app name to find in registry

        if (hEventLog != NULL) {
           REPORT_INFORMATION (UTIL_LOG_OPEN, LOG_DEBUG);
        }
    }

    if (hEventLog != NULL) {
         dwLogUsers++;           // increment count of perfctr log users
    }
    return (hEventLog);
}


VOID
MonCloseEventLog (
)
/*++

Routine Description:

      Closes the handle to the event logger if this is the last caller

Arguments:

      None

Return Value:

      None

--*/
{
    if (hEventLog != NULL) {
        dwLogUsers--;         // decrement usage
        if (dwLogUsers <= 0) {    // and if we're the last, then close up log
            REPORT_INFORMATION (UTIL_CLOSING_LOG, LOG_DEBUG);
            DeregisterEventSource (hEventLog);
        }
    }
}

DWORD
GetQueryType (
    IN LPWSTR lpValue
)
/*++

GetQueryType

    returns the type of query described in the lpValue string so that
    the appropriate processing method may be used

Arguments

    IN lpValue
        string passed to PerfRegQuery Value for processing

Return Value

    QUERY_GLOBAL
        if lpValue == 0 (null pointer)
           lpValue == pointer to Null string
           lpValue == pointer to "Global" string

    QUERY_FOREIGN
        if lpValue == pointer to "Foreign" string

    QUERY_COSTLY
        if lpValue == pointer to "Costly" string

    otherwise:

    QUERY_ITEMS

--*/
{
    WCHAR   *pwcArgChar, *pwcTypeChar;
    BOOL    bFound;

    if (lpValue == 0) {
        return QUERY_GLOBAL;
    } else if (*lpValue == 0) {
        return QUERY_GLOBAL;
    }

    // check for "Global" request

    pwcArgChar = lpValue;
    pwcTypeChar = GLOBAL_STRING;
    bFound = TRUE;  // assume found until contradicted

    // check to the length of the shortest string

    while ((*pwcArgChar != L'\0') && (*pwcTypeChar != L'\0')) {
        if (*pwcArgChar++ != *pwcTypeChar++) {
            bFound = FALSE; // no match
            break;          // bail out now
        }
    }

    if (bFound) return QUERY_GLOBAL;

    // check for "Foreign" request

    pwcArgChar = lpValue;
    pwcTypeChar = FOREIGN_STRING;
    bFound = TRUE;  // assume found until contradicted

    // check to the length of the shortest string

    while ((*pwcArgChar != L'\0') && (*pwcTypeChar != L'\0')) {
        if (*pwcArgChar++ != *pwcTypeChar++) {
            bFound = FALSE; // no match
            break;          // bail out now
        }
    }

    if (bFound) return QUERY_FOREIGN;

    // check for "Costly" request

    pwcArgChar = lpValue;
    pwcTypeChar = COSTLY_STRING;
    bFound = TRUE;  // assume found until contradicted

    // check to the length of the shortest string

    while ((*pwcArgChar != L'\0') && (*pwcTypeChar != L'\0')) {
        if (*pwcArgChar++ != *pwcTypeChar++) {
            bFound = FALSE; // no match
            break;          // bail out now
        }
    }

    if (bFound) return QUERY_COSTLY;

    // if not Global and not Foreign and not Costly,
    // then it must be an item list

    return QUERY_ITEMS;

}

BOOL
IsNumberInUnicodeList (
    IN DWORD   dwNumber,
    IN LPWSTR  lpwszUnicodeList
)
/*++

IsNumberInUnicodeList

Arguments:

    IN dwNumber
        DWORD number to find in list

    IN lpwszUnicodeList
        Null terminated, Space delimited list of decimal numbers

Return Value:

    TRUE:
            dwNumber was found in the list of unicode number strings

    FALSE:
            dwNumber was not found in the list.

--*/
{
    DWORD   dwThisNumber;
    WCHAR   *pwcThisChar;
    BOOL    bValidNumber;
    BOOL    bNewItem;
    WCHAR   wcDelimiter;    // could be an argument to be more flexible

    if (lpwszUnicodeList == NULL) return FALSE;    // null pointer, # not found

    pwcThisChar = lpwszUnicodeList;
    dwThisNumber = 0;
    wcDelimiter = (WCHAR)' ';
    bValidNumber = FALSE;
    bNewItem = TRUE;

    while (TRUE) {
        switch (EvalThisChar (*pwcThisChar, wcDelimiter)) {
            case DIGIT:
                // if this is the first digit after a delimiter, then
                // set flags to start computing the new number
                if (bNewItem) {
                    bNewItem = FALSE;
                    bValidNumber = TRUE;
                }
                if (bValidNumber) {
                    dwThisNumber *= 10;
                    dwThisNumber += (*pwcThisChar - (WCHAR)'0');
                }
                break;

            case DELIMITER:
                // a delimter is either the delimiter character or the
                // end of the string ('\0') if when the delimiter has been
                // reached a valid number was found, then compare it to the
                // number from the argument list. if this is the end of the
                // string and no match was found, then return.
                //
                if (bValidNumber) {
                    if (dwThisNumber == dwNumber) return TRUE;
                    bValidNumber = FALSE;
                }
                if (*pwcThisChar == 0) {
                    return FALSE;
                } else {
                    bNewItem = TRUE;
                    dwThisNumber = 0;
                }
                break;

            case INVALID:
                // if an invalid character was encountered, ignore all
                // characters up to the next delimiter and then start fresh.
                // the invalid number is not compared.
                bValidNumber = FALSE;
                break;

            default:
                break;

        }
        pwcThisChar++;
    }

}   // IsNumberInUnicodeList
