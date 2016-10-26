@echo off

set PROJECTFOLDER=_project

REM try to find cmake.exe in src/external/cmake/cmake.exe first, then in the PATH
set EXTERNAL=src\external
pushd %EXTERNAL%
set EXTERNAL=%CD%
popd
set CMAKEBIN=%EXTERNAL%\cmake\bin\cmake.exe
if not exist %CMAKEBIN% (
	set CMAKEBIN=
	for %%X in (cmake.exe) do (set CMAKEBIN=%%~$PATH:X)
)

set OUTPUTPATH=%~dp0%PROJECTFOLDER%

if defined CMAKEBIN (
REM run cmake
	if exist %OUTPUTPATH% rmdir %OUTPUTPATH% /s /q
	mkdir %OUTPUTPATH%
	pushd %OUTPUTPATH%
	%CMAKEBIN% -G"Visual Studio 14 2015 Win64" ..
	popd

) else (
REM print error message
	echo Couldn't locate cmake.
	echo Please make sure that cmake.exe is in the PATH or copy a cmake
	echo distribution into %EXTERNAL%\cmake\
	echo In that case cmake.exe is expected to be located at:
	echo %EXTERNAL%\cmake\bin\cmake.exe
)

pause