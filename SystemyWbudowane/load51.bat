@ECHO OFF

@C:\ADuC\Download\wsd.exe %1 /C:1 /R
IF ERRORLEVEL 1 GOTO ERROR

GOTO END

:ERROR
@ECHO.
@ECHO **************** ERROR(S) FOUND ****************

:END
@ECHO.
@ECHO ON