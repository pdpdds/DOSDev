@bcc -c -ml -I..\..\include letter.c
@bcc -ml *.obj ..\..\lib\hanio.lib ..\..\lib\pcx.lib ..\..\lib\graphl.lib graphics.lib
@dir *.exe
