#
# Constantin Knizhnik
# Moscow Software Center
# BGI for Windows
#

DEFINES = 
CC = cl

# Debug
#CCFLAGS = -I. -c -Zi $(DEFINES)

# Optimize
CCFLAGS = -I. -c -Ox $(DEFINES)

LD = cl
LDFLAGS =

all: winbgi.lib bgidemo.exe

winbgi.obj: winbgi.cpp graphics.h
	$(CC) $(CCFLAGS) winbgi.cpp 

winbgi.lib: winbgi.obj
	lib /OUT:winbgi.lib winbgi.obj


bgidemo.obj: bgidemo.c graphics.h 
	$(CC) $(CCFLAGS) bgidemo.c 

bgidemo.exe: bgidemo.obj winbgi.lib
	$(LD) $(LDFLAGS) bgidemo.obj winbgi.lib user32.lib gdi32.lib 

clean:
	del *.obj,*.ilk,*.pdb,*.zip,*.*~,*~

zip: clean
	cd ..
	del winbgi.zip
	zip -r winbgi.zip winbgi
