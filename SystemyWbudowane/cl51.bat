@ECHO OFF
@cd Temp
@set PATH=..\Keil4\bin
@REM --------------- Kompilacja ---------------
@set c51inc=..\keil4\inc;..\Include

REM dir

IF EXIST kod_SP.obj DEL kod_SP.obj

ECHO ****************** Kompilacja... *******************
ECHO.
@REM c51.exe %1 df(C8051) large pr db oe object(kod_SP.obj)
@REM c51.exe kod_SP.c df(C8051) large pr db oe object(kod_SP.obj)
@c51.exe %1 df(C8051) large pr db oe object(kod_SP.obj)
IF ERRORLEVEL 1 GOTO ERROR

@REM --------------- Konsolidacja ---------------
@set c51lib=..\keil4\lib

IF EXIST kod_SP DEL kod_SP
IF EXIST kod_SP.hex DEL kod_SP.hex

ECHO.
ECHO ****************** Konsolidacja... *****************
@l51.exe @..\link.txt
IF ERRORLEVEL 1 GOTO ERROR

ECHO.
ECHO ******************* Konwersja... *******************
ECHO.
@oh51.exe kod_SP
IF ERRORLEVEL 1 GOTO ERROR
GOTO OK

:ERROR
ECHO.
ECHO ************** B£ÊDY LUB OSTRZE¯ENIA ***************
GOTO END

:OK
@ECHO.
@ECHO ***** Kompilacja, konsolidacja i konwersja zakoñczone pomyœlnie *****
:END
@ECHO.
@ECHO ON
pause