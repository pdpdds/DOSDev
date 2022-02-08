echo off
echo Insert HAN 4.1 disk in drive A.
pause
echo on

del *.bak
del *.dsk
cd a:\source

copy inithan.c a:
copy _inithan.c a:
copy puteng.c a:
copy aputeng.c a:
copy vputeng.c a:
copy puthan.c a:
copy aputhan.c a:
copy vputhan.c a:
copy putchhan.c a:
copy xputch.c a:
copy aputch.c a:
copy vputch.c a:
copy putshan.c a:
copy cputshan.c a:
copy xputs.c a:
copy nputs.c a:
copy aputs.c a:
copy vputs.c a:
copy naputs.c a:
copy nvputs.c a:
copy vaputs.c a:
copy vbputs.c a:
copy eputs.c a:
copy advputs.c a:
copy cntrputs.c a:
copy printf.c a:
copy xprintf.c a:
copy nprintf.c a:
copy aprintf.c a:
copy vprintf.c a:
copy naprintf.c a:
copy nvprintf.c a:
copy vaprintf.c a:
copy vbprintf.c a:
copy eprintf.c a:
copy getch23.c a:
copy getchhan.c a:
copy getkey.c a:
copy getyesno.c a:
copy waitkey.c a:
copy getshan.c a:
copy advgets.c a:
copy getnumbr.c a:
copy scanf.c a:
copy window.c a:
copy clear.c a:
copy aclear.c a:
copy vclear.c a:
copy clrscr.c a:
copy aclrscr.c a:
copy vclrscr.c a:
copy rndclear.c a:
copy savetext.c a:
copy moditext.c a:
copy drawbox.c a:
copy bitbox.c a:
copy varbox.c a:
copy gotoxy.c a:
copy getcordi.c a:
copy attrcolr.c a:
copy mode.c a:
copy default.c a:
copy setchkey.c a:
copy status.c a:
copy getbuff.c a:
copy keyflag.c a:
copy hanbase.c a:
copy fontmng.c a:
copy setpath.c a:
copy xscrolup.c a:
copy xscroldn.c a:
copy offset.c a:
copy maxstrln.c a:
copy korchi.c a:
copy chhanja.c a:
copy dirbox1.c a:
copy dirbox2.c a:
copy dirbox3.c a:
copy setgr.c a:
copy actpage.c a:
copy choosegr.c a:
copy cursor.c a:
copy settwink.c a:
copy vertline.c a:
copy horiline.c a:
copy setchf.c a:
copy setwaitf.c a:
copy keybuff.c a:
copy play.c a:
copy strtrim.c a:
copy isnumber.c a:
copy whatbyte.c a:
copy arrngstr.c a:
copy cnvthan.c a:
copy menu.c a:
copy popup.c a:
copy pulldown.c a:
copy error.c a:
copy variable.c a:
copy get2font.c a:

copy hanio.lib a:\lib

copy hanio.h a:\include
copy extendio.h a:\include
copy misc.h a:\include
copy keydef.h a:\include
copy extern.h a:\include
copy sorted.h a:\include

copy *.bat a:\

cd a:\

echo off
echo  Successfully backed up..
echo on

