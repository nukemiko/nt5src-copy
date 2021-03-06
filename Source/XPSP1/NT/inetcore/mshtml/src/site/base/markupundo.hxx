/*  MARKUPUNDO.HXX
 *
 *  Purpose:
 *      Classes to support undo of Markup operations
 *
 *  Authors:
 *      Joe Beda
 *
 *  Copyright (c) 1998, Microsoft Corporation. All rights reserved.
 */

#ifndef I_MARKUPUNDO_HXX_
#define I_MARKUPUNDO_HXX_
#pragma INCMSG("--- Beg 'markupundo.hxx'")

#ifndef X_MARKUP_HXX_
#define X_MARKUP_HXX_
#include "markup.hxx"
#endif

#ifndef X_UNDO_HXX_
#define X_UNDO_HXX_
#include "undo.hxx"
#endif

MtExtern(CInsertElementUndoUnit)
MtExtern(CRemoveElementUndoUnit)
MtExtern(CInsertTextUndoUnit)
MtExtern(CRemoveTextUndoUnit)
MtExtern(CInsertSpliceUndoUnit)
MtExtern(CRemoveSpliceUndoUnit)
MtExtern(CSelectionUndoUnit)
MtExtern(CDeferredSelectionUndoUnit)

//---------------------------------------------------------------------------
//
// CUndoHelper
//
//---------------------------------------------------------------------------

class CUndoHelper
{
public:
    CUndoHelper( CDoc * pDoc ) : _pDoc(pDoc) { Assert(pDoc); }

    BOOL    AcceptingUndo();

    virtual BOOL UndoDisabled();
    virtual IOleUndoUnit * CreateUnit() = 0;

    HRESULT CreateAndSubmit(BOOL fDirtyChange = TRUE );

protected:
    CDoc * _pDoc;
};


//---------------------------------------------------------------------------
//
// CMarkupUndoBase
//
//---------------------------------------------------------------------------

class CMarkupUndoBase : public CUndoHelper
{
public:
    CMarkupUndoBase( CDoc * pDoc, CMarkup * pMarkup, DWORD dwFlags ) 
        : CUndoHelper( pDoc )
        { Init( pMarkup, dwFlags ); }

    void Init( CMarkup * pMarkup, DWORD dwFlags );

    virtual BOOL UndoDisabled() { return _pMarkup->_fNoUndoInfo; }

protected:
    CMarkup *   _pMarkup;
    DWORD       _dwFlags;
};

inline void 
CMarkupUndoBase::Init( CMarkup * pMarkup, DWORD dwFlags )
{ 
    Assert( !pMarkup || pMarkup->Doc() == _pDoc ); 
    _pMarkup = pMarkup; 
    _dwFlags = dwFlags; 
}

//---------------------------------------------------------------------------
//
// CInsertElementUndo
//
//---------------------------------------------------------------------------

class CInsertElementUndo : public CMarkupUndoBase
{
public:
    CInsertElementUndo( CMarkup * pMarkup, DWORD dwFlags )
        : CMarkupUndoBase( pMarkup->Doc(), pMarkup, dwFlags ), _pElement( NULL )
            {}

    ~CInsertElementUndo()                   { CElement::ReleasePtr( _pElement ); }

    void    SetData( CElement * pElement);

    virtual IOleUndoUnit * CreateUnit();

protected:
    CElement *  _pElement;
};

//---------------------------------------------------------------------------
//
// CInsertElementUndoUnit
//
//---------------------------------------------------------------------------

class CInsertElementUndoUnit : public CUndoUnitBase
{
public:
    DECLARE_MEMCLEAR_NEW_DELETE(Mt(CInsertElementUndoUnit))

    CInsertElementUndoUnit(CDoc * pDoc);
    ~CInsertElementUndoUnit();

    void    SetData(CElement* pElement, DWORD dwFlags);

    HRESULT PrivateDo(IOleUndoManager *pUndoManager);

private:
    CElement * _pElement;
    DWORD      _dwFlags;
};

//---------------------------------------------------------------------------
//
// CRemoveElementUndo
//
//---------------------------------------------------------------------------

class CRemoveElementUndo : public CMarkupUndoBase
{
public:
    CRemoveElementUndo( CMarkup * pMarkup, CElement * pElementRemove, DWORD dwFlags );

    ~CRemoveElementUndo()                   { CElement::ReleasePtr( _pElement ); }

    void    SetData(long cpBegin, long cpEnd);

    virtual IOleUndoUnit * CreateUnit();

    BOOL    AcceptingUndo() { return _fAcceptingUndo; }

protected:
    BOOL                _fAcceptingUndo;

    CElement *  _pElement;
    long        _cpBegin;
    long        _cpEnd;
};

//---------------------------------------------------------------------------
//
// CRemoveElementUndoUnit
//
//---------------------------------------------------------------------------

class CRemoveElementUndoUnit : public CUndoUnitBase
{
public:
    DECLARE_MEMCLEAR_NEW_DELETE(Mt(CRemoveElementUndoUnit))

    CRemoveElementUndoUnit(CDoc * pDoc);
    ~CRemoveElementUndoUnit();

    void    SetData(CMarkup * pMarkup, CElement* pElement, long cpBegin, long cpEnd, DWORD dwFlags );

    HRESULT PrivateDo(IOleUndoManager *pUndoManager);

private:
    CMarkup *   _pMarkup;
    CElement *  _pElement;
    long        _cpBegin;
    long        _cpEnd;
    DWORD       _dwFlags;
};

//---------------------------------------------------------------------------
//
// CInsertSpliceUndo
//
//---------------------------------------------------------------------------

class CInsertSpliceUndo : public CMarkupUndoBase
{
public:
    CInsertSpliceUndo( CDoc * pDoc ) 
        : CMarkupUndoBase( pDoc, NULL, NULL ), _cpBegin( -1 )
            {}

    void    SetData( long cpBegin, long cpEnd )     { _cpBegin = cpBegin; _cpEnd = cpEnd; }

    virtual IOleUndoUnit * CreateUnit();

protected:
    long                _cpBegin;
    long                _cpEnd;
};

//---------------------------------------------------------------------------
//
// CInsertSpliceUndoUnit
//
//---------------------------------------------------------------------------

class CInsertSpliceUndoUnit : public CUndoUnitBase
{
public:
    DECLARE_MEMCLEAR_NEW_DELETE(Mt(CInsertSpliceUndoUnit))

    CInsertSpliceUndoUnit(CDoc * pDoc);
    ~CInsertSpliceUndoUnit();

    void    SetData( CMarkup * pMarkup, long cpBegin, long cpEnd, DWORD dwFlags );

    HRESULT PrivateDo(IOleUndoManager *pUndoManager);

private:
    CMarkup *           _pMarkup;
    long                _cpBegin;
    long                _cpEnd;
    DWORD               _dwFlags;
};

//---------------------------------------------------------------------------
//
// CRemoveSpliceUndo
//
//---------------------------------------------------------------------------

class CRemoveSpliceUndo : public CMarkupUndoBase
{
public:
    CRemoveSpliceUndo( CDoc * pDoc );
    ~CRemoveSpliceUndo() { delete _paryRegion; MemFree(_pchRemoved); }

    void    Init( CMarkup * pMarkup, DWORD dwFlags );

    void    SetText( long cpBegin, long cchRemoved, TCHAR * pchRemoved );

    virtual IOleUndoUnit * CreateUnit();

    BOOL    AcceptingUndo();

    CSpliceRecordList * _paryRegion;
    long                _cchRemoved;
    TCHAR *             _pchRemoved;

    BOOL                _fAcceptingUndo;

protected:

    long                _cpBegin;
};

//---------------------------------------------------------------------------
//
// CRemoveSpliceUndoUnit
//
//---------------------------------------------------------------------------

class CRemoveSpliceUndoUnit : public CUndoUnitBase
{
public:
    DECLARE_MEMCLEAR_NEW_DELETE(Mt(CRemoveSpliceUndoUnit))

    CRemoveSpliceUndoUnit(CDoc * pDoc);
    ~CRemoveSpliceUndoUnit();

    void    SetData( CMarkup * pMarkup, CSpliceRecordList * paryRegion, 
                     long cchRemoved, TCHAR * pchRemoved, 
                     long cpBegin, DWORD dwFlags );

    HRESULT PrivateDo(IOleUndoManager *pUndoManager);

private:
    CMarkup *           _pMarkup;
    CSpliceRecordList * _paryRegion;
    long                _cchRemoved;
    TCHAR *             _pchRemoved;
    long                _cpBegin;
    DWORD               _dwFlags;
};

#pragma INCMSG("--- End 'markupundo.hxx'")
#else
#pragma INCMSG("*** Dup 'markupundo.hxx'")
#endif
