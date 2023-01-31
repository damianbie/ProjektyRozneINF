@echo off
set PATH=C:\MinGW\bin;%PATH%
echo Kompilator GCC. Budowanie...
gcc -O0 -g3 -Wall -c -fmessage-length=0 -o outcode.o %1 -I./Include
gcc -shared -g -o %2 outcode.o outcode.def
