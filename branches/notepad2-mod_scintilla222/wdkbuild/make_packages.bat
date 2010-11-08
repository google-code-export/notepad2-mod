@ECHO OFF
SETLOCAL
SET "PERL_PATH=G:\Installation Programs\Programs\Compiling Stuff\Other\ActivePerl-5.12.2.1202-MSWin32-x86-293621"
SET PREFFIX=scintilla222

rem Check the building environment
IF NOT EXIST "%PERL_PATH%" (
  ECHO. && ECHO:______________________________
  ECHO:[INFO] The Perl direcotry wasn't specified; the addon won't be built
  ECHO:______________________________ && ECHO.
)

IF NOT DEFINED VS100COMNTOOLS (
  ECHO. && ECHO:______________________________
  ECHO:[INFO] Visual Studio 2010 wasn't found; the installer won't be built
  ECHO:______________________________ && ECHO.
)

CD /D %~dp0
SET TOOLS_PATH=..\..\distrib\tools

CALL build.cmd
IF %ERRORLEVEL% NEQ 0 GOTO :ErrorDetected

rem Get the version
FOR /f "tokens=3,4 delims= " %%K IN (
  'FINDSTR /I /L /C:"define VERSION_MAJOR" "..\src\Version.h"') DO (
  SET "VerMajor=%%K"&Call :SubVerMajor %%VerMajor:*Z=%%)
FOR /f "tokens=3,4 delims= " %%L IN (
  'FINDSTR /I /L /C:"define VERSION_MINOR" "..\src\Version.h"') DO (
  SET "VerMinor=%%L"&Call :SubVerMinor %%VerMinor:*Z=%%)
FOR /f "tokens=3,4 delims= " %%M IN (
  'FINDSTR /I /L /C:"define VERSION_BUILD" "..\src\Version.h"') DO (
  SET "VerBuild=%%M"&Call :SubVerBuild %%VerBuild:*Z=%%)
FOR /f "tokens=3,4 delims= " %%N IN (
  'FINDSTR /I /L /C:"define VERSION_REV" "..\src\Version_rev.h"') DO (
  SET "VerRev=%%N"&Call :SubVerRev %%VerRev:*Z=%%)

SET NP2_VER=%VerMajor%.%VerMinor%.%VerBuild%


CALL :SubZipFiles Release x86-32
CALL :SubZipFiles Release_x64 x86-64

IF DEFINED VS100COMNTOOLS (
  CALL :SubInstaller x86
  CALL :SubInstaller x64
)

rem Calulate md5/sha1 hashes
PUSHD packages
"%TOOLS_PATH%\md5sum.exe" *.7z *.zip *.exe >md5hashes
"%TOOLS_PATH%\sha1sum.exe" *.7z *.zip *.exe >sha1hashes

rem Compress everything into a single ZIP file
DEL Notepad2-mod.zip >NUL 2>&1
START "" /B /WAIT "%TOOLS_PATH%\7za.exe" a -tzip -mx=9 Notepad2-mod.zip * -x!md5hashes -x!sha1hashes >NUL
IF %ERRORLEVEL% NEQ 0 GOTO :ErrorDetected

ECHO. && ECHO:______________________________
ECHO:[INFO] Notepad2-mod.zip created successfully!
ECHO:______________________________ && ECHO.

POPD
GOTO :END

:END
TITLE Finished!
ECHO. && ECHO.
ENDLOCAL && PAUSE
EXIT


:SubZipFiles
TITLE Creating the %2 ZIP file...

ECHO. && ECHO:______________________________
ECHO:[INFO] Creating the %2 ZIP file...
ECHO:______________________________ && ECHO.

MD "temp_zip" >NUL 2>&1
MD "packages" >NUL 2>&1
COPY "..\License.txt" "temp_zip\" /Y /V
COPY "..\%1\Notepad2.exe" "temp_zip\" /Y /V
COPY "..\distrib\res\cabinet\notepad2.ini" "temp_zip\Notepad2.ini" /Y /V
COPY "..\Notepad2.txt" "temp_zip\" /Y /V
COPY "..\ReadMe-mod.txt" "temp_zip\Readme.txt" /Y /V

PUSHD "temp_zip"
START "" /B /WAIT "%TOOLS_PATH%\7za.exe" a -tzip -mx=9^
 "Notepad2-mod.%NP2_VER%_r%VerRev%_%2_%PREFFIX%.zip" "License.txt" "Notepad2.exe"^
 "Notepad2.ini" "Notepad2.txt" "ReadMe.txt" >NUL
IF %ERRORLEVEL% NEQ 0 GOTO :ErrorDetected

ECHO. && ECHO:______________________________
ECHO:[INFO] Notepad2-mod.%NP2_VER%_r%VerRev%_%2_%PREFFIX%.zip created successfully!
ECHO:______________________________ && ECHO.

MOVE /Y "Notepad2-mod.%NP2_VER%_r%VerRev%_%2_%PREFFIX%.zip" "..\packages" >NUL 2>&1
POPD
RD /S /Q "temp_zip" >NUL 2>&1
GOTO :EOF

:SubInstaller
IF /I "%1"=="x86" (
  SET ARCH=Win32
  SET BINDIR=x86-32
  SET OUTDIR=Release
)
IF /I "%1"=="x64" (
  SET ARCH=x64
  SET BINDIR=x86-64
  SET OUTDIR=Release_x64
)

TITLE Building %BINDIR% installer...

ECHO. && ECHO:______________________________
ECHO:[INFO] Building %BINDIR% installer...
ECHO:______________________________ && ECHO.

PUSHD ..\distrib
MD binaries\%BINDIR% >NUL 2>&1

COPY /B /V /Y ..\%OUTDIR%\Notepad2.exe binaries\%BINDIR%\notepad2.exe
COPY /B /V /Y ..\License.txt binaries\%BINDIR%\license.txt
COPY /B /V /Y res\cabinet\notepad2.inf binaries\%BINDIR%\notepad2.inf
COPY /B /V /Y res\cabinet\notepad2.ini binaries\%BINDIR%\notepad2.ini
COPY /B /V /Y res\cabinet\notepad2.redir.ini binaries\%BINDIR%\notepad2.redir.ini
COPY /B /V /Y ..\Notepad2.txt binaries\%BINDIR%\notepad2.txt
COPY /B /V /Y ..\Readme-mod.txt binaries\%BINDIR%\readme.txt
rem Set the version for the DisplayVersion registry value
CALL tools\BatchSubstitute.bat "0.0.0.0" %NP2_VER%.%VerRev% binaries\%BINDIR%\notepad2.inf >notepad2.inf.temp
COPY /Y binaries\%BINDIR%\notepad2.inf notepad2.inf.orig >NUL
MOVE /Y notepad2.inf.temp binaries\%BINDIR%\notepad2.inf >NUL
tools\cabutcd.exe binaries\%BINDIR% res\cabinet.%BINDIR%.cab
DEL notepad2.inf.orig >NUL 2>&1
RD /Q /S binaries\%BINDIR% >NUL 2>&1

CALL "%VS100COMNTOOLS%vsvars32.bat" >NUL
devenv setup.sln /Rebuild "Full|%ARCH%"
IF %ERRORLEVEL% NEQ 0 GOTO :ErrorDetected
devenv setup.sln /Rebuild "Lite|%ARCH%"
IF %ERRORLEVEL% NEQ 0 GOTO :ErrorDetected

IF EXIST "%PERL_PATH%" (
  "%PERL_PATH%\perl\bin\perl.exe" addon_build.pl
)

MD ..\wdkbuild\packages >NUL 2>&1
IF EXIST "%PERL_PATH%" (
  MOVE setup.%BINDIR%\addon.7z    ..\wdkbuild\packages\Notepad2-mod_Addon.%BINDIR%_%PREFFIX%.7z >NUL
)
MOVE setup.%BINDIR%\setupfull.exe ..\wdkbuild\packages\Notepad2-mod_Setup.%BINDIR%_%PREFFIX%.exe >NUL
MOVE setup.%BINDIR%\setuplite.exe ..\wdkbuild\packages\Notepad2-mod_Setup_Silent.%BINDIR%_%PREFFIX%.exe >NUL

rem Cleanup
RD setup.%BINDIR% >NUL 2>&1
RD /Q binaries >NUL 2>&1
RD /Q /S addon >NUL 2>&1
RD /Q /S obj >NUL 2>&1

POPD
GOTO :EOF

:SubVerMajor
SET VerMajor=%*
GOTO :EOF
:SubVerMinor
SET VerMinor=%*
GOTO :EOF
:SubVerBuild
SET VerBuild=%*
GOTO :EOF
:SubVerRev
SET VerRev=%*
GOTO :EOF

:ErrorDetected
ECHO. && ECHO:______________________________
ECHO:[ERROR] Compilation failed!!!
ECHO:______________________________ && ECHO.

ENDLOCAL
PAUSE
EXIT