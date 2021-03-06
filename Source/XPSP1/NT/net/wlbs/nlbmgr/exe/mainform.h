#ifndef MAINFORM_H
#define MAINFORM_H

#include "stdafx.h"

#include "Document.h"

class MainForm : public CFrameWnd
{
    DECLARE_DYNCREATE( MainForm )

public:
    MainForm();
 	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
private:

    CToolBar         toolBar;
    CStatusBar       statusBar; 

    CSplitterWnd     splitterWindow;
    CSplitterWnd     splitterWindow2;

protected:

    // message handlers
    afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );

    afx_msg void clusterMenuClicked( CCmdUI* pCmdUI );

    afx_msg void hostMenuClicked( CCmdUI* pCmdUI );

    afx_msg void hostMenuClicked1( CCmdUI* pCmdUI );


    // overrides
    virtual
    BOOL
    OnCreateClient( LPCREATESTRUCT lpcs, CCreateContext* pContext );

    DECLARE_MESSAGE_MAP()
};

#endif

    
