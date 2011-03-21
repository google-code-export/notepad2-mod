@ECHO OFF
rem ******************************************************************************
rem *
rem * Notepad2-mod
rem *
rem * make_installer.bat
rem *   Batch file for building the installer for Notepad2-mod with MSVC2010
rem *
rem * See License.txt for details about distribution and modification.
rem *
rem *                                       (c) XhmikosR 2010-2011
rem *                                       http://code.google.com/p/notepad2-mod/
rem *
rem ******************************************************************************

SETLOCAL
CD /D %~dp0

rem Check the building environment
IF "%PROGRAMFILES(x86)%zzz"=="zzz" (
  SET "U_=HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall"
) ELSE (
  SET "U_=HKLM\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall"
)

FOR /F "delims=" %%a IN (
  'REG QUERY "%U_%\Inno Setup 5_is1" /v "Inno Setup: App Path"2^>Nul^|FIND "REG_"') DO (
  SET "InnoSetupPath=%%a" & CALL :SubInnoSetupPath %%InnoSetupPath:*Z=%%)

rem check for the help switches
IF /I "%~1"=="help" GOTO SHOWHELP
IF /I "%~1"=="/help" GOTO SHOWHELP
IF /I "%~1"=="-help" GOTO SHOWHELP
IF /I "%~1"=="--help" GOTO SHOWHELP
IF /I "%~1"=="/?" GOTO SHOWHELP
GOTO CHECKFIRSTARG


:SHOWHELP
TITLE "%~nx0 %1"
ECHO. & ECHO.
ECHO Usage:   %~nx0 [ICL12^|VS2010^|WDK]
ECHO.
ECHO Notes:   You can also prefix the commands with "-", "--" or "/".
ECHO          The arguments are case insesitive.
ECHO. & ECHO.
ECHO Executing "%~nx0" will use the defaults: "%~nx0 WDK"
ECHO.
ENDLOCAL
EXIT /B


:CHECKFIRSTARG
rem Check for the first switch
IF "%~1" == "" (
  SET INPUTDIRx86=bin\WDK\Release_x86
  SET INPUTDIRx64=bin\WDK\Release_x64
  SET SUFFIX=
) ELSE (
  IF /I "%~1" == "WDK" (
    SET INPUTDIRx86=bin\WDK\Release_x86
    SET INPUTDIRx64=bin\WDK\Release_x64
    SET SUFFIX=
    GOTO START
  )
  IF /I "%~1" == "/WDK" (
    SET INPUTDIRx86=bin\WDK\Release_x86
    SET INPUTDIRx64=bin\WDK\Release_x64
    SET SUFFIX=
    GOTO START
  )
  IF /I "%~1" == "-WDK" (
    SET INPUTDIRx86=bin\WDK\Release_x86
    SET INPUTDIRx64=bin\WDK\Release_x64
    SET SUFFIX=
    GOTO START
  )
  IF /I "%~1" == "--WDK" (
    SET INPUTDIRx86=bin\WDK\Release_x86
    SET INPUTDIRx64=bin\WDK\Release_x64
    SET SUFFIX=
    GOTO START
  )
  IF /I "%~1" == "VS2010" (
    SET INPUTDIRx86=bin\VS2010\Release_x86
    SET INPUTDIRx64=bin\VS2010\Release_x64
    SET SUFFIX=_vs2010
    GOTO START
  )
  IF /I "%~1" == "/VS2010" (
    SET INPUTDIRx86=bin\VS2010\Release_x86
    SET INPUTDIRx64=bin\VS2010\Release_x64
    SET SUFFIX=_vs2010
    GOTO START
  )
  IF /I "%~1" == "-VS2010" (
    SET INPUTDIRx86=bin\VS2010\Release_x86
    SET INPUTDIRx64=bin\VS2010\Release_x64
    SET SUFFIX=_vs2010
    GOTO START
  )
  IF /I "%~1" == "--VS2010" (
    SET INPUTDIRx86=bin\VS2010\Release_x86
    SET INPUTDIRx64=bin\VS2010\Release_x64
    SET SUFFIX=_vs2010
    GOTO START
  )
  IF /I "%~1" == "ICL12" (
    SET INPUTDIRx86=bin\ICL12\Release_x86
    SET INPUTDIRx64=bin\ICL12\Release_x64
    SET SUFFIX=_icl12
    GOTO START
  )
  IF /I "%~1" == "/ICL12" (
    SET INPUTDIRx86=bin\ICL12\Release_x86
    SET INPUTDIRx64=bin\ICL12\Release_x64
    SET SUFFIX=_icl12
    GOTO START
  )
  IF /I "%~1" == "-ICL12" (
    SET INPUTDIRx86=bin\ICL12\Release_x86
    SET INPUTDIRx64=bin\ICL12\Release_x64
    SET SUFFIX=_icl12
    GOTO START
  )
  IF /I "%~1" == "--ICL12" (
    SET INPUTDIRx86=bin\ICL12\Release_x86
    SET INPUTDIRx64=bin\ICL12\Release_x64
    SET SUFFIX=_icl12
    GOTO START
  )

  ECHO.
  ECHO Unsupported commandline switch!
  ECHO Run "%~nx0 help" for details about the commandline switches.
  CALL :SUBMSG "ERROR" "Compilation failed!"
)


:START
CALL :SubGetVersion
CALL :SubInstaller %INPUTDIRx86% x86
CALL :SubInstaller %INPUTDIRx64% x64


:END
TITLE Finished!
ECHO.
ENDLOCAL
EXIT /B


:SubInstaller
IF /I "%2"=="x86" (
  SET "ARCH=Win32"
  SET "BINDIR=x86-32"
)
IF /I "%2"=="x64" (
  SET "ARCH=x64"
  SET "BINDIR=x86-64"
)

PUSHD "..\distrib"

TITLE Building %BINDIR% installer...
CALL :SUBMSG "INFO" "Building %BINDIR% installer..."


POPD
EXIT /B


:SUBMSG
ECHO. & ECHO ______________________________
ECHO [%~1] %~2
ECHO ______________________________ & ECHO.
IF /I "%~1"=="ERROR" (
  PAUSE
  EXIT
) ELSE (
  EXIT /B
)


:SubInnoSetupPath
SET InnoSetupPath=%*
EXIT /B
