#ifndef __WEBBROWS_H__
#define __WEBBROWS_H__

// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CWebBrowser wrapper class

class CWebBrowser : public CWnd
{
protected:
    DECLARE_DYNCREATE(CWebBrowser)
public:
    CLSID const& GetClsid()
    {
        static CLSID const clsid
            = { 0xeab22ac3, 0x30c1, 0x11cf, { 0xa7, 0xeb, 0x0, 0x0, 0xc0, 0x5b, 0xae, 0xb } };
        return clsid;
    }
    virtual BOOL Create(LPCTSTR lpszClassName,
        LPCTSTR lpszWindowName, DWORD dwStyle,
        const RECT& rect,
        CWnd* pParentWnd, UINT nID,
        CCreateContext* pContext = NULL)
    { return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
        const RECT& rect, CWnd* pParentWnd, UINT nID,
        CFile* pPersist = NULL, BOOL bStorage = FALSE,
        BSTR bstrLicKey = NULL)
    { return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
        pPersist, bStorage, bstrLicKey); }

// Attributes
public:

// Operations
public:
    // method 'QueryInterface' not emitted because of invalid return type or parameter type
    unsigned long AddRef();
    unsigned long Release();
    // method 'GetTypeInfoCount' not emitted because of invalid return type or parameter type
    // method 'GetTypeInfo' not emitted because of invalid return type or parameter type
    // method 'GetIDsOfNames' not emitted because of invalid return type or parameter type
    // method 'Invoke' not emitted because of invalid return type or parameter type
    void GoBack();
    void GoForward();
    void GoHome();
    void GoSearch();
    void Navigate(LPCTSTR URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers);
    void Refresh();
    void Refresh2(VARIANT* Level);
    void Stop();
    LPDISPATCH GetApplication();
    LPDISPATCH GetParent();
    LPDISPATCH GetContainer();
    LPDISPATCH GetDocument();
    BOOL GetTopLevelContainer();
    CString GetType();
    long GetLeft();
    void SetLeft(long nNewValue);
    long GetTop();
    void SetTop(long nNewValue);
    long GetWidth();
    void SetWidth(long nNewValue);
    long GetHeight();
    void SetHeight(long nNewValue);
    CString GetLocationName();
    CString GetLocationURL();
    BOOL GetBusy();
};

#endif // __WEBBROWS_H__
