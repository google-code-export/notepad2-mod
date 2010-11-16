/******************************************************************************
*
*
* Notepad2
*
* Notepad2.c
*   Main application window functionality
*
* See Readme.txt for more information about this source code.
* Please send me your comments to this work.
*
* See License.txt for details about distribution and modification.
*
*                                              (c) Florian Balmer 1996-2010
*                                                  florian.balmer@gmail.com
*                                               http://www.flos-freeware.ch
*
*
******************************************************************************/
#if !defined(_WIN32_WINNT)
#define _WIN32_WINNT 0x501
#endif
#include <windows.h>
#include <commctrl.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <shellapi.h>
#include <commdlg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "scintilla.h"
#include "scilexer.h"
#include "edit.h"
#include "styles.h"
#include "helpers.h"
#include "dialogs.h"
#include "notepad2.h"
#include "scicall.h"
#include "resource.h"



/******************************************************************************
*
* Local and global Variables for Notepad2.c
*
*/
HWND      hwndStatus;
HWND      hwndToolbar;
HWND      hwndReBar;
HWND      hwndEdit;
HWND      hwndEditFrame;
HWND      hwndMain;
HWND      hwndNextCBChain = NULL;
HWND      hDlgFindReplace = NULL;

#define NUMTOOLBITMAPS  24
#define NUMINITIALTOOLS 24
#define MARGIN_FOLD_INDEX 2

TBBUTTON  tbbMainWnd[] = { {0,IDT_FILE_NEW,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {1,IDT_FILE_OPEN,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {2,IDT_FILE_BROWSE,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {3,IDT_FILE_SAVE,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {0,0,0,TBSTYLE_SEP,0,0},
                           {4,IDT_EDIT_UNDO,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {5,IDT_EDIT_REDO,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {0,0,0,TBSTYLE_SEP,0,0},
                           {6,IDT_EDIT_CUT,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {7,IDT_EDIT_COPY,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {8,IDT_EDIT_PASTE,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {0,0,0,TBSTYLE_SEP,0,0},
                           {9,IDT_EDIT_FIND,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {10,IDT_EDIT_REPLACE,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {0,0,0,TBSTYLE_SEP,0,0},
                           {11,IDT_VIEW_WORDWRAP,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {0,0,0,TBSTYLE_SEP,0,0},
                           {12,IDT_VIEW_ZOOMIN,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {13,IDT_VIEW_ZOOMOUT,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {0,0,0,TBSTYLE_SEP,0,0},
                           {14,IDT_VIEW_SCHEME,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {15,IDT_VIEW_SCHEMECONFIG,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {0,0,0,TBSTYLE_SEP,0,0},
                           {16,IDT_FILE_EXIT,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {17,IDT_FILE_SAVEAS,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {18,IDT_FILE_SAVECOPY,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {19,IDT_EDIT_CLEAR,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {20,IDT_FILE_PRINT,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {21,IDT_FILE_OPENFAV,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {22,IDT_FILE_ADDTOFAV,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0},
                           {23,IDT_VIEW_TOGGLEFOLDS,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0} };

WCHAR      szIniFile[MAX_PATH] = L"";
WCHAR      szIniFile2[MAX_PATH] = L"";
BOOL      bSaveSettings;
BOOL      bSaveRecentFiles;
BOOL      bSaveFindReplace;
WCHAR      tchLastSaveCopyDir[MAX_PATH] = L"";
WCHAR      tchOpenWithDir[MAX_PATH];
WCHAR      tchFavoritesDir[MAX_PATH];
WCHAR      tchDefaultDir[MAX_PATH];
WCHAR      tchDefaultExtension[64];
WCHAR      tchFileDlgFilters[5*1024];
WCHAR      tchToolbarButtons[512];
WCHAR      tchToolbarBitmap[MAX_PATH];
WCHAR      tchToolbarBitmapHot[MAX_PATH];
WCHAR      tchToolbarBitmapDisabled[MAX_PATH];
int       iPathNameFormat;
BOOL      fWordWrap;
int       iWordWrapMode;
int       iWordWrapIndent;
int       iWordWrapSymbols;
BOOL      bShowWordWrapSymbols;
BOOL      bMatchBraces;
BOOL      bAutoIndent;
BOOL      bAutoCloseTags;
BOOL      bShowIndentGuides;
BOOL      bHiliteCurrentLine;
BOOL      bTabsAsSpaces;
BOOL      bTabsAsSpacesG;
BOOL      bTabIndents;
int       iTabWidth;
int       iTabWidthG;
int       iIndentWidth;
int       iIndentWidthG;
BOOL      bMarkLongLines;
int       iLongLinesLimit;
int       iLongLinesLimitG;
int       iLongLineMode;
int       iWrapCol = 0;
BOOL      bShowSelectionMargin;
BOOL      bShowLineNumbers;
BOOL      bShowCodeFolding;
BOOL      bViewWhiteSpace;
BOOL      bViewEOLs;
int       iDefaultEncoding;
BOOL      bSkipUnicodeDetection;
BOOL      bLoadASCIIasUTF8;
BOOL      bLoadNFOasOEM;
BOOL      bNoEncodingTags;
int       iSrcEncoding = -1;
int       iWeakSrcEncoding = -1;
int       iDefaultEOLMode;
BOOL      bFixLineEndings;
BOOL      bAutoStripBlanks;
int       iPrintHeader;
int       iPrintFooter;
int       iPrintColor;
int       iPrintZoom;
RECT      pagesetupMargin;
BOOL      bSaveBeforeRunningTools;
int       iFileWatchingMode;
BOOL      bResetFileWatching;
int       iFileCheckInverval;
int       iEscFunction;
BOOL      bAlwaysOnTop;
BOOL      bMinimizeToTray;
BOOL      bTransparentMode;
BOOL      bTransparentModeAvailable;
BOOL      bShowToolbar;
BOOL      bShowStatusbar;

typedef struct _wi
{
  int x;
  int y;
  int cx;
  int cy;
  int max;
} WININFO;

WININFO wi;
BOOL    bStickyWinPos;

BOOL    bIsAppThemed;
int     cyReBar;
int     cyReBarFrame;
int     cxEditFrame;
int     cyEditFrame;

int     cxEncodingDlg;
int     cyEncodingDlg;
int     cxRecodeDlg;
int     cyRecodeDlg;
int     cxFileMRUDlg;
int     cyFileMRUDlg;
int     cxOpenWithDlg;
int     cyOpenWithDlg;
int     cxFavoritesDlg;
int     cyFavoritesDlg;
int     xFindReplaceDlg;
int     yFindReplaceDlg;

LPWSTR      lpFileList[32];
int         cFileList = 0;
int         cchiFileList = 0;
LPWSTR      lpFileArg = NULL;
LPWSTR      lpSchemeArg = NULL;
LPWSTR      lpMatchArg = NULL;
LPWSTR      lpEncodingArg = NULL;
LPMRULIST  pFileMRU;
LPMRULIST  mruFind;
LPMRULIST  mruReplace;

DWORD     dwLastIOError;
WCHAR      szCurFile[MAX_PATH+40];
FILEVARS   fvCurFile;
BOOL      bModified;
BOOL      bReadOnly = FALSE;
int       iEncoding;
int       iOriginalEncoding;
int       iEOLMode;

int       iDefaultCodePage;
int       iDefaultCharSet;

int       iInitialLine;
int       iInitialColumn;

int       iInitialLexer;

BOOL      bLastCopyFromMe = FALSE;
DWORD     dwLastCopyTime;

UINT      uidsAppTitle = IDS_APPTITLE;
WCHAR     szTitleExcerpt[128] = L"";
int       fKeepTitleExcerpt = 0;

HANDLE    hChangeHandle = NULL;
BOOL      bRunningWatch = FALSE;
WIN32_FIND_DATA fdCurFile;

UINT      msgTaskbarCreated = 0;

HMODULE   hModUxTheme = NULL;

EDITFINDREPLACE efrData = { "", "", "", "", 0, 0, 0, 0, 0, 0, NULL };
UINT cpLastFind = 0;
BOOL bReplaceInitialized = FALSE;

extern NP2ENCODING mEncoding[];

int iLineEndings[3] = {
  SC_EOL_CRLF,
  SC_EOL_LF,
  SC_EOL_CR
};

WCHAR wchPrefixSelection[256] = L"";
WCHAR wchAppendSelection[256] = L"";

WCHAR wchPrefixLines[256] = L"";
WCHAR wchAppendLines[256] = L"";

int   iSortOptions = 0;

BOOL      fIsElevated = FALSE;
WCHAR     wchWndClass[16] = WC_NOTEPAD2;

LPMALLOC  g_lpMalloc;
HINSTANCE g_hInstance;
HANDLE    g_hScintilla;
UINT16    g_uWinVer;
WCHAR     g_wchAppUserModelID[64];



#ifdef BOOKMARK_EDITION
    //Graphics for bookmark indicator
    /* XPM */
    static char * bookmark_pixmap[] = {
    "11 11 44 1",
    "   c #EBE9ED",
    ".  c #E5E3E7",
    "+  c #767C6D",
    "@  c #2A3120",
    "#  c #1B2312",
    "$  c #333B28",
    "%  c #E3E1E5",
    "&  c #D8D6DA",
    "*  c #444D38",
    "=  c #3F5C19",
    "-  c #63AD00",
    ";  c #73C900",
    ">  c #64AF00",
    ",  c #3D5718",
    "'  c #3E4634",
    ")  c #7B8172",
    "!  c #42601A",
    "~  c #74CB00",
    "{  c #71C600",
    "]  c #3A5317",
    "^  c #707668",
    "/  c #3F4931",
    "(  c #262C1D",
    "_  c #2F3A1E",
    ":  c #72C700",
    "<  c #74CA00",
    "[  c #0E1109",
    "}  c #3C462F",
    "|  c #62AC00",
    "1  c #21271A",
    "2  c #7A8071",
    "3  c #405D19",
    "4  c #3D5A18",
    "5  c #D9D7DB",
    "6  c #4E5841",
    "7  c #72C800",
    "8  c #63AC00",
    "9  c #3F5B19",
    "0  c #3D4533",
    "a  c #DFDDE0",
    "b  c #353E29",
    "c  c #29331B",
    "d  c #7B8272",
    "e  c #DDDBDF",
    "           ",
    "  .+@#$+%  ",
    " &*=-;>,'  ",
    " )!~~~~{]^ ",
    " /-~~~~~>( ",
    " _:~~~~~<[ ",
    " }|~~~~~|1 ",
    " 23~~~~;4+ ",
    " 56=|7890  ",
    "  a2bc}de  ",
    "           "};
#endif



//=============================================================================
//
// Flags
//
int flagNoReuseWindow      = 0;
int flagReuseWindow        = 0;
int flagMultiFileArg       = 0;
int flagSingleFileInstance = 0;
int flagStartAsTrayIcon    = 0;
int flagRelativeFileMRU    = 0;
int flagPortableMyDocs     = 0;
int flagNoFadeHidden       = 0;
int flagSimpleIndentGuides = 0;
int fNoHTMLGuess           = 0;
int fNoCGIGuess            = 0;
int fNoFileVariables       = 0;
int flagPosParam           = 0;
int flagDefaultPos         = 0;
int flagNewFromClipboard   = 0;
int flagPasteBoard         = 0;
int flagSetEncoding        = 0;
int flagSetEOLMode         = 0;
int flagJumpTo             = 0;
int flagMatchText          = 0;
int flagLexerSpecified     = 0;
int flagQuietCreate        = 0;
int flagUseSystemMRU       = 0;
int flagRelaunchElevated   = 0;
int flagDisplayHelp        = 0;



//==============================================================================
//
//  Folding Functions
//
//
typedef enum {
  EXPAND =  1,
  SNIFF  =  0,
  FOLD   = -1
} FOLD_ACTION;

#define FOLD_CHILDREN SCMOD_CTRL
#define FOLD_SIBLINGS SCMOD_SHIFT

BOOL __stdcall FoldToggleNode( int ln, FOLD_ACTION action )
{
  BOOL fExpanded = SciCall_GetFoldExpanded(ln);

  if ((action == FOLD && fExpanded) || (action == EXPAND && !fExpanded))
  {
    SciCall_ToggleFold(ln);
    return(TRUE);
  }

  return(FALSE);
}

void __stdcall FoldToggleAll( FOLD_ACTION action )
{
  BOOL fToggled = FALSE;
  int lnTotal = SciCall_GetLineCount();
  int ln;

  for (ln = 0; ln < lnTotal; ++ln)
  {
    if (SciCall_GetFoldLevel(ln) & SC_FOLDLEVELHEADERFLAG)
    {
      if (action == SNIFF)
        action = SciCall_GetFoldExpanded(ln) ? FOLD : EXPAND;

      if (FoldToggleNode(ln, action))
        fToggled = TRUE;
    }
  }

  if (fToggled)
  {
    SciCall_SetXCaretPolicy(CARET_SLOP|CARET_STRICT|CARET_EVEN,50);
    SciCall_SetYCaretPolicy(CARET_SLOP|CARET_STRICT|CARET_EVEN,5);
    SciCall_ScrollCaret();
    SciCall_SetXCaretPolicy(CARET_SLOP|CARET_EVEN,50);
    SciCall_SetYCaretPolicy(CARET_EVEN,0);
  }
}

void __stdcall FoldPerformAction( int ln, int mode, FOLD_ACTION action )
{
  if (action == SNIFF)
    action = SciCall_GetFoldExpanded(ln) ? FOLD : EXPAND;

  if (mode & (FOLD_CHILDREN | FOLD_SIBLINGS))
  {
    // ln/lvNode: line and level of the source of this fold action
    int lnNode = ln;
    int lvNode = SciCall_GetFoldLevel(lnNode) & SC_FOLDLEVELNUMBERMASK;
    int lnTotal = SciCall_GetLineCount();

    // lvStop: the level over which we should not cross
    int lvStop = lvNode;

    if (mode & FOLD_SIBLINGS)
    {
      ln = SciCall_GetFoldParent(lnNode) + 1;  // -1 + 1 = 0 if no parent
      --lvStop;
    }

    for ( ; ln < lnTotal; ++ln)
    {
      int lv = SciCall_GetFoldLevel(ln);
      BOOL fHeader = lv & SC_FOLDLEVELHEADERFLAG;
      lv &= SC_FOLDLEVELNUMBERMASK;

      if (lv < lvStop || (lv == lvStop && fHeader && ln != lnNode))
        return;
      else if (fHeader && (lv == lvNode || (lv > lvNode && mode & FOLD_CHILDREN)))
        FoldToggleNode(ln, action);
    }
  }
  else
  {
    FoldToggleNode(ln, action);
  }
}

void __stdcall FoldClick( int ln, int mode )
{
  static struct {
    int ln;
    int mode;
    DWORD dwTickCount;
  } prev;

  BOOL fGotoFoldPoint = mode & FOLD_SIBLINGS;

  if (!(SciCall_GetFoldLevel(ln) & SC_FOLDLEVELHEADERFLAG))
  {
    // Not a fold point: need to look for a double-click

    if ( prev.ln == ln && prev.mode == mode &&
         GetTickCount() - prev.dwTickCount <= GetDoubleClickTime() )
    {
      prev.ln = -1;  // Prevent re-triggering on a triple-click

      ln = SciCall_GetFoldParent(ln);

      if (ln >= 0 && SciCall_GetFoldExpanded(ln))
        fGotoFoldPoint = TRUE;
      else
        return;
    }
    else
    {
      // Save the info needed to match this click with the next click
      prev.ln = ln;
      prev.mode = mode;
      prev.dwTickCount = GetTickCount();
      return;
    }
  }

  FoldPerformAction(ln, mode, SNIFF);

  if (fGotoFoldPoint)
    EditJumpTo(hwndEdit, ln + 1, 0);
}

void __stdcall FoldAltArrow( int key, int mode )
{
  // Because Alt-Shift is already in use (and because the sibling fold feature
  // is not as useful from the keyboard), only the Ctrl modifier is supported

  if (bShowCodeFolding && (mode & (SCMOD_ALT | SCMOD_SHIFT)) == SCMOD_ALT)
  {
    int ln = SciCall_LineFromPosition(SciCall_GetCurrentPos());

    // Jump to the next visible fold point
    if (key == SCK_DOWN && !(mode & SCMOD_CTRL))
    {
      int lnTotal = SciCall_GetLineCount();
      for (ln = ln + 1; ln < lnTotal; ++ln)
      {
        if ( SciCall_GetFoldLevel(ln) & SC_FOLDLEVELHEADERFLAG &&
             SciCall_GetLineVisible(ln) )
        {
          EditJumpTo(hwndEdit, ln + 1, 0);
          return;
        }
      }
    }

    // Jump to the previous visible fold point
    else if (key == SCK_UP && !(mode & SCMOD_CTRL))
    {
      for (ln = ln - 1; ln >= 0; --ln)
      {
        if ( SciCall_GetFoldLevel(ln) & SC_FOLDLEVELHEADERFLAG &&
             SciCall_GetLineVisible(ln) )
        {
          EditJumpTo(hwndEdit, ln + 1, 0);
          return;
        }
      }
    }

    // Perform a fold/unfold operation
    else if (SciCall_GetFoldLevel(ln) & SC_FOLDLEVELHEADERFLAG)
    {
      if (key == SCK_LEFT ) FoldPerformAction(ln, mode, FOLD);
      if (key == SCK_RIGHT) FoldPerformAction(ln, mode, EXPAND);
    }
  }
}



//=============================================================================
//
//  WinMain()
//
//
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInst,
                   LPSTR lpCmdLine,int nCmdShow)
{

  MSG msg;
  HWND hwnd;
  HACCEL hAccMain;
  HACCEL hAccFindReplace;
  INITCOMMONCONTROLSEX icex;
  //HMODULE hSciLexer;

  // Set global variable g_hInstance
  g_hInstance = hInstance;

  // Set the Windows version global variable
  g_uWinVer = LOWORD(GetVersion());
  g_uWinVer = MAKEWORD(HIBYTE(g_uWinVer),LOBYTE(g_uWinVer));

  SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOOPENFILEERRORBOX);

  // check if running at least on Windows 2000
  if (!Is2k()) {
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER|
        FORMAT_MESSAGE_FROM_SYSTEM|
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        ERROR_OLD_WIN_VERSION,
        MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), // Default language
        (LPWSTR)&lpMsgBuf,
        0,
        NULL);
    MessageBox(NULL,(LPCWSTR)lpMsgBuf,L"Notepad2",MB_OK|MB_ICONEXCLAMATION);
    LocalFree(lpMsgBuf);
    return(0);
  }

  // Check if running with elevated privileges
  fIsElevated = IsElevated();

  // Default Encodings (may already be used for command line parsing)
  Encoding_InitDefaults();

  // Command Line, Ini File and Flags
  ParseCommandLine();
  FindIniFile();
  TestIniFile();
  CreateIniFile();
  LoadFlags();

  // set AppUserModelID
  PrivateSetCurrentProcessExplicitAppUserModelID(g_wchAppUserModelID);

  // Command Line Help Dialog
  if (flagDisplayHelp) {
    DisplayCmdLineHelp();
    return(0);
  }

  // Adapt window class name
  if (fIsElevated)
    StrCat(wchWndClass,L"U");
  if (flagPasteBoard)
    StrCat(wchWndClass,L"B");

  // Relaunch with elevated privileges
  if (RelaunchElevated())
    return(0);

  // Try to run multiple instances
  if (RelaunchMultiInst())
    return(0);

  // Try to activate another window
  if (ActivatePrevInst())
    return(0);

  // Init OLE and Common Controls
  OleInitialize(NULL);
  SHGetMalloc(&g_lpMalloc);

  icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
  icex.dwICC  = ICC_WIN95_CLASSES|ICC_COOL_CLASSES|ICC_BAR_CLASSES|ICC_USEREX_CLASSES;
  InitCommonControlsEx(&icex);

  msgTaskbarCreated = RegisterWindowMessage(L"TaskbarCreated");

  hModUxTheme = LoadLibrary(L"uxtheme.dll");

  Scintilla_RegisterClasses(hInstance);

  // Load Settings
  LoadSettings();

  if (!InitApplication(hInstance))
    return FALSE;

  if (!(hwnd = InitInstance(hInstance,lpCmdLine,nCmdShow)))
    return FALSE;

  hAccMain = LoadAccelerators(hInstance,MAKEINTRESOURCE(IDR_MAINWND));
  hAccFindReplace = LoadAccelerators(hInstance,MAKEINTRESOURCE(IDR_ACCFINDREPLACE));

  while (GetMessage(&msg,NULL,0,0))
  {
    if (IsWindow(hDlgFindReplace) && (msg.hwnd == hDlgFindReplace || IsChild(hDlgFindReplace,msg.hwnd)))
      if (TranslateAccelerator(hDlgFindReplace,hAccFindReplace,&msg) || IsDialogMessage(hDlgFindReplace,&msg))
        continue;

    if (!TranslateAccelerator(hwnd,hAccMain,&msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  // Save Settings is done elsewhere

  Scintilla_ReleaseResources();
  UnregisterClass(wchWndClass,hInstance);

  if (hModUxTheme)
    FreeLibrary(hModUxTheme);

  g_lpMalloc->lpVtbl->Release(g_lpMalloc);
  OleUninitialize();

  return(msg.wParam);

  hPrevInst;

}


//=============================================================================
//
//  InitApplication()
//
//
BOOL InitApplication(HINSTANCE hInstance)
{

  WNDCLASS   wc;

  wc.style         = CS_BYTEALIGNWINDOW | CS_DBLCLKS;
  wc.lpfnWndProc   = (WNDPROC)MainWndProc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hInstance     = hInstance;
  wc.hIcon         = LoadIcon(hInstance,MAKEINTRESOURCE(IDR_MAINWND));
  wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_3DFACE+1);
  wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MAINWND);
  wc.lpszClassName = wchWndClass;

  return RegisterClass(&wc);

}


//=============================================================================
//
//  InitInstance()
//
//
HWND InitInstance(HINSTANCE hInstance,LPSTR pszCmdLine,int nCmdShow)
{

  RECT rc = { wi.x, wi.y, wi.x+wi.cx, wi.y+wi.cy };
  RECT rc2;
  MONITORINFO mi;

  HMONITOR hMonitor = MonitorFromRect(&rc,MONITOR_DEFAULTTONEAREST);
  mi.cbSize = sizeof(mi);
  GetMonitorInfo(hMonitor,&mi);

  if (flagDefaultPos == 1) {
    wi.x = wi.y = wi.cx = wi.cy = CW_USEDEFAULT;
    wi.max = 0;
  }

  else if (flagDefaultPos >= 4) {
    SystemParametersInfo(SPI_GETWORKAREA,0,&rc,0);
    if (flagDefaultPos & 8)
      wi.x = (rc.right - rc.left) / 2;
    else
      wi.x = rc.left;
    wi.cx = rc.right - rc.left;
    if (flagDefaultPos & (4|8))
      wi.cx /= 2;
    if (flagDefaultPos & 32)
      wi.y = (rc.bottom - rc.top) / 2;
    else
      wi.y = rc.top;
    wi.cy = rc.bottom - rc.top;
    if (flagDefaultPos & (16|32))
      wi.cy /= 2;
    if (flagDefaultPos & 64) {
      wi.x = rc.left;
      wi.y = rc.top;
      wi.cx = rc.right - rc.left;
      wi.cy = rc.bottom - rc.top;
    }
    if (flagDefaultPos & 128) {
      wi.x += (flagDefaultPos & 8) ? 4 : 8;
      wi.cx -= (flagDefaultPos & (4|8)) ? 12 : 16;
      wi.y += (flagDefaultPos & 32) ? 4 : 8;
      wi.cy -= (flagDefaultPos & (16|32)) ? 12 : 16;
    }
  }

  else if (flagDefaultPos == 2 || flagDefaultPos == 3 ||
      wi.x == CW_USEDEFAULT || wi.y == CW_USEDEFAULT ||
      wi.cx == CW_USEDEFAULT || wi.cy == CW_USEDEFAULT) {

    // default window position
    SystemParametersInfo(SPI_GETWORKAREA,0,&rc,0);
    wi.y = rc.top + 16;
    wi.cy = rc.bottom - rc.top - 32;
    wi.cx = min(rc.right - rc.left - 32,wi.cy);
    wi.x = (flagDefaultPos == 3) ? 16 : rc.right - wi.cx - 16;
  }

  else {

    // fit window into working area of current monitor
    wi.x += (mi.rcWork.left - mi.rcMonitor.left);
    wi.y += (mi.rcWork.top - mi.rcMonitor.top);
    if (wi.x < mi.rcWork.left)
      wi.x = mi.rcWork.left;
    if (wi.y < mi.rcWork.top)
      wi.y = mi.rcWork.top;
    if (wi.x + wi.cx > mi.rcWork.right) {
      wi.x -= (wi.x + wi.cx - mi.rcWork.right);
      if (wi.x < mi.rcWork.left)
        wi.x = mi.rcWork.left;
      if (wi.x + wi.cx > mi.rcWork.right)
        wi.cx = mi.rcWork.right - wi.x;
    }
    if (wi.y + wi.cy > mi.rcWork.bottom) {
      wi.y -= (wi.y + wi.cy - mi.rcWork.bottom);
      if (wi.y < mi.rcWork.top)
        wi.y = mi.rcWork.top;
      if (wi.y + wi.cy > mi.rcWork.bottom)
        wi.cy = mi.rcWork.bottom - wi.y;
    }
    SetRect(&rc,wi.x,wi.y,wi.x+wi.cx,wi.y+wi.cy);
    if (!IntersectRect(&rc2,&rc,&mi.rcWork)) {
      wi.y = mi.rcWork.top + 16;
      wi.cy = mi.rcWork.bottom - mi.rcWork.top - 32;
      wi.cx = min(mi.rcWork.right - mi.rcWork.left - 32,wi.cy);
      wi.x = mi.rcWork.right - wi.cx - 16;
    }
  }

  hwndMain = CreateWindowEx(
               0,
               wchWndClass,
               L"Notepad2",
               WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
               wi.x,
               wi.y,
               wi.cx,
               wi.cy,
               NULL,
               NULL,
               hInstance,
               NULL);

  if (wi.max)
    nCmdShow = SW_SHOWMAXIMIZED;

  if (bAlwaysOnTop)
    SetWindowPos(hwndMain,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

  if (bTransparentMode)
    SetWindowTransparentMode(hwndMain,TRUE);

  // Current file information -- moved in front of ShowWindow()
  FileLoad(TRUE,TRUE,FALSE,FALSE,L"");

  if (!flagStartAsTrayIcon) {
    ShowWindow(hwndMain,nCmdShow);
    UpdateWindow(hwndMain);
  }
  else {
    ShowWindow(hwndMain,SW_HIDE);    // trick ShowWindow()
    ShowNotifyIcon(hwndMain,TRUE);
  }

  // Source Encoding
  if (lpEncodingArg)
    iSrcEncoding = Encoding_MatchW(lpEncodingArg);

  // Pathname parameter
  if (lpFileArg /*&& !flagNewFromClipboard*/)
  {
    // Open from Directory
    if (PathIsDirectory(lpFileArg)) {
      WCHAR tchFile[MAX_PATH];
      if (OpenFileDlg(hwndMain,tchFile,COUNTOF(tchFile),lpFileArg))
        FileLoad(FALSE,FALSE,FALSE,FALSE,tchFile);
    }
    else {
      if (FileLoad(FALSE,FALSE,FALSE,FALSE,lpFileArg)) {
        if (flagJumpTo) // Jump to position
          EditJumpTo(hwndEdit,iInitialLine,iInitialColumn);
      }
    }
    GlobalFree(lpFileArg);
  }

  else {
    if (iSrcEncoding != -1) {
      iEncoding = iSrcEncoding;
      iOriginalEncoding = iSrcEncoding;
      SendMessage(hwndEdit,SCI_SETCODEPAGE,(iEncoding == CPI_DEFAULT) ? iDefaultCodePage : SC_CP_UTF8,0);
    }
  }

  // reset
  iSrcEncoding = -1;
  flagQuietCreate = 0;
  fKeepTitleExcerpt = 0;

  // Check for /c [if no file is specified] -- even if a file is specified
  /*else */if (flagNewFromClipboard) {
    if (SendMessage(hwndEdit,SCI_CANPASTE,0,0)) {
      BOOL bAutoIndent2 = bAutoIndent;
      bAutoIndent = 0;
      EditJumpTo(hwndEdit,-1,0);
      SendMessage(hwndEdit,SCI_BEGINUNDOACTION,0,0);
      if (SendMessage(hwndEdit,SCI_GETLENGTH,0,0) > 0)
        SendMessage(hwndEdit,SCI_NEWLINE,0,0);
      SendMessage(hwndEdit,SCI_PASTE,0,0);
      SendMessage(hwndEdit,SCI_NEWLINE,0,0);
      SendMessage(hwndEdit,SCI_ENDUNDOACTION,0,0);
      bAutoIndent = bAutoIndent2;
    }
  }

  // Encoding
  if (0 != flagSetEncoding) {
    SendMessage(
      hwndMain,
      WM_COMMAND,
      MAKELONG(IDM_ENCODING_ANSI + flagSetEncoding -1,1),
      0);
    flagSetEncoding = 0;
  }

  // EOL mode
  if (0 != flagSetEOLMode) {
    SendMessage(
      hwndMain,
      WM_COMMAND,
      MAKELONG(IDM_LINEENDINGS_CRLF + flagSetEOLMode -1,1),
      0);
    flagSetEOLMode = 0;
  }

  // Match Text
  if (flagMatchText && lpMatchArg) {
    if (lstrlen(lpMatchArg) && SendMessage(hwndEdit,SCI_GETLENGTH,0,0)) {

      UINT cp = SendMessage(hwndEdit,SCI_GETCODEPAGE,0,0);
      WideCharToMultiByte(cp,0,lpMatchArg,-1,efrData.szFind,COUNTOF(efrData.szFind),NULL,NULL);
      WideCharToMultiByte(CP_UTF8,0,lpMatchArg,-1,efrData.szFindUTF8,COUNTOF(efrData.szFindUTF8),NULL,NULL);
      cpLastFind = cp;

      if (flagMatchText & 4)
        efrData.fuFlags |= SCFIND_REGEXP;
      else if (flagMatchText & 8)
        efrData.bTransformBS = TRUE;

      if (flagMatchText & 2) {
        EditJumpTo(hwndEdit,-1,0);
        EditFindPrev(hwndEdit,&efrData,FALSE);
      }
      else {
        SendMessage(hwndEdit,SCI_DOCUMENTSTART,0,0);
        EditFindNext(hwndEdit,&efrData,FALSE);
      }
    }
    GlobalFree(lpMatchArg);
  }

  // Check for Paste Board option -- after loading files
  if (flagPasteBoard) {
    bLastCopyFromMe = TRUE;
    hwndNextCBChain = SetClipboardViewer(hwndMain);
    uidsAppTitle = IDS_APPTITLE_PASTEBOARD;
    SetWindowTitle(hwndMain,uidsAppTitle,fIsElevated,IDS_UNTITLED,szCurFile,
      iPathNameFormat,bModified || iEncoding != iOriginalEncoding,
      IDS_READONLY,bReadOnly,szTitleExcerpt);
    bLastCopyFromMe = FALSE;

    dwLastCopyTime = 0;
    SetTimer(hwndMain,ID_PASTEBOARDTIMER,100,PasteBoardTimer);
  }

  // check if a lexer was specified from the command line
  if (flagLexerSpecified) {
    if (lpSchemeArg) {
      Style_SetLexerFromName(hwndEdit,szCurFile,lpSchemeArg);
      LocalFree(lpSchemeArg);
    }
    else if (iInitialLexer >=0 && iInitialLexer < NUMLEXERS)
      Style_SetLexerFromID(hwndEdit,iInitialLexer);
    flagLexerSpecified = 0;
  }

  // If start as tray icon, set current filename as tooltip
  if (flagStartAsTrayIcon)
    SetNotifyIconTitle(hwndMain);

  UpdateToolbar();
  UpdateStatusbar();

  return(hwndMain);

}


//=============================================================================
//
//  MainWndProc()
//
//  Messages are distributed to the MsgXXX-handlers
//
//
LRESULT CALLBACK MainWndProc(HWND hwnd,UINT umsg,WPARAM wParam,LPARAM lParam)
{
  static BOOL bShutdownOK;

  switch(umsg)
  {

    // Quickly handle painting and sizing messages, found in ScintillaWin.cxx
    // Cool idea, don't know if this has any effect... ;-)
    case WM_MOVE:
    case WM_MOUSEACTIVATE:
    case WM_NCHITTEST:
    case WM_NCCALCSIZE:
    case WM_NCPAINT:
    case WM_PAINT:
    case WM_ERASEBKGND:
    case WM_NCMOUSEMOVE:
    case WM_NCLBUTTONDOWN:
    case WM_WINDOWPOSCHANGING:
    case WM_WINDOWPOSCHANGED:
      return DefWindowProc(hwnd,umsg,wParam,lParam);

    case WM_CREATE:
      return MsgCreate(hwnd,wParam,lParam);


    case WM_DESTROY:
    case WM_ENDSESSION:
      if (!bShutdownOK) {

        WINDOWPLACEMENT wndpl;

        // Terminate file watching
        InstallFileWatching(NULL);

        // GetWindowPlacement
        wndpl.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(hwnd,&wndpl);

        wi.x = wndpl.rcNormalPosition.left;
        wi.y = wndpl.rcNormalPosition.top;
        wi.cx = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
        wi.cy = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top;
        wi.max = (IsZoomed(hwnd) || (wndpl.flags & WPF_RESTORETOMAXIMIZED));

        DragAcceptFiles(hwnd,FALSE);

        // Terminate clipboard watching
        if (flagPasteBoard) {
          KillTimer(hwnd,ID_PASTEBOARDTIMER);
          ChangeClipboardChain(hwnd,hwndNextCBChain);
        }

        // Destroy find / replace dialog
        if (IsWindow(hDlgFindReplace))
          DestroyWindow(hDlgFindReplace);

        // call SaveSettings() when hwndToolbar is still valid
        SaveSettings(FALSE);

        if (lstrlen(szIniFile) != 0) {

          // Cleanup unwanted MRU's
          if (!bSaveRecentFiles) {
            MRU_Empty(pFileMRU);
            MRU_Save(pFileMRU);
          }
          else
            MRU_MergeSave(pFileMRU,TRUE,flagRelativeFileMRU,flagPortableMyDocs);
          MRU_Destroy(pFileMRU);

          if (!bSaveFindReplace) {
            MRU_Empty(mruFind);
            MRU_Empty(mruReplace);
            MRU_Save(mruFind);
            MRU_Save(mruReplace);
          }
          else {
            MRU_MergeSave(mruFind,FALSE,FALSE,FALSE);
            MRU_MergeSave(mruReplace,FALSE,FALSE,FALSE);
          }
          MRU_Destroy(mruFind);
          MRU_Destroy(mruReplace);
        }

        // Remove tray icon if necessary
        ShowNotifyIcon(hwnd,FALSE);

        bShutdownOK = TRUE;
      }
      if (umsg == WM_DESTROY)
        PostQuitMessage(0);
      break;


    case WM_CLOSE:
      if (FileSave(FALSE,TRUE,FALSE,FALSE))
        DestroyWindow(hwnd);
      break;


    case WM_QUERYENDSESSION:
      if (FileSave(FALSE,TRUE,FALSE,FALSE))
        return TRUE;
      else
        return FALSE;


    // Reinitialize theme-dependent values and resize windows
    case WM_THEMECHANGED:
      MsgThemeChanged(hwnd,wParam,lParam);
      break;


    // update Scintilla colors
    case WM_SYSCOLORCHANGE:
      {
        extern PEDITLEXER pLexCurrent;
        Style_SetLexer(hwndEdit,pLexCurrent);
        return DefWindowProc(hwnd,umsg,wParam,lParam);
      }


    //case WM_TIMER:
    //  break;


    case WM_SIZE:
      MsgSize(hwnd,wParam,lParam);
      break;


    case WM_SETFOCUS:
      SetFocus(hwndEdit);

      UpdateToolbar();
      UpdateStatusbar();

      //if (bPendingChangeNotify)
      //  PostMessage(hwnd,WM_CHANGENOTIFY,0,0);
      break;


    case WM_DROPFILES:
      {
        WCHAR szBuf[MAX_PATH+40];
        HDROP hDrop = (HDROP)wParam;

        // Reset Change Notify
        //bPendingChangeNotify = FALSE;

        if (IsIconic(hwnd))
          ShowWindow(hwnd,SW_RESTORE);

        //SetForegroundWindow(hwnd);

        DragQueryFile(hDrop,0,szBuf,COUNTOF(szBuf));

        if (PathIsDirectory(szBuf)) {
          WCHAR tchFile[MAX_PATH];
          if (OpenFileDlg(hwndMain,tchFile,COUNTOF(tchFile),szBuf))
            FileLoad(FALSE,FALSE,FALSE,FALSE,tchFile);
        }

        else
          FileLoad(FALSE,FALSE,FALSE,FALSE,szBuf);

        if (DragQueryFile(hDrop,(UINT)(-1),NULL,0) > 1)
          MsgBox(MBWARN,IDS_ERR_DROP);

        DragFinish(hDrop);
      }
      break;


    case WM_COPYDATA:
      {
        PCOPYDATASTRUCT pcds = (PCOPYDATASTRUCT)lParam;

        // Reset Change Notify
        //bPendingChangeNotify = FALSE;

        SetDlgItemInt(hwnd,IDC_REUSELOCK,GetTickCount(),FALSE);

        if (pcds->dwData == DATA_NOTEPAD2_PARAMS) {
          LPNP2PARAMS params = LocalAlloc(LPTR,pcds->cbData);
          CopyMemory(params,pcds->lpData,pcds->cbData);

          if (params->flagLexerSpecified)
            flagLexerSpecified = 1;

          if (params->flagQuietCreate)
            flagQuietCreate = 1;

          if (params->flagFileSpecified) {

            BOOL  bOpened = FALSE;
            iSrcEncoding = params->iSrcEncoding;

            if (PathIsDirectory(&params->wchData)) {
              WCHAR tchFile[MAX_PATH];
              if (OpenFileDlg(hwndMain,tchFile,COUNTOF(tchFile),&params->wchData))
                bOpened = FileLoad(FALSE,FALSE,FALSE,FALSE,tchFile);
            }

            else
              bOpened = FileLoad(FALSE,FALSE,FALSE,FALSE,&params->wchData);

            if (bOpened) {

              if (0 != params->flagSetEncoding) {
                flagSetEncoding = params->flagSetEncoding;
                SendMessage(
                  hwnd,
                  WM_COMMAND,
                  MAKELONG(IDM_ENCODING_ANSI + flagSetEncoding -1,1),
                  0);
                flagSetEncoding = 0;
              }

              if (0 != params->flagSetEOLMode) {
                flagSetEOLMode = params->flagSetEOLMode;
                SendMessage(
                  hwndMain,
                  WM_COMMAND,
                  MAKELONG(IDM_LINEENDINGS_CRLF + flagSetEOLMode -1,1),
                  0);
                flagSetEOLMode = 0;
              }

              if (params->flagLexerSpecified) {
                if (params->iInitialLexer < 0) {
                  WCHAR wchExt[32] = L".";
                  lstrcpyn(CharNext(wchExt),StrEnd(&params->wchData)+1,30);
                  Style_SetLexerFromName(hwndEdit,&params->wchData,wchExt);
                }
                else if (params->iInitialLexer >=0 && params->iInitialLexer < NUMLEXERS)
                  Style_SetLexerFromID(hwndEdit,params->iInitialLexer);
              }

              if (params->flagTitleExcerpt) {
                lstrcpyn(szTitleExcerpt,StrEnd(&params->wchData)+1,COUNTOF(szTitleExcerpt));
                SetWindowTitle(hwnd,uidsAppTitle,fIsElevated,IDS_UNTITLED,szCurFile,
                  iPathNameFormat,bModified || iEncoding != iOriginalEncoding,
                  IDS_READONLY,bReadOnly,szTitleExcerpt);
              }
            }
            // reset
            iSrcEncoding = -1;
          }

          if (params->flagJumpTo) {
            if (params->iInitialLine == 0)
              params->iInitialLine = 1;
            EditJumpTo(hwndEdit,params->iInitialLine,params->iInitialColumn);
          }

          flagLexerSpecified = 0;
          flagQuietCreate = 0;

          LocalFree(params);

          UpdateStatusbar();
        }
      }
      return TRUE;


    case WM_CONTEXTMENU:
    {
      HMENU hmenu;
      int imenu = 0;
      POINT pt;
      int nID = GetDlgCtrlID((HWND)wParam);

      if ((nID != IDC_EDIT) && (nID != IDC_STATUSBAR) &&
          (nID != IDC_REBAR) && (nID != IDC_TOOLBAR))
        return DefWindowProc(hwnd,umsg,wParam,lParam);

      hmenu = LoadMenu(g_hInstance,MAKEINTRESOURCE(IDR_POPUPMENU));
      //SetMenuDefaultItem(GetSubMenu(hmenu,1),0,FALSE);

      pt.x = (int)(short)LOWORD(lParam);
      pt.y = (int)(short)HIWORD(lParam);

      switch(nID)
      {
        case IDC_EDIT:
          {
            int iSelStart = SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);
            int iSelEnd   = SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0);

            if (iSelStart == iSelEnd && pt.x != -1 && pt.y != -1)
            {
              int iNewPos;
              POINT ptc = { pt.x, pt.y };
              ScreenToClient(hwndEdit,&ptc);
              iNewPos = SendMessage(hwndEdit,SCI_POSITIONFROMPOINT,(WPARAM)ptc.x,(LPARAM)ptc.y);
              SendMessage(hwndEdit,SCI_GOTOPOS,(WPARAM)iNewPos,0);
            }

            if (pt.x == -1 && pt.y == -1)
            {
              int iCurrentPos = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
              pt.x = SendMessage(hwndEdit,SCI_POINTXFROMPOSITION,0,(LPARAM)iCurrentPos);
              pt.y = SendMessage(hwndEdit,SCI_POINTYFROMPOSITION,0,(LPARAM)iCurrentPos);
              ClientToScreen(hwndEdit,&pt);
            }
            imenu = 0;
          }
          break;

        case IDC_TOOLBAR:
        case IDC_STATUSBAR:
        case IDC_REBAR:
          if (pt.x == -1 && pt.y == -1)
            GetCursorPos(&pt);
          imenu = 1;
          break;
      }

      TrackPopupMenuEx(GetSubMenu(hmenu,imenu),
        TPM_LEFTBUTTON | TPM_RIGHTBUTTON,pt.x+1,pt.y+1,hwnd,NULL);

      DestroyMenu(hmenu);
    }
    break;


    case WM_INITMENU:
      MsgInitMenu(hwnd,wParam,lParam);
      break;


    case WM_NOTIFY:
      return MsgNotify(hwnd,wParam,lParam);


    case WM_COMMAND:
      return MsgCommand(hwnd,wParam,lParam);


    case WM_SYSCOMMAND:
      switch (wParam)
      {
        case SC_MINIMIZE:
          ShowOwnedPopups(hwnd,FALSE);
          if (bMinimizeToTray) {
            MinimizeWndToTray(hwnd);
            ShowNotifyIcon(hwnd,TRUE);
            SetNotifyIconTitle(hwnd);
            return(0);
          }
          else
            return DefWindowProc(hwnd,umsg,wParam,lParam);

        case SC_RESTORE: {
          LRESULT lrv = DefWindowProc(hwnd,umsg,wParam,lParam);
          ShowOwnedPopups(hwnd,TRUE);
          return(lrv);
        }
      }
      return DefWindowProc(hwnd,umsg,wParam,lParam);


      case WM_CHANGENOTIFY:
          if (iFileWatchingMode == 1 || bModified || iEncoding != iOriginalEncoding)
            SetForegroundWindow(hwnd);

          if (PathFileExists(szCurFile)) {

            if ((iFileWatchingMode == 2 && !bModified && iEncoding == iOriginalEncoding) ||
                 MsgBox(MBYESNO,IDS_FILECHANGENOTIFY) == IDYES) {

              int iCurPos     = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
              int iAnchorPos  = SendMessage(hwndEdit,SCI_GETANCHOR,0,0);
              int iCurTopLine = SendMessage(hwndEdit,SCI_GETFIRSTVISIBLELINE,0,0);
              int iXOffset    = SendMessage(hwndEdit,SCI_GETXOFFSET,0,0);
              BOOL bIsTail    = (iCurPos == iAnchorPos) && (iCurPos == SendMessage(hwndEdit,SCI_GETLENGTH,0,0));

              iWeakSrcEncoding = iEncoding;
              if (FileLoad(TRUE,FALSE,TRUE,FALSE,szCurFile)) {

                if (bIsTail && iFileWatchingMode == 2)
                  EditJumpTo(hwndEdit,-1,0);

                else if (SendMessage(hwndEdit,SCI_GETLENGTH,0,0) >= 4) {
                  char tch[5] = "";
                  SendMessage(hwndEdit,SCI_GETTEXT,5,(LPARAM)tch);
                  if (lstrcmpiA(tch,".LOG") != 0) {
                    int iTopLineAfterLoad;
                    SendMessage(hwndEdit,SCI_SETSEL,(WPARAM)iAnchorPos,(LPARAM)iCurPos);
                    iTopLineAfterLoad = SendMessage(hwndEdit,SCI_GETFIRSTVISIBLELINE,0,0);
                    SendMessage(hwndEdit,SCI_LINESCROLL,0,(LPARAM)(iCurTopLine - iTopLineAfterLoad));
                    SendMessage(hwndEdit,SCI_SETXOFFSET,(WPARAM)iXOffset,0);
                  }
                }
              }
            }
          }
          else {

            if (MsgBox(MBYESNO,IDS_FILECHANGENOTIFY2) == IDYES)
              FileSave(TRUE,FALSE,FALSE,FALSE);
          }

          if (!bRunningWatch)
            InstallFileWatching(szCurFile);
        break;


    //// This message is posted before Notepad2 reactivates itself
    //case WM_CHANGENOTIFYCLEAR:
    //  bPendingChangeNotify = FALSE;
    //  break;


    case WM_DRAWCLIPBOARD:
      if (!bLastCopyFromMe)
        dwLastCopyTime = GetTickCount();
      else
        bLastCopyFromMe = FALSE;
      if (hwndNextCBChain)
        SendMessage(hwndNextCBChain,WM_DRAWCLIPBOARD,wParam,lParam);
      break;


    case WM_CHANGECBCHAIN:
      if ((HWND)wParam == hwndNextCBChain)
        hwndNextCBChain = (HWND)lParam;
      if (hwndNextCBChain)
        SendMessage(hwndNextCBChain,WM_CHANGECBCHAIN,lParam,wParam);
      break;


    case WM_TRAYMESSAGE:
      switch(lParam)
      {
        case WM_RBUTTONUP: {

          HMENU hMenu = LoadMenu(g_hInstance,MAKEINTRESOURCE(IDR_POPUPMENU));
          HMENU hMenuPopup = GetSubMenu(hMenu,2);

          POINT pt;
          int iCmd;

          SetForegroundWindow(hwnd);

          GetCursorPos(&pt);
          SetMenuDefaultItem(hMenuPopup,IDM_TRAY_RESTORE,FALSE);
          iCmd = TrackPopupMenu(hMenuPopup,
                   TPM_NONOTIFY|TPM_RETURNCMD|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,
                   pt.x,pt.y,0,hwnd,NULL);

          PostMessage(hwnd,WM_NULL,0,0);

          DestroyMenu(hMenu);

          if (iCmd == IDM_TRAY_RESTORE) {
            ShowNotifyIcon(hwnd,FALSE);
            RestoreWndFromTray(hwnd);
            ShowOwnedPopups(hwnd,TRUE);
          }

          else if (iCmd == IDM_TRAY_EXIT) {
              //ShowNotifyIcon(hwnd,FALSE);
              SendMessage(hwnd,WM_CLOSE,0,0);
            }
          }
          return TRUE;

        case WM_LBUTTONUP:
          ShowNotifyIcon(hwnd,FALSE);
          RestoreWndFromTray(hwnd);
          ShowOwnedPopups(hwnd,TRUE);
          return TRUE;
      }
      break;


    default:

      if (umsg == msgTaskbarCreated) {
        if (!IsWindowVisible(hwnd))
          ShowNotifyIcon(hwnd,TRUE);
          SetNotifyIconTitle(hwnd);
        return(0);
      }

      return DefWindowProc(hwnd,umsg,wParam,lParam);

  }

  return(0);

}


//=============================================================================
//
//  MsgCreate() - Handles WM_CREATE
//
//
LRESULT MsgCreate(HWND hwnd,WPARAM wParam,LPARAM lParam)
{

  HINSTANCE hInstance = ((LPCREATESTRUCT)lParam)->hInstance;

  // Setup edit control
  hwndEdit = EditCreate(hwnd);
  InitScintillaHandle(hwndEdit);

  // Tabs
  SendMessage(hwndEdit,SCI_SETUSETABS,!bTabsAsSpaces,0);
  SendMessage(hwndEdit,SCI_SETTABINDENTS,bTabIndents,0);
  SendMessage(hwndEdit,SCI_SETTABWIDTH,iTabWidth,0);
  SendMessage(hwndEdit,SCI_SETINDENT,iIndentWidth,0);

  // Indent Guides
  Style_SetIndentGuides(hwndEdit,bShowIndentGuides);

  // Word wrap
  if (!fWordWrap)
    SendMessage(hwndEdit,SCI_SETWRAPMODE,SC_WRAP_NONE,0);
  else
    SendMessage(hwndEdit,SCI_SETWRAPMODE,(iWordWrapMode == 0) ? SC_WRAP_WORD : SC_WRAP_CHAR,0);
  if (iWordWrapIndent == 5)
    SendMessage(hwndEdit,SCI_SETWRAPINDENTMODE,SC_WRAPINDENT_SAME,0);
  else if (iWordWrapIndent == 6)
    SendMessage(hwndEdit,SCI_SETWRAPINDENTMODE,SC_WRAPINDENT_INDENT,0);
  else {
    int i = 0;
    switch (iWordWrapIndent) {
      case 1: i = 1; break;
      case 2: i = 2; break;
      case 3: i = (iIndentWidth) ? 1 * iIndentWidth : 1 * iTabWidth; break;
      case 4: i = (iIndentWidth) ? 2 * iIndentWidth : 2 * iTabWidth; break;
    }
    SendMessage(hwndEdit,SCI_SETWRAPSTARTINDENT,i,0);
    SendMessage(hwndEdit,SCI_SETWRAPINDENTMODE,SC_WRAPINDENT_FIXED,0);
  }
  if (bShowWordWrapSymbols) {
    int wrapVisualFlags = 0;
    int wrapVisualFlagsLocation = 0;
    if (iWordWrapSymbols == 0)
      iWordWrapSymbols = 22;
    switch (iWordWrapSymbols%10) {
      case 1: wrapVisualFlags |= SC_WRAPVISUALFLAG_END; wrapVisualFlagsLocation |= SC_WRAPVISUALFLAGLOC_END_BY_TEXT; break;
      case 2: wrapVisualFlags |= SC_WRAPVISUALFLAG_END; break;
    }
    switch (((iWordWrapSymbols%100)-(iWordWrapSymbols%10))/10) {
      case 1: wrapVisualFlags |= SC_WRAPVISUALFLAG_START; wrapVisualFlagsLocation |= SC_WRAPVISUALFLAGLOC_START_BY_TEXT; break;
      case 2: wrapVisualFlags |= SC_WRAPVISUALFLAG_START; break;
    }
    SendMessage(hwndEdit,SCI_SETWRAPVISUALFLAGSLOCATION,wrapVisualFlagsLocation,0);
    SendMessage(hwndEdit,SCI_SETWRAPVISUALFLAGS,wrapVisualFlags,0);
  }
  else {
    SendMessage(hwndEdit,SCI_SETWRAPVISUALFLAGS,0,0);
  }

  // Long Lines
  if (bMarkLongLines)
    SendMessage(hwndEdit,SCI_SETEDGEMODE,(iLongLineMode == EDGE_LINE)?EDGE_LINE:EDGE_BACKGROUND,0);
  else
    SendMessage(hwndEdit,SCI_SETEDGEMODE,EDGE_NONE,0);
  SendMessage(hwndEdit,SCI_SETEDGECOLUMN,iLongLinesLimit,0);

  // Margins
  SendMessage(hwndEdit,SCI_SETMARGINWIDTHN,2,0);
  SendMessage(hwndEdit,SCI_SETMARGINWIDTHN,1,(bShowSelectionMargin)?16:0);
  UpdateLineNumberWidth();
  //SendMessage(hwndEdit,SCI_SETMARGINWIDTHN,0,
  //  (bShowLineNumbers)?SendMessage(hwndEdit,SCI_TEXTWIDTH,STYLE_LINENUMBER,(LPARAM)L"_999999_"):0);

  // Code folding
  SciCall_SetMarginType(MARGIN_FOLD_INDEX, SC_MARGIN_SYMBOL);
  SciCall_SetMarginMask(MARGIN_FOLD_INDEX, SC_MASK_FOLDERS);
  SciCall_SetMarginWidth(MARGIN_FOLD_INDEX, (bShowCodeFolding) ? 11 : 0);
  SciCall_SetMarginSensitive(MARGIN_FOLD_INDEX, TRUE);
  SciCall_MarkerDefine(SC_MARKNUM_FOLDEROPEN, SC_MARK_BOXMINUS);
  SciCall_MarkerDefine(SC_MARKNUM_FOLDER, SC_MARK_BOXPLUS);
  SciCall_MarkerDefine(SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE);
  SciCall_MarkerDefine(SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNER);
  SciCall_MarkerDefine(SC_MARKNUM_FOLDEREND, SC_MARK_BOXPLUSCONNECTED);
  SciCall_MarkerDefine(SC_MARKNUM_FOLDEROPENMID, SC_MARK_BOXMINUSCONNECTED);
  SciCall_MarkerDefine(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNER);
  SciCall_SetFoldFlags(16);

  // Nonprinting characters
  SendMessage(hwndEdit,SCI_SETVIEWWS,(bViewWhiteSpace)?SCWS_VISIBLEALWAYS:SCWS_INVISIBLE,0);
  SendMessage(hwndEdit,SCI_SETVIEWEOL,bViewEOLs,0);

  hwndEditFrame = CreateWindowEx(
                    WS_EX_CLIENTEDGE,
                    WC_LISTVIEW,
                    NULL,
                    WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
                    0,0,100,100,
                    hwnd,
                    (HMENU)IDC_EDITFRAME,
                    hInstance,
                    NULL);

  if (PrivateIsAppThemed()) {

    RECT rc, rc2;

    bIsAppThemed = TRUE;

    SetWindowLongPtr(hwndEdit,GWL_EXSTYLE,GetWindowLongPtr(hwndEdit,GWL_EXSTYLE) & ~WS_EX_CLIENTEDGE);
    SetWindowPos(hwndEdit,NULL,0,0,0,0,SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);

    if (IsVista()) {
      cxEditFrame = 0;
      cyEditFrame = 0;
    }

    else {
      GetClientRect(hwndEditFrame,&rc);
      GetWindowRect(hwndEditFrame,&rc2);

      cxEditFrame = ((rc2.right-rc2.left) - (rc.right-rc.left)) / 2;
      cyEditFrame = ((rc2.bottom-rc2.top) - (rc.bottom-rc.top)) / 2;
    }
  }
  else {
    bIsAppThemed = FALSE;

    cxEditFrame = 0;
    cyEditFrame = 0;
  }

  // Create Toolbar and Statusbar
  CreateBars(hwnd,hInstance);

  // Window Initialization

  CreateWindow(
    WC_STATIC,
    NULL,
    WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
    0,0,10,10,
    hwnd,
    (HMENU)IDC_FILENAME,
    hInstance,
    NULL);

  SetDlgItemText(hwnd,IDC_FILENAME,szCurFile);

  CreateWindow(
    WC_STATIC,
    NULL,
    WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
    10,10,10,10,
    hwnd,
    (HMENU)IDC_REUSELOCK,
    hInstance,
    NULL);

  SetDlgItemInt(hwnd,IDC_REUSELOCK,GetTickCount(),FALSE);

  // Menu
  //SetMenuDefaultItem(GetSubMenu(GetMenu(hwnd),0),0);

  // Drag & Drop
  DragAcceptFiles(hwnd,TRUE);

  // File MRU
  pFileMRU = MRU_Create(L"Recent Files",MRU_NOCASE,32);
  MRU_Load(pFileMRU);

  mruFind = MRU_Create(L"Recent Find",(/*IsWindowsNT()*/1) ? MRU_UTF8 : 0,16);
  MRU_Load(mruFind);

  mruReplace = MRU_Create(L"Recent Replace",(/*IsWindowsNT()*/1) ? MRU_UTF8 : 0,16);
  MRU_Load(mruReplace);

  if (hwndEdit == NULL || hwndEditFrame == NULL ||
      hwndStatus == NULL || hwndToolbar == NULL || hwndReBar == NULL)
    return(-1);

  return(0);

}


//=============================================================================
//
//  CreateBars() - Create Toolbar and Statusbar
//
//
void CreateBars(HWND hwnd,HINSTANCE hInstance)
{
  RECT rc;

  REBARINFO rbi;
  REBARBANDINFO rbBand;

  BITMAP bmp;
  HBITMAP hbmp;
  HIMAGELIST himl;
  WCHAR szTmp[MAX_PATH];
  BOOL bExternalBitmap = TRUE;

  DWORD dwToolbarStyle = WS_TOOLBAR;
  DWORD dwStatusbarStyle = WS_CHILD | WS_CLIPSIBLINGS;
  DWORD dwReBarStyle = WS_REBAR;

  BOOL bIsAppThemed = PrivateIsAppThemed();

  int i,n;
  WCHAR tchDesc[256];
  WCHAR tchIndex[256];

  WCHAR *pIniSection = NULL;
  int   cchIniSection = 0;

  if (bShowToolbar)
    dwReBarStyle |= WS_VISIBLE;

  hwndToolbar = CreateWindowEx(0,TOOLBARCLASSNAME,NULL,dwToolbarStyle,
                               0,0,0,0,hwnd,(HMENU)IDC_TOOLBAR,hInstance,NULL);

  SendMessage(hwndToolbar,TB_BUTTONSTRUCTSIZE,(WPARAM)sizeof(TBBUTTON),0);

  // Add normal Toolbar Bitmap
  hbmp = NULL;
  if (lstrlen(tchToolbarBitmap))
  {
    if (!SearchPath(NULL,tchToolbarBitmap,NULL,COUNTOF(szTmp),szTmp,NULL))
      lstrcpy(szTmp,tchToolbarBitmap);
    hbmp = LoadImage(NULL,szTmp,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE);
  }
  if (!hbmp) {
    hbmp = LoadImage(hInstance,MAKEINTRESOURCE(IDR_MAINWND),IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION);
    bExternalBitmap = FALSE;
  }
  GetObject(hbmp,sizeof(BITMAP),&bmp);
  himl = ImageList_Create(bmp.bmWidth/NUMTOOLBITMAPS,bmp.bmHeight,ILC_COLOR32|ILC_MASK,0,0);
  ImageList_AddMasked(himl,hbmp,CLR_DEFAULT);
  DeleteObject(hbmp);
  SendMessage(hwndToolbar,TB_SETIMAGELIST,0,(LPARAM)himl);

  // Optionally add hot Toolbar Bitmap
  hbmp = NULL;
  if (lstrlen(tchToolbarBitmapHot))
  {
    if (!SearchPath(NULL,tchToolbarBitmapHot,NULL,COUNTOF(szTmp),szTmp,NULL))
      lstrcpy(szTmp,tchToolbarBitmapHot);
    if (hbmp = LoadImage(NULL,szTmp,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE))
    {
      GetObject(hbmp,sizeof(BITMAP),&bmp);
      himl = ImageList_Create(bmp.bmWidth/NUMTOOLBITMAPS,bmp.bmHeight,ILC_COLOR32|ILC_MASK,0,0);
      ImageList_AddMasked(himl,hbmp,CLR_DEFAULT);
      DeleteObject(hbmp);
      SendMessage(hwndToolbar,TB_SETHOTIMAGELIST,0,(LPARAM)himl);
    }
  }

  // Optionally add disabled Toolbar Bitmap
  hbmp = NULL;
  if (lstrlen(tchToolbarBitmapDisabled))
  {
    if (!SearchPath(NULL,tchToolbarBitmapDisabled,NULL,COUNTOF(szTmp),szTmp,NULL))
      lstrcpy(szTmp,tchToolbarBitmapDisabled);
    if (hbmp = LoadImage(NULL,szTmp,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE))
    {
      GetObject(hbmp,sizeof(BITMAP),&bmp);
      himl = ImageList_Create(bmp.bmWidth/NUMTOOLBITMAPS,bmp.bmHeight,ILC_COLOR32|ILC_MASK,0,0);
      ImageList_AddMasked(himl,hbmp,CLR_DEFAULT);
      DeleteObject(hbmp);
      SendMessage(hwndToolbar,TB_SETDISABLEDIMAGELIST,0,(LPARAM)himl);
    }
  }

  // Load toolbar labels
  pIniSection = LocalAlloc(LPTR,sizeof(WCHAR)*32*1024);
  cchIniSection = LocalSize(pIniSection)/sizeof(WCHAR);
  LoadIniSection(L"Toolbar Labels",pIniSection,cchIniSection);
  n = 1;
  for (i = 0; i < COUNTOF(tbbMainWnd); i++) {

    if (tbbMainWnd[i].fsStyle == TBSTYLE_SEP)
      continue;

    wsprintf(tchIndex,L"%02i",n++);

    if (IniSectionGetString(pIniSection,tchIndex,L"",tchDesc,COUNTOF(tchDesc))) {
      tbbMainWnd[i].iString = SendMessage(hwndToolbar,TB_ADDSTRING,0,(LPARAM)tchDesc);
      tbbMainWnd[i].fsStyle |= BTNS_AUTOSIZE | BTNS_SHOWTEXT;
    }

    else
      tbbMainWnd[i].fsStyle &= ~(BTNS_AUTOSIZE | BTNS_SHOWTEXT);
  }
  LocalFree(pIniSection);

  SendMessage(hwndToolbar,TB_SETEXTENDEDSTYLE,0,
    SendMessage(hwndToolbar,TB_GETEXTENDEDSTYLE,0,0) | TBSTYLE_EX_MIXEDBUTTONS);

  SendMessage(hwndToolbar,TB_ADDBUTTONS,NUMINITIALTOOLS,(LPARAM)tbbMainWnd);
  if (Toolbar_SetButtons(hwndToolbar,IDT_FILE_NEW,tchToolbarButtons,tbbMainWnd,COUNTOF(tbbMainWnd)) == 0)
    SendMessage(hwndToolbar,TB_ADDBUTTONS,NUMINITIALTOOLS,(LPARAM)tbbMainWnd);
  SendMessage(hwndToolbar,TB_GETITEMRECT,0,(LPARAM)&rc);
  //SendMessage(hwndToolbar,TB_SETINDENT,2,0);

  if (bShowStatusbar)
    dwStatusbarStyle |= WS_VISIBLE;

  hwndStatus = CreateStatusWindow(dwStatusbarStyle,NULL,hwnd,IDC_STATUSBAR);

  // Create ReBar and add Toolbar
  hwndReBar = CreateWindowEx(WS_EX_TOOLWINDOW,REBARCLASSNAME,NULL,dwReBarStyle,
                             0,0,0,0,hwnd,(HMENU)IDC_REBAR,hInstance,NULL);

  rbi.cbSize = sizeof(REBARINFO);
  rbi.fMask  = 0;
  rbi.himl   = (HIMAGELIST)NULL;
  SendMessage(hwndReBar,RB_SETBARINFO,0,(LPARAM)&rbi);

  rbBand.cbSize  = sizeof(REBARBANDINFO);
  rbBand.fMask   = /*RBBIM_COLORS | RBBIM_TEXT | RBBIM_BACKGROUND | */
                   RBBIM_STYLE | RBBIM_CHILD | RBBIM_CHILDSIZE /*| RBBIM_SIZE*/;
  rbBand.fStyle  = /*RBBS_CHILDEDGE |*//* RBBS_BREAK |*/ RBBS_FIXEDSIZE /*| RBBS_GRIPPERALWAYS*/;
  if (bIsAppThemed)
    rbBand.fStyle |= RBBS_CHILDEDGE;
  rbBand.hbmBack = NULL;
  rbBand.lpText     = L"Toolbar";
  rbBand.hwndChild  = hwndToolbar;
  rbBand.cxMinChild = (rc.right - rc.left) * COUNTOF(tbbMainWnd);
  rbBand.cyMinChild = (rc.bottom - rc.top) + 2 * rc.top;
  rbBand.cx         = 0;
  SendMessage(hwndReBar,RB_INSERTBAND,(WPARAM)-1,(LPARAM)&rbBand);

  SetWindowPos(hwndReBar,NULL,0,0,0,0,SWP_NOZORDER);
  GetWindowRect(hwndReBar,&rc);
  cyReBar = rc.bottom - rc.top;

  cyReBarFrame = bIsAppThemed ? 0 : 2;
}


//=============================================================================
//
//  MsgThemeChanged() - Handle WM_THEMECHANGED
//
//
void MsgThemeChanged(HWND hwnd,WPARAM wParam,LPARAM lParam)
{
  RECT rc, rc2;
  HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hwnd,GWLP_HINSTANCE);

  // reinitialize edit frame

  if (PrivateIsAppThemed()) {
    bIsAppThemed = TRUE;

    SetWindowLongPtr(hwndEdit,GWL_EXSTYLE,GetWindowLongPtr(hwndEdit,GWL_EXSTYLE) & ~WS_EX_CLIENTEDGE);
    SetWindowPos(hwndEdit,NULL,0,0,0,0,SWP_NOZORDER|SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE);

    if (IsVista()) {
      cxEditFrame = 0;
      cyEditFrame = 0;
    }

    else {
      SetWindowPos(hwndEditFrame,NULL,0,0,0,0,SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
      GetClientRect(hwndEditFrame,&rc);
      GetWindowRect(hwndEditFrame,&rc2);

      cxEditFrame = ((rc2.right-rc2.left) - (rc.right-rc.left)) / 2;
      cyEditFrame = ((rc2.bottom-rc2.top) - (rc.bottom-rc.top)) / 2;
    }
  }

  else {
    bIsAppThemed = FALSE;

    SetWindowLongPtr(hwndEdit,GWL_EXSTYLE,WS_EX_CLIENTEDGE|GetWindowLongPtr(hwndEdit,GWL_EXSTYLE));
    SetWindowPos(hwndEdit,NULL,0,0,0,0,SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);

    cxEditFrame = 0;
    cyEditFrame = 0;
  }

  // recreate toolbar and statusbar
  Toolbar_GetButtons(hwndToolbar,IDT_FILE_NEW,tchToolbarButtons,COUNTOF(tchToolbarButtons));

  DestroyWindow(hwndToolbar);
  DestroyWindow(hwndReBar);
  DestroyWindow(hwndStatus);
  CreateBars(hwnd,hInstance);
  UpdateToolbar();

  GetClientRect(hwnd,&rc);
  SendMessage(hwnd,WM_SIZE,SIZE_RESTORED,MAKELONG(rc.right,rc.bottom));
  UpdateStatusbar();
}


//=============================================================================
//
//  MsgSize() - Handles WM_SIZE
//
//
void MsgSize(HWND hwnd,WPARAM wParam,LPARAM lParam)
{

  RECT rc;
  int x,y,cx,cy;
  HDWP hdwp;

  // Statusbar
  int aWidth[6];

  if (wParam == SIZE_MINIMIZED)
    return;

  x = 0;
  y = 0;

  cx = LOWORD(lParam);
  cy = HIWORD(lParam);

  if (bShowToolbar)
  {
/*  SendMessage(hwndToolbar,WM_SIZE,0,0);
    GetWindowRect(hwndToolbar,&rc);
    y = (rc.bottom - rc.top);
    cy -= (rc.bottom - rc.top);*/

    //SendMessage(hwndToolbar,TB_GETITEMRECT,0,(LPARAM)&rc);
    SetWindowPos(hwndReBar,NULL,0,0,LOWORD(lParam),cyReBar,SWP_NOZORDER);
    // the ReBar automatically sets the correct height
    // calling SetWindowPos() with the height of one toolbar button
    // causes the control not to temporarily use the whole client area
    // and prevents flickering

    //GetWindowRect(hwndReBar,&rc);
    y = cyReBar + cyReBarFrame;    // define
    cy -= cyReBar + cyReBarFrame;  // border
  }

  if (bShowStatusbar)
  {
    SendMessage(hwndStatus,WM_SIZE,0,0);
    GetWindowRect(hwndStatus,&rc);
    cy -= (rc.bottom - rc.top);
  }

  hdwp = BeginDeferWindowPos(2);

  DeferWindowPos(hdwp,hwndEditFrame,NULL,x,y,cx,cy,
                 SWP_NOZORDER | SWP_NOACTIVATE);

  DeferWindowPos(hdwp,hwndEdit,NULL,x+cxEditFrame,y+cyEditFrame,
                 cx-2*cxEditFrame,cy-2*cyEditFrame,
                 SWP_NOZORDER | SWP_NOACTIVATE);

  EndDeferWindowPos(hdwp);

  // Statusbar width
  aWidth[0] = max(120,min(cx/3,StatusCalcPaneWidth(hwndStatus,L"Ln 9'999'999 : 9'999'999   Col 9'999'999 : 999   Sel 9'999'999")));
  aWidth[1] = aWidth[0] + StatusCalcPaneWidth(hwndStatus,L"9'999'999 Bytes");
  aWidth[2] = aWidth[1] + StatusCalcPaneWidth(hwndStatus,L"Unicode BE BOM");
  aWidth[3] = aWidth[2] + StatusCalcPaneWidth(hwndStatus,L"CR+LF");
  aWidth[4] = aWidth[3] + StatusCalcPaneWidth(hwndStatus,L"OVR");
  aWidth[5] = -1;

  SendMessage(hwndStatus,SB_SETPARTS,COUNTOF(aWidth),(LPARAM)aWidth);

  //UpdateStatusbar();

}


//=============================================================================
//
//  MsgInitMenu() - Handles WM_INITMENU
//
//
void MsgInitMenu(HWND hwnd,WPARAM wParam,LPARAM lParam)
{

  int i,i2;
  HMENU hmenu = (HMENU)wParam;

  i = lstrlen(szCurFile);
  EnableCmd(hmenu,IDM_FILE_REVERT,i);
  EnableCmd(hmenu,IDM_FILE_LAUNCH,i);
  EnableCmd(hmenu,IDM_FILE_PROPERTIES,i);
  EnableCmd(hmenu,IDM_FILE_CREATELINK,i);
  EnableCmd(hmenu,IDM_FILE_ADDTOFAV,i);

  EnableCmd(hmenu,IDM_FILE_READONLY,i);
  CheckCmd(hmenu,IDM_FILE_READONLY,bReadOnly);

  //EnableCmd(hmenu,IDM_ENCODING_UNICODEREV,!bReadOnly);
  //EnableCmd(hmenu,IDM_ENCODING_UNICODE,!bReadOnly);
  //EnableCmd(hmenu,IDM_ENCODING_UTF8SIGN,!bReadOnly);
  //EnableCmd(hmenu,IDM_ENCODING_UTF8,!bReadOnly);
  //EnableCmd(hmenu,IDM_ENCODING_ANSI,!bReadOnly);
  //EnableCmd(hmenu,IDM_LINEENDINGS_CRLF,!bReadOnly);
  //EnableCmd(hmenu,IDM_LINEENDINGS_LF,!bReadOnly);
  //EnableCmd(hmenu,IDM_LINEENDINGS_CR,!bReadOnly);

  EnableCmd(hmenu,IDM_ENCODING_RECODE,i);

  if (mEncoding[iEncoding].uFlags & NCP_UNICODE_REVERSE)
    i = IDM_ENCODING_UNICODEREV;
  else if (mEncoding[iEncoding].uFlags & NCP_UNICODE)
    i = IDM_ENCODING_UNICODE;
  else if (mEncoding[iEncoding].uFlags & NCP_UTF8_SIGN)
    i = IDM_ENCODING_UTF8SIGN;
  else if (mEncoding[iEncoding].uFlags & NCP_UTF8)
    i = IDM_ENCODING_UTF8;
  else if (mEncoding[iEncoding].uFlags & NCP_DEFAULT)
    i = IDM_ENCODING_ANSI;
  else
    i = -1;
  CheckMenuRadioItem(hmenu,IDM_ENCODING_ANSI,IDM_ENCODING_UTF8SIGN,i,MF_BYCOMMAND);

  if (iEOLMode == SC_EOL_CRLF)
    i = IDM_LINEENDINGS_CRLF;
  else if (iEOLMode == SC_EOL_LF)
    i = IDM_LINEENDINGS_LF;
  else
    i = IDM_LINEENDINGS_CR;
  CheckMenuRadioItem(hmenu,IDM_LINEENDINGS_CRLF,IDM_LINEENDINGS_CR,i,MF_BYCOMMAND);

  EnableCmd(hmenu,IDM_FILE_RECENT,(MRU_Enum(pFileMRU,0,NULL,0) > 0));

  EnableCmd(hmenu,IDM_EDIT_UNDO,SendMessage(hwndEdit,SCI_CANUNDO,0,0) /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_REDO,SendMessage(hwndEdit,SCI_CANREDO,0,0) /*&& !bReadOnly*/);

  i  = SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0) - SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);
  i2 = SendMessage(hwndEdit,SCI_CANPASTE,0,0);

  EnableCmd(hmenu,IDM_EDIT_CUT,i /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_COPY,i /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_COPYALL,SendMessage(hwndEdit,SCI_GETLENGTH,0,0) /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_COPYADD,i /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_PASTE,i2 /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_SWAP,i || i2 /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_CLEAR,i /*&& !bReadOnly*/);

  OpenClipboard(hwnd);
  EnableCmd(hmenu,IDM_EDIT_CLEARCLIPBOARD,CountClipboardFormats());
  CloseClipboard();

  //EnableCmd(hmenu,IDM_EDIT_MOVELINEUP,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_MOVELINEDOWN,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_DUPLICATELINE,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_CUTLINE,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_COPYLINE,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_DELETELINE,!bReadOnly);

  //EnableCmd(hmenu,IDM_EDIT_INDENT,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_UNINDENT,!bReadOnly);

  //EnableCmd(hmenu,IDM_EDIT_PADWITHSPACES,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_STRIP1STCHAR,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_STRIPLASTCHAR,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_TRIMLINES,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_REMOVEBLANKLINES,!bReadOnly);

  EnableCmd(hmenu,IDM_EDIT_SORTLINES,
    SendMessage(hwndEdit,SCI_LINEFROMPOSITION,(WPARAM)SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0),0) -
    SendMessage(hwndEdit,SCI_LINEFROMPOSITION,(WPARAM)SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0),0) >= 1);

  EnableCmd(hmenu,IDM_EDIT_COLUMNWRAP,i /*&& IsWindowsNT()*/);
  EnableCmd(hmenu,IDM_EDIT_SPLITLINES,i /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_JOINLINES,i /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_JOINLINESEX,i /*&& !bReadOnly*/);

  EnableCmd(hmenu,IDM_EDIT_CONVERTUPPERCASE,i /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_CONVERTLOWERCASE,i /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_INVERTCASE,i /*&& !bReadOnly*/ /*&& IsWindowsNT()*/);
  EnableCmd(hmenu,IDM_EDIT_TITLECASE,i /*&& !bReadOnly*/ /*&& IsWindowsNT()*/);
  EnableCmd(hmenu,IDM_EDIT_SENTENCECASE,i /*&& !bReadOnly*/ /*&& IsWindowsNT()*/);

  EnableCmd(hmenu,IDM_EDIT_CONVERTTABS,i /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_CONVERTSPACES,i /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_CONVERTTABS2,i /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_CONVERTSPACES2,i /*&& !bReadOnly*/);

  EnableCmd(hmenu,IDM_EDIT_URLENCODE,i /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_URLDECODE,i /*&& !bReadOnly*/);

  EnableCmd(hmenu,IDM_EDIT_ESCAPECCHARS,i /*&& !bReadOnly*/);
  EnableCmd(hmenu,IDM_EDIT_UNESCAPECCHARS,i /*&& !bReadOnly*/);

  //EnableCmd(hmenu,IDM_EDIT_INCREASENUM,i /*&& !bReadOnly*/);
  //EnableCmd(hmenu,IDM_EDIT_DECREASENUM,i /*&& !bReadOnly*/);

  EnableCmd(hmenu,IDM_VIEW_SHOWEXCERPT,i);

  i = SendMessage(hwndEdit,SCI_GETLEXER,0,0);
  EnableCmd(hmenu,IDM_EDIT_LINECOMMENT,
    !(i == SCLEX_NULL || i == SCLEX_CSS || i == SCLEX_DIFF));
  EnableCmd(hmenu,IDM_EDIT_STREAMCOMMENT,
    !(i == SCLEX_NULL || i == SCLEX_VBSCRIPT || i == SCLEX_MAKEFILE || i == SCLEX_VB || i == SCLEX_ASM ||
      i == SCLEX_SQL || i == SCLEX_PERL || i == SCLEX_PYTHON || i == SCLEX_PROPERTIES ||i == SCLEX_CONF ||
      i == SCLEX_POWERSHELL || i == SCLEX_BATCH || i == SCLEX_DIFF || i == SCLEX_BASH || i == SCLEX_TCL ||
      i == SCLEX_AU3 || i == SCLEX_LATEX));

  EnableCmd(hmenu,IDM_EDIT_INSERT_ENCODING,*mEncoding[iEncoding].pszParseNames);

  //EnableCmd(hmenu,IDM_EDIT_INSERT_SHORTDATE,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_INSERT_LONGDATE,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_INSERT_FILENAME,!bReadOnly);
  //EnableCmd(hmenu,IDM_EDIT_INSERT_PATHNAME,!bReadOnly);

  i = SendMessage(hwndEdit,SCI_GETLENGTH,0,0);
  EnableCmd(hmenu,IDM_EDIT_FIND,i);
  EnableCmd(hmenu,IDM_EDIT_SAVEFIND,i);
  EnableCmd(hmenu,IDM_EDIT_FINDNEXT,i);
  EnableCmd(hmenu,IDM_EDIT_FINDPREV,i && lstrlenA(efrData.szFind));
  EnableCmd(hmenu,IDM_EDIT_SELTONEXT,i);
  EnableCmd(hmenu,IDM_EDIT_SELTOPREV,i && lstrlenA(efrData.szFind));
  EnableCmd(hmenu,IDM_EDIT_REPLACE,i /*&& !bReadOnly*/);

  CheckCmd(hmenu,IDM_VIEW_USE2NDDEFAULT,Style_GetUse2ndDefault(hwndEdit));

  CheckCmd(hmenu,IDM_VIEW_WORDWRAP,fWordWrap);
  CheckCmd(hmenu,IDM_VIEW_LONGLINEMARKER,bMarkLongLines);
  CheckCmd(hmenu,IDM_VIEW_TABSASSPACES,bTabsAsSpaces);
  CheckCmd(hmenu,IDM_VIEW_SHOWINDENTGUIDES,bShowIndentGuides);
  CheckCmd(hmenu,IDM_VIEW_AUTOINDENTTEXT,bAutoIndent);
  CheckCmd(hmenu,IDM_VIEW_LINENUMBERS,bShowLineNumbers);
  CheckCmd(hmenu,IDM_VIEW_MARGIN,bShowSelectionMargin);
  CheckCmd(hmenu,IDM_VIEW_FOLDING,bShowCodeFolding);
  EnableCmd(hmenu,IDM_VIEW_TOGGLEFOLDS,bShowCodeFolding);
  CheckCmd(hmenu,IDM_VIEW_SHOWWHITESPACE,bViewWhiteSpace);
  CheckCmd(hmenu,IDM_VIEW_SHOWEOLS,bViewEOLs);
  CheckCmd(hmenu,IDM_VIEW_WORDWRAPSYMBOLS,bShowWordWrapSymbols);
  CheckCmd(hmenu,IDM_VIEW_MATCHBRACES,bMatchBraces);
  CheckCmd(hmenu,IDM_VIEW_TOOLBAR,bShowToolbar);
  EnableCmd(hmenu,IDM_VIEW_CUSTOMIZETB,bShowToolbar);
  CheckCmd(hmenu,IDM_VIEW_STATUSBAR,bShowStatusbar);

  i = SendMessage(hwndEdit,SCI_GETLEXER,0,0);
  //EnableCmd(hmenu,IDM_VIEW_AUTOCLOSETAGS,(i == SCLEX_HTML || i == SCLEX_XML));
  CheckCmd(hmenu,IDM_VIEW_AUTOCLOSETAGS,bAutoCloseTags /*&& (i == SCLEX_HTML || i == SCLEX_XML)*/);
  CheckCmd(hmenu,IDM_VIEW_HILITECURRENTLINE,bHiliteCurrentLine);

  i = IniGetInt(L"Settings2",L"ReuseWindow",0);
  CheckCmd(hmenu,IDM_VIEW_REUSEWINDOW,i);
  i = IniGetInt(L"Settings2",L"SingleFileInstance",0);
  CheckCmd(hmenu,IDM_VIEW_SINGLEFILEINSTANCE,i);
  bStickyWinPos = IniGetInt(L"Settings2",L"StickyWindowPosition",0);
  CheckCmd(hmenu,IDM_VIEW_STICKYWINPOS,bStickyWinPos);
  CheckCmd(hmenu,IDM_VIEW_ALWAYSONTOP,bAlwaysOnTop);
  CheckCmd(hmenu,IDM_VIEW_MINTOTRAY,bMinimizeToTray);
  CheckCmd(hmenu,IDM_VIEW_TRANSPARENT,bTransparentMode && bTransparentModeAvailable);
  EnableCmd(hmenu,IDM_VIEW_TRANSPARENT,bTransparentModeAvailable);

  CheckCmd(hmenu,IDM_VIEW_NOSAVERECENT,bSaveRecentFiles);
  CheckCmd(hmenu,IDM_VIEW_NOSAVEFINDREPL,bSaveFindReplace);
  CheckCmd(hmenu,IDM_VIEW_SAVEBEFORERUNNINGTOOLS,bSaveBeforeRunningTools);

  CheckCmd(hmenu,IDM_VIEW_CHANGENOTIFY,iFileWatchingMode);

  if (lstrlen(szTitleExcerpt))
    i = IDM_VIEW_SHOWEXCERPT;
  else if (iPathNameFormat == 0)
    i = IDM_VIEW_SHOWFILENAMEONLY;
  else if (iPathNameFormat == 1)
    i = IDM_VIEW_SHOWFILENAMEFIRST;
  else
    i = IDM_VIEW_SHOWFULLPATH;
  CheckMenuRadioItem(hmenu,IDM_VIEW_SHOWFILENAMEONLY,IDM_VIEW_SHOWEXCERPT,i,MF_BYCOMMAND);

  if (iEscFunction == 1)
    i = IDM_VIEW_ESCMINIMIZE;
  else if (iEscFunction == 2)
    i = IDM_VIEW_ESCEXIT;
  else
    i = IDM_VIEW_NOESCFUNC;
  CheckMenuRadioItem(hmenu,IDM_VIEW_NOESCFUNC,IDM_VIEW_ESCEXIT,i,MF_BYCOMMAND);

  i = lstrlen(szIniFile);
  CheckCmd(hmenu,IDM_VIEW_SAVESETTINGS,bSaveSettings && i);

  EnableCmd(hmenu,IDM_VIEW_REUSEWINDOW,i);
  EnableCmd(hmenu,IDM_VIEW_STICKYWINPOS,i);
  EnableCmd(hmenu,IDM_VIEW_SINGLEFILEINSTANCE,i);
  EnableCmd(hmenu,IDM_VIEW_NOSAVERECENT,i);
  EnableCmd(hmenu,IDM_VIEW_NOSAVEFINDREPL,i);
  EnableCmd(hmenu,IDM_VIEW_SAVESETTINGS,i);

  i = (lstrlen(szIniFile) > 0 || lstrlen(szIniFile2) > 0);
  EnableCmd(hmenu,IDM_VIEW_SAVESETTINGSNOW,i);

}


//=============================================================================
//
//  MsgCommand() - Handles WM_COMMAND
//
//
LRESULT MsgCommand(HWND hwnd,WPARAM wParam,LPARAM lParam)
{

  switch(LOWORD(wParam))
  {

    case IDM_FILE_NEW:
      FileLoad(FALSE,TRUE,FALSE,FALSE,L"");
      break;


    case IDM_FILE_OPEN:
      FileLoad(FALSE,FALSE,FALSE,FALSE,L"");
      break;


    case IDM_FILE_REVERT:
      {
        if (lstrlen(szCurFile)) {

          WCHAR tchCurFile2[MAX_PATH];

          int iCurPos     = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
          int iAnchorPos  = SendMessage(hwndEdit,SCI_GETANCHOR,0,0);
          int iCurTopLine = SendMessage(hwndEdit,SCI_GETFIRSTVISIBLELINE,0,0);
          int iXOffset    = SendMessage(hwndEdit,SCI_GETXOFFSET,0,0);

          if ((bModified || iEncoding != iOriginalEncoding) && MsgBox(MBOKCANCEL,IDS_ASK_REVERT) != IDOK)
            return(0);

          lstrcpy(tchCurFile2,szCurFile);

          iWeakSrcEncoding = iEncoding;
          if (FileLoad(TRUE,FALSE,TRUE,FALSE,tchCurFile2))
          {
            if (SendMessage(hwndEdit,SCI_GETLENGTH,0,0) >= 4) {
              char tch[5] = "";
              SendMessage(hwndEdit,SCI_GETTEXT,5,(LPARAM)tch);
              if (lstrcmpiA(tch,".LOG") != 0) {
                int iTopLineAfterLoad;
                SendMessage(hwndEdit,SCI_SETSEL,(WPARAM)iAnchorPos,(LPARAM)iCurPos);
                iTopLineAfterLoad = SendMessage(hwndEdit,SCI_GETFIRSTVISIBLELINE,0,0);
                SendMessage(hwndEdit,SCI_LINESCROLL,0,(LPARAM)(iCurTopLine - iTopLineAfterLoad));
                SendMessage(hwndEdit,SCI_SETXOFFSET,(WPARAM)iXOffset,0);
              }
            }
          }
        }
      }
      break;


    case IDM_FILE_SAVE:
      FileSave(TRUE,FALSE,FALSE,FALSE);
      break;


    case IDM_FILE_SAVEAS:
      FileSave(TRUE,FALSE,TRUE,FALSE);
      break;


    case IDM_FILE_SAVECOPY:
      FileSave(TRUE,FALSE,TRUE,TRUE);
      break;


    case IDM_FILE_READONLY:
      //bReadOnly = (bReadOnly) ? FALSE : TRUE;
      //SendMessage(hwndEdit,SCI_SETREADONLY,bReadOnly,0);
      //UpdateToolbar();
      //UpdateStatusbar();
      if (lstrlen(szCurFile))
      {
        BOOL bSuccess = FALSE;
        DWORD dwFileAttributes = GetFileAttributes(szCurFile);
        if (dwFileAttributes != INVALID_FILE_ATTRIBUTES) {
          if (bReadOnly)
            dwFileAttributes = (dwFileAttributes & ~FILE_ATTRIBUTE_READONLY);
          else
            dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
          if (!SetFileAttributes(szCurFile,dwFileAttributes))
            MsgBox(MBWARN,IDS_READONLY_MODIFY,szCurFile);
        }
        else
          MsgBox(MBWARN,IDS_READONLY_MODIFY,szCurFile);

        dwFileAttributes = GetFileAttributes(szCurFile);
        if (dwFileAttributes != INVALID_FILE_ATTRIBUTES)
          bReadOnly = (dwFileAttributes & FILE_ATTRIBUTE_READONLY);

        SetWindowTitle(hwnd,uidsAppTitle,fIsElevated,IDS_UNTITLED,szCurFile,
          iPathNameFormat,bModified || iEncoding != iOriginalEncoding,
          IDS_READONLY,bReadOnly,szTitleExcerpt);
      }
      break;


    case IDM_FILE_BROWSE:
      {
        SHELLEXECUTEINFO sei;
        WCHAR tchParam[MAX_PATH+4];
        WCHAR tchExeFile[MAX_PATH+4];

        lstrcpy(tchParam,szCurFile);
        PathQuoteSpaces(tchParam);

        if (!IniGetString(L"Settings2",L"filebrowser.exe",L"",tchExeFile,COUNTOF(tchExeFile))) {
          if (!SearchPath(NULL,L"metapath.exe",NULL,COUNTOF(tchExeFile),tchExeFile,NULL)) {
            GetModuleFileName(NULL,tchExeFile,COUNTOF(tchExeFile));
            PathRemoveFileSpec(tchExeFile);
            PathAppend(tchExeFile,L"metapath.exe");
          }
        }

        else if (PathIsRelative(tchExeFile)) {
          WCHAR tch[MAX_PATH];
          if (!SearchPath(NULL,tchExeFile,NULL,COUNTOF(tch),tch,NULL)) {
            GetModuleFileName(NULL,tch,COUNTOF(tch));
            PathRemoveFileSpec(tch);
            PathAppend(tch,tchExeFile);
            lstrcpy(tchExeFile,tch);
          }
          else
            lstrcpy(tchExeFile,tch);
        }

        ZeroMemory(&sei,sizeof(SHELLEXECUTEINFO));

        sei.cbSize = sizeof(SHELLEXECUTEINFO);
        sei.fMask = SEE_MASK_FLAG_NO_UI | /*SEE_MASK_NOZONECHECKS*/0x00800000;
        sei.hwnd = hwnd;
        sei.lpVerb = NULL;
        sei.lpFile = tchExeFile;
        sei.lpParameters = tchParam;
        sei.lpDirectory = NULL;
        sei.nShow = SW_SHOWNORMAL;

        ShellExecuteEx(&sei);

        if ((int)sei.hInstApp < 32)
          MsgBox(MBWARN,IDS_ERR_BROWSE);
      }
      break;


    case IDM_FILE_NEWWINDOW:
    case IDM_FILE_NEWWINDOW2:
      {
        SHELLEXECUTEINFO sei;
        WCHAR szModuleName[MAX_PATH];
        WCHAR szFileName[MAX_PATH];
        WCHAR szParameters[2*MAX_PATH+64];

        MONITORINFO mi;
        HMONITOR hMonitor;
        WINDOWPLACEMENT wndpl;
        int x,y,cx,cy,imax;
        WCHAR tch[64];

        if (bSaveBeforeRunningTools && !FileSave(FALSE,TRUE,FALSE,FALSE))
          break;

        GetModuleFileName(NULL,szModuleName,COUNTOF(szModuleName));

        lstrcpy(szParameters,L"-f");
        if (lstrlen(szIniFile)) {
          lstrcat(szParameters,L" \"");
          lstrcat(szParameters,szIniFile);
          lstrcat(szParameters,L"\"");
        }
        else
          lstrcat(szParameters,L"0");

        lstrcat(szParameters,L" -n");

        wndpl.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(hwnd,&wndpl);

        hMonitor = MonitorFromRect(&wndpl.rcNormalPosition,MONITOR_DEFAULTTONEAREST);
        mi.cbSize = sizeof(mi);
        GetMonitorInfo(hMonitor,&mi);

        // offset new window position +10/+10
        x = wndpl.rcNormalPosition.left + 10;
        y = wndpl.rcNormalPosition.top  + 10;
        cx = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
        cy = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top;

        // check if window fits monitor
        if ((x + cx) > mi.rcWork.right || (y + cy) > mi.rcWork.bottom) {
          x = mi.rcMonitor.left;
          y = mi.rcMonitor.top;
        }

        imax = IsZoomed(hwnd);

        wsprintf(tch,L" -p %i,%i,%i,%i,%i",x,y,cx,cy,imax);
        lstrcat(szParameters,tch);

        if (LOWORD(wParam) != IDM_FILE_NEWWINDOW2 && lstrlen(szCurFile)) {
          lstrcpy(szFileName,szCurFile);
          PathQuoteSpaces(szFileName);
          lstrcat(szParameters,L" ");
          lstrcat(szParameters,szFileName);
        }

        ZeroMemory(&sei,sizeof(SHELLEXECUTEINFO));

        sei.cbSize = sizeof(SHELLEXECUTEINFO);
        sei.fMask = /*SEE_MASK_NOZONECHECKS*/0x00800000;
        sei.hwnd = hwnd;
        sei.lpVerb = NULL;
        sei.lpFile = szModuleName;
        sei.lpParameters = szParameters;
        sei.lpDirectory = NULL;
        sei.nShow = SW_SHOWNORMAL;

        ShellExecuteEx(&sei);
      }
      break;


    case IDM_FILE_LAUNCH:
      {
        SHELLEXECUTEINFO sei;
        WCHAR wchDirectory[MAX_PATH] = L"";

        if (!lstrlen(szCurFile))
          break;

        if (bSaveBeforeRunningTools && !FileSave(FALSE,TRUE,FALSE,FALSE))
          break;

        if (lstrlen(szCurFile)) {
          lstrcpy(wchDirectory,szCurFile);
          PathRemoveFileSpec(wchDirectory);
        }

        ZeroMemory(&sei,sizeof(SHELLEXECUTEINFO));

        sei.cbSize = sizeof(SHELLEXECUTEINFO);
        sei.fMask = 0;
        sei.hwnd = hwnd;
        sei.lpVerb = NULL;
        sei.lpFile = szCurFile;
        sei.lpParameters = NULL;
        sei.lpDirectory = NULL;
        sei.nShow = SW_SHOWNORMAL;

        ShellExecuteEx(&sei);
      }
      break;


    case IDM_FILE_RUN:
      {
        WCHAR tchCmdLine[MAX_PATH+4];

        if (bSaveBeforeRunningTools && !FileSave(FALSE,TRUE,FALSE,FALSE))
          break;

        lstrcpy(tchCmdLine,szCurFile);
        PathQuoteSpaces(tchCmdLine);

        RunDlg(hwnd,tchCmdLine);
      }
      break;


    case IDM_FILE_OPENWITH:
      if (bSaveBeforeRunningTools && !FileSave(FALSE,TRUE,FALSE,FALSE))
        break;
      OpenWithDlg(hwnd,szCurFile);
      break;


    case IDM_FILE_PAGESETUP:
      EditPrintSetup(hwndEdit);
      break;

    case IDM_FILE_PRINT:
      {
        SHFILEINFO shfi;
        WCHAR *pszTitle;
        WCHAR tchUntitled[32];
        WCHAR tchPageFmt[32];

        if (lstrlen(szCurFile)) {
          SHGetFileInfo2(szCurFile,0,&shfi,sizeof(SHFILEINFO),SHGFI_DISPLAYNAME);
          pszTitle = shfi.szDisplayName;
        }
        else {
          GetString(IDS_UNTITLED,tchUntitled,COUNTOF(tchUntitled));
          pszTitle = tchUntitled;
        }

        GetString(IDS_PRINT_PAGENUM,tchPageFmt,COUNTOF(tchPageFmt));

        if (!EditPrint(hwndEdit,pszTitle,tchPageFmt))
          MsgBox(MBWARN,IDS_PRINT_ERROR,pszTitle);
      }
      break;


    case IDM_FILE_PROPERTIES:
      {
        SHELLEXECUTEINFO sei;

        if (lstrlen(szCurFile) == 0)
          break;

        ZeroMemory(&sei,sizeof(SHELLEXECUTEINFO));

        sei.cbSize = sizeof(SHELLEXECUTEINFO);
        sei.fMask = SEE_MASK_INVOKEIDLIST;
        sei.hwnd = hwnd;
        sei.lpVerb = L"properties";
        sei.lpFile = szCurFile;
        sei.nShow = SW_SHOWNORMAL;

        ShellExecuteEx(&sei);
      }
      break;

    case IDM_FILE_CREATELINK:
      {
        if (!lstrlen(szCurFile))
          break;

        if (!PathCreateDeskLnk(szCurFile))
          MsgBox(MBWARN,IDS_ERR_CREATELINK);
      }
      break;


    case IDM_FILE_OPENFAV:
      if (FileSave(FALSE,TRUE,FALSE,FALSE)) {

        WCHAR tchSelItem[MAX_PATH];

        if (FavoritesDlg(hwnd,tchSelItem))
        {
          if (PathIsLnkToDirectory(tchSelItem,NULL,0))
            PathGetLnkPath(tchSelItem,tchSelItem,COUNTOF(tchSelItem));

          if (PathIsDirectory(tchSelItem))
          {
            WCHAR tchFile[MAX_PATH];

            if (OpenFileDlg(hwndMain,tchFile,COUNTOF(tchFile),tchSelItem))
              FileLoad(TRUE,FALSE,FALSE,FALSE,tchFile);
          }
          else
            FileLoad(TRUE,FALSE,FALSE,FALSE,tchSelItem);
          }
        }
      break;


    case IDM_FILE_ADDTOFAV:
      if (lstrlen(szCurFile)) {
        SHFILEINFO shfi;
        SHGetFileInfo2(szCurFile,0,&shfi,sizeof(SHFILEINFO),SHGFI_DISPLAYNAME);
        AddToFavDlg(hwnd,shfi.szDisplayName,szCurFile);
      }
      break;


    case IDM_FILE_MANAGEFAV:
      {
        SHELLEXECUTEINFO sei;
        ZeroMemory(&sei,sizeof(SHELLEXECUTEINFO));

        sei.cbSize = sizeof(SHELLEXECUTEINFO);
        sei.fMask = 0;
        sei.hwnd = hwnd;
        sei.lpVerb = NULL;
        sei.lpFile = tchFavoritesDir;
        sei.lpParameters = NULL;
        sei.lpDirectory = NULL;
        sei.nShow = SW_SHOWNORMAL;

        // Run favorites directory
        ShellExecuteEx(&sei);
      }
      break;


    case IDM_FILE_RECENT:
      if (MRU_Enum(pFileMRU,0,NULL,0) > 0) {
        if (FileSave(FALSE,TRUE,FALSE,FALSE)) {
          WCHAR tchFile[MAX_PATH];
          if (FileMRUDlg(hwnd,tchFile))
            FileLoad(TRUE,FALSE,FALSE,FALSE,tchFile);
          }
        }
      break;


    case IDM_FILE_EXIT:
      SendMessage(hwnd,WM_CLOSE,0,0);
      break;


    case IDM_ENCODING_ANSI:
    case IDM_ENCODING_UNICODE:
    case IDM_ENCODING_UNICODEREV:
    case IDM_ENCODING_UTF8:
    case IDM_ENCODING_UTF8SIGN:
    case IDM_ENCODING_SELECT:
      {
        int iNewEncoding = iEncoding;
        if (LOWORD(wParam) == IDM_ENCODING_SELECT && !SelectEncodingDlg(hwnd,&iNewEncoding))
          break;
        else {
          switch (LOWORD(wParam)) {
            case IDM_ENCODING_UNICODE:    iNewEncoding = CPI_UNICODEBOM; break;
            case IDM_ENCODING_UNICODEREV: iNewEncoding = CPI_UNICODEBEBOM; break;
            case IDM_ENCODING_UTF8:       iNewEncoding = CPI_UTF8; break;
            case IDM_ENCODING_UTF8SIGN:   iNewEncoding = CPI_UTF8SIGN; break;
            case IDM_ENCODING_ANSI:       iNewEncoding = CPI_DEFAULT; break;
          }
        }

        if (EditSetNewEncoding(hwndEdit,
          iEncoding,iNewEncoding,
          (flagSetEncoding),lstrlen(szCurFile) == 0)) {

          if (SendMessage(hwndEdit,SCI_GETLENGTH,0,0) == 0) {
            iEncoding = iNewEncoding;
            iOriginalEncoding = iNewEncoding;
          }
          else {
            if (iEncoding == CPI_DEFAULT || iNewEncoding == CPI_DEFAULT)
              iOriginalEncoding = -1;
            iEncoding = iNewEncoding;
          }

          UpdateToolbar();
          UpdateStatusbar();

          SetWindowTitle(hwnd,uidsAppTitle,fIsElevated,IDS_UNTITLED,szCurFile,
            iPathNameFormat,bModified || iEncoding != iOriginalEncoding,
            IDS_READONLY,bReadOnly,szTitleExcerpt);
        }
      }
      break;


    case IDM_ENCODING_RECODE:
      {
        if (lstrlen(szCurFile)) {

          WCHAR tchCurFile2[MAX_PATH];

          int iNewEncoding = -1;
          if (iEncoding != CPI_DEFAULT)
            iNewEncoding = iEncoding;
          if (iEncoding == CPI_UTF8SIGN)
            iNewEncoding = CPI_UTF8;

          if ((bModified || iEncoding != iOriginalEncoding) && MsgBox(MBOKCANCEL,IDS_ASK_RECODE) != IDOK)
            return(0);

          if (RecodeDlg(hwnd,&iNewEncoding)) {

            lstrcpy(tchCurFile2,szCurFile);
            iSrcEncoding = iNewEncoding;
            FileLoad(TRUE,FALSE,TRUE,FALSE,tchCurFile2);
          }
        }
      }
      break;


    case IDM_ENCODING_SETDEFAULT:
      SelectDefEncodingDlg(hwnd,&iDefaultEncoding);
      break;


    case IDM_LINEENDINGS_CRLF:
    case IDM_LINEENDINGS_LF:
    case IDM_LINEENDINGS_CR:
      {
        int iNewEOLMode = iLineEndings[LOWORD(wParam)-IDM_LINEENDINGS_CRLF];
        iEOLMode = iNewEOLMode;
        SendMessage(hwndEdit,SCI_SETEOLMODE,iEOLMode,0);
        SendMessage(hwndEdit,SCI_CONVERTEOLS,iEOLMode,0);
        UpdateToolbar();
        UpdateStatusbar();
        SetWindowTitle(hwnd,uidsAppTitle,fIsElevated,IDS_UNTITLED,szCurFile,
          iPathNameFormat,bModified || iEncoding != iOriginalEncoding,
          IDS_READONLY,bReadOnly,szTitleExcerpt);
      }
      break;


    case IDM_LINEENDINGS_SETDEFAULT:
      SelectDefLineEndingDlg(hwnd,&iDefaultEOLMode);
      break;


    case IDM_EDIT_UNDO:
      SendMessage(hwndEdit,SCI_UNDO,0,0);
      break;


    case IDM_EDIT_REDO:
      SendMessage(hwndEdit,SCI_REDO,0,0);
      break;


    case IDM_EDIT_CUT:
      if (flagPasteBoard)
        bLastCopyFromMe = TRUE;
      SendMessage(hwndEdit,SCI_CUT,0,0);
      break;


    case IDM_EDIT_COPY:
      if (flagPasteBoard)
        bLastCopyFromMe = TRUE;
      SendMessage(hwndEdit,SCI_COPY,0,0);
      UpdateToolbar();
      break;


    case IDM_EDIT_COPYALL:
      if (flagPasteBoard)
        bLastCopyFromMe = TRUE;
      SendMessage(hwndEdit,SCI_COPYRANGE,0,SendMessage(hwndEdit,SCI_GETLENGTH,0,0));
      UpdateToolbar();
      break;


    case IDM_EDIT_COPYADD:
      if (flagPasteBoard)
        bLastCopyFromMe = TRUE;
      EditCopyAppend(hwndEdit);
      UpdateToolbar();
      break;


    case IDM_EDIT_PASTE:
      SendMessage(hwndEdit,SCI_PASTE,0,0);
      break;


    case IDM_EDIT_SWAP:
      if (SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0) -
          SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0) == 0) {
        int iNewPos = -1;
        int iPos = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
        SendMessage(hwndEdit,SCI_PASTE,0,0);
        iNewPos = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
        SendMessage(hwndEdit,SCI_SETSEL,iPos,iNewPos);
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_CLEARCLIPBOARD,1),0);
      }
      else {
        int iPos = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
        int iAnchor = SendMessage(hwndEdit,SCI_GETANCHOR,0,0);
        char *pClip = EditGetClipboardText(hwndEdit);
        if (flagPasteBoard)
          bLastCopyFromMe = TRUE;
        SendMessage(hwndEdit,SCI_BEGINUNDOACTION,0,0);
        SendMessage(hwndEdit,SCI_CUT,0,0);
        SendMessage(hwndEdit,SCI_REPLACESEL,(WPARAM)0,(LPARAM)pClip);
        if (iPos > iAnchor)
          SendMessage(hwndEdit,SCI_SETSEL,iAnchor,iAnchor + lstrlenA(pClip));
        else
          SendMessage(hwndEdit,SCI_SETSEL,iPos + lstrlenA(pClip),iPos);
        SendMessage(hwndEdit,SCI_ENDUNDOACTION,0,0);
        LocalFree(pClip);
      }
      break;


    case IDM_EDIT_CLEAR:
      SendMessage(hwndEdit,SCI_CLEAR,0,0);
      break;


    case IDM_EDIT_CLEARCLIPBOARD:
      if (OpenClipboard(hwnd)) {
        if (CountClipboardFormats() > 0) {
          EmptyClipboard();
          UpdateToolbar();
          UpdateStatusbar();
        }
        CloseClipboard();
      }
      break;


    case IDM_EDIT_SELECTALL:
      SendMessage(hwndEdit,SCI_SELECTALL,0,0);
      //SendMessage(hwndEdit,SCI_SETSEL,0,(LPARAM)-1);
      break;


    case IDM_EDIT_SELECTWORD:
      {
        int iSel =
          SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0) -
          SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);

        int iPos = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);

        if (iSel == 0) {

          int iWordStart = SendMessage(hwndEdit,SCI_WORDSTARTPOSITION,iPos,TRUE);
          int iWordEnd   = SendMessage(hwndEdit,SCI_WORDENDPOSITION,iPos,TRUE);

          if (iWordStart == iWordEnd) // we are in whitespace salad...
          {
            iWordStart = SendMessage(hwndEdit,SCI_WORDENDPOSITION,iPos,FALSE);
            iWordEnd   = SendMessage(hwndEdit,SCI_WORDENDPOSITION,iWordStart,TRUE);
            if (iWordStart != iWordEnd) {
              //if (SCLEX_HTML == SendMessage(hwndEdit,SCI_GETLEXER,0,0) &&
              //    SCE_HPHP_VARIABLE == SendMessage(hwndEdit,SCI_GETSTYLEAT,(WPARAM)iWordStart,0) &&
              //    '$' == (char)SendMessage(hwndEdit,SCI_GETCHARAT,(WPARAM)iWordStart-1,0))
              //  iWordStart--;
              SendMessage(hwndEdit,SCI_SETSEL,iWordStart,iWordEnd);
            }
          }
          else {
            //if (SCLEX_HTML == SendMessage(hwndEdit,SCI_GETLEXER,0,0) &&
            //    SCE_HPHP_VARIABLE == SendMessage(hwndEdit,SCI_GETSTYLEAT,(WPARAM)iWordStart,0) &&
            //    '$' == (char)SendMessage(hwndEdit,SCI_GETCHARAT,(WPARAM)iWordStart-1,0))
            //  iWordStart--;
            SendMessage(hwndEdit,SCI_SETSEL,iWordStart,iWordEnd);
          }

          iSel =
            SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0) -
            SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);

          if (iSel == 0) {
            int iLine = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,iPos,0);
            int iLineStart = SendMessage(hwndEdit,SCI_GETLINEINDENTPOSITION,iLine,0);
            int iLineEnd   = SendMessage(hwndEdit,SCI_GETLINEENDPOSITION,iLine,0);
            SendMessage(hwndEdit,SCI_SETSEL,iLineStart,iLineEnd);
          }
        }
        else {
          int iLine = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,iPos,0);
          int iLineStart = SendMessage(hwndEdit,SCI_GETLINEINDENTPOSITION,iLine,0);
          int iLineEnd   = SendMessage(hwndEdit,SCI_GETLINEENDPOSITION,iLine,0);
          SendMessage(hwndEdit,SCI_SETSEL,iLineStart,iLineEnd);
        }
      }

      break;


    case IDM_EDIT_SELECTLINE:
      {
        int iSelStart  = SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);
        int iSelEnd    = SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0);
        int iLineStart = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,iSelStart,0);
        int iLineEnd   = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,iSelEnd,0);
        iSelStart = SendMessage(hwndEdit,SCI_POSITIONFROMLINE,iLineStart,0);
        iSelEnd   = SendMessage(hwndEdit,SCI_POSITIONFROMLINE,iLineEnd+1,0);
        SendMessage(hwndEdit,SCI_SETSEL,iSelStart,iSelEnd);
        SendMessage(hwndEdit,SCI_CHOOSECARETX,0,0);
      }
      break;


    case IDM_EDIT_MOVELINEUP:
      EditMoveUp(hwndEdit);
      break;


    case IDM_EDIT_MOVELINEDOWN:
      EditMoveDown(hwndEdit);
      break;


    case IDM_EDIT_DUPLICATELINE:
      SendMessage(hwndEdit,SCI_LINEDUPLICATE,0,0);
      break;


    case IDM_EDIT_CUTLINE:
      if (flagPasteBoard)
        bLastCopyFromMe = TRUE;
      SendMessage(hwndEdit,SCI_LINECUT,0,0);
      break;


    case IDM_EDIT_COPYLINE:
      if (flagPasteBoard)
        bLastCopyFromMe = TRUE;
      SendMessage(hwndEdit,SCI_LINECOPY,0,0);
      UpdateToolbar();
      break;


    case IDM_EDIT_DELETELINE:
      SendMessage(hwndEdit,SCI_LINEDELETE,0,0);
      break;


    case IDM_EDIT_DELETELINELEFT:
      SendMessage(hwndEdit,SCI_DELLINELEFT,0,0);
      break;


    case IDM_EDIT_DELETELINERIGHT:
      SendMessage(hwndEdit,SCI_DELLINERIGHT,0,0);
      break;


    case IDM_EDIT_INDENT:
      {
        int iLineSelStart = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,
          SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0),0);
        int iLineSelEnd   = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,
          SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0),0);

        SendMessage(hwndEdit,SCI_SETTABINDENTS,TRUE,0);
        if (iLineSelStart == iLineSelEnd) {
          SendMessage(hwndEdit,SCI_VCHOME,0,0);
          SendMessage(hwndEdit,SCI_TAB,0,0);
        }
        else
          SendMessage(hwndEdit,SCI_TAB,0,0);
        SendMessage(hwndEdit,SCI_SETTABINDENTS,bTabIndents,0);
      }
      break;


    case IDM_EDIT_UNINDENT:
      {
        int iLineSelStart = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,
          SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0),0);
        int iLineSelEnd   = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,
          SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0),0);

        SendMessage(hwndEdit,SCI_SETTABINDENTS,TRUE,0);
        if (iLineSelStart == iLineSelEnd) {
          SendMessage(hwndEdit,SCI_VCHOME,0,0);
          SendMessage(hwndEdit,SCI_BACKTAB,0,0);
        }
        else
          SendMessage(hwndEdit,SCI_BACKTAB,0,0);
        SendMessage(hwndEdit,SCI_SETTABINDENTS,bTabIndents,0);
      }
      break;


    case IDM_EDIT_ENCLOSESELECTION:
      if (EditEncloseSelectionDlg(hwnd,wchPrefixSelection,wchAppendSelection)) {
        SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
        EditEncloseSelection(hwndEdit,wchPrefixSelection,wchAppendSelection);
        SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      }
      break;


    case IDM_EDIT_SELECTIONDUPLICATE:
      SendMessage(hwndEdit,SCI_BEGINUNDOACTION,0,0);
      SendMessage(hwndEdit,SCI_SELECTIONDUPLICATE,0,0);
      SendMessage(hwndEdit,SCI_ENDUNDOACTION,0,0);
      break;


    case IDM_EDIT_PADWITHSPACES:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditPadWithSpaces(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_STRIP1STCHAR:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditStripFirstCharacter(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_STRIPLASTCHAR:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditStripLastCharacter(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_TRIMLINES:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditStripTrailingBlanks(hwndEdit,FALSE);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_COMPRESSWS:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditCompressSpaces(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_REMOVEBLANKLINES:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditRemoveBlankLines(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_MODIFYLINES:
      if (EditModifyLinesDlg(hwnd,wchPrefixLines,wchAppendLines)) {
        SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
        EditModifyLines(hwndEdit,wchPrefixLines,wchAppendLines);
        SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      }
      break;


    case IDM_EDIT_SORTLINES:
      if (EditSortDlg(hwnd,&iSortOptions)) {
        SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
        StatusSetText(hwndStatus,255,L"...");
        StatusSetSimple(hwndStatus,TRUE);
        InvalidateRect(hwndStatus,NULL,TRUE);
        UpdateWindow(hwndStatus);
        EditSortLines(hwndEdit,iSortOptions);
        StatusSetSimple(hwndStatus,FALSE);
        SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      }
      break;


    case IDM_EDIT_COLUMNWRAP:
      {
        if (iWrapCol == 0)
          iWrapCol = iLongLinesLimit;

        if (ColumnWrapDlg(hwnd,IDD_COLUMNWRAP,&iWrapCol))
        {
          iWrapCol = max(min(iWrapCol,512),1);
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
          EditWrapToColumn(hwndEdit,iWrapCol);
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
        }
      }
      break;


    case IDM_EDIT_SPLITLINES:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      SendMessage(hwndEdit,SCI_TARGETFROMSELECTION,0,0);
      SendMessage(hwndEdit,SCI_LINESSPLIT,0,0);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_JOINLINES:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      SendMessage(hwndEdit,SCI_TARGETFROMSELECTION,0,0);
      SendMessage(hwndEdit,SCI_LINESJOIN,0,0);
      EditJoinLinesEx(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_JOINLINESEX:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditJoinLinesEx(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_CONVERTUPPERCASE:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditMakeUppercase(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_CONVERTLOWERCASE:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditMakeLowercase(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_INVERTCASE:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditInvertCase(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_TITLECASE:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditTitleCase(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_SENTENCECASE:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditSentenceCase(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_CONVERTTABS:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditTabsToSpaces(hwndEdit,iTabWidth,FALSE);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_CONVERTSPACES:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditSpacesToTabs(hwndEdit,iTabWidth,FALSE);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_CONVERTTABS2:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditTabsToSpaces(hwndEdit,iTabWidth,TRUE);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_CONVERTSPACES2:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditSpacesToTabs(hwndEdit,iTabWidth,TRUE);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_INSERT_TAG:
      {
        WCHAR wszOpen[256] = L"";
        WCHAR wszClose[256] = L"";
        if (EditInsertTagDlg(hwnd,wszOpen,wszClose))
          EditEncloseSelection(hwndEdit,wszOpen,wszClose);
      }
      break;


    case IDM_EDIT_INSERT_ENCODING:
      {
        if (*mEncoding[iEncoding].pszParseNames) {
          char msz[32];
          char *p;
          int iSelStart;
          lstrcpynA(msz,mEncoding[iEncoding].pszParseNames,COUNTOF(msz));
          if (p = StrChrA(msz,','))
            *p = 0;
          iSelStart = SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);
          SendMessage(hwndEdit,SCI_REPLACESEL,0,(LPARAM)msz);
          SendMessage(hwndEdit,SCI_SETANCHOR,(WPARAM)iSelStart,0);
        }
      }
      break;


    case IDM_EDIT_INSERT_SHORTDATE:
    case IDM_EDIT_INSERT_LONGDATE:
      {
        WCHAR tchDate[128];
        WCHAR tchTime[128];
        WCHAR tchDateTime[256];
        WCHAR tchTemplate[256];
        SYSTEMTIME st;
        char  mszBuf[MAX_PATH*3];
        UINT  uCP;
        int   iSelStart;

        GetLocalTime(&st);

        if (IniGetString(L"Settings2",
              (LOWORD(wParam) == IDM_EDIT_INSERT_SHORTDATE) ? L"DateTimeShort" : L"DateTimeLong",
              L"",tchTemplate,COUNTOF(tchTemplate))) {
          struct tm sst;
          sst.tm_isdst       = -1;
          sst.tm_sec         = (int)st.wSecond;
          sst.tm_min         = (int)st.wMinute;
          sst.tm_hour        = (int)st.wHour;
          sst.tm_mday        = (int)st.wDay;
          sst.tm_mon         = (int)st.wMonth - 1;
          sst.tm_year        = (int)st.wYear - 1900;
          sst.tm_wday        = (int)st.wDayOfWeek;
          mktime(&sst);
          wcsftime(tchDateTime,COUNTOF(tchDateTime),tchTemplate,&sst);
        }
        else {
          GetDateFormat(LOCALE_USER_DEFAULT,(
            LOWORD(wParam) == IDM_EDIT_INSERT_SHORTDATE) ? DATE_SHORTDATE : DATE_LONGDATE,
            &st,NULL,tchDate,COUNTOF(tchDate));
          GetTimeFormat(LOCALE_USER_DEFAULT,TIME_NOSECONDS,&st,NULL,tchTime,COUNTOF(tchTime));

          wsprintf(tchDateTime,L"%s %s",tchTime,tchDate);
        }

        uCP = (SendMessage(hwndEdit,SCI_GETCODEPAGE,0,0) == SC_CP_UTF8) ? CP_UTF8 : CP_ACP;
        WideCharToMultiByte(uCP,0,tchDateTime,-1,mszBuf,COUNTOF(mszBuf),NULL,NULL);
        iSelStart = SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);
        SendMessage(hwndEdit,SCI_REPLACESEL,0,(LPARAM)mszBuf);
        SendMessage(hwndEdit,SCI_SETANCHOR,(WPARAM)iSelStart,0);
      }
      break;


    case IDM_EDIT_INSERT_FILENAME:
    case IDM_EDIT_INSERT_PATHNAME:
      {
        SHFILEINFO shfi;
        WCHAR *pszInsert;
        WCHAR tchUntitled[32];
        char  mszBuf[MAX_PATH*3];
        UINT  uCP;
        int   iSelStart;

        if (lstrlen(szCurFile)) {
          if (LOWORD(wParam) == IDM_EDIT_INSERT_FILENAME) {
            SHGetFileInfo2(szCurFile,0,&shfi,sizeof(SHFILEINFO),SHGFI_DISPLAYNAME);
            pszInsert = shfi.szDisplayName;
          }
          else
            pszInsert = szCurFile;
        }

        else {
          GetString(IDS_UNTITLED,tchUntitled,COUNTOF(tchUntitled));
          pszInsert = tchUntitled;
        }

        uCP = (SendMessage(hwndEdit,SCI_GETCODEPAGE,0,0) == SC_CP_UTF8) ? CP_UTF8 : CP_ACP;
        WideCharToMultiByte(uCP,0,pszInsert,-1,mszBuf,COUNTOF(mszBuf),NULL,NULL);
        iSelStart = SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);
        SendMessage(hwndEdit,SCI_REPLACESEL,0,(LPARAM)mszBuf);
        SendMessage(hwndEdit,SCI_SETANCHOR,(WPARAM)iSelStart,0);
      }
      break;


    case IDM_EDIT_LINECOMMENT:
      switch (SendMessage(hwndEdit,SCI_GETLEXER,0,0)) {
        case SCLEX_NULL:
        case SCLEX_CSS:
        case SCLEX_DIFF:
          break;
        case SCLEX_HTML:
        case SCLEX_XML:
        case SCLEX_CPP:
        case SCLEX_PASCAL:
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
          EditToggleLineComments(hwndEdit,L"//",FALSE);
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
          break;
        case SCLEX_VBSCRIPT:
        case SCLEX_VB:
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
          EditToggleLineComments(hwndEdit,L"'",FALSE);
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
          break;
        case SCLEX_MAKEFILE:
        case SCLEX_PERL:
        case SCLEX_PYTHON:
        case SCLEX_CONF:
        case SCLEX_BASH:
        case SCLEX_TCL:
        case SCLEX_POWERSHELL:
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
          EditToggleLineComments(hwndEdit,L"#",TRUE);
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
          break;
        case SCLEX_ASM:
        case SCLEX_PROPERTIES:
        case SCLEX_AU3:
        case SCLEX_NSIS: // # could also be used instead
        case SCLEX_INNOSETUP:
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
          EditToggleLineComments(hwndEdit,L";",TRUE);
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
          break;
        case SCLEX_SQL:
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
          EditToggleLineComments(hwndEdit,L"--",TRUE);
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
          break;
        case SCLEX_BATCH:
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
          EditToggleLineComments(hwndEdit,L"rem ",TRUE);
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
          break;
        case SCLEX_LATEX:
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
          EditToggleLineComments(hwndEdit,L"%",TRUE);
          SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
          break;
      }
      break;


    case IDM_EDIT_STREAMCOMMENT:
      switch (SendMessage(hwndEdit,SCI_GETLEXER,0,0)) {
        case SCLEX_NULL:
        case SCLEX_VBSCRIPT:
        case SCLEX_MAKEFILE:
        case SCLEX_VB:
        case SCLEX_ASM:
        case SCLEX_SQL:
        case SCLEX_PERL:
        case SCLEX_PYTHON:
        case SCLEX_PROPERTIES:
        case SCLEX_CONF:
        case SCLEX_POWERSHELL:
        case SCLEX_BATCH:
        case SCLEX_DIFF:
        case SCLEX_BASH:
        case SCLEX_TCL:
        case SCLEX_AU3:
        case SCLEX_LATEX:
          break;
        case SCLEX_HTML:
        case SCLEX_XML:
        case SCLEX_CSS:
        case SCLEX_CPP:
        case SCLEX_NSIS:
          EditEncloseSelection(hwndEdit,L"/*",L"*/");
          break;
        case SCLEX_PASCAL:
        case SCLEX_INNOSETUP:
          EditEncloseSelection(hwndEdit,L"{",L"}");
      }
      break;


    case IDM_EDIT_URLENCODE:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditURLEncode(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_URLDECODE:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditURLDecode(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_ESCAPECCHARS:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditEscapeCChars(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_UNESCAPECCHARS:
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORWAIT,0);
      EditUnescapeCChars(hwndEdit);
      SendMessage(hwndEdit,SCI_SETCURSOR,SC_CURSORNORMAL,0);
      break;


    case IDM_EDIT_FINDMATCHINGBRACE:
      {
        int iBrace2 = -1;
        int iPos = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
        char c = (char)SendMessage(hwndEdit,SCI_GETCHARAT,iPos,0);
        if (StrChrA("()[]{}",c))
          iBrace2 = SendMessage(hwndEdit,SCI_BRACEMATCH,iPos,0);
        // Try one before
        else {
          iPos = SendMessage(hwndEdit,SCI_POSITIONBEFORE,iPos,0);
          c = (char)SendMessage(hwndEdit,SCI_GETCHARAT,iPos,0);
          if (StrChrA("()[]{}",c))
            iBrace2 = SendMessage(hwndEdit,SCI_BRACEMATCH,iPos,0);
        }
        if (iBrace2 != -1)
          SendMessage(hwndEdit,SCI_GOTOPOS,(WPARAM)iBrace2,0);
      }
      break;


    case IDM_EDIT_SELTOMATCHINGBRACE:
      {
        int iBrace2 = -1;
        int iPos = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
        char c = (char)SendMessage(hwndEdit,SCI_GETCHARAT,iPos,0);
        if (StrChrA("()[]{}",c))
          iBrace2 = SendMessage(hwndEdit,SCI_BRACEMATCH,iPos,0);
        // Try one before
        else {
          iPos = SendMessage(hwndEdit,SCI_POSITIONBEFORE,iPos,0);
          c = (char)SendMessage(hwndEdit,SCI_GETCHARAT,iPos,0);
          if (StrChrA("()[]{}",c))
            iBrace2 = SendMessage(hwndEdit,SCI_BRACEMATCH,iPos,0);
        }
        if (iBrace2 != -1) {
          if (iBrace2 > iPos)
            SendMessage(hwndEdit,SCI_SETSEL,(WPARAM)iPos,(LPARAM)iBrace2+1);
          else
            SendMessage(hwndEdit,SCI_SETSEL,(WPARAM)iPos+1,(LPARAM)iBrace2);
        }
      }
      break;


    case IDM_EDIT_FIND:
      if (!IsWindow(hDlgFindReplace))
        hDlgFindReplace = EditFindReplaceDlg(hwndEdit,&efrData,FALSE);
      else {
        if (GetDlgItem(hDlgFindReplace,IDC_REPLACE)) {
          SendMessage(hDlgFindReplace,WM_COMMAND,MAKELONG(IDMSG_SWITCHTOFIND,1),0);
          DestroyWindow(hDlgFindReplace);
          hDlgFindReplace = EditFindReplaceDlg(hwndEdit,&efrData,FALSE);
        }
        else {
          SetForegroundWindow(hDlgFindReplace);
          PostMessage(hDlgFindReplace,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hDlgFindReplace,IDC_FINDTEXT)),1);
        }
      }
      break;


#ifdef BOOKMARK_EDITION
    // Main Bookmark Functions
    //case IDM_EDIT_BOOKMARKNEXT:
     case BME_EDIT_BOOKMARKNEXT:
    {
        int iPos = SendMessage( hwndEdit , SCI_GETCURRENTPOS , 0 , 0);
        int iLine = SendMessage( hwndEdit , SCI_LINEFROMPOSITION , iPos , 0 );

        int bitmask = 1;
        int iNextLine = SendMessage( hwndEdit , SCI_MARKERNEXT , iLine+1 , bitmask );
        if( iNextLine == -1 )
        {
            iNextLine = SendMessage( hwndEdit , SCI_MARKERNEXT , 0 , bitmask );
        }

        if( iNextLine != -1 )
        {
            SendMessage( hwndEdit , SCI_GOTOLINE , iNextLine , 0 );
            SendMessage( hwndEdit , SCI_SETYCARETPOLICY , CARET_SLOP|CARET_STRICT|CARET_EVEN , 10 );
            SendMessage( hwndEdit , SCI_SCROLLCARET , 0 , 0 );
        }
        break;
    }

    //case IMD_EDIT_BOOKMARKPREV:
    case BME_EDIT_BOOKMARKPREV:
    {
        int iPos = SendMessage( hwndEdit , SCI_GETCURRENTPOS , 0 , 0);
        int iLine = SendMessage( hwndEdit , SCI_LINEFROMPOSITION , iPos , 0 );

        int bitmask = 1;
        int iNextLine = SendMessage( hwndEdit , SCI_MARKERPREVIOUS , iLine-1 , bitmask );
        if( iNextLine == -1 )
        {
            int nLines = SendMessage( hwndEdit , SCI_GETLINECOUNT , 0 , 0 );
            iNextLine = SendMessage( hwndEdit , SCI_MARKERPREVIOUS , nLines , bitmask );
        }

        if( iNextLine != -1 )
        {
            SendMessage( hwndEdit , SCI_GOTOLINE , iNextLine , 0 );
            SendMessage( hwndEdit , SCI_SETYCARETPOLICY , CARET_SLOP|CARET_STRICT|CARET_EVEN , 10 );
            SendMessage( hwndEdit , SCI_SCROLLCARET , 0 , 0 );
        }

        break;
    }

    //case IDM_EDIT_BOOKMARKTOGGLE:
    case BME_EDIT_BOOKMARKTOGGLE:
    {
        int iPos = SendMessage( hwndEdit , SCI_GETCURRENTPOS , 0 , 0);
        int iLine = SendMessage( hwndEdit , SCI_LINEFROMPOSITION , iPos , 0 );

        int bitmask = SendMessage( hwndEdit , SCI_MARKERGET , iLine , 0 );
        if( bitmask & 1 )
        {
            // unset
            SendMessage( hwndEdit , SCI_MARKERDELETE , iLine , 0 );
        }
        else
        {
            // define (beh�ver bara g�ra detta en g�ng egentligen)
            //SendMessage( hwndEdit , SCI_MARKERSETBACK , 0 , 74 | (203 << 8) | (0 << 16) ); //beh�ver bara g�ra detta en g�ng egentligen
            //SendMessage( hwndEdit , SCI_MARKERDEFINE , 0 , SC_MARK_ARROWS );    //beh�ver bara g�ra detta en g�ng egentligen

            if( bShowSelectionMargin )
            {
                SendMessage( hwndEdit , SCI_MARKERDEFINEPIXMAP , 0 , (LPARAM)bookmark_pixmap );
            }
            else
            {
                SendMessage( hwndEdit , SCI_MARKERSETBACK , 0 , 216 | (255 << 8) | (216 << 16) );
                SendMessage( hwndEdit , SCI_MARKERDEFINE , 0 , SC_MARK_BACKGROUND );
            }


            //SendMessage( hwndEdit , SCI_MARKERSETBACK , 0 , 180 | (255 << 8) | (180 << 16) ); //beh�ver bara g�ra detta en g�ng egentligen
            //SendMessage( hwndEdit , SCI_MARKERDEFINE , 0 , SC_MARK_BACKGROUND );    //beh�ver bara g�ra detta en g�ng egentligen

            // set
            SendMessage( hwndEdit , SCI_MARKERADD , iLine , 0 );
            //SendMessage( hwndEdit , SCI_MARKERADD , iLine , 1 );
        }

        break;
    }

    //case IDM_EDIT_BOOKMARKCLEAR:
    case BME_EDIT_BOOKMARKCLEAR:
    {
        SendMessage( hwndEdit , SCI_MARKERDELETEALL , -1 , 0 );

        break;
    }
#endif


    case IDM_EDIT_FINDNEXT:
    case IDM_EDIT_FINDPREV:
    case IDM_EDIT_SELTONEXT:
    case IDM_EDIT_SELTOPREV:
    case IDM_EDIT_REPLACENEXT:

      if (SendMessage(hwndEdit,SCI_GETLENGTH,0,0) == 0)
        break;

      if (!lstrlenA(efrData.szFind)) {
        if (LOWORD(wParam) != IDM_EDIT_REPLACENEXT)
          SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_FIND,1),0);
        else
          SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_REPLACE,1),0);
      }

      else {

        UINT cp = SendMessage(hwndEdit,SCI_GETCODEPAGE,0,0);
        if (cpLastFind != cp) {
          if (cp != SC_CP_UTF8) {

            WCHAR wch[512];

            MultiByteToWideChar(CP_UTF8,0,efrData.szFindUTF8,-1,wch,COUNTOF(wch));
            WideCharToMultiByte(cp,0,wch,-1,efrData.szFind,COUNTOF(efrData.szFind),NULL,NULL);

            MultiByteToWideChar(CP_UTF8,0,efrData.szReplaceUTF8,-1,wch,COUNTOF(wch));
            WideCharToMultiByte(cp,0,wch,-1,efrData.szReplace,COUNTOF(efrData.szReplace),NULL,NULL);
          }
          else {

            lstrcpyA(efrData.szFind,efrData.szFindUTF8);
            lstrcpyA(efrData.szReplace,efrData.szReplaceUTF8);
          }
        }
        cpLastFind = cp;
        switch (LOWORD(wParam)) {

          case IDM_EDIT_FINDNEXT:
            EditFindNext(hwndEdit,&efrData,FALSE);
            break;

          case IDM_EDIT_FINDPREV:
            EditFindPrev(hwndEdit,&efrData,FALSE);
            break;

          case IDM_EDIT_SELTONEXT:
            EditFindNext(hwndEdit,&efrData,TRUE);
            break;

          case IDM_EDIT_SELTOPREV:
            EditFindPrev(hwndEdit,&efrData,TRUE);
            break;

          case IDM_EDIT_REPLACENEXT:
            if (bReplaceInitialized)
              EditReplace(hwndEdit,&efrData);
            else
              SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_REPLACE,1),0);
            break;
        }
      }
      break;


    case IDM_EDIT_REPLACE:
      if (!IsWindow(hDlgFindReplace))
        hDlgFindReplace = EditFindReplaceDlg(hwndEdit,&efrData,TRUE);
      else {
        if (!GetDlgItem(hDlgFindReplace,IDC_REPLACE)) {
          SendMessage(hDlgFindReplace,WM_COMMAND,MAKELONG(IDMSG_SWITCHTOREPLACE,1),0);
          DestroyWindow(hDlgFindReplace);
          hDlgFindReplace = EditFindReplaceDlg(hwndEdit,&efrData,TRUE);
        }
        else {
          SetForegroundWindow(hDlgFindReplace);
          PostMessage(hDlgFindReplace,WM_NEXTDLGCTL,(WPARAM)(GetDlgItem(hDlgFindReplace,IDC_FINDTEXT)),1);
        }
      }
      break;


    case IDM_EDIT_GOTOLINE:
      EditLinenumDlg(hwndEdit);
      break;


    case IDM_VIEW_SCHEME:
      Style_SelectLexerDlg(hwndEdit);
      UpdateStatusbar();
      UpdateLineNumberWidth();
      break;


    case IDM_VIEW_USE2NDDEFAULT:
      Style_ToggleUse2ndDefault(hwndEdit);
      UpdateStatusbar();
      UpdateLineNumberWidth();
      break;


    case IDM_VIEW_SCHEMECONFIG:
      Style_ConfigDlg(hwndEdit);
      UpdateStatusbar();
      UpdateLineNumberWidth();
      break;


    case IDM_VIEW_FONT:
      Style_SetDefaultFont(hwndEdit);
      UpdateStatusbar();
      UpdateLineNumberWidth();
      break;


    case IDM_VIEW_WORDWRAP:
      fWordWrap = (fWordWrap) ? FALSE : TRUE;
      if (!fWordWrap)
        SendMessage(hwndEdit,SCI_SETWRAPMODE,SC_WRAP_NONE,0);
      else
        SendMessage(hwndEdit,SCI_SETWRAPMODE,(iWordWrapMode == 0) ? SC_WRAP_WORD : SC_WRAP_CHAR,0);
      UpdateToolbar();
      break;


    case IDM_VIEW_WORDWRAPSETTINGS:
      if (WordWrapSettingsDlg(hwnd,IDD_WORDWRAP,&iWordWrapIndent))
      {
        if (fWordWrap)
          SendMessage(hwndEdit,SCI_SETWRAPMODE,(iWordWrapMode == 0) ? SC_WRAP_WORD : SC_WRAP_CHAR,0);
        if (iWordWrapIndent == 5)
          SendMessage(hwndEdit,SCI_SETWRAPINDENTMODE,SC_WRAPINDENT_SAME,0);
        else if (iWordWrapIndent == 6)
          SendMessage(hwndEdit,SCI_SETWRAPINDENTMODE,SC_WRAPINDENT_INDENT,0);
        else {
          int i = 0;
          switch (iWordWrapIndent) {
            case 1: i = 1; break;
            case 2: i = 2; break;
            case 3: i = (iIndentWidth) ? 1 * iIndentWidth : 1 * iTabWidth; break;
            case 4: i = (iIndentWidth) ? 2 * iIndentWidth : 2 * iTabWidth; break;
          }
          SendMessage(hwndEdit,SCI_SETWRAPSTARTINDENT,i,0);
          SendMessage(hwndEdit,SCI_SETWRAPINDENTMODE,SC_WRAPINDENT_FIXED,0);
        }
        if (bShowWordWrapSymbols) {
          int wrapVisualFlags = 0;
          int wrapVisualFlagsLocation = 0;
          if (iWordWrapSymbols == 0)
            iWordWrapSymbols = 22;
          switch (iWordWrapSymbols%10) {
            case 1: wrapVisualFlags |= SC_WRAPVISUALFLAG_END; wrapVisualFlagsLocation |= SC_WRAPVISUALFLAGLOC_END_BY_TEXT; break;
            case 2: wrapVisualFlags |= SC_WRAPVISUALFLAG_END; break;
          }
          switch (((iWordWrapSymbols%100)-(iWordWrapSymbols%10))/10) {
            case 1: wrapVisualFlags |= SC_WRAPVISUALFLAG_START; wrapVisualFlagsLocation |= SC_WRAPVISUALFLAGLOC_START_BY_TEXT; break;
            case 2: wrapVisualFlags |= SC_WRAPVISUALFLAG_START; break;
          }
          SendMessage(hwndEdit,SCI_SETWRAPVISUALFLAGSLOCATION,wrapVisualFlagsLocation,0);
          SendMessage(hwndEdit,SCI_SETWRAPVISUALFLAGS,wrapVisualFlags,0);
        }
        else {
          SendMessage(hwndEdit,SCI_SETWRAPVISUALFLAGS,0,0);
        }
      }
      break;


    case IDM_VIEW_LONGLINEMARKER:
      bMarkLongLines = (bMarkLongLines) ? FALSE: TRUE;
      if (bMarkLongLines) {
        SendMessage(hwndEdit,SCI_SETEDGEMODE,(iLongLineMode == EDGE_LINE)?EDGE_LINE:EDGE_BACKGROUND,0);
        Style_SetLongLineColors(hwndEdit);
      }
      else
        SendMessage(hwndEdit,SCI_SETEDGEMODE,EDGE_NONE,0);
      UpdateStatusbar();
      break;


    case IDM_VIEW_LONGLINESETTINGS:
      if (LongLineSettingsDlg(hwnd,IDD_LONGLINES,&iLongLinesLimit)) {
        bMarkLongLines = TRUE;
        SendMessage(hwndEdit,SCI_SETEDGEMODE,(iLongLineMode == EDGE_LINE)?EDGE_LINE:EDGE_BACKGROUND,0);
        Style_SetLongLineColors(hwndEdit);
        iLongLinesLimit = max(min(iLongLinesLimit,4096),0);
        SendMessage(hwndEdit,SCI_SETEDGECOLUMN,iLongLinesLimit,0);
        UpdateStatusbar();
        iLongLinesLimitG = iLongLinesLimit;
      }
      break;


    case IDM_VIEW_TABSASSPACES:
      bTabsAsSpaces = (bTabsAsSpaces) ? FALSE : TRUE;
      SendMessage(hwndEdit,SCI_SETUSETABS,!bTabsAsSpaces,0);
      bTabsAsSpacesG = bTabsAsSpaces;
      break;


    case IDM_VIEW_TABSETTINGS:
      if (TabSettingsDlg(hwnd,IDD_TABSETTINGS,NULL))
      {
        SendMessage(hwndEdit,SCI_SETUSETABS,!bTabsAsSpaces,0);
        SendMessage(hwndEdit,SCI_SETTABINDENTS,bTabIndents,0);
        iTabWidth = max(min(iTabWidth,256),1);
        iIndentWidth = max(min(iIndentWidth,256),0);
        SendMessage(hwndEdit,SCI_SETTABWIDTH,iTabWidth,0);
        SendMessage(hwndEdit,SCI_SETINDENT,iIndentWidth,0);
        bTabsAsSpacesG = bTabsAsSpaces;
        iTabWidthG     = iTabWidth;
        iIndentWidthG  = iIndentWidth;
        if (SendMessage(hwndEdit,SCI_GETWRAPINDENTMODE,0,0) == SC_WRAPINDENT_FIXED) {
          int i = 0;
          switch (iWordWrapIndent) {
            case 1: i = 1; break;
            case 2: i = 2; break;
            case 3: i = (iIndentWidth) ? 1 * iIndentWidth : 1 * iTabWidth; break;
            case 4: i = (iIndentWidth) ? 2 * iIndentWidth : 2 * iTabWidth; break;
          }
          SendMessage(hwndEdit,SCI_SETWRAPSTARTINDENT,i,0);
        }
      }
      break;


    case IDM_VIEW_SHOWINDENTGUIDES:
      bShowIndentGuides = (bShowIndentGuides) ? FALSE : TRUE;
      Style_SetIndentGuides(hwndEdit,bShowIndentGuides);
      break;


    case IDM_VIEW_AUTOINDENTTEXT:
      bAutoIndent = (bAutoIndent) ? FALSE : TRUE;
      break;


    case IDM_VIEW_LINENUMBERS:
      bShowLineNumbers = (bShowLineNumbers) ? FALSE : TRUE;
      UpdateLineNumberWidth();
      //SendMessage(hwndEdit,SCI_SETMARGINWIDTHN,0,
      //  (bShowLineNumbers)?SendMessage(hwndEdit,SCI_TEXTWIDTH,STYLE_LINENUMBER,(LPARAM)"_999999_"):0);
      break;


    case IDM_VIEW_MARGIN:
      bShowSelectionMargin = (bShowSelectionMargin) ? FALSE : TRUE;
      SendMessage(hwndEdit,SCI_SETMARGINWIDTHN,1,(bShowSelectionMargin)?16:0);

#ifdef BOOKMARK_EDITION
        //Depending on if the margin is visible or not, choose different bookmark indication
        if( bShowSelectionMargin )
        {
            SendMessage( hwndEdit , SCI_MARKERDEFINEPIXMAP , 0 , (LPARAM)bookmark_pixmap );
        }
        else
        {
            SendMessage( hwndEdit , SCI_MARKERSETBACK , 0 , 216 | (255 << 8) | (216 << 16) );
            SendMessage( hwndEdit , SCI_MARKERDEFINE , 0 , SC_MARK_BACKGROUND );
        }
#endif

      break;


    case IDM_VIEW_FOLDING:
      bShowCodeFolding = (bShowCodeFolding) ? FALSE : TRUE;
      SciCall_SetMarginWidth(MARGIN_FOLD_INDEX, (bShowCodeFolding) ? 11 : 0);
      UpdateToolbar();
      if (!bShowCodeFolding)
        FoldToggleAll(EXPAND);
      break;


    case IDM_VIEW_TOGGLEFOLDS:
      FoldToggleAll(SNIFF);
      break;


    case IDM_VIEW_SHOWWHITESPACE:
      bViewWhiteSpace = (bViewWhiteSpace) ? FALSE : TRUE;
      SendMessage(hwndEdit,SCI_SETVIEWWS,(bViewWhiteSpace)?SCWS_VISIBLEALWAYS:SCWS_INVISIBLE,0);
      break;


    case IDM_VIEW_SHOWEOLS:
      bViewEOLs = (bViewEOLs) ? FALSE : TRUE;
      SendMessage(hwndEdit,SCI_SETVIEWEOL,bViewEOLs,0);
      break;


    case IDM_VIEW_WORDWRAPSYMBOLS:
      bShowWordWrapSymbols = (bShowWordWrapSymbols) ? 0 : 1;
      if (bShowWordWrapSymbols) {
        int wrapVisualFlags = 0;
        int wrapVisualFlagsLocation = 0;
        if (iWordWrapSymbols == 0)
          iWordWrapSymbols = 22;
        switch (iWordWrapSymbols%10) {
          case 1: wrapVisualFlags |= SC_WRAPVISUALFLAG_END; wrapVisualFlagsLocation |= SC_WRAPVISUALFLAGLOC_END_BY_TEXT; break;
          case 2: wrapVisualFlags |= SC_WRAPVISUALFLAG_END; break;
        }
        switch (((iWordWrapSymbols%100)-(iWordWrapSymbols%10))/10) {
          case 1: wrapVisualFlags |= SC_WRAPVISUALFLAG_START; wrapVisualFlagsLocation |= SC_WRAPVISUALFLAGLOC_START_BY_TEXT; break;
          case 2: wrapVisualFlags |= SC_WRAPVISUALFLAG_START; break;
        }
        SendMessage(hwndEdit,SCI_SETWRAPVISUALFLAGSLOCATION,wrapVisualFlagsLocation,0);
        SendMessage(hwndEdit,SCI_SETWRAPVISUALFLAGS,wrapVisualFlags,0);
      }
      else {
        SendMessage(hwndEdit,SCI_SETWRAPVISUALFLAGS,0,0);
      }
      break;


    case IDM_VIEW_MATCHBRACES:
      bMatchBraces = (bMatchBraces) ? FALSE : TRUE;
      if (bMatchBraces) {
        NMHDR nmhdr;
        nmhdr.hwndFrom = hwndEdit;
        nmhdr.idFrom = IDC_EDIT;
        nmhdr.code = SCN_UPDATEUI;
        SendMessage(hwnd,WM_NOTIFY,IDC_EDIT,(LPARAM)&nmhdr);
      }
      else
        SendMessage(hwndEdit,SCI_BRACEHIGHLIGHT,(WPARAM)-1,(LPARAM)-1);
      break;


    case IDM_VIEW_AUTOCLOSETAGS:
      bAutoCloseTags = (bAutoCloseTags) ? FALSE : TRUE;
      break;


    case IDM_VIEW_HILITECURRENTLINE:
      bHiliteCurrentLine = (bHiliteCurrentLine) ? FALSE : TRUE;
      Style_SetCurrentLineBackground(hwndEdit);
      break;


    case IDM_VIEW_ZOOMIN:
      SendMessage(hwndEdit,SCI_ZOOMIN,0,0);
      //UpdateLineNumberWidth();
      break;


    case IDM_VIEW_ZOOMOUT:
      SendMessage(hwndEdit,SCI_ZOOMOUT,0,0);
      //UpdateLineNumberWidth();
      break;


    case IDM_VIEW_RESETZOOM:
      SendMessage(hwndEdit,SCI_SETZOOM,0,0);
      //UpdateLineNumberWidth();
      break;


    case IDM_VIEW_TOOLBAR:
      if (bShowToolbar) {
        bShowToolbar = 0;
        ShowWindow(hwndReBar,SW_HIDE);
      }
      else {
        bShowToolbar = 1;
        UpdateToolbar();
        ShowWindow(hwndReBar,SW_SHOW);
      }
      SendWMSize(hwnd);
      break;


    case IDM_VIEW_CUSTOMIZETB:
      SendMessage(hwndToolbar,TB_CUSTOMIZE,0,0);
      break;


    case IDM_VIEW_STATUSBAR:
      if (bShowStatusbar) {
        bShowStatusbar = 0;
        ShowWindow(hwndStatus,SW_HIDE);
      }
      else {
        bShowStatusbar = 1;
        UpdateStatusbar();
        ShowWindow(hwndStatus,SW_SHOW);
      }
      SendWMSize(hwnd);
      break;


    case IDM_VIEW_STICKYWINPOS:
      bStickyWinPos = IniGetInt(L"Settings2",L"StickyWindowPosition",bStickyWinPos);
      if (!bStickyWinPos) {
        WINDOWPLACEMENT wndpl;
        WCHAR tchPosX[32], tchPosY[32], tchSizeX[32], tchSizeY[32], tchMaximized[32];

        int ResX = GetSystemMetrics(SM_CXSCREEN);
        int ResY = GetSystemMetrics(SM_CYSCREEN);

        // GetWindowPlacement
        wndpl.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(hwndMain,&wndpl);

        wi.x = wndpl.rcNormalPosition.left;
        wi.y = wndpl.rcNormalPosition.top;
        wi.cx = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
        wi.cy = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top;
        wi.max = (IsZoomed(hwndMain) || (wndpl.flags & WPF_RESTORETOMAXIMIZED));

        wsprintf(tchPosX,L"%ix%i PosX",ResX,ResY);
        wsprintf(tchPosY,L"%ix%i PosY",ResX,ResY);
        wsprintf(tchSizeX,L"%ix%i SizeX",ResX,ResY);
        wsprintf(tchSizeY,L"%ix%i SizeY",ResX,ResY);
        wsprintf(tchMaximized,L"%ix%i Maximized",ResX,ResY);

        bStickyWinPos = 1;
        IniSetInt(L"Settings2",L"StickyWindowPosition",1);

        IniSetInt(L"Window",tchPosX,wi.x);
        IniSetInt(L"Window",tchPosY,wi.y);
        IniSetInt(L"Window",tchSizeX,wi.cx);
        IniSetInt(L"Window",tchSizeY,wi.cy);
        IniSetInt(L"Window",tchMaximized,wi.max);

        InfoBox(0,L"MsgStickyWinPos",IDS_STICKYWINPOS);
      }
      else {
        bStickyWinPos = 0;
        IniSetInt(L"Settings2",L"StickyWindowPosition",0);
      }
      break;


    case IDM_VIEW_REUSEWINDOW:
      if (IniGetInt(L"Settings2",L"ReuseWindow",0))
        IniSetInt(L"Settings2",L"ReuseWindow",0);
      else
        IniSetInt(L"Settings2",L"ReuseWindow",1);
      break;


    case IDM_VIEW_SINGLEFILEINSTANCE:
      if (IniGetInt(L"Settings2",L"SingleFileInstance",0))
        IniSetInt(L"Settings2",L"SingleFileInstance",0);
      else
        IniSetInt(L"Settings2",L"SingleFileInstance",1);
      break;


    case IDM_VIEW_ALWAYSONTOP:
      if (bAlwaysOnTop) {
        bAlwaysOnTop = 0;
        SetWindowPos(hwnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
      }
      else {
        bAlwaysOnTop = 1;
        SetWindowPos(hwnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
      }
      break;


    case IDM_VIEW_MINTOTRAY:
      bMinimizeToTray =(bMinimizeToTray) ? FALSE : TRUE;
      break;


    case IDM_VIEW_TRANSPARENT:
      bTransparentMode =(bTransparentMode) ? FALSE : TRUE;
      SetWindowTransparentMode(hwnd,bTransparentMode);
      break;


    case IDM_VIEW_SHOWFILENAMEONLY:
      iPathNameFormat = 0;
      lstrcpy(szTitleExcerpt,L"");
      SetWindowTitle(hwnd,uidsAppTitle,fIsElevated,IDS_UNTITLED,szCurFile,
        iPathNameFormat,bModified || iEncoding != iOriginalEncoding,
        IDS_READONLY,bReadOnly,szTitleExcerpt);
      break;


    case IDM_VIEW_SHOWFILENAMEFIRST:
      iPathNameFormat = 1;
      lstrcpy(szTitleExcerpt,L"");
      SetWindowTitle(hwnd,uidsAppTitle,fIsElevated,IDS_UNTITLED,szCurFile,
        iPathNameFormat,bModified || iEncoding != iOriginalEncoding,
        IDS_READONLY,bReadOnly,szTitleExcerpt);
      break;


    case IDM_VIEW_SHOWFULLPATH:
      iPathNameFormat = 2;
      lstrcpy(szTitleExcerpt,L"");
      SetWindowTitle(hwnd,uidsAppTitle,fIsElevated,IDS_UNTITLED,szCurFile,
        iPathNameFormat,bModified || iEncoding != iOriginalEncoding,
        IDS_READONLY,bReadOnly,szTitleExcerpt);
      break;


    case IDM_VIEW_SHOWEXCERPT:
      EditGetExcerpt(hwndEdit,szTitleExcerpt,COUNTOF(szTitleExcerpt));
      SetWindowTitle(hwnd,uidsAppTitle,fIsElevated,IDS_UNTITLED,szCurFile,
        iPathNameFormat,bModified || iEncoding != iOriginalEncoding,
        IDS_READONLY,bReadOnly,szTitleExcerpt);
      break;


    case IDM_VIEW_NOSAVERECENT:
      bSaveRecentFiles = (bSaveRecentFiles) ? FALSE : TRUE;
      break;


    case IDM_VIEW_NOSAVEFINDREPL:
      bSaveFindReplace = (bSaveFindReplace) ? FALSE : TRUE;
      break;


    case IDM_VIEW_SAVEBEFORERUNNINGTOOLS:
      bSaveBeforeRunningTools = (bSaveBeforeRunningTools) ? FALSE : TRUE;
      break;


    case IDM_VIEW_CHANGENOTIFY:
      if (ChangeNotifyDlg(hwnd))
        InstallFileWatching(szCurFile);
      break;


    case IDM_VIEW_NOESCFUNC:
      iEscFunction = 0;
      break;


    case IDM_VIEW_ESCMINIMIZE:
      iEscFunction = 1;
      break;


    case IDM_VIEW_ESCEXIT:
      iEscFunction = 2;
      break;


    case IDM_VIEW_SAVESETTINGS:
      bSaveSettings = (bSaveSettings) ? FALSE : TRUE;
      break;


    case IDM_VIEW_SAVESETTINGSNOW:
      {
        BOOL bCreateFailure = FALSE;

        if (lstrlen(szIniFile) == 0) {

          if (lstrlen(szIniFile2) > 0) {
            if (CreateIniFileEx(szIniFile2)) {
              lstrcpy(szIniFile,szIniFile2);
              lstrcpy(szIniFile2,L"");
            }
            else
              bCreateFailure = TRUE;
          }

          else
            break;
        }

        if (!bCreateFailure) {

          if (WritePrivateProfileString(L"Settings",L"WriteTest",L"ok",szIniFile)) {

            BeginWaitCursor();
            StatusSetTextID(hwndStatus,STATUS_HELP,IDS_SAVINGSETTINGS);
            StatusSetSimple(hwndStatus,TRUE);
            InvalidateRect(hwndStatus,NULL,TRUE);
            UpdateWindow(hwndStatus);
            SaveSettings(TRUE);
            StatusSetSimple(hwndStatus,FALSE);
            EndWaitCursor();
            MsgBox(MBINFO,IDS_SAVEDSETTINGS);
          }
          else {
            dwLastIOError = GetLastError();
            MsgBox(MBWARN,IDS_WRITEINI_FAIL);
          }
        }
        else
          MsgBox(MBWARN,IDS_CREATEINI_FAIL);
      }
      break;


    case IDM_HELP_ABOUT:
      ThemedDialogBox(g_hInstance,MAKEINTRESOURCE(IDD_ABOUT),
        hwnd,AboutDlgProc);
      break;


    case CMD_ESCAPE:
      if (iEscFunction == 1)
        SendMessage(hwnd,WM_SYSCOMMAND,SC_MINIMIZE,0);
      else if (iEscFunction == 2)
        SendMessage(hwnd,WM_CLOSE,0,0);
      break;


    case CMD_SHIFTESC:
      if (FileSave(TRUE,FALSE,FALSE,FALSE))
        SendMessage(hwnd,WM_CLOSE,0,0);
      break;


    // Newline with toggled auto indent setting
    case CMD_CTRLENTER:
      bAutoIndent = (bAutoIndent) ? 0 : 1;
      SendMessage(hwndEdit,SCI_NEWLINE,0,0);
      bAutoIndent = (bAutoIndent) ? 0 : 1;
      break;


    case CMD_CTRLBACK:
      {
        int iPos        = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
        int iAnchor     = SendMessage(hwndEdit,SCI_GETANCHOR,0,0);
        int iLine       = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,(WPARAM)iPos,0);
        int iStartPos   = SendMessage(hwndEdit,SCI_POSITIONFROMLINE,(WPARAM)iLine,0);
        int iEndPos     = SendMessage(hwndEdit,SCI_GETLINEENDPOSITION,(WPARAM)iLine,0);
        int iIndentPos  = SendMessage(hwndEdit,SCI_GETLINEINDENTPOSITION,(WPARAM)iLine,0);

        if (iPos != iAnchor)
          SendMessage(hwndEdit,SCI_SETSEL,(WPARAM)iPos,(LPARAM)iPos);
        else {
          if (iPos == iStartPos)
            SendMessage(hwndEdit,SCI_DELETEBACK,0,0);
          else if (iPos <= iIndentPos)
            SendMessage(hwndEdit,SCI_DELLINELEFT,0,0);
          else
            SendMessage(hwndEdit,SCI_DELWORDLEFT,0,0);
        }
      }
      break;


    case CMD_CTRLDEL:
      {
        int iPos        = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
        int iAnchor     = SendMessage(hwndEdit,SCI_GETANCHOR,0,0);
        int iLine       = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,(WPARAM)iPos,0);
        int iStartPos   = SendMessage(hwndEdit,SCI_POSITIONFROMLINE,(WPARAM)iLine,0);
        int iEndPos     = SendMessage(hwndEdit,SCI_GETLINEENDPOSITION,(WPARAM)iLine,0);

        if (iPos != iAnchor)
          SendMessage(hwndEdit,SCI_SETSEL,(WPARAM)iPos,(LPARAM)iPos);
        else {
          if (iStartPos != iEndPos)
            SendMessage(hwndEdit,SCI_DELWORDRIGHT,0,0);
          else // iStartPos == iEndPos
            SendMessage(hwndEdit,SCI_LINEDELETE,0,0);
        }
      }
      break;


    case CMD_CTRLTAB:
      SendMessage(hwndEdit,SCI_SETTABINDENTS,FALSE,0);
      SendMessage(hwndEdit,SCI_SETUSETABS,TRUE,0);
      SendMessage(hwndEdit,SCI_TAB,0,0);
      SendMessage(hwndEdit,SCI_SETUSETABS,!bTabsAsSpaces,0);
      SendMessage(hwndEdit,SCI_SETTABINDENTS,bTabIndents,0);
      break;


    case CMD_RELOADANSI:
      {
        WCHAR tchCurFile2[MAX_PATH];
        if (lstrlen(szCurFile)) {
          iSrcEncoding = CPI_DEFAULT;
          lstrcpy(tchCurFile2,szCurFile);
          FileLoad(FALSE,FALSE,TRUE,FALSE,tchCurFile2);
        }
      }
      break;


    case CMD_RELOADOEM:
      {
        WCHAR tchCurFile2[MAX_PATH];
        if (lstrlen(szCurFile)) {
          iSrcEncoding = CPI_OEM;
          lstrcpy(tchCurFile2,szCurFile);
          FileLoad(FALSE,FALSE,TRUE,FALSE,tchCurFile2);
        }
      }
      break;


    case CMD_RELOADASCIIASUTF8:
      {
        WCHAR tchCurFile2[MAX_PATH];
        BOOL _bLoadASCIIasUTF8 = bLoadASCIIasUTF8;
        if (lstrlen(szCurFile)) {
          bLoadASCIIasUTF8 = 1;
          lstrcpy(tchCurFile2,szCurFile);
          FileLoad(FALSE,FALSE,TRUE,FALSE,tchCurFile2);
          bLoadASCIIasUTF8 = _bLoadASCIIasUTF8;
        }
      }
      break;


    case CMD_RELOADNOFILEVARS:
      {
        WCHAR tchCurFile2[MAX_PATH];
        if (lstrlen(szCurFile)) {
          int _fNoFileVariables = fNoFileVariables;
          BOOL _bNoEncodingTags = bNoEncodingTags;
          fNoFileVariables = 1;
          bNoEncodingTags = 1;
          lstrcpy(tchCurFile2,szCurFile);
          FileLoad(FALSE,FALSE,TRUE,FALSE,tchCurFile2);
          fNoFileVariables = _fNoFileVariables;
          bNoEncodingTags = _bNoEncodingTags;
        }
      }
      break;


    case CMD_LEXDEFAULT:
      Style_SetDefaultLexer(hwndEdit);
      UpdateStatusbar();
      UpdateLineNumberWidth();
      break;


    case CMD_LEXHTML:
      Style_SetHTMLLexer(hwndEdit);
      UpdateStatusbar();
      UpdateLineNumberWidth();
      break;


    case CMD_LEXXML:
      Style_SetXMLLexer(hwndEdit);
      UpdateStatusbar();
      UpdateLineNumberWidth();
      break;


    case CMD_TIMESTAMPS:
      {
        WCHAR wchFind[256] = {0};
        WCHAR wchTemplate[256] = {0};
        WCHAR *pwchSep;
        WCHAR wchReplace[256];

        SYSTEMTIME st;
        struct tm sst;

        UINT cp;
        EDITFINDREPLACE efrTS = { "", "", "", "", SCFIND_REGEXP, 0, 0, 0, 0, 0, hwndEdit };

        IniGetString(L"Settings2",L"TimeStamp",L"\\$Date:[^\\$]+\\$ | $Date: %Y/%m/%d %H:%M:%S $",wchFind,COUNTOF(wchFind));

        if (pwchSep = StrChr(wchFind,L'|')) {
          lstrcpy(wchTemplate,pwchSep+1);
          *pwchSep = 0;
        }

        StrTrim(wchFind,L" ");
        StrTrim(wchTemplate,L" ");

        if (lstrlen(wchFind) == 0 || lstrlen(wchTemplate) == 0)
          break;

        GetLocalTime(&st);
        sst.tm_isdst = -1;
        sst.tm_sec   = (int)st.wSecond;
        sst.tm_min   = (int)st.wMinute;
        sst.tm_hour  = (int)st.wHour;
        sst.tm_mday  = (int)st.wDay;
        sst.tm_mon   = (int)st.wMonth - 1;
        sst.tm_year  = (int)st.wYear - 1900;
        sst.tm_wday  = (int)st.wDayOfWeek;
        mktime(&sst);
        wcsftime(wchReplace,COUNTOF(wchReplace),wchTemplate,&sst);

        cp = SendMessage(hwndEdit,SCI_GETCODEPAGE,0,0);
        WideCharToMultiByte(cp,0,wchFind,-1,efrTS.szFind,COUNTOF(efrTS.szFind),NULL,NULL);
        WideCharToMultiByte(cp,0,wchReplace,-1,efrTS.szReplace,COUNTOF(efrTS.szReplace),NULL,NULL);

        if (SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0) !=
            SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0))
          EditReplaceAllInSelection(hwndEdit,&efrTS,TRUE);
        else
          EditReplaceAll(hwndEdit,&efrTS,TRUE);
      }
      break;


    case CMD_WEBACTION1:
    case CMD_WEBACTION2:
      {
        BOOL  bCmdEnabled = FALSE;
        LPWSTR lpszTemplateName;
        WCHAR  szCmdTemplate[256];
        char  mszSelection[512] = { 0 };
        DWORD cchSelection;
        char  *lpsz;
        LPWSTR lpszCommand;
        LPWSTR lpszArgs;
        SHELLEXECUTEINFO sei;
        WCHAR wchDirectory[MAX_PATH] = L"";

        lpszTemplateName = (LOWORD(wParam) == CMD_WEBACTION1) ? L"WebTemplate1" : L"WebTemplate2";

        bCmdEnabled = IniGetString(L"Settings2",lpszTemplateName,L"",szCmdTemplate,COUNTOF(szCmdTemplate));

        if (bCmdEnabled) {

          cchSelection = SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0) -
                          SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);

          if (cchSelection > 0 && cchSelection <= 500 && SendMessage(hwndEdit,SCI_GETSELTEXT,0,0) < COUNTOF(mszSelection))
          {
            SendMessage(hwndEdit,SCI_GETSELTEXT,0,(LPARAM)mszSelection);
            mszSelection[cchSelection] = 0; // zero terminate

            // Check lpszSelection and truncate bad WCHARs
            lpsz = StrChrA(mszSelection,13);
            if (lpsz) *lpsz = '\0';

            lpsz = StrChrA(mszSelection,10);
            if (lpsz) *lpsz = '\0';

            lpsz = StrChrA(mszSelection,9);
            if (lpsz) *lpsz = '\0';

            if (lstrlenA(mszSelection)) {

              WCHAR wszSelection[512];
              UINT uCP = (SendMessage(hwndEdit,SCI_GETCODEPAGE,0,0) == SC_CP_UTF8) ? CP_UTF8 : CP_ACP;
              MultiByteToWideChar(uCP,0,mszSelection,-1,wszSelection,COUNTOF(wszSelection));

              lpszCommand = GlobalAlloc(GPTR,sizeof(WCHAR)*(512+COUNTOF(szCmdTemplate)+MAX_PATH+32));
              wsprintf(lpszCommand,szCmdTemplate,wszSelection);
              ExpandEnvironmentStringsEx(lpszCommand,GlobalSize(lpszCommand)/sizeof(WCHAR));

              lpszArgs = GlobalAlloc(GPTR,GlobalSize(lpszCommand));
              ExtractFirstArgument(lpszCommand,lpszCommand,lpszArgs);

              if (lstrlen(szCurFile)) {
                lstrcpy(wchDirectory,szCurFile);
                PathRemoveFileSpec(wchDirectory);
              }

              ZeroMemory(&sei,sizeof(SHELLEXECUTEINFO));

              sei.cbSize = sizeof(SHELLEXECUTEINFO);
              sei.fMask = /*SEE_MASK_NOZONECHECKS*/0x00800000;
              sei.hwnd = NULL;
              sei.lpVerb = NULL;
              sei.lpFile = lpszCommand;
              sei.lpParameters = lpszArgs;
              sei.lpDirectory = wchDirectory;
              sei.nShow = SW_SHOWNORMAL;

              ShellExecuteEx(&sei);

              GlobalFree(lpszCommand);
              GlobalFree(lpszArgs);
            }
          }
        }
      }
      break;


    case CMD_FINDNEXTSEL:
    case CMD_FINDPREVSEL:
    case IDM_EDIT_SAVEFIND:
      {
        int cchSelection = SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0) -
                             SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);

        if (cchSelection == 0)
        {
          SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_SELECTWORD,1),0);
          cchSelection = SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0) -
                           SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);
        }

        if (cchSelection > 0 && cchSelection <= 500 && SendMessage(hwndEdit,SCI_GETSELTEXT,0,0) < 512)
        {
          char  mszSelection[512];
          char  *lpsz;

          SendMessage(hwndEdit,SCI_GETSELTEXT,0,(LPARAM)mszSelection);
          mszSelection[cchSelection] = 0; // zero terminate

          // Check lpszSelection and truncate newlines
          lpsz = StrChrA(mszSelection,'\n');
          if (lpsz) *lpsz = '\0';

          lpsz = StrChrA(mszSelection,'\r');
          if (lpsz) *lpsz = '\0';

          cpLastFind = SendMessage(hwndEdit,SCI_GETCODEPAGE,0,0);
          lstrcpyA(efrData.szFind,mszSelection);

          if (cpLastFind != SC_CP_UTF8)
          {
            WCHAR wszBuf[512];

            MultiByteToWideChar(cpLastFind,0,mszSelection,-1,wszBuf,COUNTOF(wszBuf));
            WideCharToMultiByte(CP_UTF8,0,wszBuf,-1,efrData.szFindUTF8,COUNTOF(efrData.szFindUTF8),NULL,NULL);
          }
          else
            lstrcpyA(efrData.szFindUTF8,mszSelection);

          efrData.fuFlags &= (~SCFIND_REGEXP);
          efrData.bTransformBS = FALSE;

          switch (LOWORD(wParam)) {

            case IDM_EDIT_SAVEFIND:
              break;

            case CMD_FINDNEXTSEL:
              EditFindNext(hwndEdit,&efrData,FALSE);
              break;

            case CMD_FINDPREVSEL:
              EditFindPrev(hwndEdit,&efrData,FALSE);
              break;
          }
        }
      }
      break;


    case CMD_INCLINELIMIT:
    case CMD_DECLINELIMIT:
      if (!bMarkLongLines)
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_VIEW_LONGLINEMARKER,1),0);
      else {
        if (LOWORD(wParam) == CMD_INCLINELIMIT)
          iLongLinesLimit++;
        else
          iLongLinesLimit--;
        iLongLinesLimit = max(min(iLongLinesLimit,4096),0);
        SendMessage(hwndEdit,SCI_SETEDGECOLUMN,iLongLinesLimit,0);
        UpdateStatusbar();
        iLongLinesLimitG = iLongLinesLimit;
      }
      break;


    case CMD_STRINGIFY:
      EditEncloseSelection(hwndEdit,L"'",L"'");
      break;


    case CMD_STRINGIFY2:
      EditEncloseSelection(hwndEdit,L"\"",L"\"");
      break;


    case CMD_EMBRACE:
      EditEncloseSelection(hwndEdit,L"(",L")");
      break;


    case CMD_EMBRACE2:
      EditEncloseSelection(hwndEdit,L"[",L"]");
      break;


    case CMD_EMBRACE3:
      EditEncloseSelection(hwndEdit,L"{",L"}");
      break;


    case CMD_EMBRACE4:
      EditEncloseSelection(hwndEdit,L"`",L"`");
      break;


    case CMD_INCREASENUM:
      EditModifyNumber(hwndEdit,TRUE);
      break;


    case CMD_DECREASENUM:
      EditModifyNumber(hwndEdit,FALSE);
      break;


    case CMD_TOGGLETITLE:
      EditGetExcerpt(hwndEdit,szTitleExcerpt,COUNTOF(szTitleExcerpt));
      SetWindowTitle(hwnd,uidsAppTitle,fIsElevated,IDS_UNTITLED,szCurFile,
        iPathNameFormat,bModified || iEncoding != iOriginalEncoding,
        IDS_READONLY,bReadOnly,szTitleExcerpt);
      break;


    case CMD_JUMP2SELSTART:
      if (SC_SEL_RECTANGLE != SendMessage(hwnd,SCI_GETSELECTIONMODE,0,0)) {
        int iAnchorPos = SendMessage(hwndEdit,SCI_GETANCHOR,0,0);
        int iCursorPos = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
        if (iCursorPos > iAnchorPos) {
          SendMessage(hwndEdit,SCI_SETSEL,iCursorPos,iAnchorPos);
          SendMessage(hwndEdit,SCI_CHOOSECARETX,0,0);
        }
      }
      break;


    case CMD_JUMP2SELEND:
      if (SC_SEL_RECTANGLE != SendMessage(hwnd,SCI_GETSELECTIONMODE,0,0)) {
        int iAnchorPos = SendMessage(hwndEdit,SCI_GETANCHOR,0,0);
        int iCursorPos = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
        if (iCursorPos < iAnchorPos) {
          SendMessage(hwndEdit,SCI_SETSEL,iCursorPos,iAnchorPos);
          SendMessage(hwndEdit,SCI_CHOOSECARETX,0,0);
        }
      }
      break;


    case CMD_COPYPATHNAME: {

        WCHAR *pszCopy;
        WCHAR tchUntitled[32];
        if (lstrlen(szCurFile))
          pszCopy = szCurFile;
        else {
          GetString(IDS_UNTITLED,tchUntitled,COUNTOF(tchUntitled));
          pszCopy = tchUntitled;
        }

        if (OpenClipboard(hwnd)) {
          HANDLE hData;
          WCHAR *pData;
          EmptyClipboard();
          hData = GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,sizeof(WCHAR) * (lstrlen(pszCopy) + 1));
          pData = GlobalLock(hData);
          StrCpyN(pData,pszCopy,GlobalSize(hData) / sizeof(WCHAR));
          GlobalUnlock(hData);
          SetClipboardData(CF_UNICODETEXT,hData);
          CloseClipboard();
        }
      }
      break;


    case CMD_OPENINIFILE:
      if (lstrlen(szIniFile)) {
        CreateIniFile();
        FileLoad(FALSE,FALSE,FALSE,FALSE,szIniFile);
      }
      break;


    case IDT_FILE_NEW:
      if (IsCmdEnabled(hwnd,IDM_FILE_NEW))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_FILE_NEW,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_FILE_OPEN:
      if (IsCmdEnabled(hwnd,IDM_FILE_OPEN))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_FILE_OPEN,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_FILE_BROWSE:
      if (IsCmdEnabled(hwnd,IDM_FILE_BROWSE))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_FILE_BROWSE,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_FILE_SAVE:
      if (IsCmdEnabled(hwnd,IDM_FILE_SAVE))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_FILE_SAVE,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_EDIT_UNDO:
      if (IsCmdEnabled(hwnd,IDM_EDIT_UNDO))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_UNDO,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_EDIT_REDO:
      if (IsCmdEnabled(hwnd,IDM_EDIT_REDO))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_REDO,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_EDIT_CUT:
      if (IsCmdEnabled(hwnd,IDM_EDIT_CUT))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_CUT,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_EDIT_COPY:
      if (IsCmdEnabled(hwnd,IDM_EDIT_COPY))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_COPY,1),0);
      else
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_COPYALL,1),0);
      break;


    case IDT_EDIT_PASTE:
      if (IsCmdEnabled(hwnd,IDM_EDIT_PASTE))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_PASTE,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_EDIT_FIND:
      if (IsCmdEnabled(hwnd,IDM_EDIT_FIND))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_FIND,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_EDIT_REPLACE:
      if (IsCmdEnabled(hwnd,IDM_EDIT_REPLACE))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_REPLACE,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_VIEW_WORDWRAP:
      if (IsCmdEnabled(hwnd,IDM_VIEW_WORDWRAP))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_VIEW_WORDWRAP,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_VIEW_ZOOMIN:
      if (IsCmdEnabled(hwnd,IDM_VIEW_ZOOMIN))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_VIEW_ZOOMIN,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_VIEW_ZOOMOUT:
      if (IsCmdEnabled(hwnd,IDM_VIEW_ZOOMOUT))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_VIEW_ZOOMOUT,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_VIEW_SCHEME:
      if (IsCmdEnabled(hwnd,IDM_VIEW_SCHEME))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_VIEW_SCHEME,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_VIEW_SCHEMECONFIG:
      if (IsCmdEnabled(hwnd,IDM_VIEW_SCHEMECONFIG))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_VIEW_SCHEMECONFIG,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_FILE_EXIT:
      SendMessage(hwnd,WM_CLOSE,0,0);
      break;


    case IDT_FILE_SAVEAS:
      if (IsCmdEnabled(hwnd,IDM_FILE_SAVEAS))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_FILE_SAVEAS,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_FILE_SAVECOPY:
      if (IsCmdEnabled(hwnd,IDM_FILE_SAVECOPY))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_FILE_SAVECOPY,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_EDIT_CLEAR:
      if (IsCmdEnabled(hwnd,IDM_EDIT_CLEAR))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_EDIT_CLEAR,1),0);
      else
        SendMessage(hwndEdit,SCI_CLEARALL,0,0);
      break;


    case IDT_FILE_PRINT:
      if (IsCmdEnabled(hwnd,IDM_FILE_PRINT))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_FILE_PRINT,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_FILE_OPENFAV:
      if (IsCmdEnabled(hwnd,IDM_FILE_OPENFAV))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_FILE_OPENFAV,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_FILE_ADDTOFAV:
      if (IsCmdEnabled(hwnd,IDM_FILE_ADDTOFAV))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_FILE_ADDTOFAV,1),0);
      else
        MessageBeep(0);
      break;


    case IDT_VIEW_TOGGLEFOLDS:
      if (IsCmdEnabled(hwnd,IDM_VIEW_TOGGLEFOLDS))
        SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_VIEW_TOGGLEFOLDS,1),0);
      else
        MessageBeep(0);
      break;

  }

  return(0);

}


//=============================================================================
//
//  MsgNotify() - Handles WM_NOTIFY
//
//
LRESULT MsgNotify(HWND hwnd,WPARAM wParam,LPARAM lParam)
{

  LPNMHDR pnmh = (LPNMHDR)lParam;
  struct SCNotification* scn = (struct SCNotification*)lParam;

  switch(pnmh->idFrom)
  {

    case IDC_EDIT:

      switch(pnmh->code)
      {
        case SCN_UPDATEUI:
          UpdateToolbar();
          UpdateStatusbar();

          // Invalidate invalid selections
          #pragma message("TODO: Remove check for invalid selections once fixed in Scintilla")
          if (SendMessage(hwndEdit,SCI_GETSELECTIONS,0,0) > 1 &&
              SendMessage(hwndEdit,SCI_GETSELECTIONMODE,0,0) != SC_SEL_RECTANGLE) {
            int iCurPos = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
            SendMessage(hwndEdit,WM_CANCELMODE,0,0);
            SendMessage(hwndEdit,SCI_CLEARSELECTIONS,0,0);
            SendMessage(hwndEdit,SCI_SETSELECTION,(WPARAM)iCurPos,(LPARAM)iCurPos);
          }

          // Brace Match
          if (bMatchBraces)
          {
            int iPos;
            char c;

            int iEndStyled = SendMessage(hwndEdit,SCI_GETENDSTYLED,0,0);
            if (iEndStyled < SendMessage(hwndEdit,SCI_GETLENGTH,0,0)) {
              int iLine = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,iEndStyled,0);
              int iEndStyled = SendMessage(hwndEdit,SCI_POSITIONFROMLINE,iLine,0);
              SendMessage(hwndEdit,SCI_COLOURISE,iEndStyled,-1);
            }

            iPos = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
            c = (char)SendMessage(hwndEdit,SCI_GETCHARAT,iPos,0);
            if (StrChrA("()[]{}",c)) {
              int iBrace2 = SendMessage(hwndEdit,SCI_BRACEMATCH,iPos,0);
              if (iBrace2 != -1) {
                int col1 = SendMessage(hwndEdit,SCI_GETCOLUMN,iPos,0);
                int col2 = SendMessage(hwndEdit,SCI_GETCOLUMN,iBrace2,0);
                SendMessage(hwndEdit,SCI_BRACEHIGHLIGHT,iPos,iBrace2);
                SendMessage(hwndEdit,SCI_SETHIGHLIGHTGUIDE,min(col1,col2),0);
              }
              else {
                SendMessage(hwndEdit,SCI_BRACEBADLIGHT,iPos,0);
                SendMessage(hwndEdit,SCI_SETHIGHLIGHTGUIDE,0,0);
              }
            }
            // Try one before
            else
            {
              iPos = SendMessage(hwndEdit,SCI_POSITIONBEFORE,iPos,0);
              c = (char)SendMessage(hwndEdit,SCI_GETCHARAT,iPos,0);
              if (StrChrA("()[]{}",c)) {
                int iBrace2 = SendMessage(hwndEdit,SCI_BRACEMATCH,iPos,0);
                if (iBrace2 != -1) {
                  int col1 = SendMessage(hwndEdit,SCI_GETCOLUMN,iPos,0);
                  int col2 = SendMessage(hwndEdit,SCI_GETCOLUMN,iBrace2,0);
                  SendMessage(hwndEdit,SCI_BRACEHIGHLIGHT,iPos,iBrace2);
                  SendMessage(hwndEdit,SCI_SETHIGHLIGHTGUIDE,min(col1,col2),0);
                }
                else {
                  SendMessage(hwndEdit,SCI_BRACEBADLIGHT,iPos,0);
                  SendMessage(hwndEdit,SCI_SETHIGHLIGHTGUIDE,0,0);
                }
              }
              else {
                SendMessage(hwndEdit,SCI_BRACEHIGHLIGHT,(WPARAM)-1,(LPARAM)-1);
                SendMessage(hwndEdit,SCI_SETHIGHLIGHTGUIDE,0,0);
              }
            }
          }
          break;

        case SCN_CHARADDED:
          // Auto indent
          if (bAutoIndent && (scn->ch == '\x0D' || scn->ch == '\x0A'))
          {
            // in CRLF mode handle LF only...
            if ((SC_EOL_CRLF == iEOLMode && scn->ch != '\x0A') || SC_EOL_CRLF != iEOLMode)
            {
              char *pLineBuf;
              char *pPos;
              //int  iIndentLen;

              int iCurPos    = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
              //int iAnchorPos = SendMessage(hwndEdit,SCI_GETANCHOR,0,0);
              int iCurLine = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,(WPARAM)iCurPos,0);
              int iLineLength = SendMessage(hwndEdit,SCI_LINELENGTH,iCurLine,0);
              //int iIndentBefore = SendMessage(hwndEdit,SCI_GETLINEINDENTATION,(WPARAM)iCurLine-1,0);

#ifdef BOOKMARK_EDITION
            // Move bookmark along with line if inserting lines (pressing return at beginning of line) because Scintilla does not do this for us
            if( iCurLine > 0 )
            {
                int iPrevLineLength = SendMessage(hwndEdit,SCI_GETLINEENDPOSITION,iCurLine-1,0) - SendMessage(hwndEdit,SCI_POSITIONFROMLINE,iCurLine-1,0)  ;
                if( iPrevLineLength == 0 )
                {
                    int bitmask = SendMessage( hwndEdit , SCI_MARKERGET , iCurLine-1 , 0 );
                    if( bitmask & 1 )
                    {
                        SendMessage( hwndEdit , SCI_MARKERDELETE , iCurLine-1 , 0 );
                        SendMessage( hwndEdit , SCI_MARKERADD , iCurLine , 0 );
                    }
                }
            }
#endif

              if (iCurLine > 0/* && iLineLength <= 2*/)
              {
                int iPrevLineLength = SendMessage(hwndEdit,SCI_LINELENGTH,iCurLine-1,0);
                if (pLineBuf = GlobalAlloc(GPTR,iPrevLineLength+1))
                {
                  SendMessage(hwndEdit,SCI_GETLINE,iCurLine-1,(LPARAM)pLineBuf);
                  *(pLineBuf+iPrevLineLength) = '\0';
                  for (pPos = pLineBuf; *pPos; pPos++) {
                    if (*pPos != ' ' && *pPos != '\t')
                      *pPos = '\0';
                  }
                  if (*pLineBuf) {
                    //int iPrevLineStartPos;
                    //int iPrevLineEndPos;
                    //int iPrevLineIndentPos;

                    SendMessage(hwndEdit,SCI_BEGINUNDOACTION,0,0);
                    SendMessage(hwndEdit,SCI_ADDTEXT,lstrlenA(pLineBuf),(LPARAM)pLineBuf);
                    SendMessage(hwndEdit,SCI_ENDUNDOACTION,0,0);

                    //iPrevLineStartPos  = SendMessage(hwndEdit,SCI_POSITIONFROMLINE,(WPARAM)iCurLine-1,0);
                    //iPrevLineEndPos    = SendMessage(hwndEdit,SCI_GETLINEENDPOSITION,(WPARAM)iCurLine-1,0);
                    //iPrevLineIndentPos = SendMessage(hwndEdit,SCI_GETLINEINDENTPOSITION,(WPARAM)iCurLine-1,0);

                    //if (iPrevLineEndPos == iPrevLineIndentPos) {
                    //  SendMessage(hwndEdit,SCI_BEGINUNDOACTION,0,0);
                    //  SendMessage(hwndEdit,SCI_SETTARGETSTART,(WPARAM)iPrevLineStartPos,0);
                    //  SendMessage(hwndEdit,SCI_SETTARGETEND,(WPARAM)iPrevLineEndPos,0);
                    //  SendMessage(hwndEdit,SCI_REPLACETARGET,0,(LPARAM)"");
                    //  SendMessage(hwndEdit,SCI_ENDUNDOACTION,0,0);
                    //}
                  }
                  GlobalFree(pLineBuf);
                  //int iIndent = SendMessage(hwndEdit,SCI_GETLINEINDENTATION,(WPARAM)iCurLine,0);
                  //SendMessage(hwndEdit,SCI_SETLINEINDENTATION,(WPARAM)iCurLine,(LPARAM)iIndentBefore);
                  //iIndentLen = /*- iIndent +*/ SendMessage(hwndEdit,SCI_GETLINEINDENTATION,(WPARAM)iCurLine,0);
                  //if (iIndentLen > 0)
                  //  SendMessage(hwndEdit,SCI_SETSEL,(WPARAM)iAnchorPos+iIndentLen,(LPARAM)iCurPos+iIndentLen);
                }
              }
            }
          }
          // Auto close tags
          else if (bAutoCloseTags && scn->ch == '>')
          {
            int iLexer = SendMessage(hwndEdit,SCI_GETLEXER,0,0);
            if (/*iLexer == SCLEX_HTML || iLexer == SCLEX_XML*/ 1)
            {
              char tchBuf[512];
              char tchIns[516] = "</";
              int  cchIns = 2;
              int  iCurPos = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);
              int  iHelper = iCurPos - (COUNTOF(tchBuf) - 1);
              int  iStartPos = max(0,iHelper);
              int  iSize = iCurPos - iStartPos;

              if (iSize >= 3) {

                struct TextRange tr = { { iStartPos, iCurPos }, tchBuf };
                SendMessage(hwndEdit,SCI_GETTEXTRANGE,0,(LPARAM)&tr);

                if (tchBuf[iSize - 2] != '/') {

                  const char* pBegin = &tchBuf[0];
                  const char* pCur = &tchBuf[iSize - 2];

                  while (pCur > pBegin && *pCur != '<' && *pCur != '>')
                    --pCur;

                  if (*pCur == '<') {
                    pCur++;
                    while (StrChrA(":_-.", *pCur) || IsCharAlphaNumericA(*pCur)) {
                      tchIns[cchIns++] = *pCur;
                      pCur++;
                    }
                  }

                  tchIns[cchIns++] = '>';
                  tchIns[cchIns] = 0;

                  if (cchIns > 3 &&
                      lstrcmpiA(tchIns,"</base>") &&
                      lstrcmpiA(tchIns,"</bgsound>") &&
                      lstrcmpiA(tchIns,"</br>") &&
                      lstrcmpiA(tchIns,"</embed>") &&
                      lstrcmpiA(tchIns,"</hr>") &&
                      lstrcmpiA(tchIns,"</img>") &&
                      lstrcmpiA(tchIns,"</input>") &&
                      lstrcmpiA(tchIns,"</link>") &&
                      lstrcmpiA(tchIns,"</meta>"))
                  {
                    SendMessage(hwndEdit,SCI_BEGINUNDOACTION,0,0);
                    SendMessage(hwndEdit,SCI_REPLACESEL,0,(LPARAM)tchIns);
                    SendMessage(hwndEdit,SCI_SETSEL,iCurPos,iCurPos);
                    SendMessage(hwndEdit,SCI_ENDUNDOACTION,0,0);
                  }
                }
              }
            }
          }
          break;

        case SCN_MODIFIED:
        case SCN_ZOOM:
          UpdateLineNumberWidth();
          break;

        case SCN_SAVEPOINTREACHED:
          bModified = FALSE;
          SetWindowTitle(hwnd,uidsAppTitle,fIsElevated,IDS_UNTITLED,szCurFile,
            iPathNameFormat,bModified || iEncoding != iOriginalEncoding,
            IDS_READONLY,bReadOnly,szTitleExcerpt);
          break;

        case SCN_MARGINCLICK:
          if (scn->margin == MARGIN_FOLD_INDEX)
            FoldClick(SciCall_LineFromPosition(scn->position), scn->modifiers);
          break;

        case SCN_KEY:
          // Also see the corresponding patch in scintilla\src\Editor.cxx
          FoldAltArrow(scn->ch, scn->modifiers);
          break;

        case SCN_SAVEPOINTLEFT:
          bModified = TRUE;
          SetWindowTitle(hwnd,uidsAppTitle,fIsElevated,IDS_UNTITLED,szCurFile,
            iPathNameFormat,bModified || iEncoding != iOriginalEncoding,
            IDS_READONLY,bReadOnly,szTitleExcerpt);
          break;
      }
      break;


    case IDC_TOOLBAR:

      switch(pnmh->code)
      {

        case TBN_ENDADJUST:
          UpdateToolbar();
          break;

        case TBN_QUERYDELETE:
        case TBN_QUERYINSERT:
          return TRUE;

        case TBN_GETBUTTONINFO:
          {
            if (((LPTBNOTIFY)lParam)->iItem < COUNTOF(tbbMainWnd))
            {
              WCHAR tch[256];
              GetString(tbbMainWnd[((LPTBNOTIFY)lParam)->iItem].idCommand,tch,COUNTOF(tch));
              lstrcpyn(((LPTBNOTIFY)lParam)->pszText,/*StrChr(tch,L'\n')+1*/tch,((LPTBNOTIFY)lParam)->cchText);
              CopyMemory(&((LPTBNOTIFY)lParam)->tbButton,&tbbMainWnd[((LPTBNOTIFY)lParam)->iItem],sizeof(TBBUTTON));
              return TRUE;
            }
          }
          return FALSE;

        case TBN_RESET:
          {
            int i; int c = SendMessage(hwndToolbar,TB_BUTTONCOUNT,0,0);
            for (i = 0; i < c; i++)
              SendMessage(hwndToolbar,TB_DELETEBUTTON,0,0);
            SendMessage(hwndToolbar,TB_ADDBUTTONS,NUMINITIALTOOLS,(LPARAM)tbbMainWnd);
            return(0);
          }

      }
      break;


    case IDC_STATUSBAR:

      switch(pnmh->code)
      {

        case NM_CLICK:
          {
            LPNMMOUSE pnmm = (LPNMMOUSE)lParam;

            switch (pnmm->dwItemSpec)
            {
              case STATUS_EOLMODE:
                SendMessage(hwndEdit,SCI_CONVERTEOLS,SendMessage(hwndEdit,SCI_GETEOLMODE,0,0),0);
                return TRUE;

              default:
                return FALSE;
            }
          }

        case NM_DBLCLK:
          {
            int i;
            LPNMMOUSE pnmm = (LPNMMOUSE)lParam;

            switch (pnmm->dwItemSpec)
            {
              case STATUS_CODEPAGE:
                SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_ENCODING_SELECT,1),0);
                return TRUE;

              case STATUS_EOLMODE:
                if (iEOLMode == SC_EOL_CRLF)
                  i = IDM_LINEENDINGS_CRLF;
                else if (iEOLMode == SC_EOL_LF)
                  i = IDM_LINEENDINGS_LF;
                else
                  i = IDM_LINEENDINGS_CR;
                i++;
                if (i > IDM_LINEENDINGS_CR)
                  i = IDM_LINEENDINGS_CRLF;
                SendMessage(hwnd,WM_COMMAND,MAKELONG(i,1),0);
                return TRUE;

              case STATUS_LEXER:
                SendMessage(hwnd,WM_COMMAND,MAKELONG(IDM_VIEW_SCHEME,1),0);
                return TRUE;

              case STATUS_OVRMODE:
                SendMessage(hwndEdit,SCI_EDITTOGGLEOVERTYPE,0,0);
                return TRUE;

              default:
                return FALSE;
            }
          }
          break;

      }
      break;


    default:

      switch(pnmh->code)
      {

        case TTN_NEEDTEXT:
          {
            WCHAR tch[256];

            if (((LPTOOLTIPTEXT)lParam)->uFlags & TTF_IDISHWND)
            {
              ;
            }

            else
            {
              GetString(pnmh->idFrom,tch,COUNTOF(tch));
              lstrcpyn(((LPTOOLTIPTEXT)lParam)->szText,/*StrChr(tch,L'\n')+1*/tch,80);
            }
          }
          break;

      }
      break;

  }


  return(0);

}


//=============================================================================
//
//  LoadSettings()
//
//
void LoadSettings()
{
  WCHAR *pIniSection = LocalAlloc(LPTR,sizeof(WCHAR)*32*1024);
  int   cchIniSection = LocalSize(pIniSection)/sizeof(WCHAR);

  LoadIniSection(L"Settings",pIniSection,cchIniSection);

  bSaveSettings =
    IniSectionGetInt(pIniSection,L"SaveSettings",1);
  if (bSaveSettings) bSaveSettings = 1;

  bSaveRecentFiles =
    IniSectionGetInt(pIniSection,L"SaveRecentFiles",0);
  if (bSaveRecentFiles) bSaveRecentFiles = 1;

  bSaveFindReplace =
    IniSectionGetInt(pIniSection,L"SaveFindReplace",0);
  if (bSaveFindReplace) bSaveFindReplace = 1;

  efrData.bFindClose = IniSectionGetInt(pIniSection,L"CloseFind",0);
  if (efrData.bFindClose) efrData.bReplaceClose = TRUE;

  efrData.bReplaceClose = IniSectionGetInt(pIniSection,L"CloseReplace",0);
  if (efrData.bReplaceClose) efrData.bReplaceClose = TRUE;

  efrData.bNoFindWrap = IniSectionGetInt(pIniSection,L"NoFindWrap",0);
  if (efrData.bNoFindWrap) efrData.bNoFindWrap = TRUE;

  if (!IniSectionGetString(pIniSection,L"OpenWithDir",L"",
        tchOpenWithDir,COUNTOF(tchOpenWithDir)))
    SHGetSpecialFolderPath(NULL,tchOpenWithDir,CSIDL_DESKTOP,TRUE);
  else
    PathAbsoluteFromApp(tchOpenWithDir,NULL,COUNTOF(tchOpenWithDir),TRUE);

  if (!IniSectionGetString(pIniSection,L"Favorites",L"",
        tchFavoritesDir,COUNTOF(tchFavoritesDir)))
    SHGetFolderPath(NULL,CSIDL_PERSONAL,NULL,SHGFP_TYPE_CURRENT,tchFavoritesDir);
  else
    PathAbsoluteFromApp(tchFavoritesDir,NULL,COUNTOF(tchFavoritesDir),TRUE);

  iPathNameFormat = IniSectionGetInt(pIniSection,L"PathNameFormat",0);
  iPathNameFormat = max(min(iPathNameFormat,2),0);

  fWordWrap = IniSectionGetInt(pIniSection,L"WordWrap",0);
  if (fWordWrap) fWordWrap = 1;

  iWordWrapMode = IniSectionGetInt(pIniSection,L"WordWrapMode",0);
  iWordWrapMode = max(min(iWordWrapMode,1),0);

  iWordWrapIndent = IniSectionGetInt(pIniSection,L"WordWrapIndent",0);
  iWordWrapIndent = max(min(iWordWrapIndent,6),0);

  iWordWrapSymbols = IniSectionGetInt(pIniSection,L"WordWrapSymbols",22);
  iWordWrapSymbols = max(min(iWordWrapSymbols%10,2),0)+max(min((iWordWrapSymbols%100-iWordWrapSymbols%10)/10,2),0)*10;

  bShowWordWrapSymbols = IniSectionGetInt(pIniSection,L"ShowWordWrapSymbols",0);
  if (bShowWordWrapSymbols) bShowWordWrapSymbols = 1;

  bMatchBraces = IniSectionGetInt(pIniSection,L"MatchBraces",1);
  if (bMatchBraces) bMatchBraces = 1;

  bAutoCloseTags = IniSectionGetInt(pIniSection,L"AutoCloseTags",0);
  if (bAutoCloseTags) bAutoCloseTags = 1;

  bHiliteCurrentLine = IniSectionGetInt(pIniSection,L"HighlightCurrentLine",0);
  if (bHiliteCurrentLine) bHiliteCurrentLine = 1;

  bAutoIndent = IniSectionGetInt(pIniSection,L"AutoIndent",1);
  if (bAutoIndent) bAutoIndent = 1;

  bShowIndentGuides = IniSectionGetInt(pIniSection,L"ShowIndentGuides",0);
  if (bShowIndentGuides) bShowIndentGuides = 1;

  bTabsAsSpaces = IniSectionGetInt(pIniSection,L"TabsAsSpaces",1);
  if (bTabsAsSpaces) bTabsAsSpaces = 1;
  bTabsAsSpacesG = bTabsAsSpaces;

  bTabIndents = IniSectionGetInt(pIniSection,L"TabIndents",1);
  if (bTabIndents) bTabIndents = 1;

  iTabWidth = IniSectionGetInt(pIniSection,L"TabWidth",2);
  iTabWidth = max(min(iTabWidth,256),1);
  iTabWidthG = iTabWidth;

  iIndentWidth = IniSectionGetInt(pIniSection,L"IndentWidth",0);
  iIndentWidth = max(min(iIndentWidth,256),0);
  iIndentWidthG = iIndentWidth;

  bMarkLongLines = IniSectionGetInt(pIniSection,L"MarkLongLines",0);
  if (bMarkLongLines) bMarkLongLines = 1;

  iLongLinesLimit = IniSectionGetInt(pIniSection,L"LongLinesLimit",72);
  iLongLinesLimit = max(min(iLongLinesLimit,4096),0);
  iLongLinesLimitG = iLongLinesLimit;

  iLongLineMode = IniSectionGetInt(pIniSection,L"LongLineMode",EDGE_LINE);
  iLongLineMode = max(min(iLongLineMode,EDGE_BACKGROUND),EDGE_LINE);

  bShowSelectionMargin = IniSectionGetInt(pIniSection,L"ShowSelectionMargin",0);
  if (bShowSelectionMargin) bShowSelectionMargin = 1;

  bShowLineNumbers = IniSectionGetInt(pIniSection,L"ShowLineNumbers",1);
  if (bShowLineNumbers) bShowLineNumbers = 1;

  bShowCodeFolding = IniSectionGetInt(pIniSection,L"ShowCodeFolding",1);
  if (bShowCodeFolding) bShowCodeFolding = 1;

  bViewWhiteSpace = IniSectionGetInt(pIniSection,L"ViewWhiteSpace",0);
  if (bViewWhiteSpace) bViewWhiteSpace = 1;

  bViewEOLs = IniSectionGetInt(pIniSection,L"ViewEOLs",0);
  if (bViewEOLs) bViewEOLs = 1;

  iDefaultEncoding = IniSectionGetInt(pIniSection,L"DefaultEncoding",0);
  iDefaultEncoding = Encoding_MapIniSetting(TRUE,iDefaultEncoding);
  if (!Encoding_IsValid(iDefaultEncoding)) iDefaultEncoding = CPI_DEFAULT;

  bSkipUnicodeDetection = IniSectionGetInt(pIniSection,L"SkipUnicodeDetection",0);
  if (bSkipUnicodeDetection) bSkipUnicodeDetection = 1;

  bLoadASCIIasUTF8 = IniSectionGetInt(pIniSection,L"LoadASCIIasUTF8",0);
  if (bLoadASCIIasUTF8) bLoadASCIIasUTF8 = 1;

  bLoadNFOasOEM = IniSectionGetInt(pIniSection,L"LoadNFOasOEM",1);
  if (bLoadNFOasOEM) bLoadNFOasOEM = 1;

  bNoEncodingTags = IniSectionGetInt(pIniSection,L"NoEncodingTags",0);
  if (bNoEncodingTags) bNoEncodingTags = 1;

  iDefaultEOLMode = IniSectionGetInt(pIniSection,L"DefaultEOLMode",0);
  iDefaultEOLMode = max(min(iDefaultEOLMode,2),0);

  bFixLineEndings = IniSectionGetInt(pIniSection,L"FixLineEndings",1);
  if (bFixLineEndings) bFixLineEndings = 1;

  bAutoStripBlanks = IniSectionGetInt(pIniSection,L"FixTrailingBlanks",0);
  if (bAutoStripBlanks) bAutoStripBlanks = 1;

  iPrintHeader = IniSectionGetInt(pIniSection,L"PrintHeader",1);
  iPrintHeader = max(min(iPrintHeader,3),0);

  iPrintFooter = IniSectionGetInt(pIniSection,L"PrintFooter",0);
  iPrintFooter = max(min(iPrintFooter,1),0);

  iPrintColor = IniSectionGetInt(pIniSection,L"PrintColorMode",3);
  iPrintColor = max(min(iPrintColor,4),0);

  iPrintZoom = IniSectionGetInt(pIniSection,L"PrintZoom",10)-10;
  iPrintZoom = max(min(iPrintZoom,20),-10);

  pagesetupMargin.left = IniSectionGetInt(pIniSection,L"PrintMarginLeft",-1);
  pagesetupMargin.left = max(pagesetupMargin.left,-1);

  pagesetupMargin.top = IniSectionGetInt(pIniSection,L"PrintMarginTop",-1);
  pagesetupMargin.top = max(pagesetupMargin.top,-1);

  pagesetupMargin.right = IniSectionGetInt(pIniSection,L"PrintMarginRight",-1);
  pagesetupMargin.right = max(pagesetupMargin.right,-1);

  pagesetupMargin.bottom = IniSectionGetInt(pIniSection,L"PrintMarginBottom",-1);
  pagesetupMargin.bottom = max(pagesetupMargin.bottom,-1);

  bSaveBeforeRunningTools = IniSectionGetInt(pIniSection,L"SaveBeforeRunningTools",0);
  if (bSaveBeforeRunningTools) bSaveBeforeRunningTools = 1;

  iFileWatchingMode = IniSectionGetInt(pIniSection,L"FileWatchingMode",0);
  iFileWatchingMode = max(min(iFileWatchingMode,2),0);

  bResetFileWatching = IniSectionGetInt(pIniSection,L"ResetFileWatching",1);
  if (bResetFileWatching) bResetFileWatching = 1;

  iEscFunction = IniSectionGetInt(pIniSection,L"EscFunction",0);
  iEscFunction = max(min(iEscFunction,2),0);

  bAlwaysOnTop = IniSectionGetInt(pIniSection,L"AlwaysOnTop",0);
  if (bAlwaysOnTop) bAlwaysOnTop = 1;

  bMinimizeToTray = IniSectionGetInt(pIniSection,L"MinimizeToTray",0);
  if (bMinimizeToTray) bMinimizeToTray = 1;

  bTransparentMode = IniSectionGetInt(pIniSection,L"TransparentMode",0);
  if (bTransparentMode) bTransparentMode = 1;

  // Check if SetLayeredWindowAttributes() is available
  bTransparentModeAvailable =
    (GetProcAddress(GetModuleHandle(L"User32"),"SetLayeredWindowAttributes") != NULL);

  IniSectionGetString(pIniSection,L"ToolbarButtons",L"",
    tchToolbarButtons,COUNTOF(tchToolbarButtons));

  bShowToolbar = IniSectionGetInt(pIniSection,L"ShowToolbar",1);
  if (bShowToolbar) bShowToolbar = 1;

  bShowStatusbar = IniSectionGetInt(pIniSection,L"ShowStatusbar",1);
  if (bShowStatusbar) bShowStatusbar = 1;

  cxEncodingDlg = IniSectionGetInt(pIniSection,L"EncodingDlgSizeX",256);
  cxEncodingDlg = max(cxEncodingDlg,0);

  cyEncodingDlg = IniSectionGetInt(pIniSection,L"EncodingDlgSizeY",262);
  cyEncodingDlg = max(cyEncodingDlg,0);

  cxRecodeDlg = IniSectionGetInt(pIniSection,L"RecodeDlgSizeX",256);
  cxRecodeDlg = max(cxRecodeDlg,0);

  cyRecodeDlg = IniSectionGetInt(pIniSection,L"RecodeDlgSizeY",262);
  cyRecodeDlg = max(cyRecodeDlg,0);

  cxFileMRUDlg = IniSectionGetInt(pIniSection,L"FileMRUDlgSizeX",412);
  cxFileMRUDlg = max(cxFileMRUDlg,0);

  cyFileMRUDlg = IniSectionGetInt(pIniSection,L"FileMRUDlgSizeY",376);
  cyFileMRUDlg = max(cyFileMRUDlg,0);

  cxOpenWithDlg = IniSectionGetInt(pIniSection,L"OpenWithDlgSizeX",384);
  cxOpenWithDlg = max(cxOpenWithDlg,0);

  cyOpenWithDlg = IniSectionGetInt(pIniSection,L"OpenWithDlgSizeY",386);
  cyOpenWithDlg = max(cyOpenWithDlg,0);

  cxFavoritesDlg = IniSectionGetInt(pIniSection,L"FavoritesDlgSizeX",334);
  cxFavoritesDlg = max(cxFavoritesDlg,0);

  cyFavoritesDlg = IniSectionGetInt(pIniSection,L"FavoritesDlgSizeY",316);
  cyFavoritesDlg = max(cyFavoritesDlg,0);

  xFindReplaceDlg = IniSectionGetInt(pIniSection,L"FindReplaceDlgPosX",0);
  yFindReplaceDlg = IniSectionGetInt(pIniSection,L"FindReplaceDlgPosY",0);

  LoadIniSection(L"Settings2",pIniSection,cchIniSection);

  bStickyWinPos = IniSectionGetInt(pIniSection,L"StickyWindowPosition",0);
  if (bStickyWinPos) bStickyWinPos = 1;

  IniSectionGetString(pIniSection,L"DefaultExtension",L"txt",
    tchDefaultExtension,COUNTOF(tchDefaultExtension));
  StrTrim(tchDefaultExtension,L" \t.\"");

  IniSectionGetString(pIniSection,L"DefaultDirectory",L"",
    tchDefaultDir,COUNTOF(tchDefaultDir));

  ZeroMemory(tchFileDlgFilters,sizeof(WCHAR)*COUNTOF(tchFileDlgFilters));
  IniSectionGetString(pIniSection,L"FileDlgFilters",L"",
    tchFileDlgFilters,COUNTOF(tchFileDlgFilters)-2);

  iFileCheckInverval = IniSectionGetInt(pIniSection,L"FileCheckInverval",2000);

  LoadIniSection(L"Toolbar Images",pIniSection,cchIniSection);
  IniSectionGetString(pIniSection,L"BitmapDefault",L"",
    tchToolbarBitmap,COUNTOF(tchToolbarBitmap));
  IniSectionGetString(pIniSection,L"BitmapHot",L"",
    tchToolbarBitmapHot,COUNTOF(tchToolbarBitmap));
  IniSectionGetString(pIniSection,L"BitmapDisabled",L"",
    tchToolbarBitmapDisabled,COUNTOF(tchToolbarBitmap));

  if (!flagPosParam /*|| bStickyWinPos*/) { // ignore window position if /p was specified

    WCHAR tchPosX[32], tchPosY[32], tchSizeX[32], tchSizeY[32], tchMaximized[32];

    int ResX = GetSystemMetrics(SM_CXSCREEN);
    int ResY = GetSystemMetrics(SM_CYSCREEN);

    wsprintf(tchPosX,L"%ix%i PosX",ResX,ResY);
    wsprintf(tchPosY,L"%ix%i PosY",ResX,ResY);
    wsprintf(tchSizeX,L"%ix%i SizeX",ResX,ResY);
    wsprintf(tchSizeY,L"%ix%i SizeY",ResX,ResY);
    wsprintf(tchMaximized,L"%ix%i Maximized",ResX,ResY);

    LoadIniSection(L"Window",pIniSection,cchIniSection);

    wi.x = IniSectionGetInt(pIniSection,tchPosX,CW_USEDEFAULT);
    wi.y = IniSectionGetInt(pIniSection,tchPosY,CW_USEDEFAULT);
    wi.cx = IniSectionGetInt(pIniSection,tchSizeX,CW_USEDEFAULT);
    wi.cy = IniSectionGetInt(pIniSection,tchSizeY,CW_USEDEFAULT);
    wi.max = IniSectionGetInt(pIniSection,tchMaximized,0);
    if (wi.max) wi.max = 1;
  }

  LocalFree(pIniSection);

  iDefaultCodePage = 0; {
    int acp = GetACP();
    if (acp == 932 || acp == 936 || acp == 949 || acp == 950 || acp == 1361)
      iDefaultCodePage = acp;
  }

  {
    CHARSETINFO ci;
    if (TranslateCharsetInfo((DWORD*)iDefaultCodePage,&ci,TCI_SRCCODEPAGE))
      iDefaultCharSet = ci.ciCharset;
    else
      iDefaultCharSet = ANSI_CHARSET;
  }

  // Scintilla Styles
  Style_Load();

}


//=============================================================================
//
//  SaveSettings()
//
//
void SaveSettings(BOOL bSaveSettingsNow)
{
  WCHAR *pIniSection = NULL;
  int   cchIniSection = 0;

  WCHAR wchTmp[MAX_PATH];

  if (lstrlen(szIniFile) == 0)
    return;

  CreateIniFile();

  if (!bSaveSettings && !bSaveSettingsNow) {
    IniSetInt(L"Settings",L"SaveSettings",bSaveSettings);
    return;
  }

  pIniSection = LocalAlloc(LPTR,sizeof(WCHAR)*32*1024);
  cchIniSection = LocalSize(pIniSection)/sizeof(WCHAR);

  IniSectionSetInt(pIniSection,L"SaveSettings",bSaveSettings);
  IniSectionSetInt(pIniSection,L"SaveRecentFiles",bSaveRecentFiles);
  IniSectionSetInt(pIniSection,L"SaveFindReplace",bSaveFindReplace);
  IniSectionSetInt(pIniSection,L"CloseFind",efrData.bFindClose);
  IniSectionSetInt(pIniSection,L"CloseReplace",efrData.bReplaceClose);
  IniSectionSetInt(pIniSection,L"NoFindWrap",efrData.bNoFindWrap);
  PathRelativeToApp(tchOpenWithDir,wchTmp,COUNTOF(wchTmp),FALSE,TRUE,flagPortableMyDocs);
  IniSectionSetString(pIniSection,L"OpenWithDir",wchTmp);
  PathRelativeToApp(tchFavoritesDir,wchTmp,COUNTOF(wchTmp),FALSE,TRUE,flagPortableMyDocs);
  IniSectionSetString(pIniSection,L"Favorites",wchTmp);
  IniSectionSetInt(pIniSection,L"PathNameFormat",iPathNameFormat);
  IniSectionSetInt(pIniSection,L"WordWrap",fWordWrap);
  IniSectionSetInt(pIniSection,L"WordWrapMode",iWordWrapMode);
  IniSectionSetInt(pIniSection,L"WordWrapIndent",iWordWrapIndent);
  IniSectionSetInt(pIniSection,L"WordWrapSymbols",iWordWrapSymbols);
  IniSectionSetInt(pIniSection,L"ShowWordWrapSymbols",bShowWordWrapSymbols);
  IniSectionSetInt(pIniSection,L"MatchBraces",bMatchBraces);
  IniSectionSetInt(pIniSection,L"AutoCloseTags",bAutoCloseTags);
  IniSectionSetInt(pIniSection,L"HighlightCurrentLine",bHiliteCurrentLine);
  IniSectionSetInt(pIniSection,L"AutoIndent",bAutoIndent);
  IniSectionSetInt(pIniSection,L"ShowIndentGuides",bShowIndentGuides);
  IniSectionSetInt(pIniSection,L"TabsAsSpaces",bTabsAsSpacesG);
  IniSectionSetInt(pIniSection,L"TabIndents",bTabIndents);
  IniSectionSetInt(pIniSection,L"TabWidth",iTabWidthG);
  IniSectionSetInt(pIniSection,L"IndentWidth",iIndentWidthG);
  IniSectionSetInt(pIniSection,L"MarkLongLines",bMarkLongLines);
  IniSectionSetInt(pIniSection,L"LongLinesLimit",iLongLinesLimitG);
  IniSectionSetInt(pIniSection,L"LongLineMode",iLongLineMode);
  IniSectionSetInt(pIniSection,L"ShowSelectionMargin",bShowSelectionMargin);
  IniSectionSetInt(pIniSection,L"ShowLineNumbers",bShowLineNumbers);
  IniSectionSetInt(pIniSection,L"ShowCodeFolding",bShowCodeFolding);
  IniSectionSetInt(pIniSection,L"ViewWhiteSpace",bViewWhiteSpace);
  IniSectionSetInt(pIniSection,L"ViewEOLs",bViewEOLs);
  IniSectionSetInt(pIniSection,L"DefaultEncoding",Encoding_MapIniSetting(FALSE,iDefaultEncoding));
  IniSectionSetInt(pIniSection,L"SkipUnicodeDetection",bSkipUnicodeDetection);
  IniSectionSetInt(pIniSection,L"LoadASCIIasUTF8",bLoadASCIIasUTF8);
  IniSectionSetInt(pIniSection,L"LoadNFOasOEM",bLoadNFOasOEM);
  IniSectionSetInt(pIniSection,L"NoEncodingTags",bNoEncodingTags);
  IniSectionSetInt(pIniSection,L"DefaultEOLMode",iDefaultEOLMode);
  IniSectionSetInt(pIniSection,L"FixLineEndings",bFixLineEndings);
  IniSectionSetInt(pIniSection,L"FixTrailingBlanks",bAutoStripBlanks);
  IniSectionSetInt(pIniSection,L"PrintHeader",iPrintHeader);
  IniSectionSetInt(pIniSection,L"PrintFooter",iPrintFooter);
  IniSectionSetInt(pIniSection,L"PrintColorMode",iPrintColor);
  IniSectionSetInt(pIniSection,L"PrintZoom",iPrintZoom+10);
  IniSectionSetInt(pIniSection,L"PrintMarginLeft",pagesetupMargin.left);
  IniSectionSetInt(pIniSection,L"PrintMarginTop",pagesetupMargin.top);
  IniSectionSetInt(pIniSection,L"PrintMarginRight",pagesetupMargin.right);
  IniSectionSetInt(pIniSection,L"PrintMarginBottom",pagesetupMargin.bottom);
  IniSectionSetInt(pIniSection,L"SaveBeforeRunningTools",bSaveBeforeRunningTools);
  IniSectionSetInt(pIniSection,L"FileWatchingMode",iFileWatchingMode);
  IniSectionSetInt(pIniSection,L"ResetFileWatching",bResetFileWatching);
  IniSectionSetInt(pIniSection,L"EscFunction",iEscFunction);
  IniSectionSetInt(pIniSection,L"AlwaysOnTop",bAlwaysOnTop);
  IniSectionSetInt(pIniSection,L"MinimizeToTray",bMinimizeToTray);
  IniSectionSetInt(pIniSection,L"TransparentMode",bTransparentMode);
  Toolbar_GetButtons(hwndToolbar,IDT_FILE_NEW,tchToolbarButtons,COUNTOF(tchToolbarButtons));
  IniSectionSetString(pIniSection,L"ToolbarButtons",tchToolbarButtons);
  IniSectionSetInt(pIniSection,L"ShowToolbar",bShowToolbar);
  IniSectionSetInt(pIniSection,L"ShowStatusbar",bShowStatusbar);
  IniSectionSetInt(pIniSection,L"EncodingDlgSizeX",cxEncodingDlg);
  IniSectionSetInt(pIniSection,L"EncodingDlgSizeY",cyEncodingDlg);
  IniSectionSetInt(pIniSection,L"RecodeDlgSizeX",cxRecodeDlg);
  IniSectionSetInt(pIniSection,L"RecodeDlgSizeY",cyRecodeDlg);
  IniSectionSetInt(pIniSection,L"FileMRUDlgSizeX",cxFileMRUDlg);
  IniSectionSetInt(pIniSection,L"FileMRUDlgSizeY",cyFileMRUDlg);
  IniSectionSetInt(pIniSection,L"OpenWithDlgSizeX",cxOpenWithDlg);
  IniSectionSetInt(pIniSection,L"OpenWithDlgSizeY",cyOpenWithDlg);
  IniSectionSetInt(pIniSection,L"FavoritesDlgSizeX",cxFavoritesDlg);
  IniSectionSetInt(pIniSection,L"FavoritesDlgSizeY",cyFavoritesDlg);
  IniSectionSetInt(pIniSection,L"FindReplaceDlgPosX",xFindReplaceDlg);
  IniSectionSetInt(pIniSection,L"FindReplaceDlgPosY",yFindReplaceDlg);

  SaveIniSection(L"Settings",pIniSection);
  LocalFree(pIniSection);

  /*
    SaveSettingsNow(): query Window Dimensions
  */

  if (bSaveSettingsNow)
  {
    WINDOWPLACEMENT wndpl;

    // GetWindowPlacement
    wndpl.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(hwndMain,&wndpl);

    wi.x = wndpl.rcNormalPosition.left;
    wi.y = wndpl.rcNormalPosition.top;
    wi.cx = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
    wi.cy = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top;
    wi.max = (IsZoomed(hwndMain) || (wndpl.flags & WPF_RESTORETOMAXIMIZED));
  }

  if (!IniGetInt(L"Settings2",L"StickyWindowPosition",0)) {

    WCHAR tchPosX[32], tchPosY[32], tchSizeX[32], tchSizeY[32], tchMaximized[32];

    int ResX = GetSystemMetrics(SM_CXSCREEN);
    int ResY = GetSystemMetrics(SM_CYSCREEN);

    wsprintf(tchPosX,L"%ix%i PosX",ResX,ResY);
    wsprintf(tchPosY,L"%ix%i PosY",ResX,ResY);
    wsprintf(tchSizeX,L"%ix%i SizeX",ResX,ResY);
    wsprintf(tchSizeY,L"%ix%i SizeY",ResX,ResY);
    wsprintf(tchMaximized,L"%ix%i Maximized",ResX,ResY);

    IniSetInt(L"Window",tchPosX,wi.x);
    IniSetInt(L"Window",tchPosY,wi.y);
    IniSetInt(L"Window",tchSizeX,wi.cx);
    IniSetInt(L"Window",tchSizeY,wi.cy);
    IniSetInt(L"Window",tchMaximized,wi.max);
  }

  // Scintilla Styles
  Style_Save();

}


//=============================================================================
//
//  ParseCommandLine()
//
//
void ParseCommandLine()
{

  LPWSTR lp1,lp2,lp3;
  BOOL bContinue = TRUE;
  BOOL bIsNotepadReplacement = FALSE;

  LPWSTR lpCmdLine = GetCommandLine();

  if (lstrlen(lpCmdLine) == 0)
    return;

  // Good old console can also send args separated by Tabs
  StrTab2Space(lpCmdLine);

  lp1 = LocalAlloc(LPTR,sizeof(WCHAR)*(lstrlen(lpCmdLine) + 1));
  lp2 = LocalAlloc(LPTR,sizeof(WCHAR)*(lstrlen(lpCmdLine) + 1));
  lp3 = LocalAlloc(LPTR,sizeof(WCHAR)*(lstrlen(lpCmdLine) + 1));

  // Start with 2nd argument
  ExtractFirstArgument(lpCmdLine,lp1,lp3);

  while (bContinue && ExtractFirstArgument(lp3,lp1,lp2))
  {

    // options
    if (lstrcmp(lp1,L"+") == 0)
      flagMultiFileArg = 2;

    else if (lstrcmp(lp1,L"-") == 0)
      flagMultiFileArg = 1;

    else if ((*lp1 == L'/') || (*lp1 == L'-'))
    {

      StrTrim(lp1,L"-/");

      // Encoding
      if (lstrcmpi(lp1,L"ANSI") == 0 || lstrcmpi(lp1,L"A") == 0 || lstrcmpi(lp1,L"MBCS") == 0)
        flagSetEncoding = IDM_ENCODING_ANSI-IDM_ENCODING_ANSI + 1;
      else if (lstrcmpi(lp1,L"UNICODE") == 0 || lstrcmpi(lp1,L"W") == 0)
        flagSetEncoding = IDM_ENCODING_UNICODE-IDM_ENCODING_ANSI + 1;
      else if (lstrcmpi(lp1,L"UNICODEBE") == 0 || lstrcmpi(lp1,L"UNICODE-BE") == 0)
        flagSetEncoding = IDM_ENCODING_UNICODEREV-IDM_ENCODING_ANSI + 1;
      else if (lstrcmpi(lp1,L"UTF8") == 0 || lstrcmpi(lp1,L"UTF-8") == 0)
        flagSetEncoding = IDM_ENCODING_UTF8-IDM_ENCODING_ANSI + 1;
      else if (lstrcmpi(lp1,L"UTF8SIG") == 0 || lstrcmpi(lp1,L"UTF-8SIG") == 0 ||
              lstrcmpi(lp1,L"UTF8SIGNATURE") == 0 || lstrcmpi(lp1,L"UTF-8SIGNATURE") == 0 ||
              lstrcmpi(lp1,L"UTF8-SIGNATURE") == 0 || lstrcmpi(lp1,L"UTF-8-SIGNATURE") == 0)
        flagSetEncoding = IDM_ENCODING_UTF8SIGN-IDM_ENCODING_ANSI + 1;

      // EOL Mode
      else if (lstrcmpi(lp1,L"CRLF") == 0 || lstrcmpi(lp1,L"CR+LF") == 0)
        flagSetEOLMode = IDM_LINEENDINGS_CRLF-IDM_LINEENDINGS_CRLF + 1;
      else if (lstrcmpi(lp1,L"LF") == 0)
        flagSetEOLMode = IDM_LINEENDINGS_LF-IDM_LINEENDINGS_CRLF + 1;
      else if (lstrcmpi(lp1,L"CR") == 0)
        flagSetEOLMode = IDM_LINEENDINGS_CR-IDM_LINEENDINGS_CRLF + 1;

      else switch (*CharUpper(lp1))
      {

        case L'N':
          flagReuseWindow = 0;
          flagNoReuseWindow = 1;
          if (*CharUpper(lp1+1) == L'S')
            flagSingleFileInstance = 1;
          else
            flagSingleFileInstance = 0;
          break;

        case L'R':
          flagReuseWindow = 1;
          flagNoReuseWindow = 0;
          if (*CharUpper(lp1+1) == L'S')
            flagSingleFileInstance = 1;
          else
            flagSingleFileInstance = 0;
          break;

        case L'F':
          if (*(lp1+1) == L'0' || *CharUpper(lp1+1) == L'O')
            lstrcpy(szIniFile,L"*?");
          else if (ExtractFirstArgument(lp2,lp1,lp2)) {
            StrCpyN(szIniFile,lp1,COUNTOF(szIniFile));
            TrimString(szIniFile);
            PathUnquoteSpaces(szIniFile);
          }
          break;

        case L'I':
          flagStartAsTrayIcon = 1;
          break;

        case L'P':
          if (StrCmpNI(lp1,L"POS:",4) == 0)
            lp1 += 3;
          else if (StrCmpNI(lp1,L"POS",3) == 0)
            lp1 += 2;
          else if (*(lp1+1) == L':')
            lp1 += 1;
          else if (bIsNotepadReplacement) {
            if (*(lp1+1) == L'T')
              ExtractFirstArgument(lp2,lp1,lp2);
            break;
          }
          if (*(lp1+1) == L'0' || *CharUpper(lp1+1) == L'O') {
            flagPosParam = 1;
            flagDefaultPos = 1;
          }
          else if (*CharUpper(lp1+1) == L'D' || *CharUpper(lp1+1) == L'S') {
            flagPosParam = 1;
            flagDefaultPos = (StrChrI((lp1+1),L'L')) ? 3 : 2;
          }
          else if (StrChrI(L"FLTRBM",*(lp1+1))) {
            WCHAR *p = (lp1+1);
            flagPosParam = 1;
            flagDefaultPos = 0;
            while (*p) {
              switch (*CharUpper(p)) {
                case L'F':
                  flagDefaultPos &= ~(4|8|16|32);
                  flagDefaultPos |= 64;
                  break;
                case L'L':
                  flagDefaultPos &= ~(8|64);
                  flagDefaultPos |= 4;
                  break;
                case  L'R':
                  flagDefaultPos &= ~(4|64);
                  flagDefaultPos |= 8;
                  break;
                case L'T':
                  flagDefaultPos &= ~(32|64);
                  flagDefaultPos |= 16;
                  break;
                case L'B':
                  flagDefaultPos &= ~(16|64);
                  flagDefaultPos |= 32;
                  break;
                case L'M':
                  if (flagDefaultPos == 0)
                    flagDefaultPos |= 64;
                  flagDefaultPos |= 128;
                  break;
              }
              p = CharNext(p);
            }
          }
          else if (ExtractFirstArgument(lp2,lp1,lp2)) {
            int itok =
              swscanf(lp1,L"%i,%i,%i,%i,%i",&wi.x,&wi.y,&wi.cx,&wi.cy,&wi.max);
            if (itok == 4 || itok == 5) { // scan successful
              flagPosParam = 1;
              if (wi.cx < 1) wi.cx = CW_USEDEFAULT;
              if (wi.cy < 1) wi.cy = CW_USEDEFAULT;
              if (wi.max) wi.max = 1;
              if (itok == 4) wi.max = 0;
            }
          }
          break;

        case L'T':
          if (ExtractFirstArgument(lp2,lp1,lp2)) {
            StrCpyN(szTitleExcerpt,lp1,COUNTOF(szTitleExcerpt));
            fKeepTitleExcerpt = 1;
          }
          break;

        case L'C':
          flagNewFromClipboard = 1;
          break;

        case L'B':
          flagPasteBoard = 1;
          break;

        case L'E':
          if (ExtractFirstArgument(lp2,lp1,lp2)) {
            if (lpEncodingArg)
              LocalFree(lpEncodingArg);
            lpEncodingArg = StrDup(lp1);
          }
          break;

        case L'G':
          if (ExtractFirstArgument(lp2,lp1,lp2)) {
            int itok =
              swscanf(lp1,L"%i,%i",&iInitialLine,&iInitialColumn);
            if (itok == 1 || itok == 2) { // scan successful
              flagJumpTo = 1;
            }
          }
          break;

        case L'M':
          {
            BOOL bFindUp  = FALSE;
            BOOL bRegex   = FALSE;
            BOOL bTransBS = FALSE;

            if (StrChr(lp1,L'-'))
              bFindUp = TRUE;
            if (StrChr(lp1,L'R'))
              bRegex = TRUE;
            if (StrChr(lp1,L'B'))
              bTransBS = TRUE;

            if (ExtractFirstArgument(lp2,lp1,lp2)) {
              if (lpMatchArg)
                GlobalFree(lpMatchArg);
              lpMatchArg = StrDup(lp1);
              flagMatchText = 1;

              if (bFindUp)
                flagMatchText |= 2;

              if (bRegex) {
                flagMatchText &= ~8;
                flagMatchText |= 4;
              }

              if (bTransBS) {
                flagMatchText &= ~4;
                flagMatchText |= 8;
              }
            }
          }
          break;

        case L'Q':
          flagQuietCreate = 1;
          break;

        case L'S':
          if (ExtractFirstArgument(lp2,lp1,lp2)) {
            if (lpSchemeArg)
              LocalFree(lpSchemeArg);
            lpSchemeArg = StrDup(lp1);
            flagLexerSpecified = 1;
          }
          break;

        case L'D':
          if (lpSchemeArg) {
            LocalFree(lpSchemeArg);
            lpSchemeArg = NULL;
          }
          iInitialLexer = 0;
          flagLexerSpecified = 1;
          break;

        case L'H':
          if (lpSchemeArg) {
            LocalFree(lpSchemeArg);
            lpSchemeArg = NULL;
          }
          iInitialLexer = 1;
          flagLexerSpecified = 1;
          break;

        case L'X':
          if (lpSchemeArg) {
            LocalFree(lpSchemeArg);
            lpSchemeArg = NULL;
          }
          iInitialLexer = 2;
          flagLexerSpecified = 1;
          break;

        case L'U':
          flagRelaunchElevated = 1;
          break;

        case L'Z':
          ExtractFirstArgument(lp2,lp1,lp2);
          flagMultiFileArg = 1;
          bIsNotepadReplacement = TRUE;
          break;

        case L'?':
          flagDisplayHelp = 1;
          break;

        default:
          break;

      }

    }

    // pathname
    else
    {
      LPWSTR lpFileBuf = LocalAlloc(LPTR,sizeof(WCHAR)*(lstrlen(lpCmdLine) + 1));

      cchiFileList = lstrlen(lpCmdLine) - lstrlen(lp3);

      if (lpFileArg)
        GlobalFree(lpFileArg);

      lpFileArg = GlobalAlloc(GPTR,sizeof(WCHAR)*(MAX_PATH+2)); // changed for ActivatePrevInst() needs
      StrCpyN(lpFileArg,lp3,MAX_PATH);
      StrTrim(lpFileArg,L" \"");

      while (cFileList < 32 && ExtractFirstArgument(lp3,lpFileBuf,lp3)) {
        PathQuoteSpaces(lpFileBuf);
        lpFileList[cFileList++] = StrDup(lpFileBuf);
      }

      bContinue = FALSE;
      LocalFree(lpFileBuf);
    }

    // Continue with next argument
    if (bContinue)
      lstrcpy(lp3,lp2);

  }

  LocalFree(lp1);
  LocalFree(lp2);
  LocalFree(lp3);

}


//=============================================================================
//
//  LoadFlags()
//
//
void LoadFlags()
{
  WCHAR *pIniSection = LocalAlloc(LPTR,sizeof(WCHAR)*32*1024);
  int   cchIniSection = LocalSize(pIniSection)/sizeof(WCHAR);

  LoadIniSection(L"Settings2",pIniSection,cchIniSection);

  if (!flagReuseWindow && !flagNoReuseWindow) {

    if (!IniSectionGetInt(pIniSection,L"ReuseWindow",0))
      flagNoReuseWindow = 1;

    if (IniSectionGetInt(pIniSection,L"SingleFileInstance",0))
      flagSingleFileInstance = 1;
  }

  if (flagMultiFileArg == 0) {
    if (IniSectionGetInt(pIniSection,L"MultiFileArg",0))
      flagMultiFileArg = 2;
  }

  if (IniSectionGetInt(pIniSection,L"RelativeFileMRU",1))
    flagRelativeFileMRU = 1;

  if (IniSectionGetInt(pIniSection,L"PortableMyDocs",flagRelativeFileMRU))
    flagPortableMyDocs = 1;

  if (IniSectionGetInt(pIniSection,L"NoFadeHidden",0))
    flagNoFadeHidden = 1;

  if (IniSectionGetInt(pIniSection,L"SimpleIndentGuides",0))
    flagSimpleIndentGuides = 1;

  if (IniSectionGetInt(pIniSection,L"NoHTMLGuess",0))
    fNoHTMLGuess = 1;

  if (IniSectionGetInt(pIniSection,L"NoCGIGuess",0))
    fNoCGIGuess = 1;

  if (IniSectionGetInt(pIniSection,L"NoFileVariables",0))
    fNoFileVariables = 1;

  IniSectionGetString(pIniSection,L"ShellAppUserModelID",L"Notepad2",
    g_wchAppUserModelID,COUNTOF(g_wchAppUserModelID));

  if (IniSectionGetInt(pIniSection,L"ShellUseSystemMRU",0))
    flagUseSystemMRU = 1;

  LocalFree(pIniSection);
}


//=============================================================================
//
//  FindIniFile()
//
//
int CheckIniFile(LPWSTR lpszFile,LPCWSTR lpszModule)
{
  WCHAR tchFileExpanded[MAX_PATH];
  WCHAR tchBuild[MAX_PATH];
  ExpandEnvironmentStrings(lpszFile,tchFileExpanded,COUNTOF(tchFileExpanded));

  if (PathIsRelative(tchFileExpanded)) {
    // program directory
    lstrcpy(tchBuild,lpszModule);
    lstrcpy(PathFindFileName(tchBuild),tchFileExpanded);
    if (PathFileExists(tchBuild)) {
      lstrcpy(lpszFile,tchBuild);
      return(1);
    }
    // %appdata%
    if (S_OK == SHGetFolderPath(NULL,CSIDL_APPDATA,NULL,SHGFP_TYPE_CURRENT,tchBuild)) {
      PathAppend(tchBuild,tchFileExpanded);
      if (PathFileExists(tchBuild)) {
        lstrcpy(lpszFile,tchBuild);
        return(1);
      }
    }
    // general
    if (SearchPath(NULL,tchFileExpanded,NULL,COUNTOF(tchBuild),tchBuild,NULL)) {
      lstrcpy(lpszFile,tchBuild);
      return(1);
    }
  }

  else if (PathFileExists(tchFileExpanded)) {
    lstrcpy(lpszFile,tchFileExpanded);
    return(1);
  }

  return(0);
}

int CheckIniFileRedirect(LPWSTR lpszFile,LPCWSTR lpszModule)
{
  WCHAR tch[MAX_PATH];
  if (GetPrivateProfileString(L"Notepad2",L"Notepad2.ini",L"",tch,COUNTOF(tch),lpszFile)) {
    if (CheckIniFile(tch,lpszModule)) {
      lstrcpy(lpszFile,tch);
      return(1);
    }
    else {
      WCHAR tchFileExpanded[MAX_PATH];
      ExpandEnvironmentStrings(tch,tchFileExpanded,COUNTOF(tchFileExpanded));
      if (PathIsRelative(tchFileExpanded)) {
        lstrcpy(lpszFile,lpszModule);
        lstrcpy(PathFindFileName(lpszFile),tchFileExpanded);
        return(1);
      }
      else {
        lstrcpy(lpszFile,tchFileExpanded);
        return(1);
      }
    }
  }
  return(0);
}

int FindIniFile() {

  int bFound = 0;
  WCHAR tchTest[MAX_PATH];
  WCHAR tchModule[MAX_PATH];
  GetModuleFileName(NULL,tchModule,COUNTOF(tchModule));

  if (lstrlen(szIniFile)) {
    if (lstrcmpi(szIniFile,L"*?") == 0)
      return(0);
    else {
      if (!CheckIniFile(szIniFile,tchModule)) {
        ExpandEnvironmentStringsEx(szIniFile,COUNTOF(szIniFile));
        if (PathIsRelative(szIniFile)) {
          lstrcpy(tchTest,tchModule);
          PathRemoveFileSpec(tchTest);
          PathAppend(tchTest,szIniFile);
          lstrcpy(szIniFile,tchTest);
        }
      }
    }
    return(1);
  }

  lstrcpy(tchTest,PathFindFileName(tchModule));
  PathRenameExtension(tchTest,L".ini");
  bFound = CheckIniFile(tchTest,tchModule);

  if (!bFound) {
    lstrcpy(tchTest,L"Notepad2.ini");
    bFound = CheckIniFile(tchTest,tchModule);
  }

  if (bFound) {
    // allow two redirections: administrator -> user -> custom
    if (CheckIniFileRedirect(tchTest,tchModule))
      CheckIniFileRedirect(tchTest,tchModule);
    lstrcpy(szIniFile,tchTest);
  }

  else {
    lstrcpy(szIniFile,tchModule);
    PathRenameExtension(szIniFile,L".ini");
  }

  return(1);
}


int TestIniFile() {

  if (lstrcmpi(szIniFile,L"*?") == 0) {
    lstrcpy(szIniFile2,L"");
    lstrcpy(szIniFile,L"");
    return(0);
  }

  if (PathIsDirectory(szIniFile) || *CharPrev(szIniFile,StrEnd(szIniFile)) == L'\\') {
    WCHAR wchModule[MAX_PATH];
    GetModuleFileName(NULL,wchModule,COUNTOF(wchModule));
    PathAppend(szIniFile,PathFindFileName(wchModule));
    PathRenameExtension(szIniFile,L".ini");
    if (!PathFileExists(szIniFile)) {
      lstrcpy(PathFindFileName(szIniFile),L"Notepad2.ini");
      if (!PathFileExists(szIniFile)) {
        lstrcpy(PathFindFileName(szIniFile),PathFindFileName(wchModule));
        PathRenameExtension(szIniFile,L".ini");
      }
    }
  }

  if (!PathFileExists(szIniFile) || PathIsDirectory(szIniFile)) {
    lstrcpy(szIniFile2,szIniFile);
    lstrcpy(szIniFile,L"");
    return(0);
  }
  else
    return(1);
}


int CreateIniFile() {

  return(CreateIniFileEx(szIniFile));
}


int CreateIniFileEx(LPCWSTR lpszIniFile) {

  if (*lpszIniFile) {

    HANDLE hFile;
    WCHAR *pwchTail;

    if (pwchTail = StrRChrW(lpszIniFile,NULL,L'\\')) {
      *pwchTail = 0;
      SHCreateDirectoryEx(NULL,lpszIniFile,NULL);
      *pwchTail = L'\\';
    }

    hFile = CreateFile(lpszIniFile,
              GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
              NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
    dwLastIOError = GetLastError();
    if (hFile != INVALID_HANDLE_VALUE) {
      if (GetFileSize(hFile,NULL) == 0) {
        DWORD dw;
        WriteFile(hFile,(LPCVOID)L"\xFEFF[Notepad2]\r\n",26,&dw,NULL);
      }
      CloseHandle(hFile);
      return(1);
    }
    else
      return(0);
  }

  else
    return(0);
}


//=============================================================================
//
//  UpdateToolbar()
//
//
#define EnableTool(id,b) SendMessage(hwndToolbar,TB_ENABLEBUTTON,id, \
                           MAKELONG(((b) ? 1 : 0), 0))

#define CheckTool(id,b)  SendMessage(hwndToolbar,TB_CHECKBUTTON,id, \
                           MAKELONG(b,0))

void UpdateToolbar()
{

  int i;

  if (!bShowToolbar)
    return;

  EnableTool(IDT_FILE_ADDTOFAV,lstrlen(szCurFile));

  EnableTool(IDT_EDIT_UNDO,SendMessage(hwndEdit,SCI_CANUNDO,0,0) /*&& !bReadOnly*/);
  EnableTool(IDT_EDIT_REDO,SendMessage(hwndEdit,SCI_CANREDO,0,0) /*&& !bReadOnly*/);

  i = SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0) - SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);
  EnableTool(IDT_EDIT_CUT,i /*&& !bReadOnly*/);
  EnableTool(IDT_EDIT_COPY,SendMessage(hwndEdit,SCI_GETLENGTH,0,0));
  EnableTool(IDT_EDIT_PASTE,SendMessage(hwndEdit,SCI_CANPASTE,0,0) /*&& !bReadOnly*/);

  i = SendMessage(hwndEdit,SCI_GETLENGTH,0,0);
  EnableTool(IDT_EDIT_FIND,i);
  //EnableTool(IDT_EDIT_FINDNEXT,i);
  //EnableTool(IDT_EDIT_FINDPREV,i && lstrlen(efrData.szFind));
  EnableTool(IDT_EDIT_REPLACE,i /*&& !bReadOnly*/);
  EnableTool(IDT_EDIT_CLEAR,i /*&& !bReadOnly*/);

  EnableTool(IDT_VIEW_TOGGLEFOLDS,bShowCodeFolding);

  CheckTool(IDT_VIEW_WORDWRAP,fWordWrap);

}


//=============================================================================
//
//  UpdateStatusbar()
//
//
void UpdateStatusbar()
{

  int iPos;
  int iLn;
  int iLines;
  int iCol;
  int iSel;
  WCHAR tchLn[32];
  WCHAR tchLines[32];
  WCHAR tchCol[32];
  WCHAR tchCols[32];
  WCHAR tchSel[32];
  WCHAR tchDocPos[256];

  int iBytes;
  WCHAR tchBytes[64];
  WCHAR tchDocSize[256];

  WCHAR tchEOLMode[32];
  WCHAR tchOvrMode[32];
  WCHAR tchLexerName[128];

#ifdef BOOKMARK_EDITION
    int iSelStart;
    int iSelEnd;
    int iLineStart;
    int iLineEnd;
    int iStartOfLinePos;
    int iLinesSelected;
    WCHAR tchLinesSelected[32];
#endif

  if (!bShowStatusbar)
    return;

  iPos = SendMessage(hwndEdit,SCI_GETCURRENTPOS,0,0);

  iLn = SendMessage(hwndEdit,SCI_LINEFROMPOSITION,iPos,0) + 1;
  wsprintf(tchLn,L"%i",iLn);
  FormatNumberStr(tchLn);

  iLines = SendMessage(hwndEdit,SCI_GETLINECOUNT,0,0);
  wsprintf(tchLines,L"%i",iLines);
  FormatNumberStr(tchLines);

  iCol = SendMessage(hwndEdit,SCI_GETCOLUMN,iPos,0) + 1;
  wsprintf(tchCol,L"%i",iCol);
  FormatNumberStr(tchCol);

  if (bMarkLongLines) {
    wsprintf(tchCols,L"%i",iLongLinesLimit);
    FormatNumberStr(tchCols);
  }

  if (SC_SEL_RECTANGLE != SendMessage(hwndEdit,SCI_GETSELECTIONMODE,0,0))
  {
    iSel = SendMessage(hwndEdit,SCI_GETSELECTIONEND,0,0) - SendMessage(hwndEdit,SCI_GETSELECTIONSTART,0,0);
    wsprintf(tchSel,L"%i",iSel);
    FormatNumberStr(tchSel);
  }
  else
    lstrcpy(tchSel,L"--");

#ifdef BOOKMARK_EDITION
    // Print number of lines selected lines in statusbar
    iSelStart = SendMessage( hwndEdit , SCI_GETSELECTIONSTART , 0 , 0 );
    iSelEnd = SendMessage( hwndEdit , SCI_GETSELECTIONEND , 0 , 0 );
    iLineStart = SendMessage( hwndEdit , SCI_LINEFROMPOSITION , iSelStart , 0 );
    iLineEnd = SendMessage( hwndEdit , SCI_LINEFROMPOSITION , iSelEnd , 0 );
    iStartOfLinePos = SendMessage( hwndEdit , SCI_POSITIONFROMLINE , iLineEnd , 0 );
    iLinesSelected = iLineEnd - iLineStart;
    if( iSelStart != iSelEnd  &&  iStartOfLinePos != iSelEnd ) iLinesSelected += 1;
    wsprintf(tchLinesSelected,L"%i",iLinesSelected);
    FormatNumberStr(tchLinesSelected);

    if (!bMarkLongLines)
        FormatString(tchDocPos,COUNTOF(tchDocPos),IDS_DOCPOS,tchLn,tchLines,tchCol,tchSel,tchLinesSelected);
    else
        FormatString(tchDocPos,COUNTOF(tchDocPos),IDS_DOCPOS2,tchLn,tchLines,tchCol,tchCols,tchSel,tchLinesSelected);

#else

  if (!bMarkLongLines)
    FormatString(tchDocPos,COUNTOF(tchDocPos),IDS_DOCPOS,tchLn,tchLines,tchCol,tchSel);
  else
    FormatString(tchDocPos,COUNTOF(tchDocPos),IDS_DOCPOS2,tchLn,tchLines,tchCol,tchCols,tchSel);
#endif

  iBytes = SendMessage(hwndEdit,SCI_GETLENGTH,0,0);
  StrFormatByteSize(iBytes,tchBytes,COUNTOF(tchBytes));

  FormatString(tchDocSize,COUNTOF(tchDocSize),IDS_DOCSIZE,tchBytes);

  Encoding_GetLabel(iEncoding);

  if (iEOLMode == SC_EOL_CR)
    lstrcpy(tchEOLMode,L"CR");
  else if (iEOLMode == SC_EOL_LF)
    lstrcpy(tchEOLMode,L"LF");
  else
    lstrcpy(tchEOLMode,L"CR+LF");

  if (SendMessage(hwndEdit,SCI_GETOVERTYPE,0,0))
    lstrcpy(tchOvrMode,L"OVR");
  else
    lstrcpy(tchOvrMode,L"INS");

  Style_GetCurrentLexerName(tchLexerName,COUNTOF(tchLexerName));

  StatusSetText(hwndStatus,STATUS_DOCPOS,tchDocPos);
  StatusSetText(hwndStatus,STATUS_DOCSIZE,tchDocSize);
  StatusSetText(hwndStatus,STATUS_CODEPAGE,mEncoding[iEncoding].wchLabel);
  StatusSetText(hwndStatus,STATUS_EOLMODE,tchEOLMode);
  StatusSetText(hwndStatus,STATUS_OVRMODE,tchOvrMode);
  StatusSetText(hwndStatus,STATUS_LEXER,tchLexerName);

  //InvalidateRect(hwndStatus,NULL,TRUE);

}


//=============================================================================
//
//  UpdateLineNumberWidth()
//
//
void UpdateLineNumberWidth()
{
  char tchLines[32];
  int  iLineMarginWidthNow;
  int  iLineMarginWidthFit;

  if (bShowLineNumbers) {

    wsprintfA(tchLines,"_%i_",SendMessage(hwndEdit,SCI_GETLINECOUNT,0,0));

    iLineMarginWidthNow = SendMessage(hwndEdit,SCI_GETMARGINWIDTHN,0,0);
    iLineMarginWidthFit = SendMessage(hwndEdit,SCI_TEXTWIDTH,STYLE_LINENUMBER,(LPARAM)tchLines);

    if (iLineMarginWidthNow != iLineMarginWidthFit) {
      //SendMessage(hwndEdit,SCI_SETMARGINWIDTHN,0,0);
      SendMessage(hwndEdit,SCI_SETMARGINWIDTHN,0,iLineMarginWidthFit);
    }
  }

  else
    SendMessage(hwndEdit,SCI_SETMARGINWIDTHN,0,0);
}


//=============================================================================
//
//  FileIO()
//
//
BOOL FileIO(BOOL fLoad,LPCWSTR psz,BOOL bNoEncDetect,int *ienc,int *ieol,
            BOOL *pbUnicodeErr,BOOL *pbFileTooBig,
            BOOL *pbCancelDataLoss,BOOL bSaveCopy)
{
  SHFILEINFO shfi;
  WCHAR tch[MAX_PATH+40];
  BOOL fSuccess;
  DWORD dwFileAttributes;

  BeginWaitCursor();

  SHGetFileInfo2(psz,0,&shfi,sizeof(SHFILEINFO),SHGFI_DISPLAYNAME);
  FormatString(tch,COUNTOF(tch),(fLoad) ? IDS_LOADFILE : IDS_SAVEFILE,shfi.szDisplayName);

  StatusSetText(hwndStatus,STATUS_HELP,tch);
  StatusSetSimple(hwndStatus,TRUE);

  InvalidateRect(hwndStatus,NULL,TRUE);
  UpdateWindow(hwndStatus);

  if (fLoad)
    fSuccess = EditLoadFile(hwndEdit,psz,bNoEncDetect,ienc,ieol,pbUnicodeErr,pbFileTooBig);
  else
    fSuccess = EditSaveFile(hwndEdit,psz,*ienc,pbCancelDataLoss,bSaveCopy);

  dwFileAttributes = GetFileAttributes(psz);
  bReadOnly = (dwFileAttributes != INVALID_FILE_ATTRIBUTES && dwFileAttributes & FILE_ATTRIBUTE_READONLY);

  StatusSetSimple(hwndStatus,FALSE);

  EndWaitCursor();

  return(fSuccess);
}


//=============================================================================
//
//  FileLoad()
//
//
BOOL FileLoad(BOOL bDontSave,BOOL bNew,BOOL bReload,BOOL bNoEncDetect,LPCWSTR lpszFile)
{
  WCHAR tch[MAX_PATH] = L"";
  WCHAR szFileName[MAX_PATH] = L"";
  BOOL fSuccess;
  BOOL bUnicodeErr = FALSE;
  BOOL bFileTooBig = FALSE;

  if (!bDontSave)
  {
    if (!FileSave(FALSE,TRUE,FALSE,FALSE))
      return FALSE;
  }

  if (bNew) {
    lstrcpy(szCurFile,L"");
    SetDlgItemText(hwndMain,IDC_FILENAME,szCurFile);
    SetDlgItemInt(hwndMain,IDC_REUSELOCK,GetTickCount(),FALSE);
    if (!fKeepTitleExcerpt)
      lstrcpy(szTitleExcerpt,L"");
    FileVars_Init(NULL,0,&fvCurFile);
    EditSetNewText(hwndEdit,"",0);
    Style_SetLexer(hwndEdit,NULL);
    UpdateLineNumberWidth();
    bModified = FALSE;
    bReadOnly = FALSE;
    iEOLMode = iLineEndings[iDefaultEOLMode];
    SendMessage(hwndEdit,SCI_SETEOLMODE,iLineEndings[iDefaultEOLMode],0);
    iEncoding = iDefaultEncoding;
    iOriginalEncoding = iDefaultEncoding;
    SendMessage(hwndEdit,SCI_SETCODEPAGE,(iDefaultEncoding == CPI_DEFAULT) ? iDefaultCodePage : SC_CP_UTF8,0);
    EditSetNewText(hwndEdit,"",0);
    SetWindowTitle(hwndMain,uidsAppTitle,fIsElevated,IDS_UNTITLED,szCurFile,
      iPathNameFormat,bModified || iEncoding != iOriginalEncoding,
      IDS_READONLY,bReadOnly,szTitleExcerpt);

    // Terminate file watching
    if (bResetFileWatching)
      iFileWatchingMode = 0;
    InstallFileWatching(NULL);

    return TRUE;
  }

  if (!lpszFile || lstrlen(lpszFile) == 0) {
    if (!OpenFileDlg(hwndMain,tch,COUNTOF(tch),NULL))
      return FALSE;
  }

  else
    lstrcpy(tch,lpszFile);

  ExpandEnvironmentStringsEx(tch,COUNTOF(tch));

  if (PathIsRelative(tch)) {
    GetCurrentDirectory(COUNTOF(szFileName),szFileName);
    PathAppend(szFileName,tch);
    if (!PathFileExists(szFileName)) {
      WCHAR wchFullPath[MAX_PATH];
      if (SearchPath(NULL,tch,NULL,COUNTOF(wchFullPath),wchFullPath,NULL)) {
        lstrcpy(szFileName,wchFullPath);
      }
    }
  }

  else
    lstrcpy(szFileName,tch);

  PathCanonicalizeEx(szFileName);
  GetLongPathNameEx(szFileName,szFileName,COUNTOF(szFileName));

  if (PathIsLnkFile(szFileName))
    PathGetLnkPath(szFileName,szFileName,COUNTOF(szFileName));

  // Ask to create a new file...
  if (!bReload && !PathFileExists(szFileName))
  {
    if (flagQuietCreate || MsgBox(MBYESNO,IDS_ASK_CREATE,szFileName) == IDYES) {
      HANDLE hFile = CreateFile(szFileName,
                      GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
                      NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
      dwLastIOError = GetLastError();
      if (fSuccess = (hFile != INVALID_HANDLE_VALUE)) {
        CloseHandle(hFile);
        FileVars_Init(NULL,0,&fvCurFile);
        EditSetNewText(hwndEdit,"",0);
        Style_SetLexer(hwndEdit,NULL);
        iEOLMode = iLineEndings[iDefaultEOLMode];
        SendMessage(hwndEdit,SCI_SETEOLMODE,iLineEndings[iDefaultEOLMode],0);
        if (iSrcEncoding != -1) {
          iEncoding = iSrcEncoding;
          iOriginalEncoding = iSrcEncoding;
        }
        else {
          iEncoding = iDefaultEncoding;
          iOriginalEncoding = iDefaultEncoding;
        }
        SendMessage(hwndEdit,SCI_SETCODEPAGE,(iEncoding == CPI_DEFAULT) ? iDefaultCodePage : SC_CP_UTF8,0);
        bReadOnly = FALSE;
        EditSetNewText(hwndEdit,"",0);
      }
    }
    else
      return FALSE;
  }

  else
    fSuccess = FileIO(TRUE,szFileName,bNoEncDetect,&iEncoding,&iEOLMode,&bUnicodeErr,&bFileTooBig,NULL,FALSE);

  if (fSuccess) {
    lstrcpy(szCurFile,szFileName);
    SetDlgItemText(hwndMain,IDC_FILENAME,szCurFile);
    SetDlgItemInt(hwndMain,IDC_REUSELOCK,GetTickCount(),FALSE);
    if (!fKeepTitleExcerpt)
      lstrcpy(szTitleExcerpt,L"");
    if (!flagLexerSpecified) // flag will be cleared
      Style_SetLexerFromFile(hwndEdit,szCurFile);
    UpdateLineNumberWidth();
    iOriginalEncoding = iEncoding;
    bModified = FALSE;
    //bReadOnly = FALSE;
    SendMessage(hwndEdit,SCI_SETEOLMODE,iEOLMode,0);
    MRU_AddFile(pFileMRU,szFileName,flagRelativeFileMRU,flagPortableMyDocs);
    if (flagUseSystemMRU)
      SHAddToRecentDocs(SHARD_PATHW,szFileName);
    SetWindowTitle(hwndMain,uidsAppTitle,fIsElevated,IDS_UNTITLED,szFileName,
      iPathNameFormat,bModified || iEncoding != iOriginalEncoding,
      IDS_READONLY,bReadOnly,szTitleExcerpt);

    // Install watching of the current file
    if (!bReload && bResetFileWatching)
      iFileWatchingMode = 0;
    InstallFileWatching(szCurFile);

    // the .LOG feature ...
    if (SendMessage(hwndEdit,SCI_GETLENGTH,0,0) >= 4) {
      char tchLog[5] = "";
      SendMessage(hwndEdit,SCI_GETTEXT,5,(LPARAM)tchLog);
      if (lstrcmpiA(tchLog,".LOG") == 0) {
        EditJumpTo(hwndEdit,-1,0);
        SendMessage(hwndEdit,SCI_BEGINUNDOACTION,0,0);
        SendMessage(hwndEdit,SCI_NEWLINE,0,0);
        SendMessage(hwndMain,WM_COMMAND,MAKELONG(IDM_EDIT_INSERT_SHORTDATE,1),0);
        SendMessage(hwndEdit,SCI_NEWLINE,0,0);
        SendMessage(hwndEdit,SCI_ENDUNDOACTION,0,0);
        EditJumpTo(hwndEdit,-1,0);
      }
    }

    // Show warning: Unicode file loaded as ANSI
    if (bUnicodeErr)
      MsgBox(MBWARN,IDS_ERR_UNICODE);
  }

  else if (!bFileTooBig)
    MsgBox(MBWARN,IDS_ERR_LOADFILE,szFileName);

  return(fSuccess);
}


//=============================================================================
//
//  FileSave()
//
//
BOOL FileSave(BOOL bSaveAlways,BOOL bAsk,BOOL bSaveAs,BOOL bSaveCopy)
{
  WCHAR tchFile[MAX_PATH];
  BOOL fSuccess = FALSE;
  BOOL bCancelDataLoss = FALSE;

  BOOL bIsEmptyNewFile = FALSE;
  if (lstrlen(szCurFile) == 0) {
    int cchText = SendMessage(hwndEdit,SCI_GETLENGTH,0,0);
    if (cchText == 0)
      bIsEmptyNewFile = TRUE;
    else if (cchText < 1023) {
      char tchText[2048];
      SendMessage(hwndEdit,SCI_GETTEXT,(WPARAM)2047,(LPARAM)tchText);
      StrTrimA(tchText," \t\n\r");
      if (lstrlenA(tchText) == 0)
        bIsEmptyNewFile = TRUE;
    }
  }

  if (!bSaveAlways && (!bModified && iEncoding == iOriginalEncoding || bIsEmptyNewFile) && !bSaveAs)
    return TRUE;

  if (bAsk)
  {
    // File or "Untitled" ...
    WCHAR tch[MAX_PATH];
    if (lstrlen(szCurFile))
      lstrcpy(tch,szCurFile);
    else
      GetString(IDS_UNTITLED,tch,COUNTOF(tch));

    switch (MsgBox(MBYESNOCANCEL,IDS_ASK_SAVE,tch)) {
      case IDCANCEL:
        return FALSE;
      case IDNO:
        return TRUE;
    }
  }

  // Read only...
  if (!bSaveAs && !bSaveCopy && lstrlen(szCurFile))
  {
    DWORD dwFileAttributes = GetFileAttributes(szCurFile);
    if (dwFileAttributes != INVALID_FILE_ATTRIBUTES)
      bReadOnly = (dwFileAttributes & FILE_ATTRIBUTE_READONLY);
    if (bReadOnly) {
      SetWindowTitle(hwndMain,uidsAppTitle,fIsElevated,IDS_UNTITLED,szCurFile,
        iPathNameFormat,bModified || iEncoding != iOriginalEncoding,
        IDS_READONLY,bReadOnly,szTitleExcerpt);
      if (MsgBox(MBYESNOWARN,IDS_READONLY_SAVE,szCurFile) == IDYES)
        bSaveAs = TRUE;
      else
        return FALSE;
    }
  }

  // Save As...
  if (bSaveAs || bSaveCopy || lstrlen(szCurFile) == 0)
  {
    WCHAR tchInitialDir[MAX_PATH] = L"";
    if (bSaveCopy && lstrlen(tchLastSaveCopyDir)) {
      lstrcpy(tchInitialDir,tchLastSaveCopyDir);
      lstrcpy(tchFile,tchLastSaveCopyDir);
      PathAppend(tchFile,PathFindFileName(szCurFile));
    }
    else
      lstrcpy(tchFile,szCurFile);

    if (SaveFileDlg(hwndMain,tchFile,COUNTOF(tchFile),tchInitialDir))
    {
      if (fSuccess = FileIO(FALSE,tchFile,FALSE,&iEncoding,&iEOLMode,NULL,NULL,&bCancelDataLoss,bSaveCopy))
      {
        if (!bSaveCopy)
        {
          lstrcpy(szCurFile,tchFile);
          SetDlgItemText(hwndMain,IDC_FILENAME,szCurFile);
          SetDlgItemInt(hwndMain,IDC_REUSELOCK,GetTickCount(),FALSE);
          if (!fKeepTitleExcerpt)
            lstrcpy(szTitleExcerpt,L"");
          Style_SetLexerFromFile(hwndEdit,szCurFile);
          UpdateStatusbar();
          UpdateLineNumberWidth();
        }
        else {
          lstrcpy(tchLastSaveCopyDir,tchFile);
          PathRemoveFileSpec(tchLastSaveCopyDir);
        }
      }
    }
    else
      return FALSE;
  }

  else
    fSuccess = FileIO(FALSE,szCurFile,FALSE,&iEncoding,&iEOLMode,NULL,NULL,&bCancelDataLoss,FALSE);

  if (fSuccess)
  {
    if (!bSaveCopy)
    {
      bModified = FALSE;
      iOriginalEncoding = iEncoding;
      MRU_AddFile(pFileMRU,szCurFile,flagRelativeFileMRU,flagPortableMyDocs);
      if (flagUseSystemMRU)
        SHAddToRecentDocs(SHARD_PATHW,szCurFile);
      SetWindowTitle(hwndMain,uidsAppTitle,fIsElevated,IDS_UNTITLED,szCurFile,
        iPathNameFormat,bModified || iEncoding != iOriginalEncoding,
        IDS_READONLY,bReadOnly,szTitleExcerpt);

      // Install watching of the current file
      if (bSaveAs && bResetFileWatching)
        iFileWatchingMode = 0;
      InstallFileWatching(szCurFile);
    }
  }

  else if (!bCancelDataLoss)
  {
    if (lstrlen(szCurFile) != 0)
      lstrcpy(tchFile,szCurFile);

    SetWindowTitle(hwndMain,uidsAppTitle,fIsElevated,IDS_UNTITLED,szCurFile,
      iPathNameFormat,bModified || iEncoding != iOriginalEncoding,
      IDS_READONLY,bReadOnly,szTitleExcerpt);

    MsgBox(MBWARN,IDS_ERR_SAVEFILE,tchFile);
  }

  return(fSuccess);
}


//=============================================================================
//
//  OpenFileDlg()
//
//
BOOL OpenFileDlg(HWND hwnd,LPWSTR lpstrFile,int cchFile,LPCWSTR lpstrInitialDir)
{
  OPENFILENAME ofn;
  WCHAR szFile[MAX_PATH];
  WCHAR szFilter[NUMLEXERS*1024];
  WCHAR tchInitialDir[MAX_PATH];

  lstrcpy(szFile,L"");
  Style_GetOpenDlgFilterStr(szFilter,COUNTOF(szFilter));

  if (!lpstrInitialDir) {
    if (lstrlen(szCurFile)) {
      lstrcpy(tchInitialDir,szCurFile);
      PathRemoveFileSpec(tchInitialDir);
    }
    else if (lstrlen(tchDefaultDir)) {
      ExpandEnvironmentStrings(tchDefaultDir,tchInitialDir,COUNTOF(tchInitialDir));
      if (PathIsRelative(tchInitialDir)) {
        WCHAR tchModule[MAX_PATH];
        GetModuleFileName(NULL,tchModule,COUNTOF(tchModule));
        PathRemoveFileSpec(tchModule);
        PathAppend(tchModule,tchInitialDir);
        PathCanonicalize(tchInitialDir,tchModule);
      }
    }
  }

  ZeroMemory(&ofn,sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hwnd;
  ofn.lpstrFilter = szFilter;
  ofn.lpstrFile = szFile;
  ofn.lpstrInitialDir = (lpstrInitialDir) ? lpstrInitialDir : tchInitialDir;
  ofn.nMaxFile = COUNTOF(szFile);
  ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | /* OFN_NOCHANGEDIR |*/
            OFN_DONTADDTORECENT | OFN_PATHMUSTEXIST |
              OFN_SHAREAWARE /*| OFN_NODEREFERENCELINKS*/;
  ofn.lpstrDefExt = (lstrlen(tchDefaultExtension)) ? tchDefaultExtension : NULL;

  if (GetOpenFileName(&ofn)) {
    lstrcpyn(lpstrFile,szFile,cchFile);
    return TRUE;
  }

  else
    return FALSE;
}


//=============================================================================
//
//  SaveFileDlg()
//
//
BOOL SaveFileDlg(HWND hwnd,LPWSTR lpstrFile,int cchFile,LPCWSTR lpstrInitialDir)
{
  OPENFILENAME ofn;
  WCHAR szNewFile[MAX_PATH];
  WCHAR szFilter[NUMLEXERS*1024];
  WCHAR tchInitialDir[MAX_PATH];

  lstrcpy(szNewFile,lpstrFile);
  Style_GetOpenDlgFilterStr(szFilter,COUNTOF(szFilter));

  if (lstrlen(lpstrInitialDir))
    lstrcpy(tchInitialDir,lpstrInitialDir);

  else if (lstrlen(szCurFile)) {
    lstrcpy(tchInitialDir,szCurFile);
    PathRemoveFileSpec(tchInitialDir);
  }
  else if (lstrlen(tchDefaultDir)) {
    ExpandEnvironmentStrings(tchDefaultDir,tchInitialDir,COUNTOF(tchInitialDir));
    if (PathIsRelative(tchInitialDir)) {
      WCHAR tchModule[MAX_PATH];
      GetModuleFileName(NULL,tchModule,COUNTOF(tchModule));
      PathRemoveFileSpec(tchModule);
      PathAppend(tchModule,tchInitialDir);
      PathCanonicalize(tchInitialDir,tchModule);
    }
  }

  ZeroMemory(&ofn,sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hwnd;
  ofn.lpstrFilter = szFilter;
  ofn.lpstrFile = szNewFile;
  ofn.lpstrInitialDir = tchInitialDir;
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags = OFN_HIDEREADONLY /*| OFN_NOCHANGEDIR*/ |
            /*OFN_NODEREFERENCELINKS |*/ OFN_OVERWRITEPROMPT |
            OFN_DONTADDTORECENT | OFN_PATHMUSTEXIST;
  ofn.lpstrDefExt = (lstrlen(tchDefaultExtension)) ? tchDefaultExtension : NULL;

  if (GetSaveFileName(&ofn)) {
    lstrcpyn(lpstrFile,szNewFile,cchFile);
    return TRUE;
  }

  else
    return FALSE;
}


/******************************************************************************
*
* ActivatePrevInst()
*
* Tries to find and activate an already open Notepad2 Window
*
*
******************************************************************************/
BOOL CALLBACK EnumWndProc(HWND hwnd,LPARAM lParam)
{
  BOOL bContinue = TRUE;
  WCHAR szClassName[64];

  if (GetClassName(hwnd,szClassName,COUNTOF(szClassName)))

    if (lstrcmpi(szClassName,wchWndClass) == 0) {

      DWORD dwReuseLock = GetDlgItemInt(hwnd,IDC_REUSELOCK,NULL,FALSE);
      if (GetTickCount() - dwReuseLock >= REUSEWINDOWLOCKTIMEOUT) {

        *(HWND*)lParam = hwnd;

        if (IsWindowEnabled(hwnd))
          bContinue = FALSE;
      }
    }
  return(bContinue);
}

BOOL CALLBACK EnumWndProc2(HWND hwnd,LPARAM lParam)
{
  BOOL bContinue = TRUE;
  WCHAR szClassName[64];

  if (GetClassName(hwnd,szClassName,COUNTOF(szClassName)))

    if (lstrcmpi(szClassName,wchWndClass) == 0) {

      DWORD dwReuseLock = GetDlgItemInt(hwnd,IDC_REUSELOCK,NULL,FALSE);
      if (GetTickCount() - dwReuseLock >= REUSEWINDOWLOCKTIMEOUT) {

        WCHAR tchFileName[MAX_PATH] = L"";

        if (IsWindowEnabled(hwnd))
          bContinue = FALSE;

        GetDlgItemText(hwnd,IDC_FILENAME,tchFileName,COUNTOF(tchFileName));
        if (lstrcmpi(tchFileName,lpFileArg) == 0)
          *(HWND*)lParam = hwnd;
        else
          bContinue = TRUE;
      }
    }
  return(bContinue);
}

BOOL ActivatePrevInst()
{
  HWND hwnd = NULL;
  COPYDATASTRUCT cds;

  if ((flagNoReuseWindow && !flagSingleFileInstance) || flagStartAsTrayIcon || flagNewFromClipboard || flagPasteBoard)
    return(FALSE);

  if (flagSingleFileInstance && lpFileArg) {

    // Search working directory from second instance, first!
    // lpFileArg is at least MAX_PATH+2 bytes
    WCHAR tchTmp[MAX_PATH];

    ExpandEnvironmentStringsEx(lpFileArg,GlobalSize(lpFileArg)/sizeof(WCHAR));

    if (PathIsRelative(lpFileArg)) {
      GetCurrentDirectory(COUNTOF(tchTmp),tchTmp);
      PathAppend(tchTmp,lpFileArg);
      if (PathFileExists(tchTmp))
        lstrcpy(lpFileArg,tchTmp);
      else {
        if (SearchPath(NULL,lpFileArg,NULL,COUNTOF(tchTmp),tchTmp,NULL))
          lstrcpy(lpFileArg,tchTmp);
        else {
          GetCurrentDirectory(COUNTOF(tchTmp),tchTmp);
          PathAppend(tchTmp,lpFileArg);
          lstrcpy(lpFileArg,tchTmp);
        }
      }
    }

    else if (SearchPath(NULL,lpFileArg,NULL,COUNTOF(tchTmp),tchTmp,NULL))
      lstrcpy(lpFileArg,tchTmp);

    GetLongPathName(lpFileArg,lpFileArg,MAX_PATH);

    EnumWindows(EnumWndProc2,(LPARAM)&hwnd);

    if (hwnd != NULL)
    {
      // Enabled
      if (IsWindowEnabled(hwnd))
      {
        LPNP2PARAMS params;
        DWORD cb = sizeof(NP2PARAMS);

        // Make sure the previous window won't pop up a change notification message
        //SendMessage(hwnd,WM_CHANGENOTIFYCLEAR,0,0);

        if (IsIconic(hwnd))
          ShowWindowAsync(hwnd,SW_RESTORE);

        if (!IsWindowVisible(hwnd)) {
          SendMessage(hwnd,WM_TRAYMESSAGE,0,WM_LBUTTONDBLCLK);
          SendMessage(hwnd,WM_TRAYMESSAGE,0,WM_LBUTTONUP);
        }

        SetForegroundWindow(hwnd);

        if (lpSchemeArg)
          cb += (lstrlen(lpSchemeArg) + 1) * sizeof(WCHAR);

        params = GlobalAlloc(GPTR,cb);
        params->flagFileSpecified = FALSE;
        params->flagQuietCreate = FALSE;
        params->flagLexerSpecified = flagLexerSpecified;
        if (flagLexerSpecified && lpSchemeArg) {
          lstrcpy(StrEnd(&params->wchData)+1,lpSchemeArg);
          params->iInitialLexer = -1;
        }
        else
          params->iInitialLexer = iInitialLexer;
        params->flagJumpTo = flagJumpTo;
        params->iInitialLine = iInitialLine;
        params->iInitialColumn = iInitialColumn;

        params->iSrcEncoding = (lpEncodingArg) ? Encoding_MatchW(lpEncodingArg) : -1;
        params->flagSetEncoding = flagSetEncoding;
        params->flagSetEOLMode = flagSetEOLMode;
        params->flagTitleExcerpt = 0;

        cds.dwData = DATA_NOTEPAD2_PARAMS;
        cds.cbData = GlobalSize(params);
        cds.lpData = params;

        SendMessage(hwnd,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds);
        GlobalFree(params);

        return(TRUE);
      }

      else // IsWindowEnabled()
      {
        // Ask...
        if (IDYES == MsgBox(MBYESNO,IDS_ERR_PREVWINDISABLED))
          return(FALSE);
        else
          return(TRUE);
      }
    }
  }

  if (flagNoReuseWindow)
    return(FALSE);

  hwnd = NULL;
  EnumWindows(EnumWndProc,(LPARAM)&hwnd);

  // Found a window
  if (hwnd != NULL)
  {
    // Enabled
    if (IsWindowEnabled(hwnd))
    {
      // Make sure the previous window won't pop up a change notification message
      //SendMessage(hwnd,WM_CHANGENOTIFYCLEAR,0,0);

      if (IsIconic(hwnd))
        ShowWindowAsync(hwnd,SW_RESTORE);

      if (!IsWindowVisible(hwnd)) {
        SendMessage(hwnd,WM_TRAYMESSAGE,0,WM_LBUTTONDBLCLK);
        SendMessage(hwnd,WM_TRAYMESSAGE,0,WM_LBUTTONUP);
      }

      SetForegroundWindow(hwnd);

      if (lpFileArg)
      {
        // Search working directory from second instance, first!
        // lpFileArg is at least MAX_PATH+2 bytes
        WCHAR tchTmp[MAX_PATH];
        LPNP2PARAMS params;
        DWORD cb = sizeof(NP2PARAMS);
        int cchTitleExcerpt;

        ExpandEnvironmentStringsEx(lpFileArg,GlobalSize(lpFileArg)/sizeof(WCHAR));

        if (PathIsRelative(lpFileArg)) {
          GetCurrentDirectory(COUNTOF(tchTmp),tchTmp);
          PathAppend(tchTmp,lpFileArg);
          if (PathFileExists(tchTmp))
            lstrcpy(lpFileArg,tchTmp);
          else {
            if (SearchPath(NULL,lpFileArg,NULL,COUNTOF(tchTmp),tchTmp,NULL))
              lstrcpy(lpFileArg,tchTmp);
          }
        }

        else if (SearchPath(NULL,lpFileArg,NULL,COUNTOF(tchTmp),tchTmp,NULL))
          lstrcpy(lpFileArg,tchTmp);

        cb += (lstrlen(lpFileArg) + 1) * sizeof(WCHAR);

        if (lpSchemeArg)
          cb += (lstrlen(lpSchemeArg) + 1) * sizeof(WCHAR);

        cchTitleExcerpt = lstrlen(szTitleExcerpt);
        if (cchTitleExcerpt)
          cb += (cchTitleExcerpt + 1) * sizeof(WCHAR);

        params = GlobalAlloc(GPTR,cb);
        params->flagFileSpecified = TRUE;
        lstrcpy(&params->wchData,lpFileArg);
        params->flagQuietCreate = flagQuietCreate;
        params->flagLexerSpecified = flagLexerSpecified;
        if (flagLexerSpecified && lpSchemeArg) {
          lstrcpy(StrEnd(&params->wchData)+1,lpSchemeArg);
          params->iInitialLexer = -1;
        }
        else
          params->iInitialLexer = iInitialLexer;
        params->flagJumpTo = flagJumpTo;
        params->iInitialLine = iInitialLine;
        params->iInitialColumn = iInitialColumn;

        params->iSrcEncoding = (lpEncodingArg) ? Encoding_MatchW(lpEncodingArg) : -1;
        params->flagSetEncoding = flagSetEncoding;
        params->flagSetEOLMode = flagSetEOLMode;

        if (cchTitleExcerpt) {
          lstrcpy(StrEnd(&params->wchData)+1,szTitleExcerpt);
          params->flagTitleExcerpt = 1;
        }
        else
          params->flagTitleExcerpt = 0;

        cds.dwData = DATA_NOTEPAD2_PARAMS;
        cds.cbData = GlobalSize(params);
        cds.lpData = params;

        SendMessage(hwnd,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds);
        GlobalFree(params);
        GlobalFree(lpFileArg);
      }
      return(TRUE);
    }
    else // IsWindowEnabled()
    {
      // Ask...
      if (IDYES == MsgBox(MBYESNO,IDS_ERR_PREVWINDISABLED))
        return(FALSE);
      else
        return(TRUE);
    }
  }
  else
    return(FALSE);
}


//=============================================================================
//
//  RelaunchMultiInst()
//
//
BOOL RelaunchMultiInst() {

  if (flagMultiFileArg == 2 && cFileList > 1) {

    int i;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    LPWSTR lpCmdLineNew = StrDup(GetCommandLine());
    LPWSTR lp1 = LocalAlloc(LPTR,sizeof(WCHAR)*(lstrlen(lpCmdLineNew) + 1));
    LPWSTR lp2 = LocalAlloc(LPTR,sizeof(WCHAR)*(lstrlen(lpCmdLineNew) + 1));

    StrTab2Space(lpCmdLineNew);

    for (i = 0; i < cFileList; i++) {

      lstrcpy(lpCmdLineNew + cchiFileList,L" /n ");
      lstrcat(lpCmdLineNew,lpFileList[i]);
      LocalFree(lpFileList[i]);

      ZeroMemory(&si,sizeof(STARTUPINFO));
      si.cb = sizeof(STARTUPINFO);
      ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));

      CreateProcess(NULL,lpCmdLineNew,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
    }
    LocalFree(lpCmdLineNew);
    LocalFree(lp1);
    LocalFree(lp2);
    GlobalFree(lpFileArg);

    return TRUE;
  }

  else {
    int i;
    for (i = 0; i < cFileList; i++)
      LocalFree(lpFileList[i]);
    return FALSE;
  }
}


//=============================================================================
//
//  RelaunchElevated()
//
//
BOOL RelaunchElevated() {

  if (!IsVista() || fIsElevated || !flagRelaunchElevated || flagDisplayHelp)
    return(FALSE);

  else {

    LPWSTR lpCmdLine;
    LPWSTR lpArg1, lpArg2;
    STARTUPINFO si;
    SHELLEXECUTEINFO sei;

    si.cb = sizeof(STARTUPINFO);
    GetStartupInfo(&si);

    lpCmdLine = GetCommandLine();
    lpArg1 = LocalAlloc(LPTR,sizeof(WCHAR)*(lstrlen(lpCmdLine) + 1));
    lpArg2 = LocalAlloc(LPTR,sizeof(WCHAR)*(lstrlen(lpCmdLine) + 1));
    ExtractFirstArgument(lpCmdLine,lpArg1,lpArg2);

    if (lstrlen(lpArg1)) {

      ZeroMemory(&sei,sizeof(SHELLEXECUTEINFO));
      sei.cbSize = sizeof(SHELLEXECUTEINFO);
      sei.fMask = SEE_MASK_FLAG_NO_UI | /*SEE_MASK_NOZONECHECKS*/0x00800000;
      sei.hwnd = NULL;
      sei.lpVerb = L"runas";
      sei.lpFile = lpArg1;
      sei.lpParameters = lpArg2;
      sei.lpDirectory = NULL;
      sei.nShow = si.wShowWindow;

      ShellExecuteEx(&sei);
    }

    LocalFree(lpArg1);
    LocalFree(lpArg2);

    return(TRUE);
  }
}


//=============================================================================
//
//  ShowNotifyIcon()
//
//
void ShowNotifyIcon(HWND hwnd,BOOL bAdd)
{

  static HICON hIcon;
  NOTIFYICONDATA nid;

  if (!hIcon)
    hIcon = LoadImage(g_hInstance,MAKEINTRESOURCE(IDR_MAINWND),
                      IMAGE_ICON,16,16,LR_DEFAULTCOLOR);

  ZeroMemory(&nid,sizeof(NOTIFYICONDATA));
  nid.cbSize = sizeof(NOTIFYICONDATA);
  nid.hWnd = hwnd;
  nid.uID = 0;
  nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
  nid.uCallbackMessage = WM_TRAYMESSAGE;
  nid.hIcon = hIcon;
  lstrcpy(nid.szTip,L"Notepad2");

  if(bAdd)
    Shell_NotifyIcon(NIM_ADD,&nid);
  else
    Shell_NotifyIcon(NIM_DELETE,&nid);

}


//=============================================================================
//
//  SetNotifyIconTitle()
//
//
void SetNotifyIconTitle(HWND hwnd)
{

  NOTIFYICONDATA nid;
  SHFILEINFO shfi;
  WCHAR tchTitle[128];
  WCHAR tchFormat[32];

  ZeroMemory(&nid,sizeof(NOTIFYICONDATA));
  nid.cbSize = sizeof(NOTIFYICONDATA);
  nid.hWnd = hwnd;
  nid.uID = 0;
  nid.uFlags = NIF_TIP;

  if (lstrlen(szTitleExcerpt)) {
    GetString(IDS_TITLEEXCERPT,tchFormat,COUNTOF(tchFormat));
    wsprintf(tchTitle,tchFormat,szTitleExcerpt);
  }

  else if (lstrlen(szCurFile)) {
    SHGetFileInfo2(szCurFile,0,&shfi,sizeof(SHFILEINFO),SHGFI_DISPLAYNAME);
    PathCompactPathEx(tchTitle,shfi.szDisplayName,COUNTOF(tchTitle)-4,0);
  }
  else
    GetString(IDS_UNTITLED,tchTitle,COUNTOF(tchTitle)-4);

  if (bModified || iEncoding != iOriginalEncoding)
    lstrcpy(nid.szTip,L"* ");
  else
    lstrcpy(nid.szTip,L"");
  lstrcat(nid.szTip,tchTitle);

  Shell_NotifyIcon(NIM_MODIFY,&nid);

}


//=============================================================================
//
//  InstallFileWatching()
//
//
void InstallFileWatching(LPCWSTR lpszFile)
{

  WCHAR tchDirectory[MAX_PATH];
  HANDLE hFind;

  // Terminate
  if (!iFileWatchingMode || !lpszFile || lstrlen(lpszFile) == 0)
  {
    if (bRunningWatch)
    {
      FindCloseChangeNotification(hChangeHandle);
      KillTimer(NULL,ID_WATCHTIMER);
      bRunningWatch = FALSE;
      //bPendingChangeNotify = FALSE;
    }
    return;
  }

  // Install
  else
  {
    // Terminate previous watching
    if (bRunningWatch) {
      FindCloseChangeNotification(hChangeHandle);
      //bPendingChangeNotify = FALSE;
    }

    // No previous watching installed, so launch the timer first
    else
      SetTimer(NULL,ID_WATCHTIMER,iFileCheckInverval,WatchTimerProc);

    lstrcpy(tchDirectory,lpszFile);
    PathRemoveFileSpec(tchDirectory);

    // Save data of current file
    hFind = FindFirstFile(szCurFile,&fdCurFile);
    if (hFind != INVALID_HANDLE_VALUE)
      FindClose(hFind);
    else
      ZeroMemory(&fdCurFile,sizeof(WIN32_FIND_DATA));

    hChangeHandle = FindFirstChangeNotification(tchDirectory,FALSE,
      FILE_NOTIFY_CHANGE_FILE_NAME  | \
      FILE_NOTIFY_CHANGE_DIR_NAME   | \
      FILE_NOTIFY_CHANGE_ATTRIBUTES | \
      FILE_NOTIFY_CHANGE_SIZE | \
      FILE_NOTIFY_CHANGE_LAST_WRITE);

    bRunningWatch = TRUE;
    //bPendingChangeNotify = FALSE;
  }
}


//=============================================================================
//
//  WatchTimerProc()
//
//
void CALLBACK WatchTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
  // Check Change Notification Handle
  if (bRunningWatch && WAIT_OBJECT_0 == WaitForSingleObject(hChangeHandle,0))
  {
    // Check if the changes affect the current file
    WIN32_FIND_DATA fdUpdated;
    HANDLE hFind = FindFirstFile(szCurFile,&fdUpdated);
    if (INVALID_HANDLE_VALUE != hFind)
      FindClose(hFind);
    else
      // The current file has been removed
      ZeroMemory(&fdUpdated,sizeof(WIN32_FIND_DATA));

    // Check if the file has been changed
    if (CompareFileTime(&fdCurFile.ftLastWriteTime,&fdUpdated.ftLastWriteTime) != 0 ||
          fdCurFile.nFileSizeLow != fdUpdated.nFileSizeLow ||
          fdCurFile.nFileSizeHigh != fdUpdated.nFileSizeHigh)
    {
      // Shutdown current watching and give control to main window
      bRunningWatch = FALSE;
      //bPendingChangeNotify = TRUE;
      FindCloseChangeNotification(hChangeHandle);
      KillTimer(NULL,ID_WATCHTIMER);
      SendMessage(hwndMain,WM_CHANGENOTIFY,0,0);
    }

    else
      FindNextChangeNotification(hChangeHandle);
  }
}


//=============================================================================
//
//  PasteBoardTimer()
//
//
void CALLBACK PasteBoardTimer(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
  if (dwLastCopyTime > 0 && GetTickCount() - dwLastCopyTime > 200) {

    if (SendMessage(hwndEdit,SCI_CANPASTE,0,0)) {

      BOOL bAutoIndent2 = bAutoIndent;
      bAutoIndent = 0;
      EditJumpTo(hwndEdit,-1,0);
      SendMessage(hwndEdit,SCI_BEGINUNDOACTION,0,0);
      if (SendMessage(hwndEdit,SCI_GETLENGTH,0,0) > 0)
        SendMessage(hwndEdit,SCI_NEWLINE,0,0);
      SendMessage(hwndEdit,SCI_PASTE,0,0);
      SendMessage(hwndEdit,SCI_NEWLINE,0,0);
      SendMessage(hwndEdit,SCI_ENDUNDOACTION,0,0);
      bAutoIndent = bAutoIndent2;
    }

    dwLastCopyTime = 0;
  }
}



///  End of Notepad2.c  \\\
