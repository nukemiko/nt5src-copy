#include "pch.h"
#pragma hdrstop


#define IDC_DSFIND      0x0000

typedef struct
{
    CLSID clsidForm;
    LPTSTR pCaption;
    LPTSTR pIconPath;
    INT idIcon;
} FORMLISTITEM, * LPFORMLISTITEM;


/*-----------------------------------------------------------------------------
/ Helper functions
/----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
/ _FindInDs
/ ---------
/   Launch the Directory Search UI given a CLSID (for the form) or a
/   scope to invoke off.
/
/ In:
/   pScope -> scope to root the search at / == NULL
/   pCLSID -> clsid for the form / == NULL
/
/ Out:
/   HRESULT
/----------------------------------------------------------------------------*/

typedef struct
{
    LPWSTR pScope;
    CLSID clsidForm;
} FINDSTATE, * LPFINDSTATE;

//
// bg thread used to display the query UI in a non-clocking way
// 

DWORD WINAPI _FindInDsThread(LPVOID pThreadData)
{
    HRESULT hres, hresCoInit;
    ICommonQuery* pCommonQuery = NULL;
    OPENQUERYWINDOW oqw;
    DSQUERYINITPARAMS dqip;
    LPFINDSTATE pFindState = (LPFINDSTATE)pThreadData;
   
    TraceEnter(TRACE_UI, "_FindInDsThread");

    hres = hresCoInit = CoInitialize(NULL);
    FailGracefully(hres, "Failed in call to CoInitialize");

    hres = CoCreateInstance(CLSID_CommonQuery, NULL, CLSCTX_INPROC_SERVER, IID_ICommonQuery, (LPVOID*)&pCommonQuery);
    FailGracefully(hres, "Failed in CoCreateInstance of CLSID_CommonQuery");

    dqip.cbStruct = SIZEOF(dqip);
    dqip.dwFlags = 0;
    dqip.pDefaultScope = NULL;
    
    if (pFindState->pScope)
        dqip.pDefaultScope = pFindState->pScope; 

    oqw.cbStruct = SIZEOF(oqw);
    oqw.dwFlags = 0;
    oqw.clsidHandler = CLSID_DsQuery;
    oqw.pHandlerParameters = &dqip;

    if (!pFindState->pScope)
    {
        oqw.dwFlags |= OQWF_DEFAULTFORM|OQWF_REMOVEFORMS;
        oqw.clsidDefaultForm = pFindState->clsidForm;
    }
    
    hres = pCommonQuery->OpenQueryWindow(NULL, &oqw, NULL);
    FailGracefully(hres, "OpenQueryWindow failed");

exit_gracefully:

    LocalFreeStringW(&pFindState->pScope);
    LocalFree(pFindState);

    DoRelease(pCommonQuery);

    if (SUCCEEDED(hresCoInit))
        CoUninitialize();

    TraceLeave();

    DllRelease();
    ExitThread(0);
    return 0;
}

//
// API for invoking the query UI
//

HRESULT _FindInDs(LPWSTR pScope, LPCLSID pCLSID)
{
    HRESULT hres;
    LPFINDSTATE pFindState;
    HANDLE hThread;
    DWORD dwThreadID;
    USES_CONVERSION;

    TraceEnter(TRACE_UI, "_FindInDs");

    if ((!pScope && !pCLSID) || (pScope && pCLSID))
        ExitGracefully(hres, E_INVALIDARG, "Bad arguments for invoking the search");

    pFindState = (LPFINDSTATE)LocalAlloc(LPTR, SIZEOF(FINDSTATE));
    TraceAssert(pFindState);

    if (!pFindState)
        ExitGracefully(hres, E_OUTOFMEMORY, "Failed to allocate state block");

    // pFindState->pScope = NULL;
    // pFindState->clsidForm = { 0 };

    if (pScope)
    {
        Trace(TEXT("Defaulting to scope: %s"), W2T(pScope));
        hres = LocalAllocStringW(&pFindState->pScope, pScope);
        FailGracefully(hres, "Failed to copy scope");
    }

    if (pCLSID)
    {
        TraceGUID("Invoking with form: ", *pCLSID);
        pFindState->clsidForm = *pCLSID;
    }

    DllAddRef();

    hThread = CreateThread(NULL, 0, _FindInDsThread, (LPVOID)pFindState, 0, &dwThreadID);
    TraceAssert(hThread);

    if (!hThread)
    {
        LocalFreeStringW(&pFindState->pScope);
        LocalFree((HLOCAL)pFindState);
        DllRelease();
        ExitGracefully(hres, E_FAIL, "Failed to create thread and issue query on it");
    }
    
    CloseHandle(hThread);
    hres = S_OK;                  // success

exit_gracefully:

    TraceLeaveResult(hres);
}



// object for invoking the find UI from the search menu in the shell (or off a context menu)

class CFindMenu : public IShellExtInit, IContextMenu
{
    private:
        LONG _cRef;
        CLSID _clsidFindEntry;
        LPWSTR _pDsObjectName;
        HDSA _hdsaFormList;            

    public:
        CFindMenu(REFCLSID clsidFindEntry);
        ~CFindMenu();

        // IUnknown
        STDMETHOD(QueryInterface)(REFIID riid, LPVOID* ppvObject);
        STDMETHOD_(ULONG, AddRef)();
        STDMETHOD_(ULONG, Release)();

        // IShellExtInit
        STDMETHODIMP Initialize(LPCITEMIDLIST pIDFolder, LPDATAOBJECT pDataObj, HKEY hKeyID);

        // IContextMenu
        STDMETHODIMP QueryContextMenu(HMENU hShellMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);
        STDMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO lpcmi);
        STDMETHODIMP GetCommandString(UINT_PTR idCmd, UINT uFlags, UINT FAR* reserved, LPSTR pszName, UINT ccMax);
};

CFindMenu::CFindMenu(REFCLSID clsidFindEntry) :
    _clsidFindEntry(clsidFindEntry),
    _pDsObjectName(NULL),
    _hdsaFormList(NULL),
    _cRef(1)
{
    DllAddRef();
}

INT _FreeFormListCB(LPVOID pItem, LPVOID pData)
{
    LPFORMLISTITEM pFormListItem = (LPFORMLISTITEM)pItem;
    TraceAssert(pFormListItem);

    LocalFreeString(&pFormListItem->pCaption);
    LocalFreeString(&pFormListItem->pIconPath);

    return 1;
}

CFindMenu::~CFindMenu()
{
    LocalFreeStringW(&_pDsObjectName);

    if (_hdsaFormList)
        DSA_DestroyCallback(_hdsaFormList, _FreeFormListCB, NULL);

    DllRelease();
}


// QI handling

ULONG CFindMenu::AddRef()
{
    return InterlockedIncrement(&_cRef);
}

ULONG CFindMenu::Release()
{
    if (InterlockedDecrement(&_cRef))
        return _cRef;

    delete this;
    return 0;
}

HRESULT CFindMenu::QueryInterface(REFIID riid, void **ppv)
{
    static const QITAB qit[] = 
    {
        QITABENT(CFindMenu, IShellExtInit),   // IID_IShellExtInit
        QITABENT(CFindMenu, IContextMenu),    // IID_IContextMenu
        {0, 0 },
    };
    return QISearch(this, qit, riid, ppv);
}


// IShellExtInit

STDMETHODIMP CFindMenu::Initialize(LPCITEMIDLIST pIDFolder, LPDATAOBJECT pDataObj, HKEY hKeyID)
{
    HRESULT hres;
    FORMATETC fmte = {(CLIPFORMAT)RegisterClipboardFormat(CFSTR_DSOBJECTNAMES), NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
    STGMEDIUM medium = { TYMED_NULL };
    LPDSOBJECTNAMES pDsObjects;
    LPWSTR pDsObjectName;
    USES_CONVERSION;
    
    TraceEnter(TRACE_UI, "CFindMenu::Initialize");

    // when building the Start->Find menu we are invoked but we are not passed
    // an IDataObject, therefore we know this is the case and we shall just
    // build the "In the Directory" form list. 

    if (!ShowDirectoryUI())  
        ExitGracefully(hres, E_FAIL, "ShowDirectoryUI returns FALSE, so failing initialize");

    if (IsEqualCLSID(_clsidFindEntry, CLSID_DsFind))
    {
        if (pDataObj &&  SUCCEEDED(pDataObj->GetData(&fmte, &medium)))
        {
            pDsObjects = (LPDSOBJECTNAMES)medium.hGlobal;
            pDsObjectName = (LPWSTR)ByteOffset(pDsObjects, pDsObjects->aObjects[0].offsetName);
            TraceAssert(pDsObjectName);

            hres = LocalAllocStringW(&_pDsObjectName, pDsObjectName);
            FailGracefully(hres, "Failed to copy scope path");
        }

        if (!_pDsObjectName)
            ExitGracefully(hres, E_FAIL, "Failed to get root scope for this object");
    }

    hres = S_OK;                  // success

exit_gracefully:

#ifdef DSUI_DEBUG
    if (SUCCEEDED(hres))
        Trace(TEXT("Find rooted at -%s-"), _pDsObjectName ? W2T(_pDsObjectName):TEXT("<not defined>"));
#endif

    ReleaseStgMedium(&medium);

    TraceLeaveResult(hres);
}


// IContextMenu handling

//
// Helper to set the icon for the given menu item
//

VOID _SetMenuItemIcon(HMENU hMenu, UINT item, UINT uID, BOOL fPosition, LPTSTR pIconFile, INT idRes, LPTSTR pCaption, HMENU hSubMenu)
{
    MENUITEMINFO mii;

    TraceEnter(TRACE_UI, "_SetMenuItemIcon");
    Trace(TEXT("hMenu %08x, item %d, pIconFile %s, idRes %d"), hMenu, item, pIconFile, idRes);
    Trace(TEXT("pCaption %s, hSubMenu %08x"), pCaption, hSubMenu);

    mii.cbSize = SIZEOF(mii);
    mii.fMask = MIIM_DATA|MIIM_SUBMENU|MIIM_TYPE|MIIM_ID;
    mii.fType = MFT_STRING;
    mii.wID = uID;
    mii.hSubMenu = hSubMenu;
    mii.cch = lstrlen(pCaption);
    mii.dwTypeData = pCaption;
    mii.dwItemData = Shell_GetCachedImageIndex(pIconFile, idRes, 0);
    TraceAssert(mii.dwItemData != -1);

    Trace(TEXT("Setting data to be %d"), mii.dwItemData);
    InsertMenuItem(hMenu, item, fPosition, &mii);

    TraceLeave();
}

STDAPI _LocalQueryMUIString(LPTSTR* ppResult, HKEY hk, LPCTSTR lpSubKey)
{
    HRESULT hr = LocalQueryString(ppResult, hk, lpSubKey);
    if (SUCCEEDED(hr))
    {
        // If any of these steps fail, don't fail the call
        TCHAR szExpanded[MAX_PATH];
        if (SUCCEEDED(SHLoadIndirectString(*ppResult, szExpanded, ARRAYSIZE(szExpanded), NULL)))
        {
            LPTSTR pszExpanded;
            if (SUCCEEDED(LocalAllocString(&pszExpanded, szExpanded)))
            {
                LocalFreeString(ppResult);
                *ppResult = pszExpanded;
            }
        }
    }
    return hr;
}

#define EXPLORER_POLICY TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer")

STDMETHODIMP CFindMenu::QueryContextMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
{
    HRESULT hres;
    TCHAR szBuffer[MAX_PATH];
    LPTSTR pBuffer = NULL;
    INT i, iItems = 0;
    FORMLISTITEM fli;
    HKEY hKey = NULL;
    HKEY hKeyForm = NULL;
    HKEY hkPolicy = NULL;

    TraceEnter(TRACE_UI, "CFindMenu::QueryContextMenu");

    // Just make sure we are allowed to surface this UI.
    
    if (!ShowDirectoryUI())  
        ExitGracefully(hres, E_FAIL, "ShowDirectoryUI returns FALSE, so failing initialize");

    // if we have no scope stored in our class then lets build the Start.Find menu entry
    // which we get from data stored in the registry.

    if (IsEqualCLSID(_clsidFindEntry, CLSID_DsStartFind))
    {
        // enumerate the entries we are going to display in Start->Find from the registry
        // this is then stored in a DSA so that we can invoke the Find UI on the
        // correct query form.

        _hdsaFormList = DSA_Create(SIZEOF(FORMLISTITEM), 4);
        TraceAssert(_hdsaFormList);

        if (!_hdsaFormList)
            ExitGracefully(hres, E_OUTOFMEMORY, "Failed to allocate find entry DSA");

        hres = GetKeyForCLSID(CLSID_DsQuery, TEXT("StartFindEntries"), &hKey);
        FailGracefully(hres, "Failed to get HKEY for the DsQuery CLSID");

        //
        // get the policy key so that we can check to see if we must disbale the entries
        //

        if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_CURRENT_USER, EXPLORER_POLICY, NULL, KEY_READ, &hkPolicy))
        {
            TraceMsg("Explorer policy key not found");
            hkPolicy = NULL;
        }
        
        for (i = 0 ; TRUE ; i++)
        {
            DWORD cchBuffer = ARRAYSIZE(szBuffer);
            if (ERROR_SUCCESS != RegEnumKeyEx(hKey, i, szBuffer, &cchBuffer, NULL, NULL, NULL, NULL))
            {
                TraceMsg("RegEnumKeyEx failed, therefore stopping enumeration");
                break;
            }
            else
            {    
                // We have a caption for the query form we want to display for the 
                // menu item, now lets pick up the GUID that is stored with it
                // so that we can invoke the form.

                if (hKeyForm)
                {
                    RegCloseKey(hKeyForm);
                    hKeyForm = NULL;
                }

                if (ERROR_SUCCESS == RegOpenKeyEx(hKey, szBuffer, NULL, KEY_READ, &hKeyForm))
                {
                    LPTSTR pszPolicy = NULL;
                    BOOL fHideEntry = FALSE;

                    // fli.clsidForm = { 0 };
                    fli.pCaption = NULL;
                    fli.pIconPath = NULL;
                    fli.idIcon = 0;

                    //
                    // lets parse out the CLSID into a value that we can put into the structure.
                    //
            
                    Trace(TEXT("Form GUID: %s"), szBuffer);
                    if (!GetGUIDFromString(szBuffer, &fli.clsidForm))
                    {
                        TraceMsg("Failed to parse the CLSID of the form");
                        continue;
                    }

                    //
                    // check to see if we have a policy key, if we do then we can disable the entry.
                    //

                    if (hkPolicy && SUCCEEDED(LocalQueryString(&pszPolicy, hKeyForm, TEXT("Policy"))))
                    {
                        Trace(TEXT("Policy value is: %s"), pszPolicy);                                                                             

                        DWORD dwType = REG_DWORD, cb = SIZEOF(fHideEntry);
                        if (ERROR_SUCCESS != RegQueryValueEx(hkPolicy, pszPolicy, NULL, &dwType, (LPBYTE)&fHideEntry, &cb))
                        {
                            TraceMsg("Failed to read the policy value");
                        }

                        LocalFreeString(&pszPolicy);
                    } 

                    //
                    // add the entry to the search menu list?
                    //

                    if (!fHideEntry)
                    {                    
                        LPTSTR pszPolicy;

                        // OK the GUID for the form parse OK and the policy says it is enabled
                        // therefore we must attempt to build a find menu entry for this object

                        if (SUCCEEDED(_LocalQueryMUIString(&fli.pCaption, hKeyForm, TEXT("LocalizedString"))) ||
                            SUCCEEDED(LocalQueryString(&fli.pCaption, hKeyForm, NULL)))
                        {               
                            Trace(TEXT("Form title: %s"), fli.pCaption);                                      

                            if (SUCCEEDED(LocalQueryString(&fli.pIconPath, hKeyForm, TEXT("Icon"))))
                            {
                               fli.idIcon = PathParseIconLocation(fli.pIconPath);
                               Trace(TEXT("Icon is: %s, resource %d"), fli.pIconPath, fli.idIcon);
                            }

                            if (-1 == DSA_AppendItem(_hdsaFormList, &fli))
                            {
                                _FreeFormListCB(&fli, NULL);
                                ExitGracefully(hres, E_OUTOFMEMORY, "Failed to allocate FORMLISTITEM structure");
                            }
                        }
                    }
                }
            }    
        }

        // we now (hopefully) have a DS full of the items we want to display on
        // the menu, so lets try and construct the menu around us.

        for (i = 0 ; i < DSA_GetItemCount(_hdsaFormList) ; i++, iItems++)
        {
            LPFORMLISTITEM pFormListItem = (LPFORMLISTITEM)DSA_GetItemPtr(_hdsaFormList, i);
            TraceAssert(pFormListItem);                           
            _SetMenuItemIcon(hMenu, i, idCmdFirst+i, TRUE, pFormListItem->pIconPath, pFormListItem->idIcon, pFormListItem->pCaption, NULL);        
        }
    }
    else
    {
        // when we are just a normal verb hanging off an objects context menu
        // then lets just load the string we want to display and show it.

        if (!LoadString(GLOBAL_HINSTANCE, IDS_FIND, szBuffer, ARRAYSIZE(szBuffer)))
            ExitGracefully(hres, E_FAIL, "Failed to load resource for menu item");

        InsertMenu(hMenu, indexMenu, MF_BYPOSITION|MF_STRING, idCmdFirst+IDC_DSFIND, szBuffer);
        iItems++;
    }

    hres = S_OK;
    
exit_gracefully:

    if (SUCCEEDED(hres))
        hres = ResultFromShort(iItems);

    if (hKey)
        RegCloseKey(hKey);
    if (hKeyForm)
        RegCloseKey(hKeyForm);
    if (hkPolicy)
        RegCloseKey(hkPolicy);

    LocalFreeString(&pBuffer);

    TraceLeaveValue(hres);
}


STDMETHODIMP CFindMenu::InvokeCommand(LPCMINVOKECOMMANDINFO lpcmi)
{
    HRESULT hres = E_FAIL;
    INT id = LOWORD(lpcmi->lpVerb);
    USES_CONVERSION;

    TraceEnter(TRACE_UI, "CFindMenu::InvokeCommand");

    if (!HIWORD(lpcmi->lpVerb))
    {
        // if we have a DSA and the verb is inside the DSA then lets invoke the
        // query UI with the correct form displayed, otherwise we can default to
        // using the scope we have (which can also be NULL)

        if (IsEqualCLSID(_clsidFindEntry, CLSID_DsStartFind) && 
                    _hdsaFormList && (id < DSA_GetItemCount(_hdsaFormList)))
        {
            LPFORMLISTITEM pFormListItem = (LPFORMLISTITEM)DSA_GetItemPtr(_hdsaFormList, id);
            TraceAssert(pFormListItem);

            TraceGUID("Invoking query form: ", pFormListItem->clsidForm);
            
            hres = _FindInDs(NULL, &pFormListItem->clsidForm);
            FailGracefully(hres, "FindInDs failed when invoking with a query form");
        }
        else
        {
            Trace(TEXT("Scope is: %s"), _pDsObjectName ? W2T(_pDsObjectName):TEXT("<none>"));

            hres = _FindInDs(_pDsObjectName, NULL);
            FailGracefully(hres, "FindInDs Failed when invoking with a scope");
        }
    }

    hres = S_OK;                  // success

exit_gracefully:

    TraceLeaveResult(hres);
}


STDMETHODIMP CFindMenu::GetCommandString(UINT_PTR idCmd, UINT uFlags, UINT FAR* reserved, LPSTR pszName, UINT ccMax)
{
    HRESULT hres = E_NOTIMPL;
    INT cc;
    
    TraceEnter(TRACE_UI, "CFindMenu::GetCommandString");

    // "Find..."? on a DS object, if so then lets load the help text
    // for it.

    if (IsEqualCLSID(_clsidFindEntry, CLSID_DsFind))
    {
        if ((idCmd == IDC_DSFIND) && (uFlags == GCS_HELPTEXT))
        {
            if (!LoadString(g_hInstance, IDS_FINDHELP, (LPTSTR)pszName, ccMax))
                ExitGracefully(hres, E_OUTOFMEMORY, "Failed to load help caption for verb");
        }
    }

    hres = S_OK;

exit_gracefully:

    TraceLeaveResult(hres);
}


// handle construction

STDAPI CDsFind_CreateInstance(IUnknown* punkOuter, IUnknown** ppunk, LPCOBJECTINFO poi)
{
    CFindMenu *pdf = new CFindMenu(*poi->pclsid);
    if (!pdf)
        return E_OUTOFMEMORY;

    HRESULT hres = pdf->QueryInterface(IID_IUnknown, (void **)ppunk);
    pdf->Release();
    return hres;
}
