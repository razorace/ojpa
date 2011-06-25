@ECHO OFF

rem ****************
rem VARIABLE DEFINES
rem ****************

set PK3DLL=jkedlls
set PK3ASSETS=jkestuff
set ASSETSFOLDER=jke

rem ***************
rem START OF SCRIPT 
rem ***************

ECHO.
ECHO ===========
ECHO Making Pk3s
ECHO ===========

ECHO Y | DEL PK3DLL
ECHO Y | DEL PK3Stuff

rem..\Utilities\zip\7za.exe a -tzip %PK3ASSETS%.pk3 .\%ASSETSFOLDER%\* -xr!.screenshots\ -xr!.svn\ -x!*.dll -x!*.so -x!.\%ASSETSFOLDER%\*.* -xr!*.nav -mx9
IF ERRORLEVEL 1 GOTO ERROR
 ..\Utilities\zip\7za.exe a -tzip %PK3DLL%.pk3 .\%ASSETSFOLDER%\*.dll -mx9

IF ERRORLEVEL 1 GOTO ERROR
ECHO.
ECHO ====================
ECHO Finished Making Pk3s
ECHO ====================
GOTO END


:ERROR
ECHO ================================================
ECHO Encountered Error While Zipping PK3S!  ABORTING!
ECHO ================================================
GOTO END


:END


