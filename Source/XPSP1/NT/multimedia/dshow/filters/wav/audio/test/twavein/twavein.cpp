//--------------------------------------------------------------------------;
//
//  File: twavein.cpp
//
//  Copyright (c) 1993,1996 Microsoft Corporation.  All rights reserved
//
//  Abstract:
//
//  Contents:
//      tstGetTestInfo()
//      MenuProc()
//      InitOptionsMenu()
//      tstInit()
//      execTest()
//      tstTerminate()
//      tstAppWndProc()
//      dprintf()
//
//  History:
//      08/03/93    T-OriG   - sample test app
//      9-Mar-95    v-mikere - adapted for Quartz source filter tests
//      15-Mar-95   v-mikere - Quartz sample test app
//      4-Apr-95    t-jmcal  - Used as template to become the WaveIn
//                             filter unit test
//
//--------------------------------------------------------------------------;

#include <streams.h>    // Streams architecture
#include <windowsx.h>   // Windows macros etc.
#include <tstshell.h>   // Include file for the test shell's APIs
#include "sink.h"       // Test sink definition
#include "twavein.h"    // Various includes, constants, prototypes, globals


// Globals

HWND    ghwndTstShell;  // A handle to the main window of the test shell.
                        // It's not used here, but may be used by test apps.

HINSTANCE hinst;        // A handle to the running instance of the test
                        // shell.  It's not used here, but may be used by
                        // test apps.

HMENU   hmenuOptions;   // A handle to the options menu

CTestSink   *gpSink;    // Test sink object for all the tests


// App Name
#ifdef _WIN32
LPSTR           szAppName = "Quartz WaveIn Sample test app - Win32";
#else
LPSTR           szAppName = "Quartz WaveIn Sample test app - Motown";
#endif

//--------------------------------------------------------------------------;
//
//  int tstGetTestInfo
//
//  Description:
//      Called by the test shell to get information about the test.  Also
//      saves a copy of the running instance of the test shell.
//
//  Arguments:
//      HINSTANCE hinstance: A handle to the running instance of the test
//          shell
//
//      LPSTR lpszTestName: Pointer to buffer of name for test.  Among
//          other things, it is used as a caption for the main window and
//          as the name of its class.
//
//      LPSTR lpszPathSection: Pointer to buffer of name of section in
//          win.ini in which the default input and output paths are
//          stored.
//
//      LPWORD wPlatform: The platform on which the tests are to be run,
//          which may be determined dynamically.  In order for a test to
//          be shown on the run list, it must have all the bits found in
//          wPlatform turned on.  It is enough for one bit to be turned off
//          to disqualify the test.  This also means that if this value is
//          zero, all tests will be run.  In order to make this more
//          mathematically precise, I shall give the relation which Test
//          Shell uses to decide whether a test with platform flags
//          wTestPlatform may run:  It may run if the following is TRUE:
//          ((wTestPlatform & wPlatform) == wPlatform)
//
//  Return (int):
//      The value which identifies the test list resouce (found in the
//      resource file).
//
//  History:
//      08/03/93    T-OriG   - sample test app
//      9-Mar-95    v-mikere - adapted for Quartz source filter tests
//
//--------------------------------------------------------------------------;

int tstGetTestInfo
(
    HINSTANCE   hinstance,
    LPSTR       lpszTestName,
    LPSTR       lpszPathSection,
    LPWORD      wPlatform
)
{
    hinst = hinstance;      // Save a copy of a handle to the running instance
    lstrcpyA (lpszTestName, szAppName);
    lstrcpyA (lpszPathSection, szAppName);
    *wPlatform = 0;         // The platform the test is running on, to be
                            // determined dynamically.
    return TEST_LIST;
} // tstGetTestInfo()



//--------------------------------------------------------------------------;
//
//  void InitOptionsMenu
//
//  Description:
//      Creates an additional app-specific menu.  Note that this
//      function is called from within tstInit as all menu installations
//      MUST be done in tstInit or else the app's behavior is undefined.
//      Also note the calls to tstInstallCustomTest which is a shell API
//      that allows custom installation of tests.  From tstshell version
//      2.0 on, it is possible to install menus the usual way and trap the
//      appropriate window messages, though the method presented here is
//      still the preferred one for Test Applications to use.
//
//      For the Quartz tests, complete menu structures and window procs
//      exist, so it is simpler to just load and append the existing menu
//      resources than to call tstInstallCustomTest once for each menu
//      option.  The window message handling is incorporated in
//      tstAppWndProc.
//
//  Arguments:
//      LRESULT (CALLBACK* MenuProc)(HWND,UINT,WPARAM,LPARAM):
//          The menu function (not used in the Quartz tests).
//
//  Return (BOOL):
//      TRUE if menu installation is successful, FALSE otherwise
//
//  History:
//      06/08/93    T-OriG   - sample test app
//      9-Mar-95    v-mikere - adapted for Quartz source filter tests
//      15-Mar-95   v-mikere - Quartz sample test app
//
//--------------------------------------------------------------------------;

BOOL InitOptionsMenu
(
    LRESULT (CALLBACK* MenuProc)(HWND, UINT, WPARAM, LPARAM)
)
{
    HMENU   hWaveInMenu;

    if (NULL == (hWaveInMenu = LoadMenu(hinst, TEXT("twaveinMenu"))))
    {
        return(FALSE);
    }

    if
    (
        !AppendMenu(GetMenu(ghwndTstShell),
                    MF_POPUP,
                    (UINT) hWaveInMenu,
                    TEXT("&WaveIn"))
    )
    {
        return(FALSE);
    }

    DrawMenuBar(ghwndTstShell);
    return TRUE;
}



//--------------------------------------------------------------------------;
//
//  BOOL tstInit
//
//  Description:
//      Called by the test shell to provide the test program with an
//      opportunity to do whatever initialization it needs to do before
//      user interaction is initiated.  It also provides the test program
//      with an opportunity to keep a copy of a handle to the main window,
//      if the test program needs it.  In order to use some of the more
//      advanced features of test shell, several installation must be done
//      here:
//
//      -- All menu installation must be done here by calling
//          tstInstallCustomTest (that is, all menus that the test
//          application wants to add).
//
//      -- If the test application wants to trap the window messages of
//          the main test shell window, it must install its default
//          window procedure here by calling tstInstallDefWindowProc.
//
//      -- If the test application would like to use the status bar for
//          displaying the name of the currently running test, it must
//          call tstDisplayCurrentTest here.
//
//      -- If the test application would like to change the stop key from
//          ESC to something else, it must do so here by calling
//          tstChangeStopVKey.
//
//      -- If the test application would like to add dynamic test cases
//          to the test list, it must first add their names to the
//          virtual string table using tstAddNewString (and add their
//          group's name too), and then add the actual tests using
//          tstAddTestCase.  The virtual string table is an abstraction
//          which behaves just like a string table from the outside with
//          the exception that it accepts dynamically added string.
//
//  Arguments:
//      HWND hwndMain: A handle to the main window
//
//  Return (BOOL):
//      TRUE if initialization went well, FALSE otherwise which will abort
//      execution.
//
//  History:
//      06/08/93    T-OriG   - sample test app
//      9-Mar-95    v-mikere - adapted for Quartz source filter tests
//
//--------------------------------------------------------------------------;

BOOL tstInit
(
    HWND    hwndMain
)
{
    HRESULT     hr;


    QzInitialize(NULL);            // Initialise COM library
    DbgInitialise(hinst);

    // It might have been nice to have this a few lines earlier
    // - but we gotta do DbgInitialise first!
    DbgLog((LOG_TRACE, 1, TEXT("Entering tstInit")));

    // Create the test sink object

    gpSink = new CTestSink(NULL, &hr, ghwndTstShell);
    ASSERT(gpSink);
    ASSERT(SUCCEEDED(hr));

    // initially it has a ref count of 0.
    gpSink->NonDelegatingAddRef();

   // Keep a copy of a handle to the main window
    ghwndTstShell = hwndMain;

    // Installs a default windows procedure which may handle messages
    // directed to Test Shell's main window.  It is vital to note that
    // this window function is substituted for DefWindowProc and not in
    // addition to it, and therefore DefWindowProc MUST be called from
    // within it in the default case (look at tstAppWndProc for an example).
    tstInstallDefWindowProc (tstAppWndProc);

    // Install the custom menus.  Look at InitOptionsMenu for more details.
    if (InitOptionsMenu(NULL)==FALSE)
        return FALSE;  // If menu installation failed, abort execution

    // This is a shell API which tells Test Shell to display the name of
    // the currenly executing API in its status bar.  It is a really nice
    // feature for test applications which do not use the toolbar for any
    // other purpose, as it comfortably notifies the user of the progress
    // of the tests.
    tstDisplayCurrentTest();

    // Change the stop key from ESC to SPACE
    tstChangeStopVKey (VSTOPKEY);

    // Add the names of the dynamic tests and dynamic groups to the
    // virtual string table.  Before the tests are added, it is important
    // to add their names, as the name ID is required by tstAddTestCase.
//    tstAddNewString (ID_TESTLAST + 1, (LPSTR) "The first test added dynamically");
//    tstAddNewString (GRP_LAST + 1, (LPSTR) "A dynamically added group");
//    tstAddNewString (ID_TESTLAST + 2, (LPSTR) "The first test in a dynamically added group");

    // Add test cases dynamically.  Note that these test cases will execute
    // on any platform, since they are added dynamically.  Real test
    // applications probably want to check the run environment and then
    // add dynamic test cases selectively.
//    tstAddTestCase (ID_TESTLAST + 1, TST_AUTOMATIC, FX_TEST1, GRP_ONE);
//    tstAddTestCase (ID_TESTLAST + 2, TST_AUTOMATIC, FX_TEST1, GRP_LAST + 1);

    DbgLog((LOG_TRACE,1, TEXT("Exiting tstInit")));
    return(TRUE);
} // tstInit()




//--------------------------------------------------------------------------;
//
//  int execTest
//
//  Description:
//      This is the actual test function which is called from within the
//      test shell.  It is passed various information about the test case
//      it is asked to run, and branches off to the appropriate test
//      function.  Note that it needs not switch on nFxID, but may also
//      use iCase or wID.
//
//  Arguments:
//      int nFxID: The test case identifier, also found in the third column
//          in the third column of the test list in the resource file
//
//      int iCase: The test case's number, which expresses the ordering
//          used by the test shell.
//
//      UINT wID: The test case's string ID, which identifies the string
//          containing the description of the test case.  Note that it is
//          also a good candidate for use in the switch statement, as it
//          is unique to each test case.
//
//      UINT wGroupID: The test case's group's string ID, which identifies
//          the string containing the description of the test case's group.
//
//  Return (int): Indicates the result of the test by using TST_FAIL,
//          TST_PASS, TST_OTHER, TST_ABORT, TST_TNYI, TST_TRAN, or TST_TERR
//
//  History:
//      06/08/93    T-OriG   - sample test app
//      9-Mar-95    v-mikere - adapted for Quartz source filter tests
//
//--------------------------------------------------------------------------;

int execTest
(
    int     nFxID,
    int     iCase,
    UINT    wID,
    UINT    wGroupID
)
{
    int ret = TST_OTHER;

    tstBeginSection(" ");

    switch(nFxID)
    {
        //
        //  The test cases
        //

        case FX_TEST1:
            ret = execTest1();
            break;

        case FX_TEST2:
            ret = execTest2();
            break;

        case FX_TEST3:
            ret = execTest3();
            break;

        case FX_TEST4:
            ret = execTest4();
            break;

        default:
            break;
    }

    tstEndSection();

    return(ret);

//Abort:
    return(TST_ABORT);
} // execTest()




//--------------------------------------------------------------------------;
//
//  void tstTerminate
//
//  Description:
//      This function is called when the test series is finished to free
//      structures and do whatever cleanup work it needs to do.  If it
//      needs not do anything, it may just return.
//
//  Arguments:
//      None.
//
//  Return (void):
//
//  History:
//      06/08/93    T-OriG   - sample test app
//      9-Mar-95    v-mikere - adapted for Quartz source filter tests
//
//--------------------------------------------------------------------------;

void tstTerminate
(
    void
)
{
    DbgLog((LOG_TRACE, 1, TEXT("Entering tstTerminate")));

    gpSink->NonDelegatingRelease();

    DbgTerminate();
    QzUninitialize();

    DbgLog((LOG_TRACE, 1, TEXT("Exiting tstTerminate")));
    return;
} // tstTerminate()




//--------------------------------------------------------------------------;
//
//  LRESULT tstAppWndProc
//
//  Description:
//      This shows how a test application can trap the window messages
//      received by the main Test Shell window.  It is installed in
//      in tstInit by calling tstInstallDefWindowProc, and receives
//      all window messages since.  This allows the test application to
//      be notified of certain event via a window without creating its
//      own hidden window or waiting in a tight PeekMessage() loop.  Note
//      that it is extremely important to call DefWindowProcA in the default
//      case as that is NOT done in tstshell's main window procedure if
//      tstInstallDefWindowProc is used.  DefWindowProcA has to be used as the
//      test shell main window is an ANSI window.
//
//  Arguments:
//      HWND hWnd: A handle to the window
//
//      UINT msg: The message to be processed
//
//      WPARAM wParam: The first parameters, meaning depends on msg
//
//      LPARAM lParam: The second parameter, meaning depends on msg
//
//  Return (LRESULT):
//
//  History:
//      08/03/93    T-OriG   - sample test app
//      9-Mar-95    v-mikere - adapted for Quartz source filter tests
//
//--------------------------------------------------------------------------;

LRESULT FAR PASCAL tstAppWndProc
(
    HWND    hWnd,
    UINT    msg,
    WPARAM  wParam,
    LPARAM  lParam
)
{
    switch (msg)
    {
        case WM_COMMAND:

        switch (GET_WM_COMMAND_ID(wParam, lParam)) {

            case IDM_DISCONNECT:
                gpSink->TestDisconnect();
                return 0;

            case IDM_CONNECT:
                gpSink->TestConnect();
                return 0;

            case IDM_STOP:
                gpSink->TestStop();
                return 0;

            case IDM_PAUSE:
                gpSink->TestPause();
                return 0;

            case IDM_RUN:
                gpSink->TestRun();
                return 0;
        }
        break;
    }
    return DefWindowProcA (hWnd, msg, wParam, lParam);
}

