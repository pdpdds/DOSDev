rem bcc -c -ml PFONT0.C  PGRKER0.C  PSPRKER0.C  SF186.C  SF2ADL.C  SF2SND.C timer.asm
bcc -ml *.obj ladlib.lib

@copy PFONT0.exe sf186.exe
@del PFONT0.exe
@dir *.exe