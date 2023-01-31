@ECHO OFF

@avrdude -p m32 -c avr109 -b 19200 -P com1 -U flash:w:test.hex
IF ERRORLEVEL 1 GOTO EXIT

GOTO END

:EXIT
@ECHO.
@ECHO **************** ERROR(S) FOUND ****************

:END
@ECHO.
@ECHO ON
@pause

