/******************************************************************************
* MiscData.cpp *
*--------------*
*  This file stores the const data used in various non-normalization front-end
*  code
*------------------------------------------------------------------------------
*  Copyright (C) 1999 Microsoft Corporation         Date: 05/02/2000
*  All Rights Reserved
*
****************************************************************** AARONHAL ***/

#include "stdafx.h"
#include"stdsentenum.h"

//--- Variable used to make sure initialization of pronunciations only happens once!
BOOL g_fAbbrevTablesInitialized = false;

//--- Helper function used to delete dynamically allocated memory (for the abbreviation
//    table) at DLL exit time...
void CleanupAbbrevTables( void ) 
{
    if ( g_fAbbrevTablesInitialized )
    {
        for ( ULONG i = 0; i < sp_countof( g_AbbreviationTable ); i++ )
        {
            if ( g_AbbreviationTable[i].pPron1 )
            {
                delete [] g_AbbreviationTable[i].pPron1;
            }
            if ( g_AbbreviationTable[i].pPron2 )
            {
                delete [] g_AbbreviationTable[i].pPron2;
            }
            if ( g_AbbreviationTable[i].pPron3 )
            {
                delete [] g_AbbreviationTable[i].pPron3;
            }
        }
        for ( i = 0; i < sp_countof( g_AmbiguousWordTable ); i++ )
        {
            if ( g_AmbiguousWordTable[i].pPron1 )
            {
                delete [] g_AmbiguousWordTable[i].pPron1;
            }
            if ( g_AmbiguousWordTable[i].pPron2 )
            {
                delete [] g_AmbiguousWordTable[i].pPron2;
            }
            if ( g_AmbiguousWordTable[i].pPron3 )
            {
                delete [] g_AmbiguousWordTable[i].pPron3;
            }
        }
        for ( i = 0; i < sp_countof( g_PostLexLookupWordTable ); i++ )
        {
            if ( g_PostLexLookupWordTable[i].pPron1 )
            {
                delete [] g_PostLexLookupWordTable[i].pPron1;
            }
            if ( g_PostLexLookupWordTable[i].pPron2 )
            {
                delete [] g_PostLexLookupWordTable[i].pPron2;
            }
            if ( g_PostLexLookupWordTable[i].pPron3 )
            {
                delete [] g_PostLexLookupWordTable[i].pPron3;
            }
        }
        if ( g_pOfA )
        {
            delete [] g_pOfA;
        }
        if ( g_pOfAn )
        {
            delete [] g_pOfAn;
        }
    }
}

const BrillPatch g_POSTaggerPatches [] =
{
    { MS_Adj, MS_Noun, CAP, MS_Unknown, MS_Unknown, NULL, NULL },
    { MS_Verb, MS_Noun, PREV1T, MS_Adj, MS_Unknown, NULL, NULL },
    { MS_Verb, MS_Noun, CAP, MS_Unknown, MS_Unknown, NULL, NULL },
    { MS_Noun, MS_Verb, PREV1T, MS_VAux, MS_Unknown, NULL, NULL },
    { MS_Conj, MS_Adv, NEXT2T, MS_Conj, MS_Unknown, NULL, NULL },
    { MS_Adj, MS_Adv, NEXT1T, MS_Verb, MS_Unknown, NULL, NULL },
    { MS_Adj, MS_Adv, PREV1TNEXT1T, MS_Noun, MS_Unknown, NULL, NULL },
    { MS_Verb, MS_Noun, PREV2T, MS_Prep, MS_Unknown, NULL, NULL },
    { MS_Noun, MS_Verb, PREV1T, MS_SubjPron, MS_Unknown, NULL, NULL },
    { MS_Noun, MS_Verb, PREV1T, MS_Pron, MS_Unknown, NULL, NULL },
    { MS_Noun, MS_Verb, PREV1T, MS_Adv, MS_Unknown, NULL, NULL },
    { MS_Verb, MS_Noun, NEXT1T, MS_VAux, MS_Unknown, NULL, NULL },
    { MS_Adj, MS_Adv, PREV1TNEXT1T, MS_Verb, MS_Adj, NULL, NULL },
    { MS_Verb, MS_Noun, PREV1TNEXT1T, MS_Noun, MS_Verb, NULL, NULL },
    { MS_Noun, MS_Adj, NEXT1T, MS_Adj, MS_Unknown, NULL, NULL },
    { MS_Verb, MS_Noun, NEXT1T, MS_CConj, MS_Unknown, NULL, NULL },
    { MS_Adj, MS_Verb, PREV1T, MS_VAux, MS_Unknown, NULL, NULL },
    { MS_Noun, MS_Verb, PREV1TNEXT1T, MS_Prep, MS_Adv, NULL, NULL },
    { MS_Noun, MS_Verb, PREV1TNEXT1T, MS_Prep, MS_Adj, NULL, NULL },
    { MS_Adv, MS_Prep, PREV1TNEXT1T, MS_Noun, MS_Prep, NULL, NULL },
    { MS_Adv, MS_Prep, PREV1TNEXT2T, MS_Verb, MS_Noun, NULL, NULL },
    { MS_Adj, MS_Adv, NEXT1T, MS_Adv, MS_Unknown, NULL, NULL },
    { MS_Noun, MS_Verb, NEXT1T, MS_ObjPron, MS_Unknown, NULL, NULL },
    { MS_Adv, MS_Adj, PREV1TNEXT1T, MS_Prep, MS_Noun, NULL, NULL },
    { MS_Noun, MS_Verb, PREV1TNEXT1T, MS_Interr, MS_Prep, NULL, NULL },
    { MS_Adj, MS_Adv, PREV1TNEXT1T, MS_Adv, MS_Adj, NULL, NULL },
    { MS_Prep, MS_Verb, PREV1T, MS_VAux, MS_Unknown, NULL, NULL },
    { MS_Adv, MS_Conj, NEXT1T, MS_SubjPron, MS_Unknown, NULL, NULL },
    { MS_Adj, MS_Noun, PREV1TNEXT1T, MS_Adj, MS_Prep, NULL, NULL },
    { MS_Noun, MS_Verb, PREV1TNEXT2T, MS_Interr, MS_Noun, NULL, NULL },
    { MS_Noun, MS_Verb, PREV1T, MS_VAux, MS_Unknown, NULL, NULL },
    { MS_Verb, MS_Noun, PREV1TNEXT1T, MS_Unknown, MS_Verb, NULL, NULL },
    { MS_Adj, MS_Verb, PREV1TNEXT1T, MS_Verb, MS_Adj, NULL, NULL },
    { MS_Conj, MS_Adv, PREV1TNEXT1T, MS_Unknown, MS_Unknown, NULL, NULL },
    { MS_Adv, MS_Prep, PREV1TNEXT2T, MS_Prep, MS_Unknown, NULL, NULL },
    { MS_Adj, MS_Adv, PREV2TNEXT1T, MS_Verb, MS_Adj, NULL, NULL },
    { MS_Adv, MS_Prep, NEXT1T, MS_Noun, MS_Unknown, NULL, NULL },
    { MS_Verb, MS_Noun, PREV1TNEXT1T, MS_Conj, MS_Verb, NULL, NULL },
    { MS_Adj, MS_Noun, PREV1TNEXT1T, MS_Prep, MS_Prep, NULL, NULL },
    { MS_Noun, MS_Verb, PREV1TNEXT1T, MS_Prep, MS_Pron, NULL, NULL },
    { MS_Adj, MS_Noun, PREV1TNEXT1T, MS_Noun, MS_Verb, NULL, NULL },
    { MS_Adj, MS_Adv, PREV1T, MS_VAux, MS_Unknown, NULL, NULL },
    { MS_Adj, MS_Noun, NEXT1T, MS_VAux, MS_Unknown, NULL, NULL },
    { MS_Adj, MS_Adv, NEXT1T, MS_VAux, MS_Unknown, NULL, NULL },
    { MS_Adj, MS_Noun, PREV1TNEXT1T, MS_Unknown, MS_Prep, NULL, NULL },
    { MS_Verb, MS_Noun, PREV2TNEXT1T, MS_Unknown, MS_Verb, NULL, NULL },
    { MS_Noun, MS_Verb, PREV1T, MS_SubjPron, MS_Unknown, NULL, NULL },
    { MS_Adj, MS_Verb, PREV1TNEXT1T, MS_Prep, MS_Adv, NULL, NULL },
    { MS_Adv, MS_Adj, PREV1TNEXT1T, MS_Conj, MS_Noun, NULL, NULL },
    { MS_Conj, MS_Adv, PREV1TNEXT1T, MS_Prep, MS_Adj, NULL, NULL },
    { MS_Adv, MS_Prep, NEXT1T, MS_Verb, MS_Unknown, NULL, NULL },
    { MS_Noun, MS_Adj, NEXT1T, MS_Adj, MS_Unknown, NULL, NULL },
    { MS_Adv, MS_Conj, NEXT1T, MS_Pron, MS_Unknown, NULL, NULL },
    { MS_Conj, MS_Adv, PREV1T, MS_VAux, MS_Unknown, NULL, NULL },
    { MS_Noun, MS_Verb, PREV1T, MS_Adv, MS_Unknown, NULL, NULL },
    { MS_Verb, MS_Noun, PREV2TNEXT1T, MS_CConj, MS_Unknown, NULL, NULL },
    { MS_Noun, MS_Verb, PREV1T, MS_VAux, MS_Unknown, NULL, NULL },
    { MS_Noun, MS_Verb, PREV1T, MS_Pron, MS_Unknown, NULL, NULL },
    { MS_Adv, MS_Adj, PREV2TNEXT1T, MS_Prep, MS_Noun, NULL, NULL },
    { MS_Noun, MS_Adj, PREV1TNEXT1T, MS_Adv, MS_Noun, NULL, NULL },
    { MS_Conj, MS_Adv, PREV1TNEXT1T, MS_Adv, MS_Adj, NULL, NULL },
    //--- Inserted by hand - convert "A" from Det to Noun after Det...
    { MS_Det, MS_Noun, CURRWPREV1T, MS_Det, MS_Unknown, L"a", NULL },
    { MS_Verb, MS_Adj, PREV1TNEXT1T, MS_Det, MS_Noun, NULL, NULL },
};

//--- IMPORTANT!!! This list must remain alphabetized for the binary search algorithm to work properly!!!
AbbrevRecord g_AbbreviationTable[] =
{
    { L"a",     L"EY 1", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"amp",   L"AE 1 M P", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"app",   L"AE 1 P", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"apr",   L"EY 1 - P R AX L", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"apt",   L"ax - p aa 1 r t - m ax n t", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"assoc", L"ax - s ow 2 - s iy - ey 1 - sh ax n", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"asst",  L"ax - s ih 1 s - t ax n t", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"aug",   L"ao 1 - g ax s t", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"ave",   L"ae 1 v - ax - n uw 2", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- Initial - never EOS
    { L"b",     L"B IY 1", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
    { L"bldg",  L"b ih 1 l - d ih ng", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"blvd",  L"b uh 1 l - ax - v aa 2 r d", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- SingleOrPluralAbbreviation
    { L"bu",    L"b uh 1 sh - ax l", MS_Noun, L"b uh 1 sh - ax l Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- Initial - never EOS
    { L"c",     L"s eh 1 l - s iy - ax s", MS_Noun, L"s iy 1", MS_Noun, NULL, MS_Unknown, 0, 3 },
    //--- SingleOrPluralAbbreviation
    { L"cal",   L"k ae 1 l - ax - r iy", MS_Noun, L"k ae 1 l - ax - r iy Z", MS_Noun, L"K AE 1 L", MS_Noun, 0, 0 },
    { L"cals",  L"k ae 1 l - ax - r iy Z", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"ch",    L"ch ae 1 p - t er", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- SingleOrPluralAbbreviation
    { L"cl",    L"s eh 1 n - t ax - l iy 2 - t er", MS_Noun, L"s eh 1 n - t ax - l iy 2 - t er Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- SingleOrPluralAbbreviation
    { L"cm",    L"s eh 1 n - t ax - m iy 2 - t er", MS_Noun, L"s eh 1 n - t ax - m iy 2 - t er Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    { L"cms",   L"s eh 1 n - t ax - m iy 2 - t er Z", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"cntr",  L"s eh 1 n - t er", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- IfEOSNotAbbreviation
    { L"co",    L"k ah 1 m - p ax - n iy", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 1, -1 },
    { L"cont",  L"k ax n - t ih 1 n - y uw D", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"corp",  L"k ao 2 r - p ax - r ey 1 - sh ax n", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- IfEOSNotAbbreviation
    { L"ct",    L"k ao 1 r t", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 1, -1 },
    { L"ctr",   L"s eh 1 n - t er", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
	//--- IfEOSNotAbbreviation
    { L"cu",    L"k y uw 1 - b ih k", MS_Noun, L"k y uw 1 - b ih k", MS_Noun, NULL, MS_Unknown, 1, 4 },
    //--- Initial - never EOS
    { L"d",     L"D IY 1", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
    //--- SingleOrPluralAbbreviation
    { L"db",    L"d eh 1 s - ax - b ax l", MS_Noun, L"d eh 1 s - ax - b ax l Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    { L"dec",   L"d ih - s eh 1 m - b er", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- SingleOrPluralAbbreviation
    { L"deg",   L"d ih - g r iy 1", MS_Noun, L"d ih - g r iy 1 z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    { L"dept",  L"d ih - p aa 1 r t - m ax n t", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"dist",  L"d ih 1 s - t r ax k t", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"doc",   L"D AA 1 K", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"doz",   L"d ah 1 z - ax n", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- DoctorDriveAbbreviation
    { L"dr",    L"d aa 1 k - t er", MS_Noun, L"D R AY 1 V", MS_Noun, NULL, MS_Unknown, 0, 1 },
    //--- Initial - never EOS
    { L"e",     L"IY 1", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
    //--- IfEOSNotAbbreviation
    { L"ed",    L"eh 2 jh - ax - k ey 1 - sh ax n", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 1, -1 },
    { L"esq",   L"eh 1 s - k w ay 2 r", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"est",   L"ax - s t ae 1 b - l ax sh T", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"etc",   L"EH T & s eh 1 t - er - ax", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- IfEOSNotAbbreviation
    { L"ex",    L"ih g - z ae 1 m - p ax l", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 1, -1 },
    { L"ext",   L"ih k - s t eh 1 n - sh ax n", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- Initial - never EOS
    { L"f",     L"f ae 1 r - ax n - h ay 2 t", MS_Noun, L"eh 1 f", MS_Noun, NULL, MS_Unknown, 0, 3 },
    { L"feb",   L"f eh 1 b - r uw - eh 2 r - iy", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- IfEOSNotAbbreviation
    { L"fig",   L"f ih 1 g - y er", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 1, -1 },
    //--- IfEOSNotAbbreviation
    { L"figs",  L"f ih 1 g - y er Z", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 1, -1 },
    //--- IfEOSNotAbbreviation
    { L"fl",    L"f l uw 1 - ih d", MS_Noun, L"f l uw 1 - ih d", MS_Noun, NULL, MS_Unknown, 1, 4 },
    { L"fn",    L"f uh 1 t - n ow 2 t", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"freq",  L"f r iy 1 - k w ax n - s iy", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"fri",   L"f r ay 1 - d ey", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- SingleOrPluralAbbreviation
    { L"ft",    L"F UH 1 T", MS_Noun, L"F IY 1 T", MS_Noun, L"F AO 1 R T", MS_Noun, 0, 0 },
    { L"fwd",   L"f ao 1 r - w er d", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- SingleOrPluralAbbreviation, IfEOSNotAbbreviation
    { L"g",     L"G R AE 1 M", MS_Noun, L"G R AE 1 M Z", MS_Noun, L"JH IY 1", MS_Noun, 1, 0 },
    //--- SingleOrPluralAbbreviation
    { L"gal",   L"g ae 1 l - ax n", MS_Noun, L"g ae 1 l - ax n Z", MS_Noun, L"G AE 1 L", MS_Noun, 0, 0 },
    //--- TITLE - never EOS
    { L"gen",   L"jh eh 1 n - er - ax l", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
    //--- IfEOSNotAbbreviation
    { L"gov",   L"g ah 1 v - er - n ER", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 1, -1 },
    //--- Initial - never EOS
    { L"h",     L"EY 1 CH", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
    //--- IfEOSNotAbbreviation
    { L"hr",    L"AW 1 ER", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 1, -1 },
    { L"hrs",   L"AW 1 ER Z", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"ht",    L"H AY 1 T", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"hwy",   L"h ay 1 w ey", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"hz",    L"H ER 1 T S", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- Initial - never EOS
    { L"i",     L"AY 1", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
    //--- SingleOrPluralAbbreviation, IfEOSNotAbbreviation
    { L"in",    L"IH 1 N CH", MS_Noun, L"IH 1 N CH AX Z", MS_Noun, L"IH 1 N", MS_Prep, 1, 0 },
    { L"inc",   L"ih n - k ao 1 r - p ax - r ey 2 - t AX D", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"incl",  L"ih n - k l uw 1 - d AX D", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"intl",  L"ih 2 n - t er - n ae 1 sh - ax - n ax l", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- IfEOSAndLowercaseNotAbbreviation
    { L"is",    L"ay 1 - l ax n d", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 2, -1 },
    //--- SingleOrPluralAbbreviation, IfEOSNotAbbreviation
    { L"j",     L"JH UW 1 L", MS_Noun, L"JH UW 1 L Z", MS_Noun, L"JH EY 1", MS_Noun, 1, 0 },
    //--- IfEOSNotAbbreviation
    { L"jan",   L"jh ae 1 n - y uw - eh 2 r - iy", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 1, -1 },
    { L"jr",    L"jh uw 1 n - y er", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"jul",   L"jh uh - l ay 1", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"jun",   L"JH UW 1 N", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- Initial - never EOS
    { L"k",     L"k eh 1 l - v ax n", MS_Noun, L"k ey 1", MS_Noun, NULL, MS_Unknown, 0, 3 },
    //--- SingleOrPluralAbbreviation
    { L"kb",    L"k ih 1 l - ax - b ay 2 t", MS_Noun, L"k ih 1 l - ax - b ay 2 t S", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- SingleOrPluralAbbreviation
    { L"kcal",  L"k ih 1 l - ax - k ae 2 l - ax - r iy", MS_Noun, L"k ih 1 l - ax - k ae 2 l - ax - r iy Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- SingleOrPluralAbbreviation
    { L"kg",    L"k ih 1 l - ax - g r ae 2 m", MS_Noun, L"k ih 1 l - ax - g r ae 2 m Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    { L"kgs",   L"k ih 1 l - ax - g r ae 2 m Z", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"khz",   L"k ih 1 l - ax - h er 2 t s", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- SingleOrPluralAbbreviation
    { L"kj",    L"k ih 1 l - ax - jh uw 2 l", MS_Noun, L"k ih 1 l - ax - jh uw 2 l z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- SingleOrPluralAbbreviation
    { L"km",    L"k ih - l aa 1 m - ih - t er", MS_Noun, L"k ih - l aa 1 m - ih - t er Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- SingleOrPluralAbbreviation
    { L"kw",    L"k ih 1 l - ax - w aa 2 t", MS_Noun, L"k ih 1 l - ax - w aa 2 t S", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- SingleOrPluralAbbreviation, IfEOSNotAbbreviation
    { L"l",     L"l iy 1 - t er", MS_Noun, L"l iy 1 - t er Z", MS_Noun, L"EH 1 L", MS_Noun, 1, 0 },
    //--- SingleOrPluralAbbreviation
    { L"lb",    L"P AW 1 N D", MS_Noun, L"P AW 1 N D Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    { L"lbs",   L"P AW 1 N D Z", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"lg",    L"L AA 1 R JH", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"ln",    L"l ey 1 n", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- Title - never EOS
    { L"lt",    L"l uw - t eh 1 n - ax n t", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
    { L"ltd",   L"l ih 1 m - ih - t AX D", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- SingleOrPluralAbbreviation, IfEOSNotAbbreviation
    { L"m",     L"M IY 1 T ER", MS_Noun, L"M IY 1 T ER Z", MS_Noun, L"EH 1 M", MS_Noun, 1, 0 },
    //--- IfEOSNotAbbreviation???
    { L"mar",   L"M AA 1 R CH", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- SingleOrPluralAbbreviation
    { L"mb",    L"m eh 1 g - ax - b ay 2 t", MS_Noun, L"m eh 1 g - ax - b ay 2 t S", MS_Noun, NULL, MS_Unknown, 0, 0 },
    { L"mfg",   L"m ae 2 n - y ax - f ae 1 k - ch er - IH NG", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- SingleOrPluralAbbreviation
    { L"mg",    L"m ih 1 l - ax - g r ae 2 m", MS_Noun, L"m ih 1 l - ax - g r ae 2 m Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    { L"mgr",   L"m ae 1 n - ih - jh ER", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"mgs",   L"m ih 1 l - ax - g r ae 2 m Z", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"mhz",   L"m eh 1 g - ax - h er 2 t s", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- SingleOrPluralAbbreviation
    { L"mi",    L"M AY 1 L", MS_Noun, L"M AY 1 L Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    { L"mic",   L"M AY 1 K", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- SingleOrPluralAbbreviation
    { L"min",   L"m ih 1 n - ax t", MS_Noun, L"m ih 1 n - ax t S", MS_Noun, L"m ih 1 n - ax - m ax m", MS_Noun, 0, 0 },
    { L"misc",  L"m ih 2 s - ax - l ey 1 - n iy - ax s", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- SingleOrPluralAbbreviation
    { L"ml",    L"m ih 1 l - ax - l iy 2 - t er", MS_Noun, L"m ih 1 l - ax - l iy 2 - t er Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- SingleOrPluralAbbreviation
    { L"mm",    L"m ih 1 l - ax - m iy 2 - t er", MS_Noun, L"m ih 1 l - ax - m iy 2 - t er Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    { L"mon",   L"m ah 1 n - d ey", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- TITLE - never EOS
    { L"mr",    L"M IH 1 S - T ER", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
    //--- TITLE - never EOS
    { L"mrs",   L"M IH 1 S - AX Z", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
    //--- IfEOSNotAbbreviation
    { L"ms",    L"M IH 1 Z", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 1, -1 },
    //--- SingleOrPluralAbbreviation
    { L"msec",  L"m ih 2 l - ax - s eh 1 k - ax n d", MS_Noun, L"m ih 2 l - ax - s eh 1 k - ax n d Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    { L"msecs", L"m ih 2 l - ax - s eh 1 k - ax n d Z", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"mt",    L"M AW 1 N T", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"mtn",   L"m aw 1 n - t ax n", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- Initial - never EOS
    { L"n",     L"EH 1 N", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
    //--- IfEOSNotAbbreviation
    { L"no",    L"N UH 1 M - B ER", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 1, -1 },
    { L"nov",   L"n ow - v eh 1 m - b er", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- Initial - never EOS
    { L"o",     L"OW 1", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
    { L"oct",   L"aa k - t ow 1 - b er", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- IfEOSNotAbbreviation
    { L"op",    L"OW 1 - P AX S", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 1, -1 },
    //--- SingleOrPluralAbbreviation
    { L"oz",    L"AW 1 N S", MS_Noun, L"AW 1 N - S AX Z", MS_Noun, L"AA 1 Z", MS_Noun, 0, 0 },
    { L"ozs",   L"AW 1 N - S AX Z", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- AbbreviationFollowedByDigit
    { L"p",     L"P EY 1 JH", MS_Noun, L"P IY 1", MS_Noun, NULL, MS_Unknown, 0, 2 },
    //--- IfEOSNotAbbreviation
    { L"pg",    L"P EY 1 JH", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 1, -1 },
    { L"pgs",   L"P EY 1 - JH AX Z", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"pkg",   L"p ae 1 k - ih jh", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"pkwy",  L"p aa 1 r k - w ey 2", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"pl",    L"P L EY 1 S", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"pp",    L"P EY 1 - JH AX Z", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- TITLE - never EOS
    { L"pres",  L"p r eh 1 z - ax - d ax n t", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
    //--- TITLE - never EOS
    { L"prof",  L"p r ax - f eh 1 - s ER", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
    { L"pt",    L"P OY 1 N T", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- Initial - never EOS
    { L"q",     L"K Y UW 1", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
    //--- SingleOrPluralAbbreviation
    { L"qt",    L"k w ao 1 r t", MS_Noun, L"k w ao 1 r t Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- Initial - never EOS
    { L"r",     L"AA 1 R", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
    { L"rd",    L"r ow 1 d", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"rec",   L"R EH 1 K", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"rep",   L"r eh 2 p - r ih - z eh 1 n - t ax - t ih v", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"rt",    L"R UW 1 T", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"rte",    L"R UW 1 T", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- Initial - never EOS
    { L"s",     L"EH 1 S", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
    //--- IfEOSAndLowercaseNotAbbreviation
    { L"sat",   L"s ae 1 t - er - d ey", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 2, -1 },
    //--- SingleOrPluralAbbreviation, IfEOSNotAbbreviation
    { L"sec",   L"s eh 1 k - ax n d", MS_Noun, L"s eh 1 k - ax n d z", MS_Noun, NULL, MS_Unknown, 1, 0 },
    //--- TITLE - never EOS
    { L"sen",   L"s eh 1 n - ax - t ER", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
    { L"sep",   L"s eh p - t eh 1 m - b er", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"sept",  L"s eh p - t eh 1 m - b er", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"sm",    L"S M AO 1 L", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 }, 
    { L"sq",    L"S K W EH 1 R", MS_Noun, L"S K W EH 1 R", MS_Noun, NULL, MS_Unknown, 0, 4 },
    //--- IfEOSNotAbbreviation
    { L"sr",    L"s iy 1 n - y er", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 1, -1 },
    //--- DoctorDriveAbbreviation
    { L"st",    L"S EY 1 N T", MS_Noun, L"S T R IY 1 T", MS_Noun, NULL, MS_Unknown, 0, 1 },
    { L"ste",   L"s w iy 1 t", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- IfEOSNotAbbreviation
    { L"sun",   L"s ah 1 n - d ey", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 1, -1 },
    //--- Initial - never EOS
    { L"t",     L"T IY 1", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
    //--- SingleOrPluralAbbreviation
    { L"tbsp",  L"t ey 1 - b ax l - s p uw 2 n", MS_Noun, L"t ey 1 - b ax l - s p uw 2 n Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    { L"tech",  L"T EH 1 K", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"thu",   L"th er 1 z - d ey", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"thur",  L"th er 1 z - d ey", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"thurs", L"th er 1 z - d ey", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- SingleOrPluralAbbreviation
    { L"tsp",   L"t iy 1 - s p uw 2 n", MS_Noun, L"t iy 1 - s p uw 2 n Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    { L"tue",   L"t uw 1 z - d ey", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"tues",  L"t uw 1 z - d ey", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- Initial - never EOS
    { L"u",     L"Y UW 1", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
    { L"univ",  L"y uw 2 - n ax - v er 1 - s ih - t iy", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- Initial - never EOS
    { L"v",     L"V IY 1", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
    { L"ver",   L"v er 1 - zh ax n", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"vers",  L"v er 1 - zh ax n", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"vol",   L"v aa 1 l - y uw m", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"vs",    L"v er 1 - s ax s", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- Initial - never EOS
    { L"w",     L"d ah 1 b - ax l - y uw", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
    //--- IfEOSAndLowercaseNotAbbreviation
    { L"wed",   L"w eh 1 n z - d ey", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 2, -1 },
    { L"wk",    L"W IY 1 K", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"wt",    L"W EY 1 T", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    { L"wy",    L"W EY 1", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- Initial - never EOS
    { L"x",     L"EH 1 K S", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
    //--- Initial - never EOS
    { L"y",     L"W AY 1", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
    //--- SingleOrPluralAbbreviation
    { L"yd",    L"Y AA 1 R D", MS_Noun, L"Y AA 1 R D Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    { L"yds",   L"Y AA 1 R D Z", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, 0, -1 },
    //--- SingleOrPluralAbbreviation
    { L"yr",    L"Y IY 1 R", MS_Noun, L"Y IY 1 R Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- Initial - never EOS
    { L"z",     L"Z IY 1", MS_Noun, NULL, MS_Unknown, NULL, MS_Unknown, -1, -1 },
};

//--- IMPORTANT!!! This list must remain alphabetized for the binary search algorithm to work properly!!!
AbbrevRecord g_AmbiguousWordTable[] =
{
    //--- ADisambig
    { L"a",     L"EY 1", MS_Noun, L"AX", MS_Det, NULL, MS_Unknown, 0, 8 },
    //--- AllCapsAbbreviation
    { L"al",    L"EY 1 & EH 1 L", MS_Noun, L"AE 1 L", MS_Noun, NULL, MS_Unknown, 0, 3 },
    //--- AllCapsAbbreviation
    { L"apr",   L"ey 1 & p iy 1 & aa 1 r", MS_Noun, L"ey 1 - p r ax l", MS_Noun, NULL, MS_Unknown, 0, 3 },
    //--- AllCapsAbbreviation
    { L"as",    L"EY 1 & EH 1 S", MS_Noun, L"AE 1 Z", MS_Conj, NULL, MS_Unknown, 0, 3 },
    //--- SingleOrPluralAbbreviation
    { L"bu",    L"b uh 1 sh - ax l", MS_Noun, L"b uh 1 sh - ax l Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- DegreeAbbreviation
    { L"c",     L"s eh 1 l - s iy - ax s", MS_Noun, L"s iy 1", MS_Noun, NULL, MS_Unknown, 0, 6 },
    //--- SingleOrPluralAbbreviation
    { L"cal",   L"k ae 1 l - ax - r iy", MS_Noun, L"k ae 1 l - ax - r iy Z", MS_Noun, L"K AE 1 L", MS_Noun, 0, 0 },
    //--- SingleOrPluralAbbreviation
    { L"cl",    L"s eh 1 n - t ax - l iy 2 - t er", MS_Noun, L"s eh 1 n - t ax - l iy 2 - t er Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- SingleOrPluralAbbreviation
    { L"cm",    L"s eh 1 n - t ax - m iy 2 - t er", MS_Noun, L"s eh 1 n - t ax - m iy 2 - t er Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- AllCapsAbbreviation
    { L"co",    L"S IY 1 & OW 1", MS_Noun, L"k ah 1 m - p ax - n iy", MS_Noun, NULL, MS_Unknown, 0, 3 },
    //--- AllCapsAbbreviation
    { L"ct",    L"s iy 1 & t iy 1", MS_Noun, L"k ao 1 r t", MS_Noun, NULL, MS_Unknown, 0, 3 },
    //--- MeasurementModifier
    { L"cu",    L"S IY 1 & Y UW 1", MS_Noun, L"k y uw 1 - b ih k", MS_Noun, NULL, MS_Unknown, 0, 7 },
    //--- SingleOrPluralAbbreviation
    { L"db",    L"d eh 1 s - ax - b ax l", MS_Noun, L"d eh 1 s - ax - b ax l Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- SingleOrPluralAbbreviation
    { L"deg",   L"d ih - g r iy 1", MS_Noun, L"d ih - g r iy 1 z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- DoctorDriveAbbreviation
    { L"dr",    L"d aa 1 k - t er", MS_Noun, L"D R AY 1 V", MS_Noun, NULL, MS_Unknown, 0, 1 },
    //--- DegreeAbbreviation
    { L"f",     L"f ae 1 r - ax n - h ay 2 t", MS_Noun, L"eh 1 f", MS_Noun, NULL, MS_Unknown, 0, 6 },
    //--- AbbreviationFollowedByDigit
    { L"fig",   L"f ih 1 g - y er", MS_Noun, L"F IH 1 G", MS_Noun, NULL, MS_Unknown, 0, 2 },
    //--- AbbreviationFollowedByDigit
    { L"figs",  L"f ih 1 g - y er Z", MS_Noun, L"F IH 1 G Z", MS_Noun, NULL, MS_Unknown, 0, 2 },
    //--- MeasurementModifier
    { L"fl",    L"eh 1 f & eh 1 l", MS_Noun, L"f l uw 1 - ih d", MS_Noun, NULL, MS_Unknown, 0, 7 },
    //--- SingleOrPluralAbbreviation
    { L"ft",    L"F UH 1 T", MS_Noun, L"F IY 1 T", MS_Noun, L"F AO 1 R T", MS_Noun, 0, 0 },
    //--- G, Gram, Grams
    { L"g",     L"G R AE 1 M", MS_Noun, L"G R AE 1 M Z", MS_Noun, L"JH IY 1", MS_Noun, 0, 5 },
    //--- DoctorDriveAbbreviation
    { L"gov",   L"g ah 1 v - er - n ER", MS_Noun, L"G AH 1 V", MS_Noun, NULL, MS_Unknown, 0, 1 },
    //--- AllCapsAbbreviation
    { L"hi",    L"EY 1 CH & AY 1", MS_Noun, L"H AY 1", MS_Interjection, NULL, MS_Unknown, 0, 3 },
    //--- HR hour hours
    { L"hr",    L"AW 1 ER", MS_Noun, L"AW 1 ER Z", MS_Noun, L"EY 1 CH AA 1 R", MS_Noun, 0, 5 },
    //--- J, Joule, Joules
    { L"j",     L"JH UW 1 L", MS_Noun, L"JH UW 1 L Z", MS_Noun, L"JH EY 1", MS_Noun, 0, 5 },
    //--- DegreeAbbreviation
    { L"k",     L"k eh 1 l - v ax n", MS_Noun, L"k ey 1", MS_Noun, NULL, MS_Unknown, 0, 6 },
    //--- SingleOrPluralAbbreviation
    { L"kb",    L"k ih 1 l - ax - b ay 2 t", MS_Noun, L"k ih 1 l - ax - b ay 2 t S", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- SingleOrPluralAbbreviation
    { L"kcal",  L"k ih 1 l - ax - k ae 2 l - ax - r iy", MS_Noun, L"k ih 1 l - ax - k ae 2 l - ax - r iy Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- SingleOrPluralAbbreviation
    { L"kg",    L"k ih 1 l - ax - g r ae 2 m", MS_Noun, L"k ih 1 l - ax - g r ae 2 m Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- SingleOrPluralAbbreviation
    { L"kj",    L"k ih 1 l - ax - jh uw 2 l", MS_Noun, L"k ih 1 l - ax - jh uw 2 l z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- SingleOrPluralAbbreviation
    { L"km",    L"k ih - l aa 1 m - ih - t er", MS_Noun, L"k ih - l aa 1 m - ih - t er Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- SingleOrPluralAbbreviation
    { L"kw",    L"k ih 1 l - ax - w aa 2 t", MS_Noun, L"k ih 1 l - ax - w aa 2 t S", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- L, Liter, Liters
    { L"l",     L"l iy 1 - t er", MS_Noun, L"l iy 1 - t er Z", MS_Noun, L"EH 1 L", MS_Noun, 0, 5 },
    //--- AllCapsAbbreviation
    { L"la",    L"EH 1 L & EY 1", MS_Noun, L"L AH 1", MS_Noun, NULL, MS_Unknown, 0, 3 },
    //--- SingleOrPluralAbbreviation
    { L"lb",    L"P AW 1 N D", MS_Noun, L"P AW 1 N D Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- AllCapsAbbreviation
    { L"lts",   L"eh 1 l & t iy 1 & eh 1 s", MS_Noun, L"l uw t eh 1 n ax n t s", MS_Noun, NULL, MS_Unknown, 0, 3 },
    //--- M, Meter, Meters
    { L"m",     L"M IY 1 - T ER", MS_Noun, L"M IY 1 - T ER Z", MS_Noun, L"EH 1 M", MS_Noun, 0, 5 },
    //--- AllCapsAbbreviation
    { L"ma",    L"EH 1 M & AA 1", MS_Noun, L"M AA 1", MS_Noun, NULL, MS_Unknown, 0, 3 },
    //--- March Mar
    { L"mar",   L"M AA 1 R CH", MS_Noun, L"M AA 1 R", MS_Verb, NULL, MS_Unknown, 0, 4 },
    //--- SingleOrPluralAbbreviation
    { L"mb",    L"m eh 1 g - ax - b ay 2 t", MS_Noun, L"m eh 1 g - ax - b ay 2 t S", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- AllCapsAbbreviation
    { L"me",    L"EH 1 M & IY 1", MS_Noun, L"M IY 1", MS_ObjPron, NULL, MS_Unknown, 0, 3 },
    //--- SingleOrPluralAbbreviation
    { L"mg",    L"m ih 1 l - ax - g r ae 2 m", MS_Noun, L"m ih 1 l - ax - g r ae 2 m Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- SingleOrPluralAbbreviation
    { L"mi",    L"M AY 1 L", MS_Noun, L"M AY 1 L Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- SingleOrPluralAbbreviation
    { L"min",   L"m ih 1 n - ax t", MS_Noun, L"m ih 1 n - ax t S", MS_Noun, L"m ih 1 n - ax - m ax m", MS_Noun, 0, 0 },
    //--- SingleOrPluralAbbreviation
    { L"ml",    L"m ih 1 l - ax - l iy 2 - t er", MS_Noun, L"m ih 1 l - ax - l iy 2 - t er Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- SingleOrPluralAbbreviation
    { L"mm",    L"m ih 1 l - ax - m iy 2 - t er", MS_Noun, L"m ih 1 l - ax - m iy 2 - t er Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- SingleOrPluralAbbreviation
    { L"mpg", L"m ay 1 l & p er 1 & g ae 1 l ax n", MS_Noun, L"m ay 1 l z & p er 1 & g ae 1 l ax n", MS_Noun, L"eh 1 m & p iy 1 & jh iy 1", MS_Noun, 0, 0 },
    //--- SingleOrPluralAbbreviation
    { L"mph",   L"m ay 1 l & p er 1 & aw 1 er", MS_Noun, L"m ay 1 l z & p er 1 & aw 1 er", MS_Noun, L"eh 1 m & p iy 1 & ey 1 ch", MS_Noun, 0, 0 },
    //--- MS millisecond milliseconds
    { L"ms",    L"m ih 2 l - ax - s eh 1 k - ax n d", MS_Noun, L"m ih 2 l - ax - s eh 1 k - ax n d Z", MS_Noun, L"eh 1 m & eh 1 s", MS_Noun, 0, 5 },
    //--- SingleOrPluralAbbreviation
    { L"msec",  L"m ih 2 l - ax - s eh 1 k - ax n d", MS_Noun, L"m ih 2 l - ax - s eh 1 k - ax n d Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- AllCapsAbbreviation
    { L"mt",    L"EH 1 M & T IY 1", MS_Noun, L"M AW 1 N T", MS_Noun, NULL, MS_Unknown, 0, 3 },
    //--- AllCapsAbbreviation
    { L"oh",    L"OW 1 & EY 1 CH", MS_Noun, L"OW 1", MS_Interjection, NULL, MS_Unknown, 0, 3 },
    //--- AllCapsAbbreviation
    { L"or",    L"OW 1 & AA 1 R", MS_Noun, L"AO 1 R", MS_CConj, NULL, MS_Unknown, 0, 3 },
    //--- SingleOrPluralAbbreviation
    { L"oz",    L"AW 1 N S", MS_Noun, L"AW 1 N S AX Z", MS_Noun, L"AA 1 Z", MS_Noun, 0, 0 },
    //--- AbbreviationFollowedByDigit
    { L"p",     L"P EY 1 JH", MS_Noun, L"P IY 1", MS_Noun, NULL, MS_Unknown, 0, 2 },
    //--- AllCapsAbbreviation
    { L"pa",    L"P IY 1 & EY 1", MS_Noun, L"P AA 1", MS_Noun, NULL, MS_Unknown, 0, 3 },
    //--- AllCapsAbbreviation
    { L"pg",    L"P IY 1 & JH IY 1", MS_Noun, L"P EY 1 JH", MS_Noun, NULL, MS_Unknown, 0, 3 },
    //--- AllCapsAbbreviation
    { L"po",    L"p iy 1 & ow 1", MS_Noun, L"p ow 1", MS_Noun, NULL, MS_Unknown, 0, 3 },
    //--- PolishDisambig
    { L"polish", L"p ow 1 l - ax sh", MS_Adj, L"p aa 1 l - ih sh", MS_Verb, L"p aa 1 l - ih sh", MS_Noun, 0, 9 },
    //--- SingleOrPluralAbbreviation
    { L"qt",    L"K W AO 1 R T", MS_Noun, L"K W AO 1 R T Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- Saturday Sat
    { L"sat",   L"s ae 1 t - er - d ey", MS_Noun, L"S AE 1 T", MS_Verb, NULL, MS_Unknown, 0, 4 },
    //--- SEC Second Seconds
    { L"sec",   L"s eh 1 k - ax n d", MS_Noun, L"s eh 1 k - ax n d Z", MS_Noun, L"EH 1 S & IY 1 & S IY 1", MS_Noun, 0, 5 },
    //--- MeasurementModifier
    { L"sq",    L"S K W EH 1 R", MS_Noun, L"S K W EH 1 R", MS_Noun, NULL, MS_Unknown, 0, 7 },
	//--- AllCapsAbbreviation
    { L"sr",    L"EH 1 S & AA 1 R", MS_Noun, L"s iy 1 n - y er", MS_Noun, NULL, MS_Unknown, 0, 3 },
    //--- DoctorDriveAbbreviation
    { L"st",    L"S EY 1 N T", MS_Noun, L"S T R IY 1 T", MS_Noun, NULL, MS_Unknown, 0, 1 },
    //--- SingleOrPluralAbbreviation
    { L"tbsp",  L"t ey 1 - b ax l - s p uw 2 n", MS_Noun, L"t ey 1 - b ax l - s p uw 2 n Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- SingleOrPluralAbbreviation
    { L"tsp",   L"t iy 1 - s p uw 2 n", MS_Noun, L"t iy 1 - s p uw 2 n Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- AllCapsAbbreviation
    { L"us",    L"Y UW 1 & EH 1 S", MS_Noun, L"AH 1 S", MS_ObjPron, NULL, MS_Unknown, 0, 3 },
    //--- Wednesday Wed
    { L"wed",   L"w eh 1 n z - d ey", MS_Noun, L"W EH 1 D", MS_Verb, NULL, MS_Unknown, 0, 4 },
    //--- AllCapsAbbreviation
    { L"wy",    L"d ah 1 b - ax l - y uw & W AY 1", MS_Noun, L"W EY 1", MS_Noun, NULL, MS_Unknown, 0, 3 },
    //--- SingleOrPluralAbbreviation
    { L"yd",    L"Y AA 1 R D", MS_Noun, L"Y AA 1 R D Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- SingleOrPluralAbbreviation
    { L"yr",    L"Y IY 1 R", MS_Noun, L"Y IY 1 R Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
};

//--- IMPORTANT!!! This list must remain alphabetized for the binary search algorithm to work properly!!!
AbbrevRecord g_PostLexLookupWordTable[] =
{
    //--- MeasurementDisambig
    { L"bu",    L"b uh 1 sh - ax l", MS_Noun, L"b uh 1 sh - ax l Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- MeasurementDisambig
    { L"cal",   L"k ae 1 l - ax - r iy", MS_Noun, L"k ae 1 l - ax - r iy Z", MS_Noun, L"K AE 1 L", MS_Noun, 0, 0 },
    //--- MeasurementDisambig
    { L"cl",    L"s eh 1 n - t ax - l iy 2 - t er", MS_Noun, L"s eh 1 n - t ax - l iy 2 - t er Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- MeasurementDisambig
    { L"cm",    L"s eh 1 n - t ax - m iy 2 - t er", MS_Noun, L"s eh 1 n - t ax - m iy 2 - t er Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- MeasurementDisambig
    { L"db",    L"d eh 1 s - ax - b ax l", MS_Noun, L"d eh 1 s - ax - b ax l Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- MeasurementDisambig
    { L"deg",   L"d ih - g r iy 1", MS_Noun, L"d ih - g r iy 1 z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- MeasurementDisambig
    { L"ft",    L"F UH 1 T", MS_Noun, L"F IY 1 T", MS_Noun, L"F AO 1 R T", MS_Noun, 0, 0 },
    //--- MeasurementDisambig
    { L"g",     L"G R AE 1 M", MS_Noun, L"G R AE 1 M Z", MS_Noun, L"JH IY 1", MS_Noun, 0, 0 },
    //--- MeasurementDisambig
    { L"gal",   L"g ae 1 l - ax n", MS_Noun, L"g ae 1 l - ax n Z", MS_Noun, L"G AE 1 L", MS_Noun, 0, 0 },
    //--- MeasurementDisambig
    { L"hr",    L"AW 1 ER", MS_Noun, L"AW 1 ER Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- MeasurementDisambig
    { L"in",    L"IH 1 N CH", MS_Noun, L"IH 1 N CH AX Z", MS_Noun, L"IH 1 N", MS_Prep, 0, 0 },
    //--- MeasurementDisambig
    { L"j",     L"JH UW 1 L", MS_Noun, L"JH UW 1 L Z", MS_Noun, L"JH EY 1", MS_Noun, 0, 0 },
    //--- MeasurementDisambig
    { L"kb",    L"k ih 1 l - ax - b ay 2 t", MS_Noun, L"k ih 1 l - ax - b ay 2 t S", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- MeasurementDisambig
    { L"kcal",  L"k ih 1 l - ax - k ae 2 l - ax - r iy", MS_Noun, L"k ih 1 l - ax - k ae 2 l - ax - r iy Z", MS_Noun, L"K AE 1 L", MS_Noun, 0, 0 },
    //--- MeasurementDisambig
    { L"kg",    L"k ih 1 l - ax - g r ae 2 m", MS_Noun, L"k ih 1 l - ax - g r ae 2 m Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- MeasurementDisambig
    { L"kj",    L"k ih 1 l - ax - jh uw 2 l", MS_Noun, L"k ih 1 l - ax - jh uw 2 l z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- MeasurementDisambig
    { L"km",    L"k ih - l aa 1 m - ih - t er", MS_Noun, L"k ih - l aa 1 m - ih - t er Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- MeasurementDisambig
    { L"kw",    L"k ih 1 l - ax - w aa 2 t", MS_Noun, L"k ih 1 l - ax - w aa 2 t S", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- MeasurementDisambig
    { L"l",     L"l iy 1 - t er", MS_Noun, L"l iy 1 - t er Z", MS_Noun, L"EH 1 L", MS_Noun, 0, 0 },
    //--- MeasurementDisambig
    { L"lb",    L"P AW 1 N D", MS_Noun, L"P AW 1 N D Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- MeasurementDisambig
    { L"m",     L"M IY 1 - T ER", MS_Noun, L"M IY 1 - T ER Z", MS_Noun, L"EH 1 M", MS_Noun, 0, 0 },
    //--- MeasurementDisambig
    { L"mb",    L"m eh 1 g - ax - b ay 2 t", MS_Noun, L"m eh 1 g - ax - b ay 2 t S", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- MeasurementDisambig
    { L"mg",    L"m ih 1 l - ax - g r ae 2 m", MS_Noun, L"m ih 1 l - ax - g r ae 2 m Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- MeasurementDisambig
    { L"mi",    L"M AY 1 L", MS_Noun, L"M AY 1 L Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- MeasurementDisambig
    { L"min",   L"m ih 1 n - ax t", MS_Noun, L"m ih 1 n - ax t S", MS_Noun, L"m ih 1 n - ax - m ax m", MS_Noun, 0, 0 },
    //--- ReadDisambig
    { L"misread", L"m ih s - r iy 1 d", MS_Verb, L"m ih s - r eh 1 d", MS_Verb, NULL, MS_Unknown, 0, 2 },
    //--- MeasurementDisambig
    { L"ml",    L"m ih 1 l - ax - l iy 2 - t er", MS_Noun, L"m ih 1 l - ax - l iy 2 - t er Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- MeasurementDisambig
    { L"mm",    L"m ih 1 l - ax - m iy 2 - t er", MS_Noun, L"m ih 1 l - ax - m iy 2 - t er Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- MeasurementDisambig
    { L"mpg", L"m ay 1 l & p er 1 & g ae 1 l ax n", MS_Noun, L"m ay 1 l z & p er 1 & g ae 1 l ax n", MS_Noun, L"eh 1 m & p iy 1 & jh iy 1", MS_Noun, 0, 0 },
    //--- MeasurementDisambig
    { L"mph",   L"m ay 1 l & p er 1 & aw 1 er", MS_Noun, L"m ay 1 l z & p er 1 & aw 1 er", MS_Noun, L"eh 1 m & p iy 1 & ey 1 ch", MS_Noun, 0, 0 },
    //--- MeasurementDisambig
    { L"msec",  L"m ih 2 l - ax - s eh 1 k - ax n d", MS_Noun, L"m ih 2 l - ax - s eh 1 k - ax n d Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- MeasurementDisambig
    { L"oz",    L"AW 1 N S", MS_Noun, L"AW 1 N S AX Z", MS_Noun, L"AA 1 Z", MS_Noun, 0, 0 },
    //--- ReadDisambig
    { L"proofread", L"p r uw 1 f - r iy 2 d", MS_Verb, L"p r uw 1 f - r eh 2 d", MS_Verb, NULL, MS_Unknown, 0, 2 },
    //--- MeasurementDisambig
    { L"qt",    L"K W AO 1 R T", MS_Noun, L"K W AO 1 R T Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- ReadDisambig
    { L"read",  L"R IY 1 D", MS_Verb, L"R EH 1 D", MS_Verb, NULL, MS_Unknown, 0, 2 },
    //--- MeasurementDisambig
    { L"sec",   L"S EH 1 k - ax n d", MS_Noun, L"s eh 1 k - ax n d z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- MeasurementDisambig
    { L"tbsp",  L"t ey 1 - b ax l - s p uw 2 n", MS_Noun, L"t ey 1 - b ax l - s p uw 2 n Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- TheDisambig
    { L"the",   L"DH IY 2", MS_Det, L"DH AX 2", MS_Det, NULL, MS_Unknown, 0, 1 },
    //--- MeasurementDisambig
    { L"tsp",   L"t iy 1 - s p uw 2 n", MS_Noun, L"t iy 1 - s p uw 2 n Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- MeasurementDisambig
    { L"yd",    L"Y AA 1 R D", MS_Noun, L"Y AA 1 R D Z", MS_Noun, NULL, MS_Unknown, 0, 0 },
    //--- MeasurementDisambig
    { L"yr",    L"Y IY 1 R", MS_Noun, L"Y IY 1 R Z", MS_Noun, NULL, MS_Unknown, 0, 0 },

};

WCHAR *g_pOfA = L"ah 2 v & ax 2 &";
WCHAR *g_pOfAn = L"ah 2 v & ax 2 n &";

const SentBreakDisambigFunc g_SentBreakDisambigTable[] = 
{
    CStdSentEnum::IsAbbreviationEOS,
    CStdSentEnum::IfEOSNotAbbreviation,
    CStdSentEnum::IfEOSAndLowercaseNotAbbreviation,
};

const PronDisambigFunc g_PronDisambigTable[] = 
{
    CStdSentEnum::SingleOrPluralAbbreviation,
    CStdSentEnum::DoctorDriveAbbreviation,
    CStdSentEnum::AbbreviationFollowedByDigit,
    CStdSentEnum::DegreeAbbreviation,
    CStdSentEnum::AbbreviationModifier,
};

const PronDisambigFunc g_AmbiguousWordDisambigTable[] =
{
    CStdSentEnum::SingleOrPluralAbbreviation,
    CStdSentEnum::DoctorDriveAbbreviation,
    CStdSentEnum::AbbreviationFollowedByDigit,
    CStdSentEnum::AllCapsAbbreviation,
    CStdSentEnum::CapitalizedAbbreviation,
    CStdSentEnum::SECAbbreviation,
    CStdSentEnum::DegreeAbbreviation,
	CStdSentEnum::AbbreviationModifier,
    CStdSentEnum::ADisambig,
    CStdSentEnum::PolishDisambig,
};

const PostLexLookupDisambigFunc g_PostLexLookupDisambigTable[] =
{
    CStdSentEnum::MeasurementDisambig,
    CStdSentEnum::TheDisambig,
    CStdSentEnum::ReadDisambig,
};

//--- IMPORTANT!!! This list must remain alphabetized for the binary search algorithm to work properly!!!
// This is an alphabetized list of all non-proper-noun words which
// appear within the list of the 200 most frequent first words
// in sentences in both the Brown and WSJ corpora.
const SPLSTR g_FirstWords[] =
{
DEF_SPLSTR( "A" ),
DEF_SPLSTR( "About" ),
DEF_SPLSTR( "According" ),
DEF_SPLSTR( "After" ),
DEF_SPLSTR( "Again" ),                                // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "All" ),
DEF_SPLSTR( "Also" ),
DEF_SPLSTR( "Although" ),
DEF_SPLSTR( "Among" ),
DEF_SPLSTR( "An" ),
DEF_SPLSTR( "And" ),
DEF_SPLSTR( "Another" ),
DEF_SPLSTR( "Any" ),                                  // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Anyway" ),                               // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Are" ),                                  // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "As" ),
DEF_SPLSTR( "At" ),
DEF_SPLSTR( "Back" ),                                 // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Because" ),
DEF_SPLSTR( "Before" ),
DEF_SPLSTR( "Besides" ),                              // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Both" ),
DEF_SPLSTR( "But" ),
DEF_SPLSTR( "By" ),
DEF_SPLSTR( "Can" ),                                  // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Consequently" ),                         // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Dear" ),                                 // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Despite" ),
DEF_SPLSTR( "Did" ),                                  // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Do" ),                                   // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Does" ),                                 // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Don't" ),                                // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "During" ),
DEF_SPLSTR( "Each" ),
DEF_SPLSTR( "Early" ),                                // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Even" ),
DEF_SPLSTR( "Every" ),                                // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Finally" ),                              // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "First" ),
DEF_SPLSTR( "Following" ),                            // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "For" ),
DEF_SPLSTR( "Four" ),
DEF_SPLSTR( "From" ),
DEF_SPLSTR( "Further" ),                              // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Furthermore" ),                          // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Generally" ),                            // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Given" ),                                // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Go" ),                                   // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Great" ),                                // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Had" ),                                  // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Have" ),                                 // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Having" ),                               // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "He" ),
DEF_SPLSTR( "Her" ),
DEF_SPLSTR( "Here" ),
DEF_SPLSTR( "His" ),
DEF_SPLSTR( "How" ),                                  // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "However" ),
DEF_SPLSTR( "I" ),
DEF_SPLSTR( "If" ),
DEF_SPLSTR( "In" ),
DEF_SPLSTR( "Indeed" ),
DEF_SPLSTR( "Initially" ),                            // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Instead" ),
DEF_SPLSTR( "Is" ),                                   // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "It" ),
DEF_SPLSTR( "Its" ),
DEF_SPLSTR( "Just" ),                                 // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Last" ),
DEF_SPLSTR( "Later" ),
DEF_SPLSTR( "Let" ),                                  // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Like" ),
DEF_SPLSTR( "Many" ),
DEF_SPLSTR( "Maybe" ),                                // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Meanwhile" ),
DEF_SPLSTR( "More" ),
DEF_SPLSTR( "Moreover" ),
DEF_SPLSTR( "Most" ),
DEF_SPLSTR( "Much" ),
DEF_SPLSTR( "My" ),    // Added to fix bug #385
DEF_SPLSTR( "Neither" ),
DEF_SPLSTR( "Never" ),                                // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Nevertheless" ),
DEF_SPLSTR( "New" ),
DEF_SPLSTR( "Next" ),                                 // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "No" ),
DEF_SPLSTR( "None" ),                                 // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Nonetheless" ),                          // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Nor" ),
DEF_SPLSTR( "Not" ),
DEF_SPLSTR( "Nothing" ),                              // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Now" ),
DEF_SPLSTR( "Of" ),
DEF_SPLSTR( "On" ),
DEF_SPLSTR( "Once" ),
DEF_SPLSTR( "One" ),
DEF_SPLSTR( "Only" ),
DEF_SPLSTR( "Or" ),
DEF_SPLSTR( "Other" ),
DEF_SPLSTR( "Others" ),
DEF_SPLSTR( "Our" ),                                  // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Over" ),
DEF_SPLSTR( "People" ),
DEF_SPLSTR( "Perhaps" ),                              // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Please" ),                               // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Previous" ),                             // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Recent" ),                               // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Right" ),                                // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Second" ),                               // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "See" ),                                  // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Several" ),
DEF_SPLSTR( "She" ),
DEF_SPLSTR( "Shortly" ),                              // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Similarly" ),                            // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Since" ),
DEF_SPLSTR( "So" ),
DEF_SPLSTR( "Some" ),
DEF_SPLSTR( "Sometimes" ),                            // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Soon" ),                                 // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Still" ),
DEF_SPLSTR( "Subsequently" ),                         // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Such" ),
DEF_SPLSTR( "Take" ),                                 // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "That" ),
DEF_SPLSTR( "The" ),
DEF_SPLSTR( "Their" ),
DEF_SPLSTR( "Then" ),
DEF_SPLSTR( "There" ),
DEF_SPLSTR( "Thereafter" ),                           // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Therefore" ),                            // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "These" ),
DEF_SPLSTR( "They" ),
DEF_SPLSTR( "This" ),
DEF_SPLSTR( "Those" ),
DEF_SPLSTR( "Though" ),
DEF_SPLSTR( "Three" ),                                // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Through" ),                              // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Thus" ),
DEF_SPLSTR( "To" ),
DEF_SPLSTR( "Today" ),
DEF_SPLSTR( "Two" ),
DEF_SPLSTR( "Under" ),
DEF_SPLSTR( "Unlike" ),                               // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Until" ),
DEF_SPLSTR( "Upon" ),                                 // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "We" ),
DEF_SPLSTR( "Well" ),                                 // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "What" ),
DEF_SPLSTR( "When" ),
DEF_SPLSTR( "Where" ),                                // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Whether" ),                              // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Which" ),                                // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "While" ),
DEF_SPLSTR( "Who" ),                                  // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Why" ),                                  // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Will" ),                                 // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "With" ),
DEF_SPLSTR( "Within" ),
DEF_SPLSTR( "Without" ),                              // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Yes" ),                                  // PaulCa added 4/14/99 (Bug 107)
DEF_SPLSTR( "Yet" ),
DEF_SPLSTR( "You" ),
DEF_SPLSTR( "Your" ),
};