#ifdef ARRAYSIZE
#undef ARRAYSIZE
#endif

#ifdef SIZEOF
#undef SIZEOF
#endif

#define CCH_MAX_STRING    256
#define CCH_NONE          20        /* ARRAYSIZE( "(None)" ), big enough for German */

#define CMSEC_COVER_WINDOW_TIMEOUT  (15 * 1000)     // 15 second timeout
#define ID_CVRWND_TIMER             0x96F251CC      // somewhat uniq id

//
// Extra bits for the TraceMsg function
//

#define TF_DUMP_DEVMODE     0x20000000
#define TF_DUMP_CSETTINGS   0x40000000
#define TF_DUMP_DEVMODELIST 0x80000000
#define TF_OC               0x01000000
#define TF_SETUP            0x02000000

// Maximum number of pages we will put in the PropertySheets
#define MAX_PAGES 24


// information about the monitor bitmap
// x, y, dx, dy define the size of the "screen" part of the bitmap
// the RGB is the color of the screen's desktop
// these numbers are VERY hard-coded to a monitor bitmap
#define MON_X   16
#define MON_Y   17
#define MON_DX  152
#define MON_DY  112
#define MON_W   184
#define MON_H   170
#define MON_RGB RGB(0, 128, 128)
#define MON_TRAY 8

#define CDPI_NORMAL     96      // Arbitrarily, 96dpi is "Normal"



BOOL DeskInitCpl(void);
void DeskShowPropSheet( HINSTANCE hInst, HWND hwndParent, LPCTSTR szCmdLine );
BOOL CALLBACK _AddDisplayPropSheetPage(HPROPSHEETPAGE hpage, LPARAM lParam);

INT_PTR APIENTRY BackgroundDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR APIENTRY ScreenSaverDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR APIENTRY AppearanceDlgProc  (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR APIENTRY AdvancedAppearanceDlgProc  (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK GeneralPageProc    (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK MultiMonitorDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


BOOL GetAdvMonitorPropPage(LPVOID lpv, LPFNADDPROPSHEETPAGE lpfnAdd, LPARAM lparam);
BOOL GetAdvMonitorPropPageParam(LPVOID lpv, LPFNADDPROPSHEETPAGE lpfnAdd, LPARAM lparam, LPARAM lparamPage);

INT_PTR APIENTRY DeskDefPropPageProc( HWND hDlg, UINT message, UINT wParam, LONG lParam);

// logging API
#define DeskOpenLog()   SetupOpenLog(FALSE)
#define DeskCloseLog()  SetupCloseLog()

BOOL
CDECL
DeskLogError(
    LogSeverity Severity,
    UINT MsgId,
    ...
    ) ;


// fixreg.c
void FixupRegistryHandlers(void);
BOOL GetDisplayKey(int i, LPTSTR szKey, DWORD cb);
void NukeDisplaySettings(void);

// background previewer includes

#define BP_NEWPAT       0x01    // pattern changed
#define BP_NEWWALL      0x02    // wallpaper changed
#define BP_TILE         0x04    // tile the wallpaper (center otherwise)
#define BP_REINIT       0x08    // reload the image (system colors changed)

#define WM_SETBACKINFO (WM_USER + 1)

#define BACKPREV_CLASS TEXT("BackgroundPreview")

BOOL FAR PASCAL RegisterLookPreviewClass(HINSTANCE hInst);

HBITMAP FAR LoadMonitorBitmap( BOOL bFillDesktop );



//#define Assert(p)   /* nothing */

#define ARRAYSIZE( a )  (sizeof(a) / sizeof(a[0]))
#define SIZEOF( a )     sizeof(a)


//
// CreateCoverWindow
//
// creates a window which obscures the display
//  flags:
//      0 means erase to black
//      COVER_NOPAINT means "freeze" the display
//
// just post it a WM_CLOSE when you're done with it
//
#define COVER_NOPAINT (0x1)
//
HWND FAR PASCAL CreateCoverWindow( DWORD flags );
void DestroyCoverWindow(HWND hwndCover);
int FmtMessageBox(HWND hwnd, UINT fuStyle, DWORD dwTitleID, DWORD dwTextID);

typedef struct tagREPLACEPAGE_LPARAM
{
    PROPSHEETHEADER FAR * ppsh;
    IThemeUIPages * ptuiPages;
} REPLACEPAGE_LPARAM;

//
// Macro to replace MAKEPOINT() since points now have 32 bit x & y
//
#define LPARAM2POINT( lp, ppt ) \
    ((ppt)->x = (int)(short)LOWORD(lp), (ppt)->y = (int)(short)HIWORD(lp))

//
// Globals
//
extern HINSTANCE hInstance;
extern TCHAR gszDeskCaption[CCH_MAX_STRING];

extern TCHAR g_szNULL[];
extern TCHAR g_szNone[CCH_NONE];
extern TCHAR g_szControlIni[];
extern TCHAR g_szPatterns[];

extern TCHAR g_szCurPattern[];   // name of currently selected pattern
extern TCHAR g_szCurWallpaper[]; // name of currently selected wallpaper
extern BOOL g_bValidBitmap;     // whether or not wallpaper is valid

extern TCHAR g_szBoot[];
extern TCHAR g_szSystemIni[];
extern TCHAR g_szWindows[];

extern HDC g_hdcMem;
extern HBITMAP g_hbmDefault;
