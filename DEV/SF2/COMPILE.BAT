bcc -Ic:/bc31/include -Lc:/bc31/lib -ml -c -B -f sf186.c psprker0.c pgrker0.c pfont0.c sf2snd.c
tlink c:\bc31\lib\c0l+sf186+psprker0+pgrker0+pfont0+sf2snd+sbl2,%1,,c:\bc31\lib\cl c:\bc31\lib\emu c:\bc31\lib\mathc ladlib,,
