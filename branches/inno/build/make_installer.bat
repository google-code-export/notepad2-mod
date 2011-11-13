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

SETLOCAL ENABLEEXTENSIONS
CD /D %~dp0

rem Check for the help switches
IF /I "%~1" == "help"   GOTO SHOWHELP
IF /I "%~1" == "/help"  GOTO SHOWHELP
IF /I "%~1" == "-help"  GOTO SHOWHELP
IF /I "%~1" == "--help" GOTO SHOWHELP
IF /I "%~1" == "/?"     GOTO SHOWHELP


REM You can set here the Inno Setup path if for example you have Inno Setup Unicode
REM installed and you want to use the ANSI Inno Setup which is in another location
SET "InnoSetupPath=H:\progs\thirdparty\isetup"

rem Check the building environment
CALL :SubDetectInnoSetup


rem Check for the first switch
IF "%~1" == "" (
  SET "COMPILER=WDK"
) ELSE (
  IF /I "%~1" == "ICL12"    SET "COMPILER=ICL12"  & GOTO START
  IF /I "%~1" == "/ICL12"   SET "COMPILER=ICL12"  & GOTO START
  IF /I "%~1" == "-ICL12"   SET "COMPILER=ICL12"  & GOTO START
  IF /I "%~1" == "--ICL12"  SET "COMPILER=ICL12"  & GOTO START
  IF /I "%~1" == "VS2010"   SET "COMPILER=VS2010" & GOTO START
  IF /I "%~1" == "/VS2010"  SET "COMPILER=VS2010" & GOTO START
  IF /I "%~1" == "-VS2010"  SET "COMPILER=VS2010" & GOTO START
  IF /I "%~1" == "--VS2010" SET "COMPILER=VS2010" & GOTO START
  IF /I "%~1" == "WDK"      SET "COMPILER=WDK"    & GOTO START
  IF /I "%~1" == "/WDK"     SET "COMPILER=WDK"    & GOTO START
  IF /I "%~1" == "-WDK"     SET "COMPILER=WDK"    & GOTO START
  IF /I "%~1" == "--WDK"    SET "COMPILER=WDK"    & GOTO START

  ECHO.
  ECHO Unsupported commandline switch!
  ECHO Run "%~nx0 help" for details about the commandline switches.
  CALL :SUBMSG "ERROR" "Compilation failed!"
)


:START
CALL :SubInstaller %COMPILER%


:END
TITLE Finished!
ECHO.
ENDLOCAL
EXIT /B


:SubInstaller
PUSHD "..\distrib"

TITLE Building %1 installer...
CALL :SUBMSG "INFO" "Building %1 installer..."
"%InnoSetupPath%\iscc.exe" /Q /O"..\build\packages" "notepad2_setup.iss" /D%1
rem  /DUSE_MSVC2010
IF %ERRORLEVEL% NEQ 0 CALL :SUBMSG "ERROR" "Compilation failed!"

POPD
EXIT /B


:SHOWHELP
TITLE "%~nx0 %1"
ECHO. & ECHO.
ECHO Usage:  %~nx0 [ICL12^|VS2010^|WDK]
ECHO.
ECHO Notes:  You can also prefix the commands with "-", "--" or "/".
ECHO         The arguments are not case sensitive.
ECHO. & ECHO.
ECHO Executing "%~nx0" will use the defaults: "%~nx0 WDK"
ECHO.
ENDLOCAL
EXIT /B


:SubDetectInnoSetup
REM Detect if we are running on 64bit WIN and use Wow6432Node, and set the path
REM of Inno Setup accordingly
IF DEFINED PROGRAMFILES(x86) (
  SET "U_=HKLM\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall"
) ELSE (
  SET "U_=HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall"
)

IF NOT DEFINED InnoSetupPath (
  FOR /F "delims=" %%a IN (
    'REG QUERY "%U_%\Inno Setup 5_is1" /v "Inno Setup: App Path"2^>Nul^|FIND "REG_"') DO (
    SET "InnoSetupPath=%%a" & CALL :SubInnoSetupPath %%InnoSetupPath:*Z=%%)
)

IF NOT EXIST "%InnoSetupPath%" CALL :SUBMSG "ERROR" "Inno Setup wasn't found!"
EXIT /B


:SubInnoSetupPath
SET InnoSetupPath=%*
EXIT /B


:SUBMSG
ECHO. & ECHO ______________________________
ECHO [%~1] %~2
ECHO ______________________________ & ECHO.
IF /I "%~1" == "ERROR" (
  PAUSE
  EXIT
) ELSE (
  EXIT /B
)