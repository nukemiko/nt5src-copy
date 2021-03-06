//****************************************************************************
//
//  File:       joycpl.h
//  Content:    Joystick cpl header file
//  History:
//   Date	By	Reason
//   ====	==	======
//   29-nov-94	craige	initial implementation
//   15-dec-94	craige	allow N joysticks
//
//  Copyright (c) Microsoft Corporation 1994, 1995
//
//****************************************************************************
#ifndef __JOYCPL_INCLUDED__
#define __JOYCPL_INCLUDED__

#include <windows.h>
#include <windowsx.h>
#define NOSTATUSBAR
#include <commctrl.h>
#include <mmsystem.h>
#include <mmddk.h>
#include <stdlib.h>
#include <regstr.h>
#include <cpl.h>
#include "rcids.h"                      

//#define WANT_SHEETS

//#ifdef DEBUG
//    void cdecl dprintf( LPSTR szFormat, ... );
//    #define DPF	dprintf
	#define DPF	TRACE	
//#else
//    #define DPF 1 ? (void)0 : (void)
//#endif

/*
 * misc. defines
 */
#define HASJOY 		0x01
#define HASRUDDERMAYBE	0x02

#define ACTIVE_COLOR	RGB( 255, 0, 0 )
#define INACTIVE_COLOR	RGB( 128, 0, 0 )

#define TIMER_ID	1

#define MAX_STR_LEN	 255
#define STR_LEN_128	 128
#define STR_LEN_64	 64
#define STR_LEN_32	 32

#define JOYPOLLTIME	25		// time between polls in milliseconds
#define JOYCHECKTIME	2500		// time between hw check in milliseconds
#define MAX_STR		256		// max size for string resources
#define ALL_BUTTONS	(JOY_BUTTON1  | \
					 JOY_BUTTON2  | \
					 JOY_BUTTON3  | \
					 JOY_BUTTON4  | \
					 JOY_BUTTON5  | \
					 JOY_BUTTON6  | \
					 JOY_BUTTON7  | \
					 JOY_BUTTON8  | \
					 JOY_BUTTON9  | \
					 JOY_BUTTON10 | \
					 JOY_BUTTON11 | \
					 JOY_BUTTON12 | \
					 JOY_BUTTON13 | \
					 JOY_BUTTON14 | \
					 JOY_BUTTON15 | \
					 JOY_BUTTON16 | \
					 JOY_BUTTON17 | \
					 JOY_BUTTON18 | \
					 JOY_BUTTON19 | \
					 JOY_BUTTON20 | \
					 JOY_BUTTON21 | \
					 JOY_BUTTON22 | \
					 JOY_BUTTON23 | \
					 JOY_BUTTON24 | \
					 JOY_BUTTON25 | \
					 JOY_BUTTON26 | \
					 JOY_BUTTON27 | \
					 JOY_BUTTON28 | \
					 JOY_BUTTON29 | \
					 JOY_BUTTON30 | \
					 JOY_BUTTON31 | \
					 JOY_BUTTON32 )

#define GETKEYNAME( pgv, str, keystr ) wsprintf( str, keystr, pgv->iJoyId+1 )

#define JOYMOVE_DRAWXY	0x00000001
#define JOYMOVE_DRAWR	0x00000002
#define JOYMOVE_DRAWZ	0x00000004
#define JOYMOVE_DRAWU	0x00000008
#define JOYMOVE_DRAWV	0x00000010
#define JOYMOVE_DRAWALL	JOYMOVE_DRAWXY | JOYMOVE_DRAWR | JOYMOVE_DRAWZ | \
			JOYMOVE_DRAWU | JOYMOVE_DRAWV

/*
 * calibration strings defined by an OEM in the registry
 */
enum {
    CALSTR1=0,
    CALSTR2,
    CALSTR3,
    CALSTR4,
    CALSTR5,
    CALSTR6,
    CALSTR7,
    CALSTR8,
    CALSTR9,
    CALSTR10,
    CALSTR11,
    CALSTR12,
    CALSTR_END
};
#define NUM_CAL_STRS	CALSTR_END

/*
 * structure for holding all OEM data in the registry
 */
typedef struct {
    LPSTR		keyname;
    LPSTR		ident_string;
    LPSTR		vxd_name;
    LPSTR		xy_label;
    LPSTR		z_label;
    LPSTR		r_label;
    LPSTR		u_label;
    LPSTR		v_label;
    LPSTR		pov_label;
    LPSTR		testmove_desc;
    LPSTR		testbutton_desc;
    LPSTR		testmove_cap;
    LPSTR		testbutton_cap;
    LPSTR		testwin_cap;
    LPSTR		cal_cap;
    LPSTR		calwin_cap;
    LPSTR		cal_strs[NUM_CAL_STRS];
    JOYREGHWSETTINGS	hws;
} OEMLIST;

/*
 * generic joystick data
 */
typedef struct {
    LPJOYREGHWCONFIG	joyHWDefaults;
    OEMLIST		*oemList;
    int			oemCount;
    BOOL		bHasUserVals;
    BOOL		bDeleteUserVals;
    JOYREGUSERVALUES	userVals;
    LPSTR		regCfgKey;
    LPSTR		regCurrCfgKey;
    LPSTR		regSettingsCfgKey;
    HBRUSH		hbUp;
    HBRUSH		hbDown;
    BOOL		bHasTimer;
    BOOL		bUseTimer;
    #if !defined( WANT_SHEETS )
	struct _GLOBALVARS *pgvlist;
    #endif
    BOOL		bResetUserVals;
    BOOL		bHasNonStandardUserVals;
} JOYDATA, *LPJOYDATA;

/*
 * structure passed to each sheet
 */
typedef struct {
    LPJOYDATA	pjd;
    int		iJoyId;
} JOYDATAPTR, *LPJOYDATAPTR;

/*
 * structure defining all variables used globally by a tab
 */
typedef struct _GLOBALVARS {
    LPJOYDATA		pjd;
    JOYREGHWCONFIG	joyHWCurr;
    JOYREGHWCONFIG	joyHWOrig;
    JOYRANGE		joyRange;
    BOOL		bOrigPOVIsPoll;
    BOOL		bOrigPOVIsButtonCombos;
    DWORD		dwMaxAxes;
    int			iJoyId;
    /* these vars only used by the sheet */
    unsigned 		joyActiveFlags;
} GLOBALVARS, *LPGLOBALVARS;

/*
 * function prototypes
 */
/* joycal.c */
void DoCalibrate( LPGLOBALVARS pgv, HWND hwnd );

/* joycpl.c */
BOOL CALLBACK JoystickDlg( HWND	hwnd, UINT umsg, WPARAM wParam, LPARAM lParam);
LPVOID DoAlloc( DWORD size );
void DoFree( LPVOID ptr );
void RegistryUpdated( LPGLOBALVARS pgv );
void GetDevCaps( LPGLOBALVARS pgv );
void RegSaveCurrentJoyHW( LPGLOBALVARS pgv );
LPJOYDATA JoystickDataInit( void );
void JoystickDataFini( LPJOYDATA pjd );
LRESULT GetHelpFileName(LPSTR lpszHelpFileName, short* ulSize);
void OnHelp(LPARAM lParam);
#ifdef DEBUG
void cdecl MBOX(LPSTR szFormat, ...);
#endif

/* joymisc.c */
BOOL JoyError( HWND hwnd );
void ChangeIcon( HWND hwnd, int idi, int idc );
void CauseRedraw( LPJOYINFOEX pji, BOOL do_buttons );
void SetOEMText( LPGLOBALVARS pgv, HWND hwnd, BOOL istest );
void ShowControls( LPJOYREGHWCONFIG pcfg, HWND hwnd );
void DoJoyMove( LPGLOBALVARS pgv, HWND hwnd, LPJOYINFOEX pji, LPJOYINFOEX poji, DWORD drawflags );

/* joytest.c */
typedef void (*LPUPDCFGFN)( LPVOID parm );
void DoTest( LPGLOBALVARS pgv, HWND hwnd, LPUPDCFGFN pupdcfgfn, LPVOID pparm );
BOOL CALLBACK TestProc( HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam);

#endif
