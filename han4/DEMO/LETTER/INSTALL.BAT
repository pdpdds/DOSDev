@echo off
c:
cd\
copy b:letter.bat c:\
md letter
cd letter
copy b:.
del letter.bat
letter.exe

