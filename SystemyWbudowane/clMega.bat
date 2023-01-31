@ECHO OFF

cd .\Temp

IF EXIST test.hex DEL test.hex

rem IF NOT %1==test.c XCOPY %1 test.c /-Y

:START
@make.exe all
IF ERRORLEVEL 1 GOTO EXIT

GOTO END

:EXIT
@ECHO.
@ECHO **************** ERROR(S) FOUND ****************

:END
@ECHO.
@ECHO ON