/*
 * New editor name:  TDE, the Thomson-Davis Editor.
 * Author:           Frank Davis
 * Date:             June 5, 1991
 *
 * This modification of Douglas Thomson's code is released into the
 * public domain, Frank Davis.  You may distribute it freely.
 *
 * This info is displayed in the intro screen and the pop-up help screen
 * when F1 is pressed.
 */

char *credit_screen[] = {
"旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커",
"�                                                    �",
"�            TDE, the Thomson-Davis Editor           �",
"�                    Version 3.0                     �",
"�                    June 5, 1993                    �",
"�                                                    �",
"�                                                    �",
"�      This program is released into the public      �",
"�   domain.  You may use and distribute it freely.   �",
"�                                                    �",
"읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸",
NULL
};


char *help_screen[] = {
"$  help",
"컴컴� Word Processing 컴컴컴컫 #-Shift   @-Alt   ^-Ctrl 쩡컴� File commands 컴컴",
"  @V  toggle word wrap       쳐컴컴 Block commands 컴컴캑@F1 dir list   @F2 attr",
"^F6/^F7  left/right margin   � @B  mark box block       � F2 save    #F2 save as",
"  ^F8  set paragraph margin  � @L  mark line block      � F3 quit     F4 file   ",
"  ^B   WordStar format pargr � @X  mark stream block    �#F4 edit F  @F4 next F ",
"  @F8  left justify line     � @U  unmark block         �#F12/^F12 define Grep  ",
"  @F9  right justify line    � @M  move    @C  copy     � F12 Repeat Grep       ",
" @F10  center justify line   � @K  kopy    @G  group del쳐컴컴컴 Toggles 컴컴컴�",
"                             � @F  fill    @O  overlay  � ^F1  tog cursor sync  ",
"컴컴� Window Commands 컴컴컴캑 @S  sort    @N  number   � ^F2  eol/tab display  ",
" F8 vertical     #F9 resize  �#@S  swap BOX w/cursor    � ^F3  CRLF | LF at eol ",
" F9 horizontal   ^F9 zoom    � @<  up case  @>  lo case � ^F4  tog trim trailing",
"F10 next        #F10 previous� @?  hi bit  #@?  e-mail  � ^F5  IGNORE/MATCH case",
"   ^F10  next hidden window  �#@<  rot13   #@>  fix uue �  @R  ruler   @I indent",
"컴컴컴� Misc commands 컴컴컴캑 @T  trim trailing space  �  @Z  tog ^Z at eof    ",
" @A  add L     ^2  Date stamp� @P  print file/block     � #@T  tog inflate tabs ",
" @J  join L    ^]  paren bal � @W  write block to file  �^tab  set ltabs/ptabs  ",
" @=  dup L     @-  kill eol  � @E  block expand tabs    �@tab  tog smart tabs   ",
" @D  delete L     @1-@3  mark�#@C  block compress tabs  쳐� Find/Replace/Diff 컴",
" ^_  split L    #@1-#@3  goto�#@I  block indent tabs    � #F5/#F6  define find  ",
"ESC  undo L      ^\\  redraw  쳐컴컴컴컴 Macros 컴컴컴컫켸 F5/F6 next/prev find  ",
" ^U  undelete L  ^T  word del�  @F3  record on/off    � #F7 define reg exp      ",
" Enter  Emacs L              �  #F1  save    #F3 load � F7/@F7 next/prev reg exp",
"#Enter  next L       @Q  Quit�   ^P  pause            � #F11/F11 def/rep diff   ",
"^Enter  beg next L   @H  Help� ^ESC  clear macros     � #F8 replace string      ",
NULL
};
