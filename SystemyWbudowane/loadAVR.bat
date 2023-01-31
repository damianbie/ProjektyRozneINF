@ECHO OFF

@avrdude -p m32 -c avr109 -b %3 -P %2 -U flash:w:%1.hex
IF ERRORLEVEL 1 GOTO EXIT

GOTO END

:EXIT
@ECHO.
@ECHO **************** ERROR(S) FOUND ****************

:END
@ECHO.
@ECHO ON
rem @pause

