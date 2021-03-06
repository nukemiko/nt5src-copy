#ifndef PASSWORD_H_INCLUDED
#define PASSWORD_H_INCLUDED


// password dialog (used for users cpl)

class CPasswordDialog: public CDialog
{
public:
    CPasswordDialog(TCHAR* pszResourceName, TCHAR* pszDomainUser, DWORD cchDomainUser, 
        TCHAR* pszPassword, DWORD cchPassword, DWORD dwError): 
        m_pszResourceName(pszResourceName),
        m_pszDomainUser(pszDomainUser),
        m_cchDomainUser(cchDomainUser),
        m_pszPassword(pszPassword),
        m_cchPassword(cchPassword),
        m_dwError(dwError)
        {}

protected:
    // Message handlers
    virtual INT_PTR DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
    BOOL OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);

    // Data
    TCHAR* m_pszResourceName;

    TCHAR* m_pszDomainUser;
    DWORD m_cchDomainUser;

    TCHAR* m_pszPassword;
    DWORD m_cchPassword;

    DWORD m_dwError;
};


// base class containing common stuff for the password prop page and set password dialog

class CPasswordPageBase
{
public:
    CPasswordPageBase(CUserInfo* pUserInfo): m_pUserInfo(pUserInfo) {}

protected:
    // Helpers
    BOOL DoPasswordsMatch(HWND hwnd);

protected:
    // Data
    CUserInfo* m_pUserInfo;
};

class CPasswordWizardPage: public CPropertyPage, public CPasswordPageBase
{
public:
    CPasswordWizardPage(CUserInfo* pUserInfo): CPasswordPageBase(pUserInfo) {}

protected:
    // Message handlers
    virtual INT_PTR DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
    BOOL OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
    BOOL OnNotify(HWND hwnd, int idCtrl, LPNMHDR pnmh);
};

class CChangePasswordDlg: public CDialog, public CPasswordPageBase
{
public:
    CChangePasswordDlg(CUserInfo* pUserInfo): CPasswordPageBase(pUserInfo) {}

    // Message handlers
    virtual INT_PTR DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
    BOOL OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
};



#endif //!PASSWORD_H_INCLUDED
