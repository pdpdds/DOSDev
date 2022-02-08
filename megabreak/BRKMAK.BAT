REM ** Makefile for MegaBreak **

del megbreak.obj
CALL wpp386 /d2 /3s megbreak.cpp
CALL wlink system dos4g option stack=8192 file megbreak name megbreak

