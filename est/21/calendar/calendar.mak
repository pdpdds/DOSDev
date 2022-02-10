.AUTODEPEND

#		*Translator Definitions*
CC = bccx +CALENDAR.CFG
TASM = TASM
TLINK = tlinkx


#		*Implicit Rules*
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

#		*List Macros*


EXE_dependencies =  \
  ..\21lib\init_66.obj \
  l2s.obj \
  cal_main.obj

#		*Explicit Rules*
calendar.exe: calendar.cfg $(EXE_dependencies)
  $(TLINK) /x/c/P-/LD:\BC\LIB @&&|
c0l.obj+
..\21lib\init_66.obj+
l2s.obj+
cal_main.obj
calendar
		# no map file
emu.lib+
mathl.lib+
cl.lib
|


#		*Individual File Dependencies*
l2s.obj: l2s.c 

cal_main.obj: cal_main.c 

#		*Compiler Configuration File*
calendar.cfg: calendar.mak
  copy &&|
-ml
-C
-H=CALENDAR.SYM
-ID:\BC\INCLUDE
-LD:\BC\LIB
-P-.C
| calendar.cfg


