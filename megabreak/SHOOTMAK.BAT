REM ** Makefile for MegaBreak **

del shooter.obj
CALL wpp386 /d2 /3s shooter.cpp
CALL wlink system dos4g option stack=8192 file shooter name shooter

