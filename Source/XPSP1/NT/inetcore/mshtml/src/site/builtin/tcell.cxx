//+---------------------------------------------------------------------------
//
//  Microsoft Forms
//  Copyright (C) Microsoft Corporation, 1994-1996
//
//  File:       tcell.cxx
//
//  Contents:   CTableCell and related classes.
//
//----------------------------------------------------------------------------

#include "headers.hxx"

#pragma MARK_DATA(__FILE__)
#pragma MARK_CODE(__FILE__)
#pragma MARK_CONST(__FILE__)

#ifndef X_TABLE_HXX_
#define X_TABLE_HXX_
#include "table.hxx"
#endif

#ifndef X_OTHRGUID_H_
#define X_OTHRGUID_H_
#include "othrguid.h"
#endif

#ifndef X_DOWNLOAD_HXX_
#define X_DOWNLOAD_HXX_
#include "download.hxx"
#endif

#ifndef X_LTCELL_HXX_
#define X_LTCELL_HXX_
#include "ltcell.hxx"
#endif

#ifndef X_LTROW_HXX_
#define X_LTROW_HXX_
#include "ltrow.hxx"
#endif

#ifndef X_STRBUF_HXX_
#define X_STRBUF_HXX_
#include "strbuf.hxx"
#endif

#ifndef X_QI_IMPL_H_
#define X_QI_IMPL_H_
#include "qi_impl.h"
#endif

#ifndef X_SWITCHES_HXX_
#define X_SWITCHES_HXX_
#include "switches.hxx"
#endif

#ifndef X_PEER_HXX_
#define X_PEER_HXX_
#include "peer.hxx"
#endif

#ifndef X_MSHTMDID_H_
#define X_MSHTMDID_H_
#include "mshtmdid.h"
#endif

MtDefine(CTableCell, Elements, "CTableCell")
MtDefine(CTableCaption, Elements, "CTableCaption")
MtExtern(CharFormatSteal)
MtExtern(ParaFormatSteal)
MtExtern(FancyFormatSteal)

ExternTag(tagTableRecalc);
ExternTag(tagTableCalc);
ExternTag(tagFormatCaches);

CElement::ACCELS CTableCell::s_AccelsTCellRun    = CElement::ACCELS (&CTxtSite::s_AccelsTxtSiteRun,    IDR_ACCELS_TCELL_RUN);

#ifndef NO_PROPERTY_PAGE
const CLSID * const CTableCell::s_apclsidPages[] =
{
    // Browse-time pages
    NULL,
    // Edit-time pages
#if DBG==1
    &CLSID_CCDGenericPropertyPage,
    &CLSID_CInlineStylePropertyPage,
#endif
    NULL
};
#endif // NO_PROPERTY_PAGE


const CElement::CLASSDESC CTableCell::s_classdesc =
{
    {
        &CLSID_HTMLTableCell,           // _pclsid
        0,                              // _idrBase
#ifndef NO_PROPERTY_PAGE
        s_apclsidPages,                 // _apClsidPages
#endif // NO_PROPERTY_PAGE
        s_acpi,                         // _pcpi
        ELEMENTDESC_NEVERSCROLL    |
        ELEMENTDESC_TEXTSITE       |
        ELEMENTDESC_NOOFFSETCTX    |
        ELEMENTDESC_NOBKGRDRECALC  |
        ELEMENTDESC_TABLECELL,          // _dwFlags
        &IID_IHTMLTableCell,            // _piidDispinterface
        &s_apHdlDescs,                  // _apHdlDesc
    },
    (void *)s_apfnpdIHTMLTableCell,     // _pfnTearOff
    &s_AccelsTCellRun                   // _pAccelsRun
};


HRESULT
CTableCell::CreateElement(CHtmTag *pht,
                  CDoc *pDoc, CElement **ppElement)
{
    Assert(ppElement);

    *ppElement = new CTableCell(pht->GetTag(), pDoc);
    RRETURN ((*ppElement) ? S_OK : E_OUTOFMEMORY);
}

//+------------------------------------------------------------------------
//
//  Member:     CTableCell::PrivateQueryInterface, IUnknown
//
//  Synopsis:   Private unknown QI.
//
//-------------------------------------------------------------------------

HRESULT
CTableCell::PrivateQueryInterface(REFIID iid, void ** ppv)
{
    *ppv = NULL;
    switch(iid.Data1)
    {
        QI_HTML_TEAROFF(this, IHTMLTableCell2, NULL)
        default:
            RRETURN(THR_NOTRACE(super::PrivateQueryInterface(iid, ppv)));
    }

    if (!*ppv)
        RRETURN(E_NOINTERFACE);

    ((IUnknown *)*ppv)->AddRef();
    return S_OK;
}

//+------------------------------------------------------------------------
//
//  Member:     CTableCell::EnterTree, CElement
//
//  Synopsis:   Add the cell to the table.
//
//  Returns:    HRESULT
//
//-------------------------------------------------------------------------

HRESULT
CTableCell::EnterTree()
{
    HRESULT hr = S_OK;

    if (!GetMarkup()->_fInnerHTMLMarkup)
    {
        CTable *pTable = Table();
        CTableLayout * pTableLayout = pTable? pTable->TableLayoutCache() : NULL;

        // Only maintain the table layout cache incrementally until the table
        // has finished parsing.
        if (pTableLayout && (!pTableLayout->IsCompleted() || pTableLayout->_fTableOM))
        {
            if (IsCaption(Tag()))
            {
                CTableCaption * pCaption = DYNCAST(CTableCaption, this);

                Assert(pTableLayout);
                hr = pTableLayout->AddCaption(pCaption);
            }
            else
            {
                CTableRow *pRow = Row();
                if (pRow)
                {
                    CTableRowLayout * pRowLayout = pRow->RowLayoutCache();

                    Assert(pRowLayout);
                    hr = pRowLayout->AddCell(this);
                }
                else
                {
                    if (pTableLayout)
                        pTableLayout->MarkTableLayoutCacheDirty();
                }
            }
        }
    }

    RRETURN(hr);
}

//+------------------------------------------------------------------------
//
//  Member:     CTableCell::OnPropertyChange
//
//  Synopsis:   Process property changes on the cell
//
//-------------------------------------------------------------------------

HRESULT
CTableCell::OnPropertyChange(DISPID dispid, DWORD dwFlags, const PROPERTYDESC *ppropdesc)
{
    HRESULT hr;
    CTableCellLayout * pCellLayout = Layout();

    if (pCellLayout)
    {
        switch (dispid)
        {
        case DISPID_A_POSITION:
        // case DISPID_A_DISPLAY:   // this will be handled by ::Notify
            pCellLayout->HandlePositionDisplayChange();
            break;

        case DISPID_A_BORDERWIDTH:
        case DISPID_A_BORDERTOPWIDTH:
        case DISPID_A_BORDERRIGHTWIDTH:
        case DISPID_A_BORDERBOTTOMWIDTH:
        case DISPID_A_BORDERLEFTWIDTH:
            pCellLayout->ResetMinMax();
            break;

        case DISPID_IHTMLTABLECELL_ROWSPAN/*(DISPID_TABLECELL+1)*/:
        case DISPID_IHTMLTABLECELL_COLSPAN/*(DISPID_TABLECELL+2)*/: 
            {
                CTable           * pTable = Table();
                CTableLayout     * pTableLayout = pTable ? pTable->TableLayoutCache() : NULL;

                if (pTableLayout) 
                {
                    pTableLayout->Fixup();
                }
            }
            break;
        }
    }

    hr = THR(super::OnPropertyChange(dispid, dwFlags, ppropdesc));

    RRETURN(hr);
}

                    
//+------------------------------------------------------------------------
//
//  Member:     CTableCell::Save
//
//  Synopsis:   Save the cell to the stream
//
//-------------------------------------------------------------------------

HRESULT
CTableCell::Save(CStreamWriteBuff *pStmWrBuff, BOOL fEnd)
{
    HRESULT hr;

    hr = super::Save(pStmWrBuff, fEnd);
    if (hr)
        goto Cleanup;

    if (fEnd && pStmWrBuff->TestFlag(WBF_FORMATTED_PLAINTEXT))
    {
        // Save a space between <TD> tags duing plaintext save.
        hr = pStmWrBuff->Write(_T(" "));
        if (hr)
            goto Cleanup;
    }

Cleanup:
    RRETURN(hr);
}


//+------------------------------------------------------------------------
//
//  Member:     CTableCell::RowSpan, CTableCell
//
//  Synopsis:   Returns number of rows cell spans
//
//  Returns:    rows
//
//-------------------------------------------------------------------------

int
CTableCell::RowSpanHelper(int cRowSpan)
{
#if DBG==1
    CTableLayout * pTableLayout = Table()->TableLayoutCache();
#endif
    Assert(pTableLayout->IsTableLayoutCacheCurrent());

    CTableSection * pSection = Section();
    int iRowIdx = max(RowIndex(), 0);
    int cRowsLeftInSection   = pSection->_iRow + pSection->_cRows - iRowIdx;

    if (!cRowsLeftInSection)
    {
        // this can happen when a cell is cut out of a tree (treestress bug 72131)
        // and it winds up getting notifications in the process
        return 1;
    }

    // Rows left in sections include this table cell's row.
    Assert(cRowsLeftInSection >= 1 && cRowsLeftInSection <= pSection->_cRows);

    // Don't return a span going past the end of the section (or the number
    // of rows in the table for that matter).
    if (cRowSpan > cRowsLeftInSection)
    {
        // Section sanity check.
        Assert(pSection->_iRow + pSection->_cRows <= pTableLayout->GetRows());
        return cRowsLeftInSection;
    }
    else
    {
        // Since we are within the section we know we didn't pass the end
        // of the table.  Assert that.
        Assert(iRowIdx + cRowSpan <= pTableLayout->_aryRows.Size());

        return cRowSpan;
    }
}


//+------------------------------------------------------------------------
//
//  Member:     CTableCell::ApplyCellFormat
//
//  Synopsis:   Apply attributes
//
//  Arguments:  pCFI - Format Info needed for cascading
//
//  Returns:    HRESULT
//
//-------------------------------------------------------------------------

HRESULT
CTableCell::ApplyCellFormat(CFormatInfo *pCFI)
{
    CColorValue ccvBorderColor;
    CUnitValue  cuvZeroPoints, cuvZeroPercent, cuvInheritedWidth;
    LONG        lZeroPoints, lZeroPercent;
    BYTE        i;
    HRESULT     hr;

    BOOL fComputeFFOnly = pCFI->_pNodeContext->_iCF != -1;

    // Dont inherit down the no-break bit
    if (pCFI->_pcf->_fNoBreakInner)
    {
        pCFI->PrepareCharFormat();
        pCFI->_cf()._fNoBreakInner = FALSE;
        pCFI->UnprepareForDebug();
    }
    
    cuvZeroPoints.SetPoints(0);
    lZeroPoints = cuvZeroPoints.GetRawValue();
    cuvZeroPercent.SetValue(0, CUnitValue::UNIT_PERCENT);
    lZeroPercent = cuvZeroPercent.GetRawValue();

    if (!fComputeFFOnly)
    {
        if (Tag() == ETAG_TH)
        {
            pCFI->PrepareCharFormat();
            pCFI->_cf()._fBold = TRUE;
            pCFI->_cf()._wWeight = 700;
            pCFI->UnprepareForDebug();
        }
    }

    // Override inherited colors as necessary
    ccvBorderColor = GetAAborderColor();
    if (ccvBorderColor.IsDefined())
    {
        pCFI->PrepareFancyFormat();

        for (i = 0; i < SIDE_MAX; i++)
        {
            pCFI->_ff()._bd.SetBorderColor(i, ccvBorderColor);
        }

        pCFI->_ff()._bd._ccvBorderColorLight   =
        pCFI->_ff()._bd._ccvBorderColorHilight =
        pCFI->_ff()._bd._ccvBorderColorDark    =
        pCFI->_ff()._bd._ccvBorderColorShadow  = ccvBorderColor;
        pCFI->_ff()._bd._fOverrideTablewideBorderDefault = TRUE;

        pCFI->UnprepareForDebug();
    }

    ccvBorderColor = GetAAborderColorLight();
    if (ccvBorderColor.IsDefined())
    {
        pCFI->PrepareFancyFormat();
        pCFI->_ff()._bd._ccvBorderColorLight   =
        pCFI->_ff()._bd._ccvBorderColorHilight = ccvBorderColor;
        pCFI->_ff()._bd._fOverrideTablewideBorderDefault = TRUE;
        pCFI->UnprepareForDebug();
    }

    ccvBorderColor = GetAAborderColorDark();
    if (ccvBorderColor.IsDefined())
    {
        pCFI->PrepareFancyFormat();
        pCFI->_ff()._bd._ccvBorderColorDark    =
        pCFI->_ff()._bd._ccvBorderColorShadow  = ccvBorderColor;
        pCFI->_ff()._bd._fOverrideTablewideBorderDefault = TRUE;
        pCFI->UnprepareForDebug();
    }

    if (pCFI->_pff->_bd._bBorderColorsSetUnique)
    {
        pCFI->PrepareFancyFormat();
        pCFI->_ff()._bd._bBorderColorsSetUnique = FALSE;
        pCFI->UnprepareForDebug();
    }

    CUnitValue cuvNull; cuvNull.SetNull();
    for (i = 0; i < SIDE_MAX; i++)
    {
        if (    !pCFI->_pff->_bd.GetBorderWidth(i).IsNull()
            ||  pCFI->_pff->_bd.GetBorderStyle(i) != (BYTE)-1)
        {
            pCFI->PrepareFancyFormat();
            pCFI->_ff()._bd.SetBorderWidth(i, cuvNull);
            pCFI->_ff()._bd.SetBorderStyle(i, (BYTE)-1);
            pCFI->UnprepareForDebug();
        }
    }

    if (!fComputeFFOnly)
    {
        // Don't inherit blockquote indentation for all of the cells.

        if (    pCFI->_ppf->_cuvLeftIndentPoints.GetRawValue() != lZeroPoints
            ||  pCFI->_ppf->_cuvLeftIndentPercent.GetRawValue() != lZeroPercent
            ||  pCFI->_ppf->_cuvRightIndentPoints.GetRawValue() != lZeroPoints
            ||  pCFI->_ppf->_cuvRightIndentPercent.GetRawValue() != lZeroPercent
            ||  pCFI->_ppf->_cuvOffsetPoints.GetRawValue() != lZeroPoints
            ||  pCFI->_ppf->_cuvNonBulletIndentPoints.GetRawValue() != lZeroPoints
            ||  !pCFI->_ppf->_cListing.IsReset())
        {
            pCFI->PrepareParaFormat();
            pCFI->_pf()._cuvLeftIndentPoints = cuvZeroPoints;
            pCFI->_pf()._cuvLeftIndentPercent = cuvZeroPercent;
            pCFI->_pf()._cuvRightIndentPoints = cuvZeroPoints;
            pCFI->_pf()._cuvRightIndentPercent = cuvZeroPercent;
            pCFI->_pf()._cuvOffsetPoints = cuvZeroPoints;
            pCFI->_pf()._cuvNonBulletIndentPoints = cuvZeroPoints;
            pCFI->_pf()._cListing.Reset();
            pCFI->UnprepareForDebug();
        }
    }

    //
    // Save and clear out any inherited width
    //


    cuvInheritedWidth = pCFI->_pff->GetWidth();
    if (!cuvInheritedWidth.IsNull())
    {
        pCFI->PrepareFancyFormat();
        pCFI->_ff().ClearWidth();
        pCFI->UnprepareForDebug();
    }

    //
    // Apply standard formats
    //

    Assert(pCFI->_pNodeContext && SameScope(pCFI->_pNodeContext, this));
    hr = THR( ApplyDefaultFormat(pCFI) );
    if (hr)
        goto Cleanup;

    //
    // Clear all margin values
    // NOTE: Margins are not supported on cells or captions
    //

    if (    pCFI->_pff->_cuvSpaceBefore.GetRawValue() != lZeroPoints
        ||  pCFI->_pff->_cuvSpaceAfter.GetRawValue() != lZeroPoints
        ||  !pCFI->_pff->GetMargin(SIDE_TOP).IsNull()
        ||  !pCFI->_pff->GetMargin(SIDE_RIGHT).IsNull()
        ||  !pCFI->_pff->GetMargin(SIDE_BOTTOM).IsNull()
        ||  !pCFI->_pff->GetMargin(SIDE_LEFT).IsNull())
    {
        //
        // Note: (srinib) when margins are implemented  on table cell's,
        // verify ApplyInnerOuterFormats for margins.
        //
        CUnitValue uvNull(0, CUnitValue::UNIT_NULLVALUE);

        pCFI->PrepareFancyFormat();
        pCFI->_ff()._cuvSpaceBefore = cuvZeroPoints;
        pCFI->_ff()._cuvSpaceAfter = cuvZeroPoints;
        pCFI->_ff().SetExplicitMargin(SIDE_TOP, FALSE);
        pCFI->_ff().SetExplicitMargin(SIDE_RIGHT, FALSE);
        pCFI->_ff().SetExplicitMargin(SIDE_BOTTOM, FALSE);
        pCFI->_ff().SetExplicitMargin(SIDE_LEFT, FALSE);
        pCFI->_ff().SetMargin(SIDE_TOP, uvNull);
        pCFI->_ff().SetMargin(SIDE_RIGHT, uvNull);
        pCFI->_ff().SetMargin(SIDE_BOTTOM, uvNull);
        pCFI->_ff().SetMargin(SIDE_LEFT, uvNull);
        pCFI->_ff()._fHasMargins = FALSE;
        pCFI->UnprepareForDebug();
    }

    //
    // If there was no explicit width on the cell, use the inherited width
    //

    if (pCFI->_pff->GetWidth().IsNull())
    {
        _fInheritedWidth = TRUE;

        pCFI->PrepareFancyFormat();
        pCFI->_ff().SetWidth(cuvInheritedWidth);
        pCFI->UnprepareForDebug();
    }

    if (!fComputeFFOnly)
    {
        if (Tag() == ETAG_CAPTION || Tag() == ETAG_TC)
        {
            // Note: The caption.pdl specifies the align as DISPID_A_BLOCKALIGN - which means
            // it gets written into the _blockAlign - but caption has two extra ( vertical )
            // enum values that we need to map onto the equivalent vertical alignment values.
            // The code that handles this maping you can find in ApplyParagraphAlignment() (cpfp.cxx)

            // Map Caption enum to ControlAlign enum
            if (    pCFI->_ppf->_bTableVAlignment == htmlCaptionVAlignTop
                ||  pCFI->_ppf->_bTableVAlignment == htmlCaptionAlignTop)
            {
                pCFI->PrepareParaFormat();
                pCFI->_pf()._bTableVAlignment = htmlCellVAlignTop;
                DYNCAST(CTableCaption,this)->_uLocation = CTableCaption::CAPTION_TOP;
                pCFI->UnprepareForDebug();
            }
            else if (   pCFI->_ppf->_bTableVAlignment == htmlCaptionVAlignBottom
                    ||  pCFI->_ppf->_bTableVAlignment == htmlCaptionAlignBottom)
            {
                pCFI->PrepareParaFormat();
                pCFI->_pf()._bTableVAlignment = htmlCellVAlignBottom;
                DYNCAST(CTableCaption,this)->_uLocation = CTableCaption::CAPTION_BOTTOM;
                pCFI->UnprepareForDebug();
            }
        }
    }

Cleanup:
    RRETURN(hr);
}

static HRESULT VerticalAlignFromCellVAlign(CFormatInfo * pCFI)
{
    //
    // Set only block align.
    // 

    HRESULT hr = S_OK;
    htmlControlAlign   ca;  

    switch ((htmlCellVAlign)pCFI->_ppf->_bTableVAlignment)
    {
    case htmlCellVAlignNotSet:
        ca = htmlControlAlignNotSet;
        break;
    case htmlCellVAlignTop:
        ca = htmlControlAlignTop;
        break;
    case htmlCellVAlignMiddle:
        ca = htmlControlAlignMiddle;
        break;
    case htmlCellVAlignBottom:
        ca = htmlControlAlignBottom;
        break;
    case htmlCellVAlignBaseline:
        ca = htmlControlAlignBaseline;
        break;
    default:
        ca = htmlControlAlignNotSet;   // keep compiler happy
        Assert(0);
        hr = E_INVALIDARG;
        break;
    }

    if (pCFI->_pff->_bControlAlign != ca)
    {
        pCFI->PrepareFancyFormat();
        pCFI->_ff()._bControlAlign = ca;
        pCFI->UnprepareForDebug();
    }

    RRETURN(hr);
}

// Look at two different elements and determine if their formats might be different.
static BOOL PotentiallyDifferentFormats (CElement *pElement1, CElement *pElement2)
{
    // If both are missing, then we're the same.
    if (pElement1 != NULL || pElement2 != NULL)
    {
        // Let's compare class and ID.
        if (pElement2 != NULL && pElement1 != NULL)
        {
            LPCTSTR pstr;
            LPCTSTR pstr1;

            // Are there classes at all?
            pstr=pElement2->GetAAclassName();
            pstr1=pElement1->GetAAclassName();
            if (pstr1 != NULL || pstr != NULL)
            {
                if (pstr1 != NULL && pstr != NULL)
                {
                    if (_tcscmp(pstr, pstr1) != 0)
                        goto Cleanup;
                }
                else
                    goto Cleanup;
            }

            // Same thing for the IDs.
            pstr1=pElement2->GetAAid();
            pstr=pElement1->GetAAid();
            if (pstr1 != NULL || pstr != NULL)
            {
                if (pstr1 != NULL && pstr != NULL)
                {
                    if (_tcscmp(pstr, pstr1) != 0)
                        goto Cleanup;
                }
                else
                    goto Cleanup;
            }
        }
        // One element exists the other is NULL.
        else
            goto Cleanup;
    }

    return FALSE;

Cleanup:
    return TRUE;
}

//+------------------------------------------------------------------------
//
//  Member:     CTableCell::CheckSameFormat
//
//  Synopsis:   Check if the other cell in the other row/col has the same
//              format and use it if it is...
//-------------------------------------------------------------------------

BOOL
CTableCell::CheckSameFormat
(
    CTreeNode  * pNodeTarget,
    CTableCell * pCell1,
    CTableRow * pRow,
    CTableRow * pRow1,
    CTableCol * pCol,
    CTableCol * pCol1
)
{
    Assert(this == pNodeTarget->Element());

    // If we are computing formats for pseudo elements, then we cannot steal formats
    // since the formats we will compute will be different than the normal stolen
    // formats because of the first line/letter settings (bug 87707)
    if (GetMarkup()->HasCFState() || !pCell1->_fStealingAllowed)
        return FALSE;

    BOOL fSameRow = pRow == pRow1;
    BOOL fSameCol = pCol == pCol1;
    BOOL fPotentiallyDifferent=FALSE;   // Optimism.
    // Do we have the same class or ID as the column we're comparing?
    if (fSameRow)
    {
        fPotentiallyDifferent = PotentiallyDifferentFormats (pCol, pCol1);
    }
    else if (fSameCol)
    {
        fPotentiallyDifferent = PotentiallyDifferentFormats (pRow, pRow1);
    }

    if (fPotentiallyDifferent)
    {
        return FALSE;
    }

    CTreeNode * pNodeCell1 = pCell1->GetFirstBranch();
    CTreeNode * pNodeRow   = pRow->GetFirstBranch();
    CTreeNode * pNodeRow1  = pRow1->GetFirstBranch();
    CTreeNode * pNodeCol   = pCol? pCol->GetFirstBranch() : NULL;
    CTreeNode * pNodeCol1  = pCol1? pCol1->GetFirstBranch() : NULL;

    if (pNodeCell1->_iFF >= 0)
    {
        const CFancyFormat *pFF = GetFancyFormatEx(pNodeCell1->_iFF);

        if (pFF->_pszFilters || pFF->_fHasExpressions)
            return FALSE;
    }

    if (pNodeRow1->_iFF >= 0)
    {
        const CFancyFormat *pFF = GetFancyFormatEx(pNodeRow1->_iFF);

        // Table rows don't have filters
        if (pFF->_fHasExpressions)
            return FALSE;
    }

    if (HasPeerHolder() && GetPeerHolder()->TestFlagMulti(CPeerHolder::NEEDAPPLYSTYLE))
    {
        return FALSE;
    }

    if (pNodeCell1->_iPF >= 0 &&
        (_pAA == NULL && pCell1->_pAA == NULL ||
         (_pAA != NULL && pCell1->_pAA != NULL && _pAA->Compare(pCell1->_pAA)) ) &&
        ((pNodeRow->_iPF >= 0 && pRow == pRow1) ||
            (pNodeRow1->_iPF >= 0 &&
            pNodeRow->_iPF == pNodeRow1->_iPF &&
            pNodeRow->_iCF == pNodeRow1->_iCF &&
            pNodeRow->_iFF == pNodeRow1->_iFF)) &&
        ((!pCol && !pCol1) ||
            (pCol && pCol1 && ((pNodeCol->_iPF >= 0 && pCol == pCol1) ||
                               (pNodeCol1->_iPF >= 0 &&
                                pNodeCol->_iPF == pNodeCol1->_iPF &&
                                pNodeCol->_iCF == pNodeCol1->_iCF &&
                                pNodeCol->_iFF == pNodeCol1->_iFF)))))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


//+------------------------------------------------------------------------
//
//  Function:   ApplyFormatUptoTable
//
//  Synopsis:   Apply format of all the elements above the passed element (p)
//              upto the table element (not including the table element)
//
//              We need this function because of ETAG_TC elemnts exclude
//              ETAG_TABLE elements when we apply the format to them.
//
//  Arguments:  pCFI - Format Info needed for cascading
//              p - Element to start from
//
//  Note:       This is a recursive function. It can be written in a generic
//              way if anybody would need this function (we could of passed
//              the ETAG_TABLE as a parameter). But since there is no need
//              for it yet (it is remainig as a part of the table code)
//
//-------------------------------------------------------------------------

HRESULT
CTableCell::ApplyFormatUptoTable(CFormatInfo * pCFI)
{
    CTreeNode * pNodeParent = pCFI->_pNodeContext->Parent();
    HRESULT     hr = S_OK;

    if (pNodeParent->Tag() != ETAG_TABLE)
    {
        pCFI->_pNodeContext = pNodeParent;
        hr = ApplyFormatUptoTable(pCFI);
        if (hr)
            goto Cleanup;
        pCFI->_pNodeContext = pNodeParent;
        hr = THR(pNodeParent->Element()->ApplyDefaultFormat(pCFI));
    }

Cleanup:
    RRETURN (hr);
}


//+------------------------------------------------------------------------
//
//  Member:     CTableCell::ComputeFormatsVirtual
//
//  Synopsis:   Compute Char and Para formats induced by this element and
//              every other element above it in the HTML tree.
//
//  Arguments:  pCFI - Format Info needed for cascading
//
//  Returns:    HRESULT
//
//  Note:       We override this here to put our defaults into the format
//              FIRST, and also to cache vertical alignment here in the object
//
//-------------------------------------------------------------------------

HRESULT
CTableCell::ComputeFormatsVirtual(CFormatInfo * pCFI, CTreeNode * pNodeTarget)
{
    SwitchesBegTimer(SWITCHES_TIMER_COMPUTEFORMATS);

    // (KTam) This is bogus, the only reason we ask for layout in this fn
    // is so we can check _fDisplayNoneCell (which ought to be obtained
    // from the cell element).  In the meantime, first layout is OK.
    CTableCellLayout   *pLayout = Layout(GUL_USEFIRSTLAYOUT);
    CTableCol *         pCol = NULL;
    CTableCol *         pColGroup = NULL;
    CTableRow *         pRow = NULL;
    CTableSection *     pSection = NULL;
    ELEMENT_TAG         etag;
    CTable *            pTable;
    CTableLayout *      pTableLayout = NULL;
    const CParaFormat * pPFRow = NULL;
    const CParaFormat * pPFCol = NULL;
    const CParaFormat * pPFColGroup = NULL;
    HRESULT             hr = S_OK;
    CUnitValue          cuvColWidth;
    CUnitValue          cuvZeroPoints;
    CUnitValue          cuvZeroPercent;
    WHEN_DBG( BOOL      fDisableTLCAssert = FALSE; )
    CTreeNode         * pNodeTable;
    CTreeNode         * pNodeRow = NULL;
    CTreeNode         * pNodeCol = NULL;
    CTreeNode         * pNodeColGroup = NULL;
    CTableCell        * pCellNeighbor = NULL;
    CTableCol *         pColNeighbor = NULL;
    CTreeNode         * pNodeInherit;
    THREADSTATE *       pts = GetThreadState();
    LONG                lZeroPoints;
    BOOL                fComputeFFOnly = pNodeTarget->_iCF != -1;
    BOOL                fSetWidthFromCol = FALSE;
    COMPUTEFORMATSTYPE  eExtraValues = pCFI->_eExtraValues;

    if(eExtraValues == ComputeFormatsType_GetInheritedIntoTableValue)
    {
        // It is not violated, we just want to disable the format stealing stuff when
        // the puprose of calling ComputeFormats is to get the inherited value.
        goto TableStructureViolated;
    }


    cuvZeroPercent.SetValue(0, CUnitValue::UNIT_PERCENT);
    cuvZeroPoints.SetPoints(0);
    lZeroPoints = cuvZeroPoints.GetRawValue();

    Assert(pCFI);
    Assert( SameScope( this, pNodeTarget ) );

    etag         = Tag();
    pTable       = Table();

    // If we are not in a table, we are messed up, and we'll just
    // use the CElement::ComputeFormat handling.
    if (!pTable)
        goto TableStructureViolated;

    pNodeTable   = pTable->GetFirstBranch();
    pTableLayout = pTable->TableLayoutCache();

    WHEN_DBG( fDisableTLCAssert = pTableLayout->_fDisableTLCAssert;
              pTableLayout->_fDisableTLCAssert = TRUE; )

    // Note:  We are not doing a pTableLayout->EnsureTableLayoutCache()
    // even though the current version might be dirty (can happen in
    // databinding).  In those cases we WANT to work with the old version
    // in case elements would disappear on us.

    // if there are no attributes to worry about try to use an other cell's format
    if (!IsCaption(etag))
    {
        Assert(!pCol || SameScope(pCol->GetFirstBranch()->Ancestor(ETAG_COLGROUP), pColGroup));

        pSection  = Section();
        pRow      = Row();
        pColGroup = pTableLayout->GetColGroup(ColIndex());
        pCol      = pTableLayout->GetCol(ColIndex());

        if (!pRow || !pSection)
            goto TableStructureViolated;

        // make sure row and col cached it's format...
        pNodeRow = pRow->GetFirstBranch();
        pPFRow = pNodeRow->GetParaFormat();
        if (pColGroup)
        {
            pNodeColGroup = pColGroup->GetFirstBranch();
            if (pNodeColGroup)
                pPFColGroup = pNodeColGroup->GetParaFormat();
        }
        if (pCol)
        {
            pNodeCol = pCol->GetFirstBranch();
            pPFCol = pNodeCol->GetParaFormat();
        }

        if (   (eExtraValues == ComputeFormatsType_Normal) 
            && pTableLayout->IsTableLayoutCacheCurrent() 
            && !pLayout->_fDisplayNoneCell)
        {
            CTableRow *       pRowNeighbor;
            CTableRowLayout * pRowLayout = pRow->RowLayoutCache();
            int               iCol = ColIndex(), iRow;

            iRow = pRow->_iRow;   // faster then  = RowIndex()

            // try to steal format from the cell in the same column from the previous data-bound section
            if (pTableLayout->IsGenerated(iRow) && pSection != pTableLayout->_aryBodys[0])
            {
                // use the template's format
                iRow = iRow - pSection->_iRow; // relative row number
                Assert (iRow < pSection->_cRows);
                if((pTableLayout->_aryBodys[0]->_iRow + iRow) >= 0)
                {
                    pRowNeighbor = pTableLayout->_aryRows[pTableLayout->_aryBodys[0]->_iRow + iRow];
                    if (pRowNeighbor && pRowNeighbor->RowLayoutCache()->GetCells() > iCol)
                    {
                        pCellNeighbor = Cell(pRowNeighbor->RowLayoutCache()->_aryCells[iCol]);
                        if (pCellNeighbor && CheckSameFormat(pNodeTarget, pCellNeighbor, pRow, pRowNeighbor, pCol, pCol))
                        {
                            goto StealFormat;
                        }
                    }
                }
            }

            // try to steal format from the cell in the same row from the previous column 
            if (      iCol > 0 && iCol <= pRowLayout->GetCells()            
                   && (pCellNeighbor = pRowLayout->GetCell(iCol-1)) != this  
                   && pCellNeighbor
                   && pCellNeighbor->Section() == pSection
                   && pTableLayout->GetColGroup(iCol-1) == pColGroup
                   && (pColNeighbor = pTableLayout->GetCol(iCol-1)) == pCol
                   && pCellNeighbor->Tag() == etag)
            {
                if (CheckSameFormat(pNodeTarget, pCellNeighbor, pRow, pRow, pCol, pColNeighbor))
                {
                    goto StealFormat;
                }
            }

            // try to steal format from the cell in the same column from the previous row
            if (iRow > 0 && iRow <= pTableLayout->GetRows())
            {
                pRowNeighbor = pTableLayout->GetRow(iRow-1);
                CTableRowLayout *pRowNeighborLayout = pRowNeighbor->RowLayoutCache();
                if (pRowNeighborLayout->GetCells() > iCol                       &&
                    (pCellNeighbor = pRowNeighborLayout->GetCell(iCol)) != this &&
                    pCellNeighbor                                               &&
                    IsReal(pRowNeighborLayout->_aryCells[iCol])                 &&
                    pCellNeighbor->Section() == pSection                        &&
                    pCellNeighbor->Tag() == etag)
                {
                    Assert(pCellNeighbor->Layout(GUL_USEFIRSTLAYOUT)->ColGroup() == pColGroup);
                    if (CheckSameFormat(pNodeTarget, pCellNeighbor, pRow, pRowNeighbor, pCol, pCol))
                    {
                        goto StealFormat;
                    }
                }
            }
        }
    }

    if (etag == ETAG_TC)
        pNodeInherit = pNodeTable->Parent();
    else if(etag == ETAG_CAPTION)
        pNodeInherit = pNodeTable;
    else if (!pCol || !pColGroup)
        pNodeInherit = pNodeRow;
    else
        pNodeInherit = pNodeTable;

    if (    pNodeInherit->_iCF == -1
        ||  pNodeInherit->_iFF == -1)
    {
        SwitchesEndTimer(SWITCHES_TIMER_COMPUTEFORMATS);

        hr = THR(pNodeInherit->Element()->ComputeFormats(pCFI, pNodeInherit));

        SwitchesBegTimer(SWITCHES_TIMER_COMPUTEFORMATS);

        if (hr)
            goto Cleanup;
    }

    Assert(pNodeInherit->_iCF >= 0);
    Assert(pNodeInherit->_iPF >= 0);
    Assert(pNodeInherit->_iFF >= 0);
    //
    // NOTE: From this point forward any errors must goto Error instead of Cleanup!
    //

    pCFI->Reset();

    //
    // Setup Fancy Format
    //

    if (IsCaption(etag))
    {
        pCFI->_iffSrc = pts->_iffDefault;
        pCFI->_pffSrc = pCFI->_pff = pts->_pffDefault;
        Assert(pCFI->_pffSrc->_pszFilters == NULL);
    }
    else
    {
        pCFI->_iffSrc = pNodeInherit->_iFF;
        pCFI->_pffSrc = pCFI->_pff = &(*pts->_pFancyFormatCache)[pCFI->_iffSrc];
        pCFI->_fHasExpandos = (pCFI->_pff->_iExpandos >= 0);

        if (pCFI->_pff->_pszFilters)
        {
            // note: we are letting the cell inherit zoom and rotation from row, because row can't have a layout
            pCFI->PrepareFancyFormat();
            pCFI->_ff()._pszFilters = NULL;
            pCFI->UnprepareForDebug();
        }
    }

    //
    // Setup Char and Para formats
    //

    pCFI->_icfSrc = pNodeInherit->_iCF;
    pCFI->_pcfSrc = pCFI->_pcf = &(*pts->_pCharFormatCache)[pCFI->_icfSrc];
    pCFI->_ipfSrc = pNodeInherit->_iPF;
    pCFI->_ppfSrc = pCFI->_ppf = &(*pts->_pParaFormatCache)[pCFI->_ipfSrc];

    //
    // Apply formats
    //

    if (etag == ETAG_TC)
    {
        // We treat <TC> (text in between table cells) as if the text is outside the
        // table for Netscape compatibility, so do not apply the table's format.

        pCFI->_pNodeContext = pNodeTarget;
        hr = ApplyFormatUptoTable(pCFI);
        if (hr)
            goto Error;
    }

    if (!fComputeFFOnly)
    {
        // Because we're re-using the pCFI between applies, we need to explictly
        // clear those members that we don't want to "inherit" as we apply.
        // For example, width/height should not be inherited from the table

        if (    pCFI->_ppf->_bBlockAlign != htmlAlignNotSet
            ||  pCFI->_ppf->_cuvRightIndentPoints.GetRawValue() != lZeroPoints
            ||  pCFI->_ppf->_cuvLeftIndentPoints.GetRawValue() != lZeroPoints)
        {
            pCFI->PrepareParaFormat();
            pCFI->_pf()._bBlockAlign = htmlAlignNotSet;
            pCFI->_pf()._cuvRightIndentPoints.SetRawValue(lZeroPoints);
            pCFI->_pf()._cuvLeftIndentPoints.SetRawValue(lZeroPoints);
            pCFI->UnprepareForDebug();
        }
    }

    //
    //  Initialize padding with the CELLPADDING attribute
    //

    if (etag != ETAG_CAPTION)
    {
        CUnitValue uvPadding = pTable->GetAAcellPadding();

        if (uvPadding.IsNull())
        {
            uvPadding.SetValue(1, CUnitValue::UNIT_PIXELS);
        }

        if (uvPadding.GetPixelValue())
        {
            pCFI->PrepareFancyFormat();
            pCFI->_ff().SetPadding(SIDE_LEFT, uvPadding);
            pCFI->_ff().SetPadding(SIDE_RIGHT, uvPadding);
            pCFI->_ff().SetPadding(SIDE_TOP, uvPadding);
            pCFI->_ff().SetPadding(SIDE_BOTTOM, uvPadding);
            pCFI->UnprepareForDebug();
        }
    }

    else if (!pTableLayout->_fCollapse)
    {
        // Add caption spacing for Netscape compatibility.
        CUnitValue          uvPadding = pTable->GetAAcellPadding();
        CUnitValue          uvSpacing = pTable->GetAAcellSpacing();
        CUnitValue          uvBorder  = pTable->GetAAborder();
        CUnitValue          uv;
        htmlCaptionVAlign   valign;
        long                cyPaddingBottom;

        if (uvPadding.IsNull())
        {
            uvPadding.SetValue(1, CUnitValue::UNIT_PIXELS);
        }

        if (uvSpacing.IsNull())
        {
            uvSpacing.SetValue(2, CUnitValue::UNIT_PIXELS);
        }

        if (uvBorder.IsNull())
        {
            uvBorder.SetValue(0, CUnitValue::UNIT_PIXELS);
        }

        if (    uvPadding.GetPixelValue()
            ||  uvSpacing.GetPixelValue()
            ||  uvBorder.GetPixelValue())
        {
            pCFI->PrepareFancyFormat();
            pCFI->_ff().SetPadding(SIDE_TOP, uvPadding);

            uv.SetValue(uvPadding.GetPixelValue() + uvSpacing.GetPixelValue() + uvBorder.GetPixelValue(), CUnitValue::UNIT_PIXELS);
            pCFI->_ff().SetPadding(SIDE_LEFT, uv);
            pCFI->_ff().SetPadding(SIDE_RIGHT, uv);

            CAttrArray::FindSimple( *GetAttrArray(), &s_propdescCTableCaptionvAlign.a, (DWORD *)&valign);

            if (valign == htmlCaptionVAlignNotSet)
            {
                CAttrArray::FindSimple( *GetAttrArray(), &s_propdescCTableCaptionalign.a, (DWORD *)&valign);

                valign = (valign == htmlCaptionAlignBottom
                                ? htmlCaptionVAlignBottom
                                : htmlCaptionVAlignTop);
            }

            // Netscape: doesn't adjust TOP for a caption, instead it adjust the bottom of the "TOP" captions
            cyPaddingBottom = uvPadding.GetPixelValue() + (valign == htmlCaptionVAlignBottom
                                                                ? uvSpacing.GetPixelValue()
                                                                : uvBorder.GetPixelValue() + (2 * uvSpacing.GetPixelValue()));

            uv.SetValue(cyPaddingBottom, CUnitValue::UNIT_PIXELS);
            pCFI->_ff().SetPadding(SIDE_BOTTOM, uv);
            pCFI->UnprepareForDebug();
        }
    }

    if (!fComputeFFOnly)
    {
        if (pCFI->_pcf->_fRelative)
        {
            pCFI->PrepareCharFormat();
            pCFI->_cf()._fRelative = FALSE;
            pCFI->UnprepareForDebug();
        }
    }

    if (etag != ETAG_CAPTION && etag != ETAG_TC)
    {
        if (pCol || pColGroup)
        {
            if (pColGroup)
            {
                pCFI->_pNodeContext = pColGroup->GetFirstBranch();
                hr = THR(pColGroup->ApplyDefaultFormat(pCFI));
                if (hr)
                    goto Error;
            }

            if (pCol)
            {
                pCFI->_pNodeContext = pCol->GetFirstBranch();
                hr = THR(pCol->ApplyDefaultFormat(pCFI));
                if (hr)
                    goto Error;
            }

            // Cache column width.
            // Table is always horizontal => width is in physical coordinate system.
            cuvColWidth = pCFI->_pff->GetWidth();
            fSetWidthFromCol = !cuvColWidth.IsNull();

            pCFI->_pNodeContext = pSection->GetFirstBranch();
            hr = THR(pSection->ApplyDefaultFormat(pCFI));
            if (hr)
                goto Error;

            Assert (pNodeRow);
            pCFI->_pNodeContext = pNodeRow;
            hr = THR(pRow->ApplyDefaultFormat(pCFI));
            if (hr)
                goto Error;

        }

        if (!fComputeFFOnly)
        {
            // (alexa) The following is a precedence order for each attribute (HTML3 Table Model Spec),
            // where x > y denotes that x takes precedence over y
            // for ALIGN (_blockAlign), CHAR and CHAROFF
            // cell > column > column groups > row > row groups > default
            // for VALIGN (_bTableVAlignment), LANG, DIR, STYLE
            // cell > row > row groups > column > column groups > table > default

            // 1a. Set default ALIGN property
            if (pCFI->_bBlockAlign == htmlBlockAlignNotSet &&
                etag == ETAG_TH)
            {
                // By default, htmlAlignNotSet is htmlAlignLeft, so do not set it to
                // htmlAlignLeft it will mess up alignment on contained sites which
                // are not aligned left by default (ex. HR).
                pCFI->_bBlockAlign = htmlBlockAlignCenter;

            }

            // 2a. Apply horizontal alignment for table cells in different order
            if (pPFRow->_bBlockAlignInner != htmlBlockAlignNotSet)
            {
                pCFI->_bBlockAlign = pPFRow->_bBlockAlignInner;
            }

            // 3a. then apply alignment from column groups
            if (pPFColGroup && pPFColGroup->_bBlockAlign != htmlBlockAlignNotSet)
            {
                pCFI->_bBlockAlign = pPFColGroup->_bBlockAlign;
            }

            // 3b. then inherit align from columns
            if (pPFCol && pPFCol->_bBlockAlign != htmlBlockAlignNotSet)
            {
                pCFI->_bBlockAlign = pPFCol->_bBlockAlign;
            }

            // 4a. Cell will apply its own ALIGN format if specified when we call ApplyCellFormat()

            // 1b. Set default VALIGN format if it was not set by COLs and Rows already,
            // if it was set, the correct precedence already took place during
            // COL and ROW ApplyFormat() above.
            if (pCFI->_ppf->_bTableVAlignment == htmlCellVAlignNotSet)
            {
                Assert(etag == ETAG_TH || etag == ETAG_TD || etag == ETAG_TC);
                pCFI->PrepareParaFormat();
                pCFI->_pf()._bTableVAlignment = htmlCellVAlignMiddle;
                pCFI->UnprepareForDebug();
            }
        }
    }
    else
    {
        if (!fComputeFFOnly)
        {
            // set the default ALIGN/VALIGN format for CAPTION
            pCFI->_bBlockAlign = (etag == ETAG_CAPTION)? htmlBlockAlignCenter : htmlBlockAlignNotSet;

            if (pCFI->_ppf->_bTableVAlignment != htmlCellVAlignTop)
            {
                pCFI->PrepareParaFormat();
                pCFI->_pf()._bTableVAlignment = htmlCellVAlignTop;
                pCFI->UnprepareForDebug();
            }
        }
    }

    if (    pCFI->_pff->_cuvSpaceBefore.GetRawValue() != lZeroPoints
        ||  pCFI->_pff->_cuvSpaceAfter.GetRawValue() != lZeroPoints
        ||  !pCFI->_pff->GetWidth().IsNull()
        ||  !pCFI->_pff->GetPosition(SIDE_TOP).IsNull()
        ||  !pCFI->_pff->GetPosition(SIDE_LEFT).IsNull()
        ||  !pCFI->_pff->GetMargin(SIDE_TOP).IsNull()
        ||  !pCFI->_pff->GetMargin(SIDE_RIGHT).IsNull()
        ||  !pCFI->_pff->GetMargin(SIDE_BOTTOM).IsNull()
        ||  !pCFI->_pff->GetMargin(SIDE_RIGHT).IsNull()
        ||  pCFI->_pff->_bDisplay != styleDisplayNotSet
        ||  pCFI->_pff->_bPositionType != stylePositionNotSet
        ||  pCFI->_pff->_fPositioned
        ||  pCFI->_pff->_fAutoPositioned
        ||  pCFI->_pff->_fScrollingParent
        ||  pCFI->_pff->_fZParent
        ||  pCFI->_pff->_iExpandos != -1
        ||  pCFI->_pff->_iCustomCursor != -1
        ||  pCFI->_pff->_iPEI != -1
        ||  pCFI->_pff->_fHasExpressions != 0
        ||  pCFI->_pff->_lZIndex != 0
        ||  pCFI->_pff->_pszFilters
        ||  pCFI->_pff->_bPageBreaks != 0
        ||  pCFI->_pff->HasCSSVerticalAlign()
        ||  pCFI->_pff->GetVerticalAlign() != styleVerticalAlignNotSet
        ||  pCFI->_pff->_fLayoutFlowChanged
        ||  fSetWidthFromCol)
    {
        CUnitValue uvNull(0, CUnitValue::UNIT_NULLVALUE);

        pCFI->PrepareFancyFormat();
        pCFI->_ff()._cuvSpaceBefore.SetValue(0, CUnitValue::UNIT_POINT);
        pCFI->_ff()._cuvSpaceAfter.SetValue(0, CUnitValue::UNIT_POINT);
        pCFI->_ff().ClearWidth();
        // set the correct width (column width)
        if (fSetWidthFromCol)
        {
            pCFI->_ff().SetWidth(cuvColWidth);
        }
        pCFI->_ff().SetPosition(SIDE_TOP, uvNull);
        pCFI->_ff().SetPosition(SIDE_LEFT, uvNull);
        pCFI->_ff().SetExplicitMargin(SIDE_TOP, FALSE);
        pCFI->_ff().SetExplicitMargin(SIDE_RIGHT, FALSE);
        pCFI->_ff().SetExplicitMargin(SIDE_BOTTOM, FALSE);
        pCFI->_ff().SetExplicitMargin(SIDE_LEFT, FALSE);
        pCFI->_ff().SetMargin(SIDE_TOP, uvNull);
        pCFI->_ff().SetMargin(SIDE_RIGHT, uvNull);
        pCFI->_ff().SetMargin(SIDE_BOTTOM, uvNull);
        pCFI->_ff().SetMargin(SIDE_LEFT, uvNull);
        pCFI->_ff()._bDisplay = styleDisplayNotSet;
        pCFI->_ff()._bPositionType = stylePositionNotSet;
        pCFI->_ff()._lZIndex = 0;
        pCFI->_ff()._fPositioned = FALSE;
        pCFI->_ff()._fAutoPositioned = FALSE;
        pCFI->_ff()._fScrollingParent = FALSE;
        pCFI->_ff()._fZParent = FALSE;
        pCFI->_ff()._pszFilters = NULL;
        pCFI->_ff()._bPageBreaks = 0;
        pCFI->_ff()._iExpandos = -1;
        pCFI->_ff()._iCustomCursor = -1   ;     
        pCFI->_ff()._iPEI = -1;
        pCFI->_ff()._fHasExpressions = FALSE;
        pCFI->_ff().SetCSSVerticalAlign(FALSE);
        pCFI->_ff().SetVerticalAlign(styleVerticalAlignNotSet);
        pCFI->_ff()._fLayoutFlowChanged = FALSE;
        pCFI->UnprepareForDebug();
    }

    //
    // Due to the complexity of alignment, we must keep these values
    // synchronised.
    //
    pCFI->_bCtrlBlockAlign = pCFI->_bBlockAlign;

    // copy the display & visibility values onto the pCFI, so if
    // visibility/display style change, pcf is correctly updated.
    pCFI->_fDisplayNone      = pCFI->_pcf->_fDisplayNone;
    pCFI->_fVisibilityHidden = pCFI->_pcf->_fVisibilityHidden;

    // Apply the cell itself
    pCFI->_pNodeContext = pNodeTarget;
    hr = THR(ApplyCellFormat(pCFI));
    if (hr)
        goto Error;

    // Transfer the cascading table valign into the vertical align
    hr = VerticalAlignFromCellVAlign(pCFI);
    if (hr)
        goto Error;

    // Vertical alignment for layouts has no meaning (TD has a layout),
    // so it is safe to turn off _fNeedsVerticalAlign flag.
    // We are doing this for perf reasons, when TD has a "vertical-align" property set.
    // In case of vertical text we set _fNeedsVerticalAlign to TRUE in ApplyDefaultFormat, 
    // because of middle vertical alignment of vertical text. So in this case don't 
    // turn off this flag.
    if (   pCFI->_pcf->_fNeedsVerticalAlign
        && !fComputeFFOnly
        && !pCFI->_pcf->HasVerticalLayoutFlow())
    {
        pCFI->PrepareCharFormat();
        pCFI->_cf()._fNeedsVerticalAlign = FALSE;
        pCFI->UnprepareForDebug();
    }

    //
    // NETSCAPE: If the user specified both WIDTH and NOWRAP, ignore NOWRAP if a fixed
    //           WIDTH was supplied (e.g., in pixels, not percent).
    //           PRE tags within the cell should still apply.
    //
    {
        // Since table is always horizontal, we get physical width.
        const CUnitValue & cuvWidth = pCFI->_pff->GetWidth();
        if (    pCFI->_fNoBreak
            &&  !cuvWidth.IsNull()
            &&  !cuvWidth.IsPercent())
        {
            pCFI->_fNoBreak = FALSE;
        }
    }

    hr = ApplyInnerOuterFormats(pCFI);
    if (hr)
        goto Error;

    if (   Tag() == ETAG_CAPTION
        && (pCFI->_pff->_bPositionType != stylePositionNotSet
        ||  pCFI->_pff->_fPositioned))
    {
        // don't support positioning on captions
        pCFI->PrepareFancyFormat();
        pCFI->_ff()._bPositionType = stylePositionNotSet;
        pCFI->_ff()._fPositioned = FALSE;
        pCFI->UnprepareForDebug();
    }

    {
        CMarkup * pMarkup = GetMarkup();
        if ((_fHasFilterSitePtr || pCFI->_fHasFilters) && !pMarkup->IsPrintMedia())
        {
            ComputeFilterFormat(pCFI);
        }
    }

    if(eExtraValues == ComputeFormatsType_Normal)
    {
        hr = THR(pNodeTarget->CacheNewFormats(pCFI));
        pCFI->_cstrFilters.Free();  // Arrggh!!! NOTE (michaelw)  This should really happen 
                                    // somewhere else (when you know where, put it there)
                                    // Fix CElement::ComputeFormats also
        if (hr)
            goto Error;

        // TODO (michaelw) Why do this after caching the new format?
        //                   Why not do it before and not be affected
        //                   by some strange new error return
        //

        // Update expressions in the recalc engine
        //
        // If we had expressions or have expressions then we need to tell the recalc engine
        // 
        if (pCFI->_pff->_fHasExpressions)
        {
            Doc()->AddExpressionTask(this);
            pCFI->NoStealing();
        }
    }

Cleanup:

    // Set the _fBlockNess cache bit on the node to save a little time later.
    // Need to do this here because we don't call super.
    pNodeTarget->_fBlockNess = TRUE;

    WHEN_DBG( pTableLayout->_fDisableTLCAssert = fDisableTLCAssert; )

    SwitchesEndTimer(SWITCHES_TIMER_COMPUTEFORMATS);

    RRETURN(hr);

Error:
    pCFI->Cleanup();
    goto Cleanup;

StealFormat:
    pNodeInherit = pCellNeighbor->GetFirstBranch();
    Assert(   (pNodeTarget->_iPF == -1) 
           || (pNodeTarget->_iCF == -1)
           || (pNodeTarget->_iFF == -1));

    pNodeTarget->_iPF = pNodeInherit->_iPF;
    if (pNodeTarget->_iPF >= 0)
        pts->_pParaFormatCache->AddRefData( pNodeTarget->_iPF );

    pNodeTarget->_iCF = pNodeInherit->_iCF;
    if (pNodeTarget->_iCF >= 0)
        pts->_pCharFormatCache->AddRefData( pNodeTarget->_iCF );

    pNodeTarget->_iFF = pNodeInherit->_iFF;
    if (pNodeTarget->_iFF >= 0)
        pts->_pFancyFormatCache->AddRefData( pNodeTarget->_iFF );

    MtAdd(Mt(CharFormatSteal), 1, 0);
    MtAdd(Mt(ParaFormatSteal), 1, 0);
    MtAdd(Mt(FancyFormatSteal), 1, 0);
    _fInheritedWidth = pCellNeighbor->_fInheritedWidth;

    goto Cleanup;

TableStructureViolated:

    WHEN_DBG( if (pTableLayout) pTableLayout->_fDisableTLCAssert = fDisableTLCAssert; )
    SwitchesEndTimer(SWITCHES_TIMER_COMPUTEFORMATS);
    return super::ComputeFormatsVirtual(pCFI, pNodeTarget);
}

//+---------------------------------------------------------------------------
//
//  Member:     CTableCell::Notify, CSite
//
//  Synopsis:   Handle notification
//
//----------------------------------------------------------------------------

void
CTableCell::Notify(CNotification *pNF)
{
    HRESULT hr = S_OK;

    super::Notify(pNF);

    switch (pNF->Type())
    {
    case NTYPE_ELEMENT_ENTERTREE:
        hr = EnterTree();
        break;
    case NTYPE_ELEMENT_EXITTREE_1:
        if (Tag() == ETAG_TC && !(pNF->DataAsDWORD() & EXITTREE_DESTROY) )
        {
            CTable * pTable = Table();
            CTableLayout * pTableLayout = pTable ? pTable->TableLayoutCache() : NULL;

            if (pTableLayout && pTableLayout->IsRepeating())
            {
                Assert(pTable->IsDatabound());
                int             ic, cC;
                CTableCaption  *pCaption;
                CTableCaption **ppCaption;

                for (cC = pTableLayout->_aryCaptions.Size(), ic = 0, ppCaption = pTableLayout->_aryCaptions;
                     cC > 0;
                     cC--, ppCaption++, ic++)
                {
                    pCaption = *ppCaption;
                    if (pCaption == this)
                    {
                        pTableLayout->DeleteCaption(ic);
                        break;
                    }
                }
            }
        }
        break;
    }

    return;
}




#define _cxx_
#include "caption.hdl"


const CElement::CLASSDESC CTableCaption::s_classdesc =
{
    {
        &CLSID_HTMLTableCaption,        // _pclsid
        0,                              // _idrBase
#ifndef NO_PROPERTY_PAGE
        s_apclsidPages,                 // _apClsidPages
#endif // NO_PROPERTY_PAGE
        s_acpi,                         // _pcpi
        ELEMENTDESC_NEVERSCROLL |
        ELEMENTDESC_TEXTSITE    |
        ELEMENTDESC_TABLECELL   |
        ELEMENTDESC_NOBKGRDRECALC,      // _dwFlags
        &IID_IHTMLTableCaption,         // _piidDispinterface
        &s_apHdlDescs,                  // _apHdlDesc
    },
    (void *)s_apfnpdIHTMLTableCaption,  // _pfnTearOff
    &s_AccelsTCellRun                   // _pAccelsRun
};


HRESULT
CTableCaption::CreateElement(CHtmTag *pht,
                  CDoc *pDoc, CElement **ppElement)
{
    Assert(ppElement);

    *ppElement = new CTableCaption(pht->GetTag(), pDoc);
    RRETURN ((*ppElement) ? S_OK : E_OUTOFMEMORY);
}



// copy constructor
CHeightUnitValue::CHeightUnitValue (const CUnitValue &uv)
: CNiceUnitValue(uv)
{
}
// copy constructor
CHeightUnitValue::CHeightUnitValue (const CHeightUnitValue &uv)
: CNiceUnitValue(uv)
{
}

// copy constructor
CWidthUnitValue::CWidthUnitValue (const CUnitValue &uv)
: CNiceUnitValue(uv)
{
}

// copy constructor
CWidthUnitValue::CWidthUnitValue (const CWidthUnitValue &uv)
: CNiceUnitValue(uv)
{
}

// copy constructor
CNiceUnitValue::CNiceUnitValue (const CUnitValue &uv)
: CUnitValue(uv)
{
}

int CTableCell::RowIndex() const
{
    Assert( Tag() != ETAG_CAPTION && Tag() != ETAG_TC );
    CTableRow *pRow = Row();
    return (pRow) ? pRow->_iRow : -1;
}

// Returns true if currect cell is in given range of cells
BOOL
CTableCell::IsInRange(RECT *pRect)
{
    if (IsDisplayNone() || GetFirstBranch()->GetFancyFormat()->_bPositionType == stylePositionabsolute)
        return FALSE;

    int nRow = RowIndex();
    int nCol = ColIndex();

    if(nRow < pRect->top  || nRow > pRect->bottom ||
            nCol < pRect->left || nCol > pRect->right)
        return FALSE;

    return TRUE;
}


BOOL
CTableCell::IsNoPositionedElementsUnder()
{
    CTreeNode *  pNodeSelf = GetFirstBranch();
    CTreePos  *  ptp = pNodeSelf->GetBeginPos();
    for (;;)
    {
        ptp = ptp->NextTreePos();
        Assert (ptp);
        switch( ptp->Type() )
        {
        case CTreePos::NodeEnd:
            if (ptp->Branch() == pNodeSelf && ptp->IsEdgeScope())
                return TRUE;
        case CTreePos::NodeBeg:
            if (ptp->Branch()->GetFancyFormat()->_fPositioned)
                goto Cleanup;
        }
    }
Cleanup:
    return FALSE;
}
