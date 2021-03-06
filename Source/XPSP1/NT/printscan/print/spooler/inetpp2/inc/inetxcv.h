#ifndef _INET_XCV_H
#define _INET_XCV_H


typedef struct INET_CONFIGUREPORT_RESPDATA {
    DWORD dwVersion;
    DWORD dwError;
    DWORD dwErrorStringLen;
    WCHAR dwErrorString[1];
} INET_CONFIGUREPORT_RESPDATA;
typedef INET_CONFIGUREPORT_RESPDATA *PINET_CONFIGUREPORT_RESPDATA;

typedef struct INET_XCV_GETCONFIGURATION_REQ_DATA {
    DWORD dwVersion;
} INET_XCV_GETCONFIGURATION_REQ_DATA;
typedef INET_XCV_GETCONFIGURATION_REQ_DATA *PINET_XCV_GETCONFIGURATION_REQ_DATA;

typedef struct INET_XCV_CONFIGURATION {
    DWORD dwVersion;
    DWORD dwAuthMethod;
    WCHAR szUserName[MAX_USERNAME_LEN];
    WCHAR szPassword[MAX_PASSWORD_LEN];
    BOOL  bIgnoreSecurityDlg;
    BOOL  bSettingForAll;
    BOOL  bPasswordChanged;
} INET_XCV_CONFIGURATION;
typedef INET_XCV_CONFIGURATION *PINET_XCV_CONFIGURATION;

#define INET_XCV_SET_CONFIGURATION  L"SetConfiguration"
#define INET_XCV_GET_CONFIGURATION  L"GetConfiguration"
#define INET_XCV_DELETE_PORT        L"DeletePort"
#define INET_XCV_ADD_PORT           L"AddPort"

#define MAX_INET_XCV_NAME_LEN 256

#endif
