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
CALL :SubDetectInnoSetup

rem check for the help switches
IF /I "%~1"=="help"   GOTO SHOWHELP
IF /I "%~1"=="/help"  GOTO SHOWHELP
IF /I "%~1"=="-help"  GOTO SHOWHELP
IF /I "%~1"=="--help" GOTO SHOWHELP
IF /I "%~1"=="/?"     GOTO SHOWHELP


rem Check for the first switch
IF "%~1" == "" (
  SET "BUILDTYPE=WDK"
) ELSE (
  IF /I "%~1" == "ICL12"    SET "BUILDTYPE=ICL12"  & GOTO CHECKSECONDARG
  IF /I "%~1" == "/ICL12"   SET "BUILDTYPE=ICL12"  & GOTO CHECKSECONDARG
  IF /I "%~1" == "-ICL12"   SET "BUILDTYPE=ICL12"  & GOTO CHECKSECONDARG
  IF /I "%~1" == "--ICL12"  SET "BUILDTYPE=ICL12"  & GOTO CHECKSECONDARG
  IF /I "%~1" == "VS2010"   SET "BUILDTYPE=VS2010" & GOTO CHECKSECONDARG
  IF /I "%~1" == "/VS2010"  SET "BUILDTYPE=VS2010" & GOTO CHECKSECONDARG
  IF /I "%~1" == "-VS2010"  SET "BUILDTYPE=VS2010" & GOTO CHECKSECONDARG
  IF /I "%~1" == "--VS2010" SET "BUILDTYPE=VS2010" & GOTO CHECKSECONDARG
  IF /I "%~1" == "WDK"      SET "BUILDTYPE=WDK"    & GOTO CHECKSECONDARG
  IF /I "%~1" == "/WDK"     SET "BUILDTYPE=WDK"    & GOTO CHECKSECONDARG
  IF /I "%~1" == "-WDK"     SET "BUILDTYPE=WDK"    & GOTO CHECKSECONDARG
  IF /I "%~1" == "--WDK"    SET "BUILDTYPE=WDK"    & GOTO CHECKSECONDARG

  ECHO.
  ECHO Unsupported commandline switch!
  ECHO Run "%~nx0 help" for details about the commandline switches.
  CALL :SUBMSG "ERROR" "Compilation failed!"
)


:CHECKSECONDARG
rem Check for the second switch
IF "%~2" == "" (
  SET "ARCH=all"
) ELSE (
  IF /I "%~2" == "x86"   SET "ARCH=x86" & GOTO START
  IF /I "%~2" == "/x86"  SET "ARCH=x86" & GOTO START
  IF /I "%~2" == "-x86"  SET "ARCH=x86" & GOTO START
  IF /I "%~2" == "--x86" SET "ARCH=x86" & GOTO START
  IF /I "%~2" == "x64"   SET "ARCH=x64" & GOTO START
  IF /I "%~2" == "/x64"  SET "ARCH=x64" & GOTO START
  IF /I "%~2" == "-x64"  SET "ARCH=x64" & GOTO START
  IF /I "%~2" == "--x64" SET "ARCH=x64" & GOTO START
  IF /I "%~2" == "all"   SET "ARCH=all" & GOTO START
  IF /I "%~2" == "/all"  SET "ARCH=all" & GOTO START
  IF /I "%~2" == "-all"  SET "ARCH=all" & GOTO START
  IF /I "%~2" == "--all" SET "ARCH=all" & GOTO START

  ECHO.
  ECHO Unsupported commandline switch!
  ECHO Run "%~nx0 help" for details about the commandline switches.
  CALL :SUBMSG "ERROR" "Compilation failed!"
)


:START
IF /I "%ARCH%" == "x86" CALL :SubInstaller %BUILDTYPE% & GOTO END
IF /I "%ARCH%" == "x64" CALL :SubInstaller %BUILDTYPE% is64bit & GOTO END
IF /I "%ARCH%" == "all" (
  CALL :SubInstaller %BUILDTYPE%
  CALL :SubInstaller %BUILDTYPE% is64bit
)

:END
TITLE Finished!
ECHO.
ENDLOCAL
EXIT /B


:SubInstaller
PUSHD "..\distrib"

TITLE Building %1 %2 installer...
CALL :SUBMSG "INFO" "Building %1 %2 installer..."
"%InnoSetupPath%\iscc.exe" /Q "notepad2_setup.iss" /D%1 /D%2
IF %ERRORLEVEL% NEQ 0 CALL :SUBMSG "ERROR" "Compilation failed!"

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


:SubDetectInnoSetup
REM Detect if we are running on 64bit WIN and use Wow6432Node, and set the path
REM of Inno Setup accordingly
IF "%PROGRAMFILES(x86)%zzz"=="zzz" (
  SET "U_=HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall"
) ELSE (
  SET "U_=HKLM\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall"
)

FOR /F "delims=" %%a IN (
  'REG QUERY "%U_%\Inno Setup 5_is1" /v "Inno Setup: App Path"2^>Nul^|FIND "REG_"') DO (
  SET "InnoSetupPath=%%a" & CALL :SubInnoSetup %%InnoSetupPath:*Z=%%)
EXIT /B


:SubInnoSetup
SET InnoSetupPath=%*
EXIT /B