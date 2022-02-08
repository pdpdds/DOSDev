
GAME_PORT       EQU     201H
BUT_A           EQU     10H
BUT_B           EQU     20H
BUT_UP          EQU     08H
BUT_DN          EQU     04H
BUT_RT          EQU     01H
BUT_LF          EQU     02H
BIT0            EQU     01H
BIT1            EQU     02H
BIT2            EQU     04H
BIT3            EQU     08H
BIT4            EQU     10H
BIT5            EQU     20H
BIT6            EQU     40H
BIT7            EQU     80H
BUF_SIZE        EQU     10H
Y_START         EQU     0CH
REP_COUNT       EQU     04H
CLR_COUNT       EQU     03H             ;clear joy buffer counter
SLEEP_CNT       EQU     44H
BACK_RATE       EQU     -8H             ;hit back value
BACK_REP        EQU     06H             ;hit back times
BACK_REF        EQU     10H             ;hit back reference distance
DELAY_RATE      EQU     07H             ;delay rate for timer
Y_OFFDOWN       EQU     10H
AI_NEAR         EQU     72
AI_FAR          EQU     150

INCLUDE BIOS.H
INCLUDE DOS.H
INCLUDE VGA.H
INCLUDE IO.H


        .MODEL  LARGE
        .DATA

        include variable.inc

        EXTRN   palette:BYTE,flength:WORD,alloc1:WORD,alloc0:WORD,alloc3:WORD
        EXTRN   j1_adelay_lo:WORD,j1_adelay_hi:WORD
        EXTRN   j1_bdelay_lo:WORD,j1_bdelay_hi:WORD
        EXTRN   j2_adelay_lo:WORD,j2_adelay_hi:WORD
        EXTRN   j2_bdelay_lo:WORD,j2_bdelay_hi:WORD
        EXTRN   p1_man:WORD,p2_man:WORD,counter:WORD,c_level:WORD
        EXTRN   score_lo:WORD,score_hi:WORD,main_select:WORD
        PUBLIC  p1_pannel,p2_pannel,p1_win,p2_win,RAN56,RAN57

;  couldn't reset variable area
round           dw      0
round_flag      dw      0
timecnt         dw      0
p1_win          dw      0               ;win counter
p2_win          dw      0
who_win         dw      0
delaycnt        dw      1                       ;system delay counter
;
fptr            dw      ?                       ;file pointer
prg_psp         dw      ?                       ;program's psp
p1_ptr          dw      ?
p2_ptr          dw      ?
page_addr       dw      0000h                   ;fill page start address
scan_addr       dw      0000h                   ;scan start address
scr_x           dw      0050h                   ;screen x position
scr_y           db      Y_START                 ;screen y position
priority        db      0                       ;priority p1 or p2
ytouch          dw      0                       ;y level touch var.
scene_no        dw      0
p1_y            dw      0000h                   ;fighter 1 y position
p2_y            dw      0000h                   ;fighter 2 y position
p1_x            dw      0050h                   ;fighter 1 x position
p2_x            dw      0050h                   ;fighter 2 x position
p1_move         dw      ?                       ;move register
p2_move         dw      ?
sdbuf           db      600 dup(?)              ;shadow buffer
; variables for random
RAN56           db      ?
RAN57           db      ?
RAN58           db      ?
random          db      ?
; variables for AI
p1_aiptr        dw      0
p2_aiptr        dw      0
p1_aitype       db      0
p2_aitype       db      0
p1_aival        db      0
p2_aival        db      0
p1_aicnt        db      0
p2_aicnt        db      0
pathattr        LABEL   BYTE
                db      00,01,00,00,02,02,02,00,01,01,00,00,04,04,00,03
                db      04,04,04,04,04,04,04,04,04,04,06,05,07,06,05,07
                db      07,07
specpath1       db      6 dup(0)
specpath2       db      6 dup(0)
aitable         db      192 dup(0)
; blood control
p1_die          db      0       ; 1 = p1 will be die 80h = die
p2_die          db      0
p1_blood        dw      0
p2_blood        dw      0
p1_losenow      dw      0
p2_losenow      dw      0
; sleep check variable
p1_sleep        dw      0
p2_sleep        dw      0
p1_sleepcnt     dw      0
p2_sleepcnt     dw      0
; sprite control variables
p1_num          db      0
p2_num          db      0
p1_atkatt       db      0                       ;attack attribute
p2_atkatt       db      0
p1_atkatk       db      0                       ;attack attribute
p2_atkatk       db      0
p1_sprbak       db      0
p2_sprbak       db      0
p1_direct       db      0                       ;point p1 & p2 direct
p2_direct       db      0ffh
p1_levelx       db      0
p2_levelx       db      0
p1_forcenum     db      0
p1_forceno      db      0
p1_forcecnt     db      0
p2_forcenum     db      0
p2_forceno      db      0
p2_forcecnt     db      0                       ;force change path
lf_move         dw      0
lf_txtx         dw      0
lf_width        dw      0
lf_px           dw      0
lf_x            dw      0
rt_move         dw      0
rt_txtx         dw      0
rt_width        dw      0
rt_px           dw      0
rt_x            dw      0
lf_pretx        dw      0
rt_pretx        dw      0
p1_txtx         dw      0
p2_txtx         dw      0
p1_pretx        dw      0
p2_pretx        dw      0
p1_txty         dw      0
p2_txty         dw      0
p1_subx         dw      0                       ;sub x axies for p1
p2_subx         dw      0
p1_width        db      0
p2_width        db      0
p1_length       db      0
p2_length       db      0
p1_pthno        db      0                       ;player 1 path number
p2_pthno        db      0                       ;       2
p1_pthcnt       db      0
p2_pthcnt       db      0
p1_type         db      0
p1_px           db      0
p1_py           db      0
p1_stop         db      0
p1_sprite       db      0
p1_vecdelay     db      0
p1_xvec         db      0
p1_yvec         db      0
p2_type         db      0
p2_px           db      0
p2_py           db      0
p2_stop         db      0
p2_sprite       db      0
p2_vecdelay     db      0
p2_xvec         db      0
p2_yvec         db      0
p1_attr         db      0
p2_attr         db      0
p1_veccnt       db      0                       ;vector counter p1
p2_veccnt       db      0                       ;   ....        p2
p1_hitnum       db      0
p2_hitnum       db      0
p1_hitflg       db      0
p2_hitflg       db      0
p1_ptrbuf       db      72*4 dup(0)             ;picture pointer for p1
p2_ptrbuf       db      72*4 dup(0)             ;   ....             p2
p1_pth          db      1536 dup(0)             ;p1 path
p2_pth          db      1536 dup(0)             ;p2 path
p1_atrbuf       db      1224 dup(0)
p2_atrbuf       db      1224 dup(0)
; variables for air attack
p1_airtxtx      dw      0
p1_airtxty      dw      0
p1_airwid       dw      0
p1_airwid2      dw      0
p1_aircnt       db      0
p1_airveccnt    db      0
p1_airsubx      db      0
p1_airsuby      db      0
p1_airattr      db      0
p1_airflag      db      0
p1_airdir       db      0
p1_airtype      db      0
p1_airx         db      0
p1_airy         db      0
p1_airstop      db      0
p1_airsprite    db      0
p1_airdelay     db      0
p1_airvecx      db      0
p1_airvecy      db      0
p2_airtxtx      dw      0
p2_airtxty      dw      0
p2_airwid       dw      0
p2_airwid2      dw      0
p2_aircnt       db      0
p2_airveccnt    db      0
p2_airsubx      db      0
p2_airsuby      db      0
p2_airattr      db      0
p2_airflag      db      0
p2_airdir       db      0
p2_airtype      db      0
p2_airx         db      0
p2_airy         db      0
p2_airstop      db      0
p2_airsprite    db      0
p2_airdelay     db      0
p2_airvecx      db      0
p2_airvecy      db      0
; variables for sub picture
p1_pictype      db      0
p1_picx         db      0
p1_picy         db      0
p1_picattr      db      0
p1_picno        db      0
p1_picwid       db      0
p2_pictype      db      0
p2_picx         db      0
p2_picy         db      0
p2_picattr      db      0
p2_picno        db      0
p2_picwid       db      0
; variables for hit back
p1_hitback      db      0
p2_hitback      db      0
; variables for game counter
second_ptr      label   word
                dw      0,210,420,630,840,1050,1260,1470,1680,1890
second          dw      0
second_buf      db      2100    dup(0)          ;14(W)*15(H)
; variables for star cycle
star_ptr        label   word
                dw      2,496,990,1281,1572
star_buf        db      1861    dup(0)          ;star and light buffer
;
p1_light        dw      0
p1_lightx       dw      0
p1_lighty       dw      0
p2_light        dw      0
p2_lightx       dw      0
p2_lighty       dw      0
; data for joystick or keyboard
p1_pannel          db      04h                     ;bit0(1) keyboard 1(2)
p2_pannel          db      01h                     ;bit2(3) joystick 1(2)
now_key         db      ?
joyvalue        db      ?
j1_joy          db      ?
j2_joy          db      ?
j1_trg          db      ?
j2_trg          db      ?
j1_last         db      ?
j2_last         db      ?
j1_jbptr        dw      0
j2_jbptr        dw      0
j1_tick1        dw      0
j1_tick2        dw      0
j2_tick1        dw      0
j2_tick2        dw      0
j1_joybuf       label   byte
                db      16 dup(0)
j2_joybuf       label   byte
                db      16 dup(0)
joy_count_x     dw      0
joy_count_y     dw      0
; variables for keyboard
escape          db      0
key1last        db      0
key2last        db      0
key1val         db      0
key2val         db      0
keybuf          label   byte
                db      32 dup(0)
key1table       label   byte
                db      1eh,2ch,2dh,2eh,20h,12h,11h,10h
key2table       label   byte
                db      4bh,4fh,50h,51h,4dh,49h,48h,47h
arrortable      label   byte
                db      02h,06h,04h,05h,01h,09h,08h,0ah
; data for error message
string1         db      'Out of memory',0dh,0ah,'$'
string2         db      'Cannot open file',0dh,0ah,'$'
string3         db      'still ok !$'
; data for control datas
atthandnum      LABEL   BYTE
                db      27,26,27,27,28,28,28,27,26
attlegnum       LABEL   BYTE
                db      30,29,30,30,31,32,33,30,29
forcenum        LABEL   BYTE
                db      0,0,0,0,1,1,1,0,0
nextpath        LABEL   BYTE
                db      00,00,00,00,00,00,00,00,00,00,00,00,14,00,00,00
                db      12,14,14,14,14,12,14,13,12,14,00,00,00,00,00,00
                db      00,00,00,00,00,00,00,00
; data for file name
bdfile          db      'board.xbb',00h
sdfile          db      'shadow.xlp',00h
digfile         db      '0-9.xbb',00h
starfile        db      'star.xbb',00h
man2col         db      'man.col',00h
bdcol           db      'board.col',00h
mufile          db      'a0.mid',00h
cfgfile         db      'choose.cfg',00h
namefile        db      'name.xbb',00h
ainame          db      'aidata.dat',00h
; data for man
scenename       LABEL   WORD
                dw      offset scenefile1
                dw      offset scenefile2
                dw      offset scenefile3
                dw      offset scenefile4
                dw      offset scenefile5
                dw      offset scenefile6
                dw      offset scenefile7
                dw      offset scenefile8
                dw      offset scenefile9
                dw      offset scenefile10
                dw      offset scenefile11

figname         LABEL   WORD
                dw      offset manfile1
                dw      offset manfile2
                dw      offset manfile3
                dw      offset manfile4
                dw      offset manfile5
                dw      offset manfile6
                dw      offset manfile7
                dw      offset manfile8
                dw      offset manfile9
                dw      offset manfile10
                dw      offset manfile11

pathname        LABEL   WORD
                dw      offset manpath1
                dw      offset manpath2
                dw      offset manpath3
                dw      offset manpath4
                dw      offset manpath5
                dw      offset manpath6
                dw      offset manpath7
                dw      offset manpath8
                dw      offset manpath9
                dw      offset manpath10
                dw      offset manpath11

attrname        LABEL   WORD
                dw      offset manattr1
                dw      offset manattr2
                dw      offset manattr3
                dw      offset manattr4
                dw      offset manattr5
                dw      offset manattr6
                dw      offset manattr7
                dw      offset manattr8
                dw      offset manattr9
                dw      offset manattr10
                dw      offset manattr11

winname         LABEL   WORD
                dw      offset winfile1
                dw      offset winfile2
                dw      offset winfile3
                dw      offset winfile4
                dw      offset winfile5
                dw      offset winfile6
                dw      offset winfile7
                dw      offset winfile8
                dw      offset winfile9
                dw      offset winfile10
                dw      offset winfile11

winpath         LABEL   WORD
                dw      offset winpath1
                dw      offset winpath2
                dw      offset winpath3
                dw      offset winpath4
                dw      offset winpath5
                dw      offset winpath6
                dw      offset winpath7
                dw      offset winpath8
                dw      offset winpath9
                dw      offset winpath10
                dw      offset winpath11

scenefile1      db      'i5.xgb',00h
scenefile2      db      'i12.xgb',00h
scenefile3      db      'i4.xgb',00h
scenefile4      db      'i7.xgb',00h
scenefile5      db      'i2.xgb',00h
scenefile6      db      'i3.xgb',00h
scenefile7      db      'i10.xgb',00h
scenefile8      db      'i9.xgb',00h
scenefile9      db      'i8.xgb',00h
scenefile10     db      'i1.xgb',00h
scenefile11     db      'i13.xgb',00h

manfile1        db      'aa.fig',00h
manfile2        db      'bb.fig',00h
manfile3        db      'cc.fig',00h
manfile4        db      'ee.fig',00h
manfile5        db      'ff.fig',00h
manfile6        db      'gg.fig',00h
manfile7        db      'hh.fig',00h
manfile8        db      'ii.fig',00h
manfile9        db      'jj.fig',00h
manfile10       db      'dd.fig',00h
manfile11       db      'kk.fig',00h

manpath1        db      'aa.pah',00h
manpath2        db      'bb.pah',00h
manpath3        db      'cc.pah',00h
manpath4        db      'ee.pah',00h
manpath5        db      'ff.pah',00h
manpath6        db      'gg.pah',00h
manpath7        db      'hh.pah',00h
manpath8        db      'ii.pah',00h
manpath9        db      'jj.pah',00h
manpath10       db      'dd.pah',00h
manpath11       db      'kk.pah',00h

manattr1        db      'aa.pra',00h
manattr2        db      'bb.pra',00h
manattr3        db      'cc.pra',00h
manattr4        db      'ee.pra',00h
manattr5        db      'ff.pra',00h
manattr6        db      'gg.pra',00h
manattr7        db      'hh.pra',00h
manattr8        db      'ii.pra',00h
manattr9        db      'jj.pra',00h
manattr10       db      'dd.pra',00h
manattr11       db      'kk.pra',00h

winfile1        db      'aawin.fig',00h
winfile2        db      'bbwin.fig',00h
winfile3        db      'ccwin.fig',00h
winfile4        db      'eewin.fig',00h
winfile5        db      'ffwin.fig',00h
winfile6        db      'ggwin.fig',00h
winfile7        db      'hhwin.fig',00h
winfile8        db      'iiwin.fig',00h
winfile9        db      'jjwin.fig',00h
winfile10       db      'ddwin.fig',00h
winfile11       db      'kkwin.fig',00h

winpath1        db      'aawin.pah',00h
winpath2        db      'bbwin.pah',00h
winpath3        db      'ccwin.pah',00h
winpath4        db      'eewin.pah',00h
winpath5        db      'ffwin.pah',00h
winpath6        db      'ggwin.pah',00h
winpath7        db      'hhwin.pah',00h
winpath8        db      'iiwin.pah',00h
winpath9        db      'jjwin.pah',00h
winpath10       db      'ddwin.pah',00h
winpath11       db      'kkwin.pah',00h

        ASSUME  CS:_TEXT
_TEXT   SEGMENT WORD PUBLIC     'CODE'
        PUBLIC  fight_main,random_gen
        EXTRN   pw:NEAR,fade:NEAR,decompress:NEAR,mkfade:NEAR
        EXTRN   delay:NEAR,R_file:FAR,fadeinit:NEAR
        EXTRN   _midi_stop:FAR,load_music:NEAR,play_music:NEAR
;  ==============================
;  =  INITIAL VGA               =
;  ==============================
initial_vga     PROC    NEAR
        OUTPB   3ceh,06h        ;
        OUTPB   3cfh,01h        ;set 128k page
        OUTPB   3d4h,12h        ;
        OUTPB   3d5h,7Fh        ;set scan line 383
        ret
initial_vga     ENDP
;
reset_vga       PROC    NEAR
        OUTPB   3ceh,06h        ;
        OUTPB   3cfh,01h        ;set 128k page
        OUTPB   3d4h,12h        ;
        OUTPB   3d5h,8Fh        ;set scan line 383
        SETADR  0
        ret
reset_vga       ENDP
;  ==============================
;  =  INITIAL VARIABLE          =
;  ==============================
initial_var     PROC    NEAR
        xor     ax,ax
        mov     es,ax
        mov     ax,es:[046ch]
        mov     RAN56,al
        mov     RAN57,ah
        xor     ax,ax
        mov     p1_pthno,al
        mov     p2_pthno,al
        mov     p1_pthcnt,al
        mov     p2_pthcnt,al
        call    pth_control
        mov     ax,320
        add     ax,scr_x
        sub     al,p2_px
        sbb     ah,0
        sub     al,p2_px
        sbb     ah,0
        mov     p2_subx,ax
        sub     al,p2_width
        sbb     ah,0
        mov     p2_x,ax
        mov     al,p1_px
        mov     p1_levelx,al
        mov     al,p2_px
        mov     p2_levelx,al
        call    set_specatr
        ret
;
set_specatr:
        mov     bx,offset p1_atrbuf
        xor     ax,ax
        mov     cx,6
setp1spec:
        mov     al,[bx]
        test    al,0ffh
        jz      setp1specend
        sub     al,34
        mov     si,ax
        mov     al,[bx+1]
        and     al,0fh
        mov     offset specpath1[si],al
        add     bx,12
        loop    setp1spec
setp1specend:
        mov     bx,offset p2_atrbuf
        xor     ax,ax
        mov     cx,6
setp2spec:
        mov     al,[bx]
        test    al,0ffh
        jz      setp2specend
        sub     al,34
        mov     si,ax
        mov     al,[bx+1]
        and     al,0fh
        mov     offset specpath2[si],al
        add     bx,12
        loop    setp2spec
setp2specend:
        ret
;
initial_var     ENDP
;
sprite_info1    PROC    NEAR
        xor     bx,bx
        mov     bl,p1_sprite
        shl     bx,1
        shl     bx,1
        mov     si,[bx+2+offset p1_ptrbuf]
        push    ds
        mov     ax,[bx+offset p1_ptrbuf]
        add     ax,p1_ptr
        mov     ds,ax
        lodsw
        pop     ds
        mov     p1_width,al
        mov     p1_length,ah
        ret
sprite_info1    ENDP
;
sprite_info2    PROC    NEAR
        xor     bx,bx
        mov     bl,p2_sprite
        shl     bx,1
        shl     bx,1
        mov     si,[bx+2+offset p2_ptrbuf]
        push    ds
        mov     ax,[bx+offset p2_ptrbuf]
        add     ax,p2_ptr
        mov     ds,ax
        lodsw
        pop     ds
        mov     p2_width,al
        mov     p2_length,ah
        ret
sprite_info2    ENDP
;
; initial local var
initial_local_var       PROC    NEAR
        mov     ax,0
        mov     p1_y,ax
        mov     p2_y,ax
        mov     p1_die,al
        mov     p2_die,al
        mov     p1_sleep,ax
        mov     p1_sleepcnt,ax
        mov     p2_sleep,ax
        mov     p2_sleepcnt,ax
        mov     p1_stop,al
        mov     p2_stop,al
        mov     j1_joy,al
        mov     j2_joy,al
        mov     j1_trg,al
        mov     j2_trg,al
        mov     j1_last,al
        mov     j2_last,al
        mov     p1_airflag,al
        mov     p2_airflag,al
        mov     escape,0
        mov     p1_direct,0
        mov     p2_direct,0ffh
        mov     ax,7ch
        mov     p1_blood,ax
        mov     p2_blood,ax
        mov     scr_x,50h
        mov     second,100
        mov     counter,DELAY_RATE
        mov     p1_x,50h
        ret
initial_local_var       ENDP
;  ==============================
;  =  display load              =
;  ==============================
show_load       PROC    NEAR
        mov     al,1
        call    mkfade
        mov     si,11
        mov     es,alloc0
        call    far ptr R_file
        mov     ax,alloc1
        push    ds
        push    ax
        xor     ax,ax
        mov     si,ax
        push    ax
        mov     di,offset palette
        mov     dx,flength
        mov     ds,alloc0
        call    decompress
        pop     ax
        pop     ax
        pop     ds
        call    fadeinit
        call    f_scr
        call    f_color
        ret
show_load       ENDP
;  ==============================
;  =  RANDOM GENERATOR          =
;  ==============================
random_gen      PROC    NEAR
        xor     ax,ax
        mov     es,ax
        mov     al,es:[046ch]
        mov     ah,al
        shr     ah,1
        rcr     al,1
        rcr     al,1
        rcr     al,1
        rcr     al,1
        xor     al,RAN57
        mov     ah,al
        shr     ah,1
        rcr     al,1
        rcr     al,1
        xor     al,RAN57
        mov     ah,al
        shr     ah,1
        rcr     al,1
        xor     al,RAN56
        rcr     al,1
        jnc     ra10
        clc
        jnc     ra20
ra10:   stc
ra20:   rcl     RAN56,1
        rcl     RAN57,1
        rcl     RAN58,1
        mov     random,al
        ret
random_gen      ENDP
;  ==============================
;  =  check delay               =
;  ==============================
chkdelay        PROC    NEAR
        xor     ax,ax
        mov     es,ax
        mov     r0,ax
        mov     r1,ax
cdy10:  mov     ax,es:[046ch]
        cmp     ax,es:[046ch]
        jz      cdy10
        mov     ax,es:[046ch]
cdy20:  add     r0,1
        adc     r1,0
        cmp     ax,es:[046ch]
        jz      cdy20
        mov     delaycnt,1
        cmp     r1,0
        ja      cdy30
        cmp     r0,0c000h
        jna     cdy90
cdy30:  mov     ax,r1
        mov     bx,0800h
        mul     bx
        add     delaycnt,ax
        add     delaycnt,6000h
cdy90:
        ret
chkdelay        ENDP
;  ==============================
;  =  TEST JOYSTICK             =
;  ==============================
test_joy        PROC    NEAR
testjoy1:
        INB     GAME_PORT,al
        and     al,0fh
        jnz     testjoy1
        xor     ax,ax
        mov     j1_adelay_hi,ax
        mov     j1_bdelay_hi,ax
        OUTPB   GAME_PORT,03h
tjy00:
        INB     GAME_PORT,ah
        test    ah,BIT0
        jz      tjy10
        inc     j1_adelay_hi
tjy10:
        test    ah,BIT1
        jz      tjy20
        inc     j1_bdelay_hi
tjy20:  test    ah,03h
        jnz     tjy00
        mov     ax,j1_adelay_hi
        shr     ax,1
        mov     j1_adelay_lo,ax         ;set delay time lo
        mov     ax,j1_adelay_hi
        add     ax,j1_adelay_lo
        mov     j1_adelay_hi,ax         ;set delay time hi
        mov     ax,j1_bdelay_hi
        shr     ax,1
        mov     j1_bdelay_lo,ax         ;set delay time lo
        mov     ax,j1_bdelay_hi
        add     ax,j1_bdelay_lo
        mov     j1_bdelay_hi,ax         ;set delay time hi
        ret
test_joy        ENDP
;
;  ==============================
;  =  READ JOYSTICK             =
;  ==============================
read_joy        PROC    NEAR
readjoy1:
        INB     GAME_PORT,al
        and     al,0fh
        jnz     readjoy1
        xor     ax,ax
        mov     joy_count_x,ax
        mov     joy_count_y,ax
        mov     joyvalue,al
        OUTPB   GAME_PORT,03h
rjy00:
        INB     GAME_PORT,ah
        test    ah,BIT0
        jz      rjy10
        inc     joy_count_x
rjy10:
        test    ah,BIT1
        jz      rjy20
        inc     joy_count_y
rjy20:  test    ah,03h
        jnz     rjy00
;
        INB     GAME_PORT,joyvalue
        and     joyvalue,30h
        xor     joyvalue,30h
        mov     ax,joy_count_x
        cmp     ax,j1_adelay_lo
        ja      rjy30
        or      joyvalue,02h
rjy30:  cmp     ax,j1_adelay_hi
        jc      rjy40
        or      joyvalue,01h
rjy40:  mov     ax,joy_count_y
        cmp     ax,j1_bdelay_lo
        ja      rjy50
        or      joyvalue,08h
rjy50:  cmp     ax,j1_bdelay_hi
        jc      rjy60
        or      joyvalue,04h
rjy60:
        ret
read_joy        ENDP
;
read_joy2       PROC    NEAR
readjoy2:
        INB     GAME_PORT,al
        and     al,0fh
        jnz     readjoy2
        xor     ax,ax
        mov     joy_count_x,ax
        mov     joy_count_y,ax
        mov     joyvalue,al
        OUTPB   GAME_PORT,0ch
rjy200:
        INB     GAME_PORT,al
        test    al,BIT2
        jz      rjy210
        inc     joy_count_x
rjy210:
        test    al,BIT3
        jz      rjy220
        inc     joy_count_y
rjy220: test    al,0ch
        jnz     rjy200
;
        INB     GAME_PORT,joyvalue
        and     joyvalue,0c0h
        xor     joyvalue,0c0h
        shr     joyvalue,1
        shr     joyvalue,1
        mov     ax,joy_count_x
        cmp     ax,j2_adelay_lo
        ja      rjy230
        or      joyvalue,02h
rjy230: cmp     ax,j2_adelay_hi
        jc      rjy240
        or      joyvalue,01h
rjy240: mov     ax,joy_count_y
        cmp     ax,j2_bdelay_lo
        ja      rjy250
        or      joyvalue,08h
rjy250: cmp     ax,j2_bdelay_hi
        jc      rjy260
        or      joyvalue,04h
rjy260:
        ret
read_joy2       ENDP
;  ==============================
;  =  READ KEYBOARD             =
;  ==============================
read_key        PROC    NEAR
        mov     al,key1val
        mov     joyvalue,al
        ret
read_key        ENDP
;
read_key2       PROC    NEAR
        mov     al,key2val
        mov     joyvalue,al
        ret
read_key2       ENDP
;  ==============================
;  =  READ CONTROL              =
;  ==============================
read_ctl        PROC    NEAR
        mov     al,p1_pannel
        cmp     al,04
        jnz     rcl05
        mov     al,RAN56
        mov     RAN58,al
        call    com_ai_p1
        jmp     rcl09
rcl05:
        mov     ah,j1_joy
        call    chk_ctl
rcl09:
        mov     al,joyvalue
        mov     j1_joy,al
        call    buf_joy_p1
        mov     al,j1_joy
        mov     ah,al
        xor     al,j1_last
        and     al,ah
        mov     j1_trg,al               ;set triggle value
        mov     j1_last,ah
rcl10:
        mov     al,p2_pannel
        cmp     al,04
        jnz     rcl15
        mov     al,RAN57
        mov     RAN58,al
        call    com_ai_p2
        jmp     rcl19
rcl15:
        mov     ah,j2_joy
        call    chk_ctl
rcl19:
        mov     al,joyvalue
        mov     j2_joy,al
        call    buf_joy_p2
        mov     al,j2_joy
        mov     ah,al
        xor     al,j2_last
        and     al,ah
        mov     j2_trg,al
        mov     j2_last,ah
rcl20:
        cmp     main_select,4
        jnz     rcl40
        mov     ax,03h
        int     33h
        test    bx,7
        jz      rcl40
        mov     escape,80h
rcl40:
        ret
read_ctl        ENDP
;
chk_ctl         PROC    NEAR
        cmp     al,02h
        jnz     ccl10
        call    read_key
        jmp     ccl90
ccl10:
        cmp     al,03h
        jnz     ccl20
        call    read_key2
        jmp     ccl90
ccl20:
        cmp     al,00h
        jnz     ccl30
        call    read_joy
        jmp     ccl90
ccl30:
        cmp     al,01h
        jnz     ccl90
        call    read_joy2
ccl90:
        ret
chk_ctl         ENDP
;  =================================
;  =  COMPUTER AI                  =
;  =================================
com_ai_p1       PROC    NEAR
        mov     ax,p1_x
        sub     ax,p2_x
        cwd
        xor     ax,dx
        sub     ax,dx
        cmp     ax,AI_NEAR              ;near
        ja      p1aichkx00
        mov     bl,0
        jmp     p1aichkxover
p1aichkx00:
        cmp     ax,AI_FAR               ;far
        ja      p1aichkx10
        mov     bl,1
        jmp     p1aichkxover
p1aichkx10:
        mov     bl,2
p1aichkxover:
        mov     bh,bl
        test    p1_aitype,0ffh
        jz      p1ai05
        test    p1_aicnt,0ffh
        jnz     p1ai00
        mov     si,p1_aiptr
        mov     al,[offset p1_atrbuf+712+si]
        test    al,0ffh
        jz      p1ai05
        mov     p1_aitype,al
        and     al,0fh
        mov     p1_aicnt,al
        mov     al,[offset p1_atrbuf+713+si]
        mov     p1_aival,al
        add     p1_aiptr,2
p1ai00:
        mov     al,p1_aival
        mov     joyvalue,al
        dec     p1_aicnt
        jmp     p1ai10
p1ai05:
        test    p1_type,06h
        jz      p1ai10
        call    p1aichkp2
p1ai10:
        test    p1_aitype,BIT4
        jz      p1ai20
        test    p2_airflag,0ffh
        jnz     p1ai18
        mov     al,p2_pthno
        test    p2_forcenum,0ffh
        jz      p1ai15
        mov     al,p2_forcenum
p1ai15:
        cmp     al,26
        jc      p1ai20
p1ai18: mov     al,p1_aival
        and     al,0fch
        or      al,BUT_LF
        mov     joyvalue,al
        jmp     p1ai30
p1ai20:
        mov     al,p1_aitype
        and     al,60h
        jz      p1ai30
        test    bh,0ffh
        jnz     p1ai30
        cmp     p2_pthno,15
        jz      p1ai25
        mov     bl,p1_direct
        cmp     bl,p2_direct
        jz      p1ai30
p1ai25: shr     al,1
        or      joyvalue,al
p1ai30:
        test    p1_direct,0ffh
        jz      p1ai99
        mov     al,joyvalue
        mov     ah,al
        and     al,BUT_RT
        and     ah,BUT_LF
        and     joyvalue,0fch
        shr     ah,1
        shl     al,1
        or      al,ah
        or      joyvalue,al
p1ai99:
        ret
;
p1aichkp2:
;  calc distance
        xor     ax,ax
        mov     al,p2_pthno
        test    p2_forcenum,0ffh
        jz      p1aip200
        mov     al,p2_forcenum
p1aip200:
        cmp     ax,33
        jna     p1aip210
        add     ax,6
p1aip210:
        mov     si,ax
        mov     al,pathattr[si]
        mov     ah,12
        mul     ah
        mov     cl,RAN58
        and     cl,03h
        shl     bl,1
        shl     bl,1
        add     cl,bl
        xor     ch,ch
        add     ax,cx
        mov     si,ax
        mov     al,[offset aitable+si]
        xor     ah,ah
        mov     cl,4
        shl     ax,cl
        mov     p1_aiptr,ax
        mov     si,ax
        mov     al,[offset p1_atrbuf+712+si]
        mov     p1_aitype,al
        and     al,0fh
        mov     p1_aicnt,al
        mov     al,[offset p1_atrbuf+713+si]
        mov     p1_aival,al
        add     p1_aiptr,2
p1aipth99:
        mov     joyvalue,al
        ret
com_ai_p1       ENDP
;
com_ai_p2       PROC    NEAR
        mov     ax,p1_x
        sub     ax,p2_x
        cwd
        xor     ax,dx
        sub     ax,dx
        cmp     ax,AI_NEAR
        ja      p2aichkx00
        mov     bl,0
        jmp     p2aichkxover
p2aichkx00:
        cmp     ax,AI_FAR
        ja      p2aichkx10
        mov     bl,1
        jmp     p2aichkxover
p2aichkx10:
        mov     bl,2
p2aichkxover:
        mov     bh,bl
        test    p2_aitype,0ffh
        jz      p2ai05
        test    p2_aicnt,0ffh
        jnz     p2ai00
        mov     si,p2_aiptr
        mov     al,[offset p2_atrbuf+712+si]
        test    al,0ffh
        jz      p2ai05
        mov     p2_aitype,al
        and     al,0fh
        mov     p2_aicnt,al
        mov     al,[offset p2_atrbuf+713+si]
        mov     p2_aival,al
        add     p2_aiptr,2
p2ai00:
        mov     al,p2_aival
        mov     joyvalue,al
        dec     p2_aicnt
        jmp     p2ai10
p2ai05:
        test    p2_type,06h
        jz      p2ai10
        call    p2aichkp1
p2ai10:
        test    p2_aitype,BIT4
        jz      p2ai20
        test    p1_airflag,0ffh
        jnz     p2ai18
        mov     al,p1_pthno
        test    p1_forcenum,0ffh
        jz      p2ai15
        mov     al,p1_forcenum
p2ai15:
        cmp     al,26
        jc      p2ai20
p2ai18: mov     al,p2_aival
        and     al,0fch
        or      al,BUT_LF
        mov     joyvalue,al
        jmp     p2ai30
p2ai20:
        mov     al,p2_aitype
        and     al,60h
        jz      p2ai30
        test    bh,0ffh
        jnz     p2ai30
        cmp     p1_pthno,15
        jz      p2ai25
        mov     bl,p1_direct
        cmp     bl,p2_direct
        jz      p2ai30
p2ai25: shr     al,1
        or      joyvalue,al
p2ai30:
        test    p2_direct,0ffh
        jz      p2ai99
        mov     al,joyvalue
        mov     ah,al
        and     al,BUT_RT
        and     ah,BUT_LF
        and     joyvalue,0fch
        shr     ah,1
        shl     al,1
        or      al,ah
        or      joyvalue,al
p2ai99:
        ret
;
p2aichkp1:
;  calc distance
        xor     ax,ax
        mov     al,p1_pthno
        test    p1_forcenum,0ffh
        jz      p2aip200
        mov     al,p1_forcenum
p2aip200:
        mov     si,ax
        mov     al,pathattr[si]
        mov     ah,12
        mul     ah
        mov     cl,RAN58
        and     cl,03h
        shl     bl,1
        shl     bl,1
        add     cl,bl
        xor     ch,ch
        add     ax,cx
        mov     si,ax
        mov     al,[offset aitable+si]
        xor     ah,ah
        mov     cl,4
        shl     ax,cl
        mov     p2_aiptr,ax
        mov     si,ax
        mov     al,[offset p2_atrbuf+712+si]
        mov     p2_aitype,al
        and     al,0fh
        mov     p2_aicnt,al
        mov     al,[offset p2_atrbuf+713+si]
        mov     p2_aival,al
        add     p2_aiptr,2
p2aipth99:
        mov     joyvalue,al
        ret
com_ai_p2       ENDP
;  =================================
;  =  put joy value to buffer      =
;  =================================
buf_joy_p1      PROC    NEAR
        mov     al,j1_joy
        and     al,0fh
        mov     ah,j1_last
        and     ah,0fh
        test    al,0ffh
        jz      nowp1zero
        cmp     al,ah
        jz      notp1buf
        mov     j1_tick1,0
p1putbuf:
        mov     si,j1_jbptr
        mov     j1_joybuf[si],al
        inc     si
        and     si,000fh
        mov     j1_jbptr,si
        jmp     clrp1zero
notp1buf:
        inc     j1_tick1
        cmp     j1_tick1,REP_COUNT
        jna     clrp1zero
        mov     j1_tick1,0
        jmp     p1putbuf
nowp1zero:
        mov     j1_tick1,0
chkp1clear:
        inc     j1_tick2
        cmp     j1_tick2,CLR_COUNT
        jna     bufp1over
        call    clr_p1buf
clrp1zero:
        mov     j1_tick2,0
bufp1over:
        ret
;
clr_p1buf:
        xor     ax,ax
        mov     si,ax
        mov     cx,BUF_SIZE
cp1b:   mov     j1_joybuf[si],al
        inc     si
        loop    cp1b
        mov     j1_jbptr,0
        ret
buf_joy_p1      ENDP
;
buf_joy_p2      PROC    NEAR
        mov     al,j2_joy
        and     al,0fh
        mov     ah,j2_last
        and     ah,0fh
        test    al,0ffh
        jz      nowp2zero
        cmp     al,ah
        jz      notp2buf
        mov     j2_tick1,0
p2putbuf:
        mov     si,j2_jbptr
        mov     j2_joybuf[si],al
        inc     si
        and     si,000fh
        mov     j2_jbptr,si
        jmp     clrp2zero
notp2buf:
        inc     j2_tick1
        cmp     j2_tick1,REP_COUNT
        jna     clrp2zero
        mov     j2_tick1,0
        jmp     p2putbuf
nowp2zero:
        mov     j2_tick1,0
chkp2clear:
        inc     j2_tick2
        cmp     j2_tick2,CLR_COUNT
        jna     bufp2over
        call    clr_p2buf
clrp2zero:
        mov     j2_tick2,0
bufp2over:
        ret
;
clr_p2buf:
        xor     ax,ax
        mov     si,ax
        mov     cx,BUF_SIZE
cp2b:   mov     j2_joybuf[si],al
        inc     si
        loop    cp2b
        mov     j2_jbptr,0
        ret
buf_joy_p2      ENDP
;  ==============================
;  =  JOYSTICK CONTROL          =
;  ==============================
joy_control_p1  PROC    NEAR
        test    p1_type,BIT1
        jnz     chkp1arror
        jmp     nolf
chkp1arror:
        test    p1_direct,0ffh
        jnz     chkp1direct
        mov     ax,p1_pretx             ;direct = 0
        cmp     ax,p2_pretx
        jna     chkp1main
        add     al,p1_width
        adc     ah,0
        mov     bx,p2_txtx
        add     bl,p2_width
        adc     bh,0
        cmp     ax,bx
        jna     chkp1main
        mov     p1_direct,0ffh
;       call    adj_p1_x
        jmp     chkp1main
chkp1direct:
        mov     ax,p1_txtx
        add     al,p1_width
        adc     ah,0
        mov     bx,p2_txtx
        add     bl,p2_width
        adc     bh,0
        cmp     ax,bx
        ja      chkp1main
        mov     ax,p1_pretx
        cmp     ax,p2_pretx
        ja      chkp1main
        mov     p1_direct,0h
;       call    adj_p1_x
chkp1main:
        mov     al,j1_joy
        and     al,0fh
        mov     ah,p1_direct
        test    ah,0ffh
        jz      setp1arror
        mov     ah,al
        and     ah,03h
        and     al,0fch
        mov     bl,ah
        and     bl,01h
        shl     bl,1
        shr     ah,1
        or      ah,bl
        or      al,ah
setp1arror:
        cmp     al,BUT_UP+BUT_RT
        jnz     j1up
        mov     p1_pthno,5
        mov     p1_stop,0
        mov     p1_pthcnt,0
        jmp     nolf
j1up:
        cmp     al,BUT_UP+BUT_LF
        jnz     j1updn
        mov     p1_pthno,6
        mov     p1_stop,0
        mov     p1_pthcnt,0
        jmp     nolf
j1updn:
        cmp     al,BUT_DN+BUT_RT
        jnz     j1dnrt
        mov     p1_pthno,1
        mov     p1_stop,0
        mov     p1_pthcnt,0
        jmp     nolf
j1dnrt:
        cmp     al,BUT_DN+BUT_LF
        jnz     j1dnlf
        mov     p1_pthno,8
        test    p2_airflag,0ffh
        jnz     j1dndef
        cmp     p2_pthno,26
        jnc     j1dndef
        test    p2_forcenum,0ffh
        jnz     j1dndef
        mov     p1_pthno,1
j1dndef:
        mov     p1_stop,0
        mov     p1_pthcnt,0
        jmp     nolf
j1dnlf:
        test    al,BUT_UP
        jz      noup
        mov     p1_pthno,4
        mov     p1_stop,0
        mov     p1_pthcnt,0
noup:   test    al,BUT_DN
        jz      nodn
        mov     p1_pthno,1
        mov     p1_stop,0
        mov     p1_pthcnt,0
nodn:   test    al,BUT_RT
        jz      nort
        cmp     p1_pthno,2
        jz      nort
        mov     p1_pthno,2
        mov     p1_stop,0
        mov     p1_pthcnt,0
nort:   test    al,BUT_LF
        jz      nolf
        test    p2_airflag,0ffh
        jnz     nortdef
        cmp     p2_pthno,26
        jnc     nortdef
        test    p2_forcenum,0ffh
        jnz     nortdef
        cmp     p1_pthno,3
        jz      nolf
        mov     p1_pthno,3
        jmp     nortdef2
nortdef:
        mov     p1_pthno,7
nortdef2:
        mov     p1_stop,0
        mov     p1_pthcnt,0
nolf:
jcover:
        ret
joy_control_p1  ENDP
;
joy_control_p2  PROC    NEAR
        test    p2_type,BIT1
        jnz     chkp2arror
        jmp     nolf2
chkp2arror:
        test    p2_direct,0ffh
        jnz     chkp2direct
        mov     ax,p2_pretx
        cmp     ax,p1_pretx
        jc      chkp2main
        add     al,p2_width
        adc     ah,0
        mov     bx,p1_txtx
        add     bl,p1_width
        adc     bh,0
        cmp     ax,bx
        jc      chkp2main
        mov     p2_direct,0ffh
;       call    adj_p2_x
        jmp     chkp2main
chkp2direct:
        mov     ax,p2_txtx
        add     al,p2_width
        adc     ah,0
        mov     bx,p1_txtx
        add     bl,p1_width
        adc     bh,0
        cmp     ax,bx
        jnc     chkp2main
        mov     ax,p2_pretx
        cmp     ax,p1_pretx
        jnc     chkp2main
        mov     p2_direct,0h
;       call    adj_p2_x
chkp2main:
        mov     al,j2_joy
        and     al,0fh
        mov     ah,p2_direct
        test    ah,0ffh
        jz      setp2arror
        mov     ah,al
        and     ah,03h
        and     al,0fch
        mov     bl,ah
        and     bl,01h
        shl     bl,1
        shr     ah,1
        or      ah,bl
        or      al,ah
setp2arror:
        cmp     al,BUT_UP+BUT_RT
        jnz     j2up
        mov     p2_pthno,5
        mov     p2_stop,0
        mov     p2_pthcnt,0
        jmp     nolf2
j2up:
        cmp     al,BUT_UP+BUT_LF
        jnz     j2updn
        mov     p2_pthno,6
        mov     p2_stop,0
        mov     p2_pthcnt,0
        jmp     nolf2
j2updn:
        cmp     al,BUT_DN+BUT_RT
        jnz     j2dnrt
        mov     p2_pthno,1
        mov     p2_stop,0
        mov     p2_pthcnt,0
        jmp     nolf2
j2dnrt:
        cmp     al,BUT_DN+BUT_LF
        jnz     j2dnlf
        mov     p2_pthno,8
        test    p1_airflag,0ffh
        jnz     j2dndef
        cmp     p1_pthno,26
        jnc     j2dndef
        test    p1_forcenum,0ffh
        jnz     j2dndef
        mov     p2_pthno,1
j2dndef:
        mov     p2_stop,0
        mov     p2_pthcnt,0
        jmp     nolf2
j2dnlf:
        test    al,BUT_UP
        jz      noup2
        mov     p2_pthno,4
        mov     p2_stop,0
        mov     p2_pthcnt,0
noup2:  test    al,BUT_DN
        jz      nodn2
        mov     p2_pthno,1
        mov     p2_stop,0
        mov     p2_pthcnt,0
nodn2:  test    al,BUT_RT
        jz      nort2
        cmp     p2_pthno,2
        jz      nort2
        mov     p2_pthno,2
        mov     p2_stop,0
        mov     p2_pthcnt,0
nort2:  test    al,BUT_LF
        jz      nolf2
        test    p1_airflag,0ffh
        jnz     nort2def
        cmp     p1_pthno,26
        jnc     nort2def
        test    p1_forcenum,0ffh
        jnz     nort2def
        cmp     p2_pthno,3
        jz      nolf2
        mov     p2_pthno,3
        jmp     nort2def2
nort2def:
        mov     p2_pthno,7
nort2def2:
        mov     p2_stop,0
        mov     p2_pthcnt,0
nolf2:
jcover2:
        ret
joy_control_p2  ENDP
;  ==============================
;  =  Shake man                 =
;  ==============================
shake_man       PROC    NEAR
        cmp     p1_pthno,15
        jnz     shake_1pend
        call    f_starp1
        test    j1_trg,0ffh
        jz      shake_1pend
        test    p1_stop,0ffh
        jz      shake_1pend
        dec     p1_stop
        jnz     shake_1pend
        inc     p1_pthcnt
shake_1pend:
        cmp     p2_pthno,15
        jnz     shake_2pend
        call    f_starp2
        test    j2_trg,0ffh
        jz      shake_2pend
        test    p2_stop,0ffh
        jz      shake_2pend
        dec     p2_stop
        jnz     shake_2pend
        inc     p2_pthcnt
shake_2pend:
        test    p1_light,0ffffh
        jz      light_p1_end
        xor     bx,bx
        mov     es,alloc1
        mov     ax,p1_lighty
        mov     di,320
        mul     di
        mov     di,ax
        mov     ax,p1_lightx
        mov     si,p1_light
        mov     x0,0
        call    flight
        inc     p1_light
        cmp     p1_light,5
        jna     light_p1_end
        mov     p1_light,0
light_p1_end:
        test    p2_light,0ffffh
        jz      light_p2_end
        xor     bx,bx
        mov     es,alloc1
        mov     ax,p2_lighty
        mov     di,320
        mul     di
        mov     di,ax
        mov     ax,p2_lightx
        mov     si,p2_light
        mov     x0,4
        call    flight
        inc     p2_light
        cmp     p2_light,5
        jna     light_p2_end
        mov     p2_light,0
light_p2_end:
        ret
;
f_starp1:
        xor     bx,bx
        mov     es,alloc1
        mov     ax,p1_txty
        sub     ax,9
        mov     di,320
        mul     di
        mov     di,ax
        mov     ax,p1_txtx
        test    p1_direct,0ffh
        jz      fp1starrt
        add     al,p1_width
        adc     ah,0
        sub     ax,41           ;star width
        jmp     fp1starrt
f_starp2:
        xor     bx,bx
        mov     es,alloc1
        mov     ax,p2_txty
        sub     ax,9
        mov     di,320
        mul     di
        mov     di,ax
        mov     ax,p2_txtx
        test    p2_direct,0ffh
        jz      fp1starrt
        add     al,p2_width
        adc     ah,00
        sub     ax,41           ;star width
fp1starrt:
        push    ax
        sub     ax,scr_x
        test    ax,8000h
        jz      fp1starxin
        xor     ax,0ffffh
        inc     ax
        mov     bl,al
        mov     ax,0
fp1starxin:
        add     di,ax
        mov     dx,di
        pop     ax
        add     ax,41
        mov     cx,scr_x
        add     cx,319
        sub     cx,ax
        jnc     fp1starxrtin
        xor     cx,0ffffh
        inc     cx
        mov     bh,cl
fp1starxrtin:
        mov     si,counter
        and     si,02h
        mov     ax,star_ptr[si]
        add     ax,offset star_buf
        mov     si,ax
        mov     r0,12
fp1star00:
        xor     cx,cx
        mov     cl,41
        sub     cl,bl
        sbb     cl,bh
        test    cl,80h
        jz      fp1star05
        ret
fp1star05:
        mov     al,bh
        xor     bh,bh
        add     si,bx
        add     bl,al
        adc     bh,0
        mov     r1,cx
        cld
fp1star10:
        lodsb
        test    al,0ffh
        jz      fp1star20
        or      al,0a0h
        stosb
        jmp     fp1star30
fp1star20:
        inc     di
fp1star30:
        loop    fp1star10
        mov     cx,r1
        add     si,bx
        mov     ax,es
        add     ax,14h
        mov     es,ax
        mov     di,dx
        dec     r0
        jnz     fp1star10
        ret
;
flight:
        push    ax
        sub     ax,scr_x
        test    ax,8000h
        jz      fp1lightxin
        xor     ax,0ffffh
        inc     ax
        mov     bl,al
        mov     ax,0
fp1lightxin:
        add     di,ax
        mov     dx,di
        pop     ax
        add     ax,17
        mov     cx,scr_x
        add     cx,319
        sub     cx,ax
        jnc     fp1lightxrtin
        xor     cx,0ffffh
        inc     cx
        mov     bh,cl
fp1lightxrtin:
        shr     si,1
        add     si,2
        shl     si,1
        mov     ax,star_ptr[si]
        add     ax,offset star_buf
        mov     si,ax
        mov     r0,17
fp1light00:
        xor     cx,cx
        mov     cl,17
        sub     cl,bl
        sbb     cl,bh
        test    cl,80h
        jz      fp1light05
        ret
fp1light05:
        mov     al,bh
        xor     bh,bh
        add     si,bx
        add     bl,al
        adc     bh,0
        mov     r1,cx
        cld
fp1light10:
        lodsb
        test    al,0ffh
        jz      fp1light20
        add     al,160
        add     al,x0
        stosb
        jmp     fp1light30
fp1light20:
        inc     di
fp1light30:
        loop    fp1light10
        mov     cx,r1
        add     si,bx
        mov     ax,es
        add     ax,14h
        mov     es,ax
        mov     di,dx
        dec     r0
        jnz     fp1light10
        ret
shake_man       ENDP
;  ==============================
;  =  ATTACK CONTROL            =
;  ==============================
attack_control  PROC    NEAR
        test    p1_type,BIT2            ;accept p1 attack
        jnz     attack_control_p1
        jmp     attack_p1_end
attack_control_p1:
        test    j1_trg,0f0h             ;press attack key
        jnz     p1_atkctl_main
        jmp     attack_p1_end
p1_atkctl_main:
        mov     priority,0
        call    chk_joybuf_p1
        jc      p1_atkctl_main2
        jmp     p1_attack_main2
p1_atkctl_main2:
        mov     al,[bx+2]               ;get byte 2
        test    al,080h
        jz      p1atkmain90
        test    p2_atkatk,BIT4
        jnz     p1_atkctl_main25
        test    p2_atkatt,BIT6
        jz      p1_atkctl_main3
p1_atkctl_main25:
        jmp     p1_attack_main2
p1_atkctl_main3:
        push    bx
        call    p1_check_crash
        pop     bx
        test    p2_hitnum,0ffh
        jz      p1_attack_main2
        mov     al,p2_hitnum
        call    setp2path
        mov     al,p1_direct
        test    byte ptr [bx+2],40h
        jz      p1_atkctl_main4
        xor     al,0ffh
p1_atkctl_main4:
        mov     p2_direct,al
        mov     ax,p1_x
        mov     p2_x,ax
        mov     ax,p1_y
        mov     p2_y,ax
;
        mov     al,p1_px
        sub     al,p2_px
        cbw
        shl     ax,1
        add     ax,p1_subx
        mov     p2_subx,ax
        mov     al,p2_px
        mov     p2_levelx,al
;
        and     p2_type,0fdh
        and     p1_type,0fdh
        push    bx
        xor     ah,ah
        mov     al,[bx]
        call    check_p2_lose
        mov     p2_sleep,0
        mov     p2_sleepcnt,0
        pop     bx
        mov     al,[bx+2]
p1atkmain90:
        and     al,30h
        cmp     al,30h
        jz      p1atkmain95
        test    al,20h
        jnz     p1atkmain93
        test    p1_y,0ffffh
        jnz     p1_attack_main2
        jmp     p1atkmain95
p1atkmain93:
        test    p1_y,0ffffh
        jz      p1_attack_main2
p1atkmain95:
        mov     al,[bx]
        call    setp1path
        jmp     attack_p1_end
p1_attack_main2:
        mov     al,j1_trg
        test    al,BUT_A
        jnz     p1_attack_hand
        jmp     p1_attack_leg
p1_attack_hand:
        mov     al,p1_pthno
        cmp     al,8
        ja      p1_attack_leg
        xor     ah,ah
        mov     si,ax
        mov     al,atthandnum[si]
        mov     ah,forcenum[si]
        test    ah,0ffh
        jnz     p1_ahand_15
        jmp     p1_aleg_hof
p1_ahand_15:
        test    p1_y,0ffffh
        jnz     p1_ahand_20
        mov     al,27
        jmp     p1_aleg_hof
p1_ahand_20:
        mov     p1_forcenum,al
        mov     p1_forceno,0
        call    set_p1_force
        jmp     p1_aleg_over
p1_attack_leg:
        test    al,BUT_B
        jz      attack_p1_end
        mov     al,p1_pthno
        cmp     al,8
        ja      attack_p1_end
        xor     ah,ah
        mov     si,ax
        mov     al,attlegnum[si]
        mov     ah,forcenum[si]
        test    ah,0ffh
        jz      p1_aleg_hof
        test    p1_y,0ffffh
        jnz     p1_aleg_20
        mov     al,30
        jmp     p1_aleg_hof
p1_aleg_20:
        mov     p1_forcenum,al
        mov     p1_forceno,0
        call    set_p1_force
        jmp     p1_aleg_over
p1_aleg_hof:
        mov     p1_pthno,al
        mov     p1_stop,0
        mov     p1_pthcnt,0
p1_aleg_over:
        and     p1_type,0fdh
attack_p1_end:
;
        test    p2_type,BIT2
        jnz     attack_control_p2
        jmp     attack_p2_end
attack_control_p2:
        test    j2_trg,0f0h
        jnz     p2_atkctl_main
        jmp     attack_p2_end
p2_atkctl_main:
        mov     priority,1
        call    chk_joybuf_p2
        jc      p2_atkctl_main2
        jmp     p2_attack_main2
p2_atkctl_main2:
        mov     al,[bx+2]
        test    al,080h
        jz      p2atkmain90
        test    p1_atkatk,BIT4
        jnz     p2_atkctl_main25
        test    p1_atkatt,BIT6
        jz      p2_atkctl_main3
p2_atkctl_main25:
        jmp     p2_attack_main2
p2_atkctl_main3:
        push    bx
        call    p2_check_crash
        pop     bx
        test    p1_hitnum,0ffh
        jz      p2_attack_main2
        mov     al,p1_hitnum
        call    setp1path
        mov     al,p2_direct
        test    byte ptr [bx+2],40h
        jz      p2_atkctl_main4
        xor     al,0ffh
p2_atkctl_main4:
        mov     p1_direct,al
        mov     ax,p2_x
        mov     p1_x,ax
        mov     ax,p2_y
        mov     p1_y,ax
;
        mov     al,p2_px
        sub     al,p1_px
        cbw
        shl     ax,1
        add     ax,p2_subx
        mov     p1_subx,ax
        mov     al,p1_px
        mov     p1_levelx,al
;
        and     p2_type,0fdh
        and     p1_type,0fdh
        push    bx
        xor     ah,ah
        mov     al,[bx]
        call    check_p1_lose
        mov     p1_sleep,0
        mov     p1_sleepcnt,0
        pop     bx
        mov     al,[bx+2]
p2atkmain90:
        and     al,30h
        cmp     al,30h
        jz      p2atkmain95
        test    al,20h
        jnz     p2atkmain93
        test    p2_y,0ffffh
        jnz     p2_attack_main2
        jmp     p2atkmain95
p2atkmain93:
        test    p2_y,0ffffh
        jz      p2_attack_main2
p2atkmain95:
        mov     al,[bx]
        call    setp2path
        jmp     attack_p2_end
p2_attack_main2:
        mov     al,j2_trg
        test    al,BUT_A
        jnz     p2_attack_hand
        jmp     p2_attack_leg
p2_attack_hand:
        mov     al,p2_pthno
        cmp     al,8
        ja      p2_attack_leg
        xor     ah,ah
        mov     si,ax
        mov     al,atthandnum[si]
        mov     ah,forcenum[si]
        test    ah,0ffh
        jnz     p2_ahand_15
        jmp     p2_aleg_hof
p2_ahand_15:
        test    p2_y,0ffffh
        jnz     p2_ahand_20
        mov     al,27
        jmp     p2_aleg_hof
p2_ahand_20:
        mov     p2_forcenum,al
        mov     p2_forceno,0
        call    set_p2_force
        jmp     p2_aleg_over
p2_attack_leg:
        test    al,BUT_B
        jz      attack_p2_end
        mov     al,p2_pthno
        cmp     al,8
        ja      attack_p2_end
        xor     ah,ah
        mov     si,ax
        mov     al,attlegnum[si]
        mov     ah,forcenum[si]
        test    ah,0ffh
        jz      p2_aleg_hof
        test    p2_y,0ffffh
        jnz     p2_aleg_20
        mov     al,30
        jmp     p2_aleg_hof
p2_aleg_20:
        mov     p2_forcenum,al
        mov     p2_forceno,0
        call    set_p2_force
        jmp     p2_aleg_over
p2_aleg_hof:
        mov     p2_pthno,al
        mov     p2_stop,0
        mov     p2_pthcnt,0
p2_aleg_over:
        and     p2_type,0fdh
attack_p2_end:
        ret
;
setp1path:
        mov     p1_pthno,al
        mov     p1_stop,0
        mov     p1_pthcnt,0
        mov     p1_forcenum,0
        and     p1_type,0fbh
        ret
;
setp2path:
        mov     p2_pthno,al
        mov     p2_stop,0
        mov     p2_pthcnt,0
        mov     p2_forcenum,0
        and     p2_type,0fbh
        ret
attack_control  ENDP
;
set_p1_force    PROC    NEAR
        xor     bx,bx
        mov     bl,p1_forcenum
        shl     bx,1
        mov     ax,[bx+offset p1_pth]
        mov     si,offset p1_pth
        add     si,ax
        xor     ax,ax
        mov     al,p1_forceno
        mov     cl,3
        shl     ax,cl
        add     si,ax
        lodsb
        test    al,0ffh
        jnz     set_p1_fmain
        mov     p1_forcenum,0
        mov     al,p1_sprbak
        mov     p1_sprite,al
        call    sprite_info1
        jmp     set_p1_forceend
set_p1_fmain:
        mov     ax,ds
        mov     es,ax
        mov     di,offset p1_px
        cld
        movsb
        movsb
        lodsb
        mov     p1_forcecnt,al
        mov     p1_attr,al
        and     p1_forcecnt,03fh
        inc     di
        movsb
        call    sprite_info1
set_p1_forceend:
        test    p1_direct,0ffh
        jz      p1_forceproc
        mov     al,p1_levelx
        sub     al,p1_px
        cbw
        shl     ax,1
        add     p1_subx,ax
        mov     ax,p1_subx
        sub     al,p1_width
        sbb     ah,0
        mov     p1_x,ax
p1_forceproc:
        mov     al,p1_px
        mov     p1_levelx,al
        mov     ax,p1_x
        add     al,p1_width
        adc     ah,0
        mov     p1_subx,ax
        xor     ax,ax
        mov     al,p1_px
        add     ax,p1_x
        mov     p1_txtx,ax
        mov     p1_move,ax
        mov     p1_pretx,ax
        test    ax,8000h
        jz      p1_forceproc2
        mov     p1_pretx,0
p1_forceproc2:
        and     p1_type,0fbh
        ret
set_p1_force    ENDP
;
set_p2_force    PROC    NEAR
        xor     bx,bx
        mov     bl,p2_forcenum
        shl     bx,1
        mov     ax,[bx+offset p2_pth]
        mov     si,offset p2_pth
        add     si,ax
        xor     ax,ax
        mov     al,p2_forceno
        mov     cl,3
        shl     ax,cl
        add     si,ax
        lodsb
        test    al,0ffh
        jnz     set_p2_fmain
        mov     p2_forcenum,0
        mov     al,p2_sprbak
        mov     p2_sprite,al
        call    sprite_info2
        jmp     set_p2_forceend
set_p2_fmain:
        mov     ax,ds
        mov     es,ax
        mov     di,offset p2_px
        cld
        movsb
        movsb
        lodsb
        mov     p2_forcecnt,al
        mov     p2_attr,al
        and     p2_forcecnt,03fh
        inc     di
        movsb
        call    sprite_info2
set_p2_forceend:
        test    p2_direct,0ffh
        jz      p2_forceproc
        mov     al,p2_levelx
        sub     al,p2_px
        cbw
        shl     ax,1
        add     p2_subx,ax
        mov     ax,p2_subx
        sub     al,p2_width
        sbb     ah,0
        mov     p2_x,ax
p2_forceproc:
        mov     al,p2_px
        mov     p2_levelx,al
        mov     ax,p2_x
        add     al,p2_width
        adc     ah,0
        mov     p2_subx,ax
        xor     ax,ax
        mov     al,p2_px
        add     ax,p2_x
        mov     p2_txtx,ax
        mov     p2_move,ax
        mov     p2_pretx,ax
        test    ax,8000h
        jz      p2_forceproc2
        mov     p2_pretx,0
p2_forceproc2:
        and     p2_type,0fbh
        ret
set_p2_force    ENDP
;
p1_check_crash  PROC    NEAR
        mov     p2_hitnum,0
        mov     ah,al
        and     ah,30h                  ;check air or ground attack
        cmp     ah,30h
        jz      p1chkcshmain
        test    ah,20h
        jnz     p1chkcshair
        mov     bx,p1_y
        or      bx,p2_y
        test    bx,0ffffh
        jz      p1chkcshmain
        jmp     p1_crash_end
p1chkcshair:
        test    p1_y,0ffffh
        jz      p1noair
        test    p2_y,0ffffh
        jnz     p1chkcshmain
p1noair:
        jmp     p1_crash_end
p1chkcshmain:
        xor     bx,bx
        mov     bl,al
        and     bl,0fh
        shl     bx,1
        shl     bx,1
        shl     bx,1
        xor     ax,ax
        mov     al,[bx+offset p1_atrbuf+72*5]
        mov     r0,ax
        mov     al,[bx+offset p1_atrbuf+72*5+1]
        mov     r2,ax
        mov     al,[bx+offset p1_atrbuf+72*5+2]
        mov     r1,ax
        mov     al,[bx+offset p1_atrbuf+72*5+3]
        mov     r3,ax
        test    p1_direct,0ffh
        jnz     p1_crash_rt
        mov     ax,p1_x
        add     ax,r0
        mov     r0,ax
        add     r1,ax
        jmp     p1_crash_ok
p1_crash_rt:
        xor     ax,ax
        mov     al,p1_px
        shl     ax,1
        add     ax,p1_subx
        sub     ax,r0
        sbb     ax,r1
        add     r1,ax
        cwd
        xor     dx,0ffffh
        and     ax,dx
        mov     r0,ax
        test    r1,8000h
        jz      p1_crash_ok
        mov     r1,0
p1_crash_ok:
        test    r0,8000h
        jz      p1_crash_ok2
        mov     r0,0
p1_crash_ok2:
        test    r1,8000h
        jz      p1_crash_ok4
        mov     r1,0
p1_crash_ok4:
        mov     ax,r2
        sub     ax,p1_y
        mov     r2,ax
        add     r3,ax                   ;calc y position over
        mov     ax,p2_pretx
        cmp     ax,r1
        ja      p1_crash_end
        mov     ax,p2_subx
        add     al,p2_px
        adc     ah,0
        cmp     ax,r0
        jna     p1_crash_end           ;check x position over
        mov     ax,r3
        cmp     ax,p2_txty
        jna     p1_crash_end
        mov     ax,r2
        mov     cx,p2_txty
        add     cl,p2_length
        adc     ch,0
        cmp     ax,cx
        ja      p1_crash_end
        inc     p1_hitflg
        mov     al,[bx+offset p1_atrbuf+72*5+4]
        test    al,0ffh
        jnz     p1_set_crhact
        mov     al,[bx+offset p1_atrbuf+72*5+5]
        jmp     p1_crhact_end
p1_set_crhact:
        xor     bx,bx
        mov     bl,p2_sprite
        shl     bx,1
        shl     bx,1
        mov     ah,[bx+offset p2_atrbuf+72]
        mov     bx,p2_y
        mov     dl,p2_pthno
        call    check_hitted_act
p1_crhact_end:
        mov     p2_hitnum,al
p1_crash_end:
        ret
p1_check_crash  ENDP
;
p2_check_crash  PROC    NEAR
        mov     p1_hitnum,0
        mov     ah,al
        and     ah,30h
        cmp     ah,30h
        jz      p2chkcshmain
        test    ah,20h
        jnz     p2chkcshair
        mov     bx,p1_y
        or      bx,p2_y
        test    bx,0ffffh
        jz      p2chkcshmain
        jmp     p2_crash_end
p2chkcshair:
        test    p1_y,0ffffh
        jz      p2noair
        test    p2_y,0ffffh
        jnz     p2chkcshmain
p2noair:
        jmp     p2_crash_end
p2chkcshmain:
        xor     bx,bx
        mov     bl,al
        and     bl,0fh
        shl     bx,1
        shl     bx,1
        shl     bx,1
        xor     ax,ax
        mov     al,[bx+offset p2_atrbuf+72*5]
        mov     r0,ax
        mov     al,[bx+offset p2_atrbuf+72*5+1]
        mov     r2,ax
        mov     al,[bx+offset p2_atrbuf+72*5+2]
        mov     r1,ax
        mov     al,[bx+offset p2_atrbuf+72*5+3]
        mov     r3,ax
        test    p2_direct,0ffh
        jnz     p2_crash_rt
        mov     ax,p2_x
        add     ax,r0
        mov     r0,ax
        add     r1,ax
        jmp     p2_crash_ok
p2_crash_rt:
        xor     ax,ax
        mov     al,p2_px
        shl     ax,1
        add     ax,p2_subx
        sub     ax,r0
        sbb     ax,r1
        add     r1,ax
        cwd
        xor     dx,0ffffh
        and     ax,dx
        mov     r0,ax
        test    r1,8000h
        jz      p2_crash_ok
        mov     r1,0
p2_crash_ok:
        test    r0,8000h
        jz      p2_crash_ok2
        mov     r0,0
p2_crash_ok2:
        test    r1,8000h
        jz      p2_crash_ok4
        mov     r1,0
p2_crash_ok4:
        mov     ax,r2
        sub     ax,p2_y
        mov     r2,ax
        add     r3,ax                   ;calc y position over
        mov     ax,p1_pretx
        cmp     ax,r1
        ja      p2_crash_end
        mov     ax,p1_subx
        add     al,p1_px
        adc     ah,0
        cmp     ax,r0
        jna     p2_crash_end           ;check x position over
        mov     ax,r3
        cmp     ax,p1_txty
        jna     p2_crash_end
        mov     ax,r2
        mov     cx,p1_txty
        add     cl,p1_length
        adc     ch,0
        cmp     ax,cx
        ja      p2_crash_end
        inc     p2_hitflg
        mov     al,[bx+offset p2_atrbuf+72*5+4]
        test    al,0ffh
        jnz     p2_set_crhact
        mov     al,[bx+offset p2_atrbuf+72*5+5]
        jmp     p2_crhact_end
p2_set_crhact:
        xor     bx,bx
        mov     bl,p1_sprite
        shl     bx,1
        shl     bx,1
        mov     ah,[bx+offset p1_atrbuf+72]
        mov     bx,p1_y
        mov     dl,p2_pthno
        call    check_hitted_act
p2_crhact_end:
        mov     p1_hitnum,al
p2_crash_end:
        ret
p2_check_crash  ENDP
;  ==============================
;  =  CHECK JOY BUFFER          =
;  ==============================
chk_joybuf_p1   PROC    NEAR
        mov     si,j1_jbptr
        mov     di,4
        mov     bx,offset p1_atrbuf
        test    si,0ffffh
        jnz     cjbf100
        mov     si,BUF_SIZE
cjbf100:
        dec     si
cjbf105:
        mov     r0,si
cjbf107:
        mov     dl,[bx]
        test    dl,0ffh
        jz      cjbf195
        mov     si,r0
        mov     dl,j1_trg
        and     dl,0f0h
        cmp     dl,[bx+3]
        jnz     cjbf130
cjbf109:
        mov     al,j1_joybuf[si]
        test    p1_direct,0ffh
        jz      cjbf110
        mov     ah,al
        and     ah,03h
        and     al,0fch
        mov     dl,ah
        and     dl,01h
        shl     dl,1
        shr     ah,1
        or      ah,dl
        or      al,ah
cjbf110:
        test    byte ptr [bx][di],0ffh
        jz      cjbf190
        test    byte ptr [bx][di],80h
        jz      cjbf115
        test    al,[bx][di]
        jz      cjbf130
        jmp     cjbf118
cjbf115:
        cmp     al,[bx][di]
        jnz     cjbf130
cjbf118:
        test    si,0ffffh
        jnz     cjbf120
        mov     si,BUF_SIZE
cjbf120:
        dec     si
        inc     di
        jmp     cjbf109
cjbf130:  ;not same
        mov     di,4
        add     bx,12
        jmp     cjbf107
cjbf190:
        stc
        jmp     cjbf199
cjbf195:
        clc
cjbf199:
        ret
chk_joybuf_p1   ENDP
;
chk_joybuf_p2   PROC    NEAR
        mov     si,j2_jbptr
        mov     di,4
        mov     bx,offset p2_atrbuf
        test    si,0ffffh
        jnz     cjbf200
        mov     si,BUF_SIZE
cjbf200:
        dec     si
cjbf205:
        mov     r0,si
cjbf207:
        mov     dl,[bx]
        test    dl,0ffh
        jz      cjbf295
        mov     si,r0
        mov     dl,j2_trg
        and     dl,0f0h
        cmp     dl,[bx+3]
        jnz     cjbf230
cjbf209:
        mov     al,j2_joybuf[si]
        test    p2_direct,0ffh
        jz      cjbf210
        mov     ah,al
        and     ah,03h
        and     al,0fch
        mov     dl,ah
        and     dl,01h
        shl     dl,1
        shr     ah,1
        or      ah,dl
        or      al,ah
cjbf210:
        test    byte ptr [bx][di],0ffh
        jz      cjbf290
        test    byte ptr [bx][di],80h
        jz      cjbf215
        test    al,[bx][di]
        jz      cjbf230
        jmp     cjbf218
cjbf215:
        cmp     al,[bx][di]
        jnz     cjbf230
cjbf218:
        test    si,0ffffh
        jnz     cjbf220
        mov     si,BUF_SIZE
cjbf220:
        dec     si
        inc     di
        jmp     cjbf209
cjbf230:  ;not same
        mov     di,4
        add     bx,12
        jmp     cjbf207
cjbf290:
        stc
        jmp     cjbf299
cjbf295:
        clc
cjbf299:
        ret
chk_joybuf_p2   ENDP
;  ==============================
;  =  SET SCREEN X,Y            =
;  ==============================
set_scrxy       PROC    NEAR
;  calc y absolute offset
        test    p1_type,BIT7
        jz      setscrxy00
        call    adj_p1_x
setscrxy00:
        test    p2_type,BIT7
        jz      setscrxy10
        call    adj_p2_x
setscrxy10:
        xor     dx,dx
        mov     dl,p1_py
        sub     dx,p1_y
        mov     p1_txty,dx
        xor     dx,dx
        mov     dl,p2_py
        sub     dx,p2_y
        mov     p2_txty,dx
        mov     ytouch,0
;
        mov     ax,p1_pretx
        cmp     ax,p2_pretx
        ja      p2right
        xor     ax,ax
        mov     al,p1_width
        mov     lf_width,ax
        mov     al,p2_width
        mov     rt_width,ax
        mov     al,p1_px
        mov     lf_px,ax
        mov     al,p2_px
        mov     rt_px,ax
        mov     ax,p1_x
        mov     lf_x,ax
        mov     ax,p2_x
        mov     rt_x,ax
        mov     ax,p1_move
        mov     lf_move,ax
        mov     ax,p1_txtx
        mov     lf_txtx,ax
        mov     ax,p2_move
        mov     rt_move,ax
        mov     ax,p2_txtx
        mov     rt_txtx,ax
        mov     ax,p1_pretx
        mov     lf_pretx,ax
        mov     ax,p2_pretx
        mov     rt_pretx,ax
        test    p1_direct,0ffh
        jnz     lfset
        test    p2_direct,0ffh
        jz      lfset
        jmp     startset
lfset:
        jmp     checkyend
p2right:
        xor     ax,ax
        mov     al,p1_width
        mov     rt_width,ax
        mov     al,p2_width
        mov     lf_width,ax
        mov     al,p2_px
        mov     lf_px,ax
        mov     al,p1_px
        mov     rt_px,ax
        mov     ax,p2_x
        mov     lf_x,ax
        mov     ax,p1_x
        mov     rt_x,ax
        mov     ax,p1_move
        mov     rt_move,ax
        mov     ax,p1_txtx
        mov     rt_txtx,ax
        mov     ax,p2_move
        mov     lf_move,ax
        mov     ax,p2_txtx
        mov     lf_txtx,ax
        mov     ax,p1_pretx
        mov     rt_pretx,ax
        mov     ax,p2_pretx
        mov     lf_pretx,ax
        test    p1_direct,0ffh
        jz      rtset
        test    p2_direct,0ffh
        jnz     rtset
        jmp     startset
rtset:
        jmp     checkyend
startset:
        mov     al,p1_type
        or      al,p2_type
        and     al,20h
        jz      checkystart
        jmp     checkyend
checkystart:
        mov     dx,p2_txty
        cmp     dx,p1_txty
        ja      p2lower
        mov     dx,p1_txty
p2lower:
        xor     ax,ax
        mov     al,p1_length
        add     ax,p1_txty
        xor     bx,bx
        mov     bl,p2_length
        add     bx,p2_txty
        cmp     ax,bx
        ja      ssxy10
        mov     bx,ax
ssxy10:
        sub     bx,dx
        jc      checkyend
        cmp     bx,8                    ;touch area large than 4
        jna     checkyend
        mov     ytouch,1
checkyend:
        mov     ax,lf_move
        cmp     ax,lf_txtx
        jnz     entersetxy
        jmp     setrtxy2
entersetxy:
        ja      setlffront
        jmp     setlfback
setlffront:
;       cmp     lf_move,479
;       jna     ssxy20
;       mov     lf_move,479
;ssxy20:
        mov     ax,scr_x
        cmp     ax,158                  ;screen reach right boundary ?
        jna     lffront                 ;front still can move
        test    ytouch,0ffh
        jnz     setlfnotouch
        jmp     lfnotouch
setlfnotouch:
        mov     ax,lf_width
        add     ax,lf_txtx              ;front already in botton
        cmp     ax,rt_txtx
        ja      lftouch
        mov     ax,lf_width
        add     ax,lf_move              ;beyond the level
        sub     ax,rt_txtx
        jnc     lftouch00
        jmp     lfnotouch
lftouch00:
        mov     bx,lf_move
        sub     bx,ax
        mov     lf_txtx,bx
lftouch:
        mov     ax,rt_width
        add     ax,rt_txtx              ;touch another man
        mov     bx,479
        sub     bx,ax                   ;touch right boundary
        jnc     stsy00
        jmp     setrtxy
stsy00:
        mov     ax,lf_move
        sub     ax,lf_txtx
        cmp     ax,bx
        jc      stsy10
        mov     ax,bx
stsy10:
        add     rt_txtx,ax
        add     rt_move,ax
        add     lf_txtx,ax
        jmp     setrtxy
lfnotouch:                              ;no touch another man
        mov     ax,lf_move
        mov     lf_txtx,ax
        jmp     setrtxy
lffront:
        mov     ax,scr_x                ;no reach right boundary
        add     ax,40h                  ;add 1/5 screen
        cmp     ax,lf_txtx
        jna     shortjmp
        jmp     lfnolevel
shortjmp:
        test    ytouch,0ffh
        jnz     lfrt99
        jmp     lffront00
lfrt99:
        mov     ax,lf_width
        add     ax,lf_txtx              ;beyond the level
        cmp     ax,rt_txtx
        ja      lfrt10
        mov     ax,lf_width
        add     ax,lf_move              ;beyond the level
        sub     ax,rt_txtx
        jnc     lfrt00
        jmp     lffront00
lfrt00:
        mov     bx,lf_move
        sub     bx,ax
        mov     lf_txtx,bx
lfrt10:
        mov     ax,scr_x                ;touch another man
        add     ax,100h
        mov     bx,rt_width
        add     bx,rt_txtx
        cmp     bx,ax
        ja      lffrt00
        mov     ax,lf_move              ;another man front the level
        sub     ax,lf_txtx
        mov     bx,ax
        add     ax,rt_txtx
        mov     rt_txtx,ax
        add     bx,rt_move
        mov     rt_move,bx
        mov     ax,lf_move
        mov     lf_txtx,ax
        jmp     setrtxy
lffrt00:                                ;front the level
        mov     ax,lf_move              ;case 5
        sub     ax,lf_txtx
        mov     bx,ax
        add     ax,scr_x
        mov     scr_x,ax
        sub     ax,159
        jc      lffrt10
        mov     scr_x,159
lffrt10:
        mov     ax,rt_width
        add     ax,rt_txtx
        sub     ax,479                  ;479 = 320 + 159
        jc      lffrt20
        sub     bx,ax
lffrt20:
        add     rt_txtx,bx
        add     rt_move,bx
        add     lf_txtx,bx
        jmp     setrtxy
lfnolevel:
        test    ytouch,0ffh
        jz      lfnl00
        mov     ax,lf_txtx
        add     ax,lf_width
        cmp     ax,rt_txtx
        jna     lfnl00
        mov     ax,lf_move
        sub     ax,lf_txtx
        add     rt_txtx,ax
        add     rt_move,ax
lfnl00:
        mov     ax,lf_move              ;no reach level line
        sub     ax,lf_txtx
        shr     ax,1
        add     scr_x,ax
        cmp     scr_x,159
        jna     lffront00
        mov     scr_x,159
lffront00:                              ;no touch another man
        mov     ax,lf_move
        mov     lf_txtx,ax
        jmp     setrtxy
setlfback:                              ;left man back
        mov     ax,scr_x
        test    ax,0ffffh
        jnz     slfbk50
        test    lf_move,8000h
        jz      slfbk40
        mov     ax,lf_txtx
        mov     lf_move,ax
slfbk40:
        mov     ax,lf_move
        mov     lf_txtx,ax
        jmp     setrtxy
slfbk50:                                ;no reach end line
        mov     ax,scr_x
        add     ax,40h
        cmp     ax,lf_txtx
        ja      slfbk55
        mov     ax,lf_move
        mov     lf_txtx,ax
        jmp     setrtxy
slfbk55:
        mov     ax,scr_x                ;after level line
        add     ax,100h
        mov     bx,rt_width
        add     bx,rt_txtx
        cmp     bx,ax
        jna     slfbk60
;
        mov     ax,scr_x                ;inner level line
        add     ax,319
        mov     bx,rt_width
        add     bx,rt_txtx
        cmp     bx,ax
        jna     slfbk56
        mov     ax,scr_x
        cmp     ax,lf_move
        jc      slfbk70
        mov     lf_move,ax
slfbk70:
        jmp     overborder1
slfbk56:
        mov     ax,lf_txtx              ;scroll screen
        sub     ax,lf_move
        shr     ax,1
        jmp     slfbk80
slfbk60:                                ;another man front level line
        mov     ax,lf_txtx              ;case 12
        sub     ax,lf_move
slfbk80:
        sub     scr_x,ax
        jnc     slfbk82
        mov     scr_x,0
slfbk82:
        mov     bx,rt_txtx
        add     bx,rt_width
        mov     dx,scr_x
        add     dx,319
        cmp     bx,dx
        jna     slfbk84
        sub     bx,319
        mov     scr_x,bx
slfbk84:
        mov     ax,scr_x
        cmp     ax,lf_move
        jna     overborder1
        mov     lf_move,ax
overborder1:
        mov     ax,lf_move
        mov     lf_txtx,ax
setrtxy:                                ;right man control
        mov     ax,lf_txtx
        sub     ax,lf_px
        mov     lf_x,ax
        mov     ax,rt_txtx
        sub     ax,rt_px
        mov     rt_x,ax
setrtxy2:                                ;right man control
        mov     ax,rt_move
        cmp     ax,rt_txtx
        jnz     enterrtxy
        jmp     setxyend2
enterrtxy:
        jna     setrtfront
        jmp     srtxy200
setrtfront:
        mov     ax,scr_x
        test    ax,0ffffh
        jnz     srtxy100
        test    ytouch,0ffh
        jnz     srtxy99
        jmp     srtxy50
srtxy99:
        mov     ax,lf_width
        add     ax,lf_txtx              ;front already in botton
        cmp     ax,rt_txtx
        ja      srtxy10
        sub     ax,rt_move
        jnc     srtxy00
        jmp     srtxy50
srtxy00:
        add     ax,rt_move
        mov     rt_txtx,ax
srtxy10:
        test    lf_txtx,8000h
        jz      srtxy20
        jmp     setxyend
srtxy20:
        mov     ax,rt_txtx
        sub     ax,rt_move
        sub     lf_txtx,ax
        jnc     srtxy50
;       add     ax,lf_txtx
;       mov     bx,rt_txtx
;       sub     bx,ax
;       mov     rt_move,bx
        mov     ax,lf_txtx
        add     ax,lf_width
        mov     rt_move,ax
        mov     lf_txtx,0
srtxy50:                                ;no touch another man
        mov     ax,rt_move
        mov     rt_txtx,ax
        jmp     setxyend
srtxy100:                               ;not in end line
        mov     ax,scr_x
        add     ax,100h
        mov     bx,rt_txtx
        add     bx,rt_width
        cmp     bx,ax
        jna     srtxy101
        jmp     srtxy150
srtxy101:
        test    ytouch,0ffh
        jnz     rtynotouch0
        jmp     srtxy240
rtynotouch0:
        mov     ax,lf_width
        add     ax,lf_txtx              ;behine the level
        cmp     ax,rt_txtx
        ja      srtxy105
        sub     ax,rt_move
        jnc     srtxy102
        jmp     srtxy240
srtxy102:
        add     ax,rt_move
        mov     rt_txtx,ax
srtxy105:
        mov     ax,scr_x                ;touch man
        add     ax,40h
        cmp     ax,lf_txtx
        ja      srtxy110
        mov     ax,rt_txtx
        sub     ax,rt_move
        mov     bx,lf_txtx
        sub     bx,ax
        mov     lf_txtx,bx
        mov     ax,rt_move
        mov     rt_txtx,ax
        jmp     setxyend
srtxy110:                               ;another  man behine the level
        mov     ax,rt_txtx              ;case 5
        sub     ax,rt_move
        mov     bx,scr_x
        sub     bx,ax
        jnc     overborder
        mov     bx,0
overborder:
        mov     scr_x,bx
        sub     lf_txtx,ax
        jnc     srtxy120
        add     ax,lf_txtx
        mov     lf_txtx,0
srtxy120:
        sub     rt_txtx,ax
        jmp     setxyend
srtxy150:
        test    ytouch,0ffh
        jz      srtxy115
        mov     ax,lf_width
        add     ax,lf_txtx
        cmp     ax,rt_txtx
        jna     srtxy115
        mov     ax,rt_txtx              ;touch another man
        sub     ax,rt_move
        sub     lf_txtx,ax
srtxy115:
        mov     ax,rt_txtx
        sub     ax,rt_move
        shr     ax,1
        sub     scr_x,ax
        jnc     srtxy117
        mov     scr_x,0
srtxy117:
        test    ytouch,0ffh
        jz      srtxy130
        mov     ax,lf_width
        add     ax,lf_txtx
        sub     ax,rt_move
        jc      srtxy130
        sub     lf_txtx,ax
srtxy130:
        mov     ax,rt_move
        mov     rt_txtx,ax
        jmp     setxyend
srtxy200:                               ;right man back
        mov     ax,scr_x
        cmp     ax,158
        jna     srtxy250
        mov     ax,rt_move
        add     ax,rt_width
        cmp     ax,478
        jna     srtxy240
        mov     bx,rt_txtx
        mov     rt_move,bx
srtxy240:
        mov     ax,rt_move
        mov     rt_txtx,ax
        jmp     setxyend
srtxy250:                               ;back screen not in bottom
        mov     ax,scr_x
        add     ax,100h
        mov     bx,rt_width
        add     bx,rt_txtx
        cmp     bx,ax
        ja      srtxy252
        mov     ax,rt_move
        mov     rt_txtx,ax
        jmp     setxyend
srtxy252:
        mov     ax,scr_x                ;right man after level line
        add     ax,40h
        cmp     ax,lf_pretx
        jna     srtxy260
;
        mov     ax,scr_x
        cmp     ax,lf_pretx
        jc      srtxy257
        mov     bx,rt_width
        add     bx,rt_move
        mov     ax,scr_x
        add     ax,319
        cmp     bx,ax
        jc      srtxy255
        sub     ax,rt_width
        mov     rt_move,ax
srtxy255:
        jmp     srtxy280
srtxy257:
        mov     ax,rt_move
        sub     ax,rt_txtx
        shr     ax,1
        jmp     srtxy270
srtxy260:                               ;case 12
        mov     ax,rt_move
        sub     ax,rt_txtx
srtxy270:
        add     scr_x,ax
        cmp     scr_x,159
        jna     srtxy265
        mov     scr_x,159
srtxy265:
        mov     bx,lf_txtx
        cmp     bx,scr_x
        jnc     srtxy268
        mov     scr_x,bx
srtxy268:
        mov     ax,rt_move
        add     ax,rt_width
        mov     bx,scr_x
        add     bx,319
        cmp     ax,bx
        jna     srtxy280
        sub     bx,rt_width
        mov     rt_move,bx
srtxy280:
        mov     ax,rt_move
        mov     rt_txtx,ax
setxyend:
        mov     ax,rt_txtx
        sub     ax,rt_px
        mov     rt_x,ax
        mov     ax,lf_txtx
        sub     ax,lf_px
        mov     lf_x,ax
setxyend2:
        mov     ax,p1_txtx
        test    ax,8000h
        jz      preend1
        mov     ax,0
preend1:
        mov     bx,p2_txtx
        test    bx,8000h
        jz      preend2
        mov     bx,0
preend2:
        cmp     ax,bx
        ja      setxyend3
        mov     ax,lf_txtx
        mov     p1_txtx,ax
        mov     ax,rt_txtx
        mov     p2_txtx,ax
        mov     ax,lf_x
        mov     p1_x,ax
        mov     ax,rt_x
        mov     p2_x,ax
        jmp     setxyend4
setxyend3:
        mov     ax,rt_txtx
        mov     p1_txtx,ax
        mov     ax,lf_txtx
        mov     p2_txtx,ax
        mov     ax,lf_x
        mov     p2_x,ax
        mov     ax,rt_x
        mov     p1_x,ax
setxyend4:
        test    p1_direct,0ffh
        jz      xchgsubx
        xor     ax,ax
        mov     al,p1_width
        add     ax,p1_x
        mov     p1_subx,ax
xchgsubx:
        test    p2_direct,0ffh
        jz      xchgsubx2
        xor     ax,ax
        mov     al,p2_width
        add     ax,p2_x
        mov     p2_subx,ax
xchgsubx2:
        mov     ax,p1_y
        cmp     ax,p2_y
        jnc     p1higher
        mov     ax,p2_y
p1higher:
        mov     bx,Y_START
        shr     ax,1
        shr     ax,1
        shr     ax,1
        sub     bx,ax
        jnc     setceiling
        mov     bx,0
setceiling:
        mov     scr_y,bl
sethighend:
        ret
set_scrxy       ENDP
;  ==============================
;  =  Adjust x position         =
;  ==============================
adj_p1_x        PROC    NEAR
        xor     ax,ax
        mov     al,p1_width
        mov     lf_width,ax
        mov     al,p2_width
        mov     rt_width,ax
        mov     ax,p1_txtx
        mov     lf_txtx,ax
        mov     ax,p1_move
        mov     lf_move,ax
        mov     ax,p2_txtx
        mov     rt_txtx,ax
        mov     ax,p2_move
        mov     rt_move,ax
        mov     ax,p1_pretx
        mov     lf_pretx,ax
        mov     ax,p2_pretx
        mov     rt_pretx,ax
        call    adj_xposition
        mov     ax,lf_txtx
        mov     p1_txtx,ax
        sub     al,p1_px
        sbb     ah,0
        mov     p1_x,ax
        mov     ax,lf_move
        mov     p1_move,ax
        mov     ax,rt_txtx
        mov     p2_txtx,ax
        sub     al,p2_px
        sbb     ah,0
        mov     p2_x,ax
        mov     ax,rt_move
        mov     p2_move,ax
        ret
adj_p1_x        ENDP
;
adj_p2_x        PROC    NEAR
        xor     ax,ax
        mov     al,p1_width
        mov     rt_width,ax
        mov     al,p2_width
        mov     lf_width,ax
        mov     ax,p2_txtx
        mov     lf_txtx,ax
        mov     ax,p2_move
        mov     lf_move,ax
        mov     ax,p1_txtx
        mov     rt_txtx,ax
        mov     ax,p1_move
        mov     rt_move,ax
        mov     ax,p1_pretx
        mov     rt_pretx,ax
        mov     ax,p2_pretx
        mov     lf_pretx,ax
        call    adj_xposition
        mov     ax,lf_txtx
        mov     p2_txtx,ax
        sub     al,p2_px
        sbb     ah,0
        mov     p2_x,ax
        mov     ax,lf_move
        mov     p2_move,ax
        mov     ax,rt_txtx
        mov     p1_txtx,ax
        sub     al,p1_px
        sbb     ah,0
        mov     p1_x,ax
        mov     ax,rt_move
        mov     p1_move,ax
        ret
adj_p2_x        ENDP
;
adj_xposition:
        xor     dx,dx
        mov     ax,lf_txtx
        add     ax,lf_width
        mov     r0,ax
        mov     ax,rt_txtx
        add     ax,rt_width
        mov     r1,ax
        mov     ax,lf_pretx
        cmp     ax,r1
        ja      axn00
        mov     ax,rt_pretx
        cmp     ax,r0
        ja      axn00
        mov     dx,0ffffh               ;check bomb
axn00:
        test    lf_txtx,8000h
        jz      axn10
        mov     lf_txtx,0
        mov     lf_move,0
        test    dx,0ffffh
        jz      axn09
        test    rt_txtx,0ffffh
        jnz     axn05
        mov     ax,rt_txtx
        add     ax,rt_width
        mov     lf_txtx,ax
        mov     lf_move,ax
        jmp     axn99
axn05:
        mov     ax,lf_txtx
        add     ax,lf_width
        mov     rt_txtx,ax
        mov     rt_move,ax
axn09:
        jmp     axn99
axn10:  mov     ax,lf_txtx
        add     ax,lf_width
        cmp     ax,479
        jna     axn20
        mov     ax,479
        sub     ax,lf_width
        mov     lf_txtx,ax
        mov     lf_move,ax
        test    dx,0ffffh
        jz      axn19
        mov     ax,rt_txtx
        add     ax,rt_width
        cmp     ax,479
        jc      axn15
        mov     ax,rt_txtx
        sub     ax,lf_width
        mov     lf_txtx,ax
        mov     lf_move,ax
        jmp     axn99
axn15:
        mov     ax,lf_txtx
        sub     ax,rt_width
        mov     rt_txtx,ax
        mov     rt_move,ax
axn19:
        jmp     axn99
axn20:  test    dx,0ffffh
        jnz     axn25
        jmp     axn99
axn25:
        mov     ax,lf_txtx
        test    ax,8000h
        jz      axn26
        mov     ax,0
axn26:
        mov     bx,rt_txtx
        test    bx,8000h
        jz      axn27
        mov     bx,0
        mov     ax,1
axn27:
        cmp     ax,bx
        ja      axn50
        mov     ax,rt_txtx
        sub     ax,lf_width
        test    ax,8000h
        jz      axn30
        mov     lf_txtx,0
        mov     lf_move,0
        mov     ax,lf_width
        mov     rt_txtx,ax
        mov     rt_move,ax
        jmp     axn99
axn30:
        cmp     ax,scr_x
        ja      axn40
        mov     ax,scr_x
        add     ax,lf_width
        mov     rt_txtx,ax
        mov     rt_move,ax
        mov     ax,scr_x
axn40:
        mov     lf_txtx,ax
        mov     lf_move,ax
        jmp     axn99
axn50:
        mov     ax,rt_txtx
        add     ax,rt_width
        add     ax,lf_width
        cmp     ax,479
        jna     axn60
        mov     ax,479
        sub     ax,lf_width
        mov     lf_txtx,ax
        mov     lf_move,ax
        sub     ax,rt_width
        mov     rt_txtx,ax
        mov     rt_move,ax
        jmp     axn99
axn60:
        mov     bx,scr_x
        add     bx,319
        cmp     bx,ax
        jna     axn70
        sub     ax,lf_width
        mov     lf_txtx,ax
        mov     lf_move,ax
        jmp     axn99
axn70:
        sub     bx,lf_width
        mov     lf_txtx,bx
        mov     lf_move,bx
        sub     bx,rt_width
        mov     rt_txtx,bx
        mov     rt_move,bx
axn99:
        ret
;  ==============================
;  =  SET POINTER               =
;  ==============================
set_pointer     PROC    NEAR
        mov     ax,alloc3
        mov     p1_ptr,ax
        add     ax,24b5h
        mov     p2_ptr,ax
        ret
set_pointer     ENDP
;  ==============================
;  =  read files                =
;  ==============================
R_bgn   PROC    NEAR
; read  background
        mov     si,scene_no
        shl     si,1
        mov     dx,scenename[si]
        mov     ax,3d00h
        int     21h
        jnc     rbn05
        jmp     rbn10
rbn05:
        mov     fptr,ax
        mov     flength,0
        push    ds
        pop     es
        push    ds
        mov     ds,es:alloc3
rbn06:  xor     dx,dx
        mov     ax,es:fptr
        H_READ2 08000h,ax
        cmp     ax,0
        je      rbn07
        add     es:flength,ax
        mov     ax,ds
        add     ax,800h
        mov     ds,ax
        jmp     rbn06
rbn07:  pop     ds
        H_CLOSE fptr
; decompress background
        mov     ax,alloc0
        push    ds
        push    ax
        xor     ax,ax
        mov     si,ax
        push    ax
        mov     di,offset palette
        mov     dx,flength
        mov     ds,alloc3
        call    decompress
        pop     ax
        pop     ax
        pop     ds
; read score board
        H_OPEN  bdfile,00h
        jnc     rbn20
        jmp     rbn10
rbn20:
        mov     fptr,ax
        push    ds
        pop     es
        push    ds
        mov     ax,alloc0
        add     ax,1000h
        mov     ds,ax
        mov     dx,8000h
        mov     ax,es:fptr
        H_READ2 2800h,ax
        pop     ds
        H_CLOSE fptr
;
        H_OPEN  digfile,00h
        jc      rbn10
        mov     fptr,ax
        H_READ  second_buf,2100,fptr
        H_CLOSE fptr
        H_OPEN  starfile,00h
        jc      rbn10
        mov     fptr,ax
        H_READ  star_buf,1861,fptr
        H_CLOSE fptr
        jmp     rbn99
rbn10:
        PRINT   string2
rbn99:
        ret
R_bgn   ENDP
;  ===============================
;  =  read ai data               =
;  ===============================
R_ai    PROC    NEAR
        H_OPEN  ainame,00h
        jc      rai80
        mov     fptr,ax
        mov     cx,192
        mov     ax,c_level
        cmp     main_select,4
        jnz     rai10
        mov     ax,1
rai10:
        mul     cx
        mov     cx,dx
        mov     dx,ax
        mov     ax,4200h
        mov     bx,fptr
        int     21h
        H_READ  aitable,192,fptr
        H_CLOSE fptr
        jmp     rai90
rai80:  PRINT   string2
rai90:
        ret
R_ai    ENDP
;  ==============================
;  =  READ SHADOW  FILE         =
;  ==============================
R_shadow        PROC    NEAR
        H_OPEN  sdfile,00h
        jc      rsw10
        mov     fptr,ax
        H_READ  sdbuf,551,fptr
        H_CLOSE fptr
        jmp     rsw99
rsw10:  PRINT string2
rsw99:  ret
R_shadow        ENDP
;  ==============================
;  =  READ PATH                 =
;  ==============================
R_path          PROC    NEAR
        mov     si,p1_man
        shl     si,1
        mov     dx,pathname[si]         ;file open
        mov     ax,3d00h                ;
        int     21h                     ;
        jc      rph10
        mov     fptr,ax
        H_READ  p1_pth,1536,fptr
        H_CLOSE fptr
        jmp     rph20
rph10:  PRINT   string2
rph20:  mov     si,p2_man
        shl     si,1
        mov     dx,pathname[si]         ;file open
        mov     ax,3d00h                ;
        int     21h                     ;
        jc      rph40
        mov     fptr,ax
        H_READ  p2_pth,1536,fptr
        H_CLOSE fptr
        jmp     rph99
rph40:  PRINT   string2
rph99:                                  ;read database
        mov     si,p1_man
        shl     si,1
        mov     dx,attrname[si]          ;file open
        mov     ax,3d00h                ;
        int     21h                     ;
        jc      rar10
        mov     fptr,ax
        H_READ  p1_atrbuf,1224,fptr
        H_CLOSE fptr
        jmp     rar20
rar10:  PRINT   string2
rar20:
        mov     si,p2_man
        shl     si,1
        mov     dx,attrname[si]          ;file open
        mov     ax,3d00h                ;
        int     21h                     ;
        jc      rar30
        mov     fptr,ax
        H_READ  p2_atrbuf,1224,fptr
        H_CLOSE fptr
        jmp     rar99
rar30:  PRINT   string2
rar99:
        ret
R_path          ENDP
;  ==============================
;  =  Set Score board           =
;  ==============================
S_board         PROC    NEAR
        H_OPEN  namefile,00h
        jnc     sbd00
        PRINT   string2
        jmp     sbd90
sbd00:  mov     fptr,ax
        mov     ax,ds
        mov     es,ax
        push    ds
        mov     ax,p1_ptr
        mov     ds,ax
sbd10:  xor     dx,dx
        mov     ax,es:fptr
        H_READ2 1024,ax
        cmp     ax,0
        je      sbd20
        mov     ax,ds
        add     ax,40h
        mov     ds,ax
        jmp     sbd10
sbd20:  pop     ds
        H_CLOSE fptr
        push    ds
        mov     ax,alloc0
        add     ax,1000h
        mov     es,ax
        mov     bx,p1_man
        shl     bx,1
        mov     ax,p1_ptr
        mov     ds,ax
        mov     si,[bx]
        mov     di,8000h+6738
        call    putdata
        pop     ds
;
        test    p1_win,0ffffh
        jz      fp1boxend
        xor     dx,dx
fp1boxmain:
        inc     dx
        push    ds
        mov     ax,alloc0
        add     ax,1000h
        mov     es,ax
        mov     bx,22
        mov     ax,p1_ptr
        mov     ds,ax
        mov     si,[bx]
        mov     di,8000h+6
        mov     ax,12
        mul     dl
        add     di,ax
        call    putdata
        pop     ds
        cmp     dx,p1_win
        jc      fp1boxmain
fp1boxend:
;
        push    ds
        mov     ax,alloc0
        add     ax,1000h
        mov     es,ax
        mov     bx,p2_man
        shl     bx,1
        mov     ax,p1_ptr
        mov     ds,ax
        mov     si,[bx]
        mov     di,8000h+7022
        lodsw
        mov     cx,ax
        xor     ch,ch
        sub     di,cx
        mov     si,[bx]
        call    putdata
        pop     ds
sbd90:
;
        test    p2_win,0ffffh
        jz      fp2boxend
        xor     dx,dx
fp2boxmain:
        inc     dx
        push    ds
        mov     ax,alloc0
        add     ax,1000h
        mov     es,ax
        mov     bx,22
        mov     ax,p1_ptr
        mov     ds,ax
        mov     si,[bx]
        mov     di,8000h+304
        mov     ax,12
        mul     dl
        sub     di,ax
        call    putdata
        pop     ds
        cmp     dx,p2_win
        jc      fp2boxmain
fp2boxend:
        call    set_round
        ret
;
set_round:
        push    ds
        mov     bx,12
        shl     bx,1
        mov     es,alloc1
        xor     di,di
        mov     ax,p1_ptr
        mov     ds,ax
        mov     si,[bx]
        cld
        mov     al,00           ;clear buffer
        mov     cx,1442         ;
        rep     stosb           ;
        xor     di,di           ;
        mov     ax,0f60h
        stosw
        lodsw
        mov     cx,ax
        mov     bl,ah
        xor     ch,ch
        push    di
        push    cx
srd00:  rep     movsb
        pop     cx
        pop     di
        push    di
        push    cx
        mov     ax,es
        add     ax,06h
        mov     es,ax
        dec     bl
        jnz     srd00
        pop     cx
        pop     di
        pop     ds
;
        mov     si,round
        inc     si
        shl     si,1
        mov     ax,second_ptr[si]
        add     ax,offset second_buf
        mov     si,ax
        mov     es,alloc1
        mov     di,83
        cld
        mov     cx,14
        mov     bl,15
        push    di
        push    cx
srd10:  rep     movsb
        pop     cx
        pop     di
        push    di
        push    cx
        mov     ax,es
        add     ax,06h
        mov     es,ax
        dec     bl
        jnz     srd10
        pop     cx
        pop     di
;  set fight
        push    ds
        mov     es,alloc1
        mov     bx,26
        mov     ax,p1_ptr
        mov     ds,ax
        mov     si,[bx]
        mov     di,96*15+2
        mov     cx,96*15+2
        rep     movsb
        pop     ds
        ret
S_board         ENDP
;
f_round PROC    NEAR
        push    ds
        mov     es,alloc1
        mov     cx,timecnt
        shl     cx,1
        mov     bx,48
        sub     bx,timecnt
        jnc     fnd00
        mov     bx,0
        mov     cx,96
fnd00:
        mov     ax,alloc1
        mov     ds,ax
        mov     si,bx
        mov     di,17920+112
        add     di,bx
        call    putround
        pop     ds
        test    round_flag,0ffffh
        jnz     fnd10
        add     timecnt,4
        cmp     timecnt,40h
        jna     fnd05
        inc     round_flag
fnd05:  jmp     fnd99
fnd10:  cmp     round_flag,1
        jnz     fnd20
        sub     timecnt,4
        test    timecnt,0ffffh
        jnz     fnd15
        mov     timecnt,4
        inc     round_flag
        push    ds
        mov     es,alloc1
        mov     ds,alloc1
        mov     si,96*15+2
        xor     di,di
        mov     cx,96*15+2
        rep     movsb
        pop     ds
fnd15:  jmp     fnd99
fnd20:  cmp     round_flag,2
        jnz     fnd30
        add     timecnt,4
        cmp     timecnt,40h
        jna     fnd25
        inc     round_flag
fnd25:  jmp     fnd99
fnd30:  sub     timecnt,4
        test    timecnt,0ffffh
        jnz     fnd99
        mov     round_flag,0ffffh
fnd99:
        ret
;
putround:
        cld
        lodsw
        mov     bl,15
        push    si
        push    di
        push    cx
pnd10:  lodsb
        test    al,0ffh
        jz      pnd20
        or      al,0a0h
        stosb
        jmp     pnd30
pnd20:  inc     di
pnd30:  dec     cx
        jnz     pnd10
        pop     cx
        pop     di
        pop     si
        push    si
        push    di
        push    cx
        mov     ax,es
        add     ax,14h
        mov     es,ax
        mov     ax,ds
        add     ax,06h
        mov     ds,ax
        dec     bl
        jnz     pnd10
        pop     cx
        pop     di
        pop     si
        ret
f_round ENDP
;  putdata
;  source ds:si destination es:di
;  destroy ax,bl,cx
putdata PROC    NEAR
        cld
        lodsw
        mov     cx,ax
        mov     bl,ah
        xor     ch,ch
        push    di
        push    cx
pda10:  rep     movsb
        pop     cx
        pop     di
        push    di
        push    cx
        mov     ax,es
        add     ax,14h
        mov     es,ax
        dec     bl
        jnz     pda10
        pop     cx
        pop     di
        ret
putdata ENDP
;
;  putdata2 zero will pass
;  source ds:si destination es:di
;  bh = add color value
;  destroy ax,bl,cx
putdata2        PROC    NEAR
        cld
        lodsw
        mov     cx,ax
        mov     bl,ah
        xor     ch,ch
        push    di
        push    cx
pd210:  lodsb
        test    al,0fh
        jz      pd220
        or      al,bh
        stosb
        jmp     pd230
pd220:  inc     di
pd230:  loop    pd210
        pop     cx
        pop     di
        push    di
        push    cx
        mov     ax,es
        add     ax,14h
        mov     es,ax
        dec     bl
        jnz     pd210
        pop     cx
        pop     di
        ret
putdata2        ENDP
;  ==============================
;  =  Round end proc            =
;  ==============================
round_end_proc  PROC    NEAR
        test    round_flag,0ffffh
        jnz     rep10
        mov     j1_joy,0
        mov     j2_joy,0
        inc     round_flag
rep10:  test    round_flag,8000h
        jz      rep11
        jmp     rep40
rep11:  test    p1_die,0ffh
        jz      rep13
        test    p1_blood,0ffh
        jz      rep12
        jmp     rep99
rep12:  cmp     p1_die,80h
        jz      rep15
        jmp     rep99
rep13:  cmp     p1_pthno,0
        jz      rep15
        jmp     rep99
rep15:  test    p2_die,0ffh
        jz      rep18
        test    p2_blood,0ffh
        jz      rep17
        jmp     rep99
rep17:  cmp     p2_die,80h
        jnz     rep99
        jmp     rep20
rep18:  cmp     p2_pthno,0
        jnz     rep99
rep20:  mov     round_flag,08000h
        mov     p1_airflag,0
        mov     p2_airflag,0
        mov     ax,p1_blood
        sub     ax,p1_losenow
        mov     bx,p2_blood
        sub     bx,p2_losenow
        cmp     ax,bx
        jz      rep80
        ja      rep30
        inc     p2_win
        call    l_p2win
        mov     ax,p2_man
        or      ax,8000h
        mov     who_win,ax
        jmp     rep99
rep30:  inc     p1_win
        call    l_p1win
        mov     ax,p1_man
        mov     who_win,ax
        jmp     rep99
rep40:  test    p1_win,0ffffh
        jz      rep45
        test    p1_type,64
        jnz     rep80
rep45:  test    p2_win,0ffffh
        jz      rep99
        test    p2_type,64
        jz      rep99
rep80:  mov     round_flag,0ffffh
rep99:
        ret
;
l_p1win:
        mov     si,p1_man
        shl     si,1
        mov     dx,winname[si]          ;file open
        mov     ax,3d00h                ;
        int     21h                     ;
        jc      lp110
        mov     fptr,ax
        mov     ax,4200h
        mov     bx,fptr
        mov     cx,0
        mov     dx,32*3
        int     21h
        mov     dx,offset p1_ptrbuf
        H_READ2 72*4,fptr
        mov     ax,ds
        mov     es,ax
        push    ds
        mov     ax,p1_ptr
        mov     ds,ax
lp1start:
        xor     dx,dx
        mov     ax,es:fptr
        H_READ2 8192,ax
        cmp     ax,0
        je      lp1end
        mov     ax,ds
        add     ax,200h
        mov     ds,ax
        jmp     lp1start
lp1end:
        pop     ds
        H_CLOSE fptr
        jmp     lp150
lp110:  PRINT string2
lp150:
        mov     si,p1_man
        shl     si,1
        mov     dx,winpath[si]         ;file open
        mov     ax,3d00h                ;
        int     21h                     ;
        jc      lp160
        mov     fptr,ax
        H_READ  p1_pth,1536,fptr
        H_CLOSE fptr
        jmp     lp170
lp160:  PRINT string2
lp170:  mov     al,random
        and     al,01
        mov     p1_pthno,al
        mov     p1_stop,0
        mov     p1_pthcnt,0
        ret
;
l_p2win:
        mov     si,p2_man
        shl     si,1
        mov     dx,winname[si]          ;file open
        mov     ax,3d00h                ;
        int     21h                     ;
        jc      lp210
        mov     fptr,ax
        mov     ax,4200h
        mov     bx,fptr
        mov     cx,0
        mov     dx,32*3
        int     21h
        mov     dx,offset p2_ptrbuf
        H_READ2 72*4,fptr
        mov     ax,ds
        mov     es,ax
        push    ds
        mov     ax,p2_ptr
        mov     ds,ax
lp2start:
        xor     dx,dx
        mov     ax,es:fptr
        H_READ2 8192,ax
        cmp     ax,0
        je      lp2end
        mov     ax,ds
        add     ax,200h
        mov     ds,ax
        jmp     lp2start
lp2end:
        pop     ds
        H_CLOSE fptr
        jmp     lp220
lp210:  PRINT string2
lp220:
        mov     si,p2_man
        shl     si,1
        mov     dx,winpath[si]         ;file open
        mov     ax,3d00h                ;
        int     21h                     ;
        jc      lp230
        mov     fptr,ax
        H_READ  p2_pth,1536,fptr
        H_CLOSE fptr
        jmp     lp240
lp230:  PRINT   string2
lp240:  mov     al,random               ;read database
        and     al,01
        mov     p2_pthno,al
        mov     p2_stop,0
        mov     p2_pthcnt,0
        ret
round_end_proc  ENDP
;  ==============================
;  =  Final control             =
;  ==============================
f_control       PROC    NEAR
        mov     ax,p1_blood
        cmp     ax,p2_blood
        jnz     fctl10          ;Draw game
        push    ds
        mov     ax,0a000h
        mov     es,ax
        mov     bx,32   ;16 * 2
        mov     ax,p1_ptr
        mov     ds,ax
        mov     si,[bx]
        mov     di,17280+110
        mov     bh,160
        call    putdata2
        pop     ds
        jmp     fctl99
fctl10:
        cmp     p1_pannel,04h
        jz      fctl50
        cmp     p2_pannel,04h
        jz      fctl50          ; 2 player
        push    ds
        mov     ax,0a000h
        mov     es,ax
        mov     bx,who_win      ; win number
        and     bx,00ffh
        shl     bx,1
        mov     ax,p1_ptr
        mov     ds,ax
        mov     si,[bx]
        lodsw
        pop     ds
        push    ds
        xor     ah,ah
        push    ax
        add     ax,40
        shr     ax,1
        mov     cx,160
        sub     cx,ax
        pop     ax
        mov     r0,cx
        add     r0,ax
        add     r0,9                    ; win pos
        mov     di,17280
        add     di,cx
        mov     ax,p1_ptr
        mov     ds,ax
        mov     si,[bx]
        mov     bh,160
        call    putdata2
        pop     ds
        push    ds
        mov     ax,0a000h
        mov     es,ax
        mov     di,17280
        add     di,r0
        mov     bx,34   ;17 * 2
        mov     ax,p1_ptr
        mov     ds,ax
        mov     si,[bx]
        mov     bh,160
        call    putdata2
        pop     ds
        jmp     fctl99
fctl50:                 ; single game
        mov     ax,who_win
        cmp     ax,p1_man
        jnz     fctl55
        mov     bx,28
        jmp     fctl60
fctl55: mov     bx,30
fctl60: mov     ax,0a000h
        mov     es,ax
        push    ds
        mov     ax,p1_ptr
        mov     ds,ax
        mov     si,[bx]
        lodsw
        xor     ah,ah
        shr     ax,1
        mov     cx,160
        sub     cx,ax
        mov     di,17280
        add     di,cx
        mov     si,[bx]
        mov     bh,160
        call    putdata2
        pop     ds
        mov     ax,who_win
        cmp     ax,p1_man
        jnz     fctl99
        call    C_score
fctl99:
        ret
f_control       ENDP
;  ==============================
;  =  Calculate score           =
;  ==============================
C_score PROC    NEAR
;  clear "you win"
        mov     cx,27
        call    Delay
        call    f_scr2
;  fill "score" "time" "blood" "bonus"
        push    ds
        mov     ax,alloc1
        mov     es,ax
        mov     bx,42   ;21 * 2
        mov     ax,p1_ptr
        mov     ds,ax
        mov     si,[bx]
        mov     di,17280+80
        mov     bh,160
        call    putdata2
        pop     ds
;
        push    ds
        mov     ax,alloc1
        mov     es,ax
        mov     bx,36   ;18 * 2
        mov     ax,p1_ptr
        mov     ds,ax
        mov     si,[bx]
        mov     di,17280+7360+80
        mov     bh,160
        call    putdata2
        pop     ds
;
        push    ds
        mov     ax,alloc1
        mov     es,ax
        mov     bx,38   ;19 * 2
        mov     ax,p1_ptr
        mov     ds,ax
        mov     si,[bx]
        mov     di,17280+12480+80
        mov     bh,160
        call    putdata2
        pop     ds
;
        push    ds
        mov     ax,alloc1
        mov     es,ax
        mov     bx,40   ;20 * 2
        mov     ax,p1_ptr
        mov     ds,ax
        mov     si,[bx]
        mov     di,17280+19520+80
        mov     bh,160
        call    putdata2
        pop     ds
; fill score value
        mov     ax,second
        mov     dx,100
        mul     dx
        mov     r2,ax
        mov     r3,dx
        call    ltostr
        mov     di,17280+7360+168
        mov     ax,alloc1
        mov     es,ax
        call    f_digital
        mov     ax,p1_blood
        mov     dx,100
        mul     dx
        add     r2,ax
        adc     r3,dx
        call    ltostr
        mov     di,17280+12480+168
        mov     ax,alloc1
        mov     es,ax
        call    f_digital
;  backup pic
        push    ds
        mov     ax,alloc0
        mov     es,ax
        xor     di,di
        xor     si,si
        mov     ax,alloc1
        mov     ds,ax
        mov     cx,08000h
        rep     movsw
        pop     ds
;
        call    Sub_bonus
        call    f_scr2
        mov     cx,18
        call    delay
csre0:  call    Sub_bonus
        call    f_scr2
;  restore pic
        push    ds
        mov     ax,alloc1
        mov     es,ax
        xor     di,di
        xor     si,si
        mov     ax,alloc0
        mov     ds,ax
        mov     cx,08000h
        rep     movsw
        pop     ds
; sub score to score buffer
        call    read_ctl
        test    j1_trg,30h
        jz      csre5
        mov     ax,r2
        mov     cx,r3
        add     score_lo,ax
        adc     score_hi,cx
        mov     r2,0
        mov     r3,0
        jmp     csre0
csre5:  add     score_lo,100
        adc     score_hi,0
        sub     r2,100
        sbb     r3,0
        jnc     csre0
        sub     score_lo,100
        sbb     score_hi,0
        mov     cx,36
        call    delay
        ret
;
Sub_bonus:
;  fill score value
        mov     ax,score_lo
        mov     dx,score_hi
        call    ltostr
        mov     di,17280+168
        mov     ax,alloc1
        mov     es,ax
        call    f_digital
; fill bonus value
        mov     ax,r2
        mov     dx,r3
        call    ltostr
        mov     di,17280+19520+168
        mov     ax,alloc1
        mov     es,ax
        call    f_digital
        ret
C_score ENDP
;  ==============================
;  =  Fill digital              =
;  ==============================
f_digital       PROC    NEAR
        push    ds
        mov     bx,offset j1_joybuf
        push    bx
        mov     bx,44
        mov     ax,p1_ptr
        mov     ds,ax
        mov     si,[bx]
        add     si,2
        pop     bx
        mov     dl,6
        push    es
        push    si
fdg10:  push    ds
        mov     ax,@data
        mov     ds,ax
        mov     al,[bx+5]
        pop     ds
        cmp     al,0ffh
        jz      fdg40
        mov     ah,99
        mul     ah
        add     si,ax
        push    di
        mov     ch,11
        mov     cl,9
fdg15:  lodsb
        test    al,0fh
        jz      fdg20
        or      al,160
        stosb
        jmp     fdg30
fdg20:  inc     di
fdg30:  dec     cl
        jnz     fdg15
        mov     cl,9
        pop     di
        push    di
        mov     ax,es
        add     ax,14h
        mov     es,ax
        dec     ch
        jnz     fdg15
        pop     di
fdg40:  add     di,12
        pop     si
        pop     es
        push    es
        push    si
        dec     bx
        dec     dl
        jnz     fdg10
        pop     si
        pop     es
        pop     ds
        ret
f_digital       ENDP
;  ==============================
;  =  double word to buffer     =
;  =  8 digitals high zero pass =
;  =  in  dx:ax  out j1_joybuf  =
;  ==============================
ltostr  PROC    NEAR
        mov     bx,10
        mov     cx,6
        mov     si,0
lstr0:  div     bx
        mov     j1_joybuf[si],dl
        inc     si
        xor     dx,dx
        loop    lstr0
        mov     cx,5
        mov     si,5
lstr2:  test    j1_joybuf[si],0ffh
        jnz     lstr9
        mov     j1_joybuf[si],0ffh
        dec     si
        loop    lstr2
lstr9:
        ret
ltostr  ENDP
;  ==============================
;  =  READ MAN                  =
;  ==============================
R_man           PROC    NEAR
        mov     si,p1_man
        shl     si,1
        mov     dx,figname[si]          ;file open
        mov     ax,3d00h                ;
        int     21h                     ;
        jc      rmn10
        mov     fptr,ax
        mov     dx,offset palette
        add     dx,192*3
        H_READ2 32*3,ax
        mov     dx,offset p1_ptrbuf
        H_READ2 72*4,fptr
        mov     ax,ds
        mov     es,ax
        push    ds
        mov     ax,p1_ptr
        mov     ds,ax
rp1start:
        xor     dx,dx
        mov     ax,es:fptr
        H_READ2 8192,ax
        cmp     ax,0
        je      rp1end
        mov     ax,ds
        add     ax,200h
        mov     ds,ax
        jmp     rp1start
rp1end:
        pop     ds
        H_CLOSE fptr
        jmp     rmn50
rmn10:  PRINT string2
rmn50:
        mov     si,p2_man
        shl     si,1
        mov     dx,figname[si]          ;file open
        mov     ax,3d00h                ;
        int     21h                     ;
        jc      rmn60
        mov     fptr,ax
        mov     dx,offset palette
        add     dx,224*3
        H_READ2 32*3,ax
        mov     dx,offset p2_ptrbuf
        H_READ2 72*4,fptr
        mov     ax,ds
        mov     es,ax
        push    ds
        mov     ax,p2_ptr
        mov     ds,ax
rp2start:
        xor     dx,dx
        mov     ax,es:fptr
        H_READ2 8192,ax
        cmp     ax,0
        je      rp2end
        mov     ax,ds
        add     ax,200h
        mov     ds,ax
        jmp     rp2start
rp2end:
        pop     ds
        H_CLOSE fptr
        jmp     rmn70
rmn60:  PRINT string2
rmn70:
        H_OPEN  bdcol,00h
        jc      fbd80
        mov     fptr,ax
        mov     dx,offset palette
        add     dx,160*3
        H_READ2 32*3,ax
        H_CLOSE fptr
        jmp     fbd90
fbd80:  PRINT string2
fbd90:  mov     ax,p1_man
        cmp     ax,p2_man
        jnz     fbd199
        H_OPEN  man2col,00h
        jc      fbd180
        mov     fptr,ax
        mov     ax,p1_man
        mov     bx,48
        mul     bx
        mov     cx,dx
        mov     dx,ax
        mov     ax,4200h
        mov     bx,fptr
        int     21h
        mov     ax,fptr
        mov     dx,offset palette
        add     dx,224*3
        H_READ2 16*3,ax
        H_CLOSE fptr
        jmp     fbd199
fbd180:  PRINT string2
fbd199:
        ret
R_man           ENDP

;  ==============================
;  =  fill colors               =
;  ==============================
f_color PROC    NEAR
        mov     dx,3c8h
        mov     al,00h
        out     dx,al
        mov     cx,768
        mov     dx,3c9h
        mov     bx,offset palette
fcr10:
        mov     al,[bx]
        out     dx,al
        inc     bx
        loop    fcr10
        ret
f_color ENDP
;  ==============================
;  =  fill screen buffer        =
;  ==============================
f_sbuf  PROC    NEAR
        push    ds
        mov     es,alloc1               ;fill into screen buffer
        mov     dx,scr_x
        mov     ax,alloc0
        mov     ds,ax
        xor     di,di
        mov     bx,204
fsf00:
        mov     si,dx
        mov     cx,0160
        cld
        rep     movsw
        add     ax,1eh
        mov     ds,ax
        dec     bx
        jnz     fsf00
        pop     ds
        ret
f_sbuf  ENDP
;
f_sbuf2 PROC    NEAR
        push    ds
        xor     ax,ax
        mov     al,scr_y
        mov     bx,320
        mul     bx
        mov     di,ax
        xor     ax,ax
        mov     al,scr_y
        mov     bx,480
        mul     bx
        add     ax,scr_x
        mov     dx,ax
        mov     es,alloc1               ;fill into screen buffer
        mov     ax,alloc0
        mov     ds,ax
        mov     bx,192
fsf20:
        mov     si,dx
        mov     cx,0160
        cld
        rep     movsw
        add     ax,1eh
        mov     ds,ax
        dec     bx
        jnz     fsf20
        pop     ds
        ret
f_sbuf2 ENDP
;  ==============================
;  =  fill screen               =
;  ==============================
f_scr   PROC    NEAR
        push    ds
        mov     ds,alloc1
        mov     si,0
        mov     ax,0a000h
        mov     es,ax
        mov     di,0
        mov     cx,8000h
        cld
        rep     movsw
        pop     ds
        ret
f_scr   ENDP
;
f_scr2  PROC    NEAR
;       VBLANK
        mov     ah,scr_y
        mov     al,80
        mul     ah
        mov     scan_addr,ax
        SETADR  scan_addr
;
        push    ds
        xor     si,si
        mov     ax,0a000h
        mov     es,ax
        mov     ds,alloc1
        mov     di,0
        mov     cx,8000h
        cld
        rep     movsw
        pop     ds
        ret
f_scr2  ENDP
;
shake   PROC    NEAR
        VBLANK
        mov     ah,scr_y
        sub     ah,3
        mov     al,80
        mul     ah
        add     ax,page_addr
        mov     scan_addr,ax
        SETADR  scan_addr
        mov     cx,0ffffh
delay0: loop    delay0
        mov     cx,08000h
delay1: loop    delay1
        VBLANK
        mov     ah,scr_y
        mov     al,80
        mul     ah
        add     ax,page_addr
        mov     scan_addr,ax
        SETADR  scan_addr
        ret
shake   ENDP
c_scr   PROC    NEAR
        mov     ax,0a000h
        mov     es,ax
        xor     di,di
        mov     cx,0ffffh
        mov     al,15
        rep     stosb
        ret
c_scr   ENDP
;  =================================
;  =  FILL SECOND                  =
;  =================================
f_second        PROC    NEAR
        mov     ax,counter
        and     ax,DELAY_RATE
        cmp     ax,DELAY_RATE
        jz      f_smain
        ret
f_smain:
        test    second,0ffffh
        jz      d_second
        dec     second
d_second:
        mov     ax,alloc0
        add     ax,1000h
        mov     es,ax
        mov     ax,second
        aam
        push    ax
        mov     al,ah
        xor     ah,ah
        shl     ax,1
        mov     si,ax
        mov     ax,second_ptr[si]
        add     ax,offset second_buf
        mov     si,ax
        mov     di,8000h+3025
        mov     bx,15
        cld
fdig1:  mov     cx,14
        rep     movsb
        mov     ax,es
        add     ax,14h
        mov     es,ax
        mov     di,8000h+3025
        dec     bx
        jnz     fdig1
        pop     ax
        xor     ah,ah
        shl     ax,1
        mov     si,ax
        mov     ax,second_ptr[si]
        add     ax,offset second_buf
        mov     si,ax
        mov     ax,alloc0
        add     ax,1000h
        mov     es,ax
        mov     di,8000h+3025+16
        mov     bx,15
fdig2:  mov     cx,14
        rep     movsb
        mov     ax,es
        add     ax,14h
        mov     es,ax
        mov     di,8000h+3025+16
        dec     bx
        jnz     fdig2
        ret
f_second        ENDP
;  =================================
;  =  FILL SCORE BOARD             =
;  =================================
f_board PROC    NEAR
        push    ds
        mov     es,alloc1
        xor     ax,ax
        mov     al,scr_y
        mov     bx,320
        mul     bx
        mov     di,ax           ;calc score board addr
;
        mov     ax,alloc0
        add     ax,1000h
        mov     ds,ax
        mov     si,8000h
        mov     cx,1400h
fbd00:  lodsw
        cmp     ax,0
        jz      fbd10
        cmp     al,0
        jz      fbd05
;
        or      al,0a0h
        stosb
        jmp     fbd08
fbd05:  inc     di
fbd08:  mov     al,ah
        cmp     al,0
        jz      fbd15
;
        or      al,0a0h
        stosb
        jmp     fbd20
fbd10:  inc     di
fbd15:  inc     di
fbd20:  loop    fbd00
        pop     ds
        ret
f_board ENDP
;
;  =================================
;  =  Decrease bloood              =
;  =================================
d_blood PROC    NEAR
        test    p1_sleep,0ffffh
        jz      dp1_bloodchk
        mov     ax,counter
        and     ax,0fh
        cmp     ax,0fh
        jnz     dbdp1
        dec     p1_sleep
dbdp1:  inc     p1_sleepcnt
        cmp     p1_sleepcnt,SLEEP_CNT
        jna     dp1_bloodchk
        mov     p1_sleep,0
        mov     p1_sleepcnt,0
dp1_bloodchk:
        test    p1_losenow,0ffh
        jnz     dp1_blood
        jmp     dp1_bloodend
dp1_blood:
        call    dis_p1_blood
        dec     p1_losenow
        dec     p1_blood
        test    p1_losenow,0ffh
        jz      dp1_bloodend
        call    dis_p1_blood
        dec     p1_losenow
        dec     p1_blood
dp1_bloodend:
;
        test    p2_sleep,0ffffh
        jz      dp2_bloodchk
        mov     ax,counter
        and     ax,0fh
        cmp     ax,0fh
        jnz     dbdp2
        dec     p2_sleep
dbdp2:  inc     p2_sleepcnt
        cmp     p2_sleepcnt,SLEEP_CNT
        jna     dp2_bloodchk
        mov     p2_sleep,0
        mov     p2_sleepcnt,0
dp2_bloodchk:
        test    p2_losenow,0ffh
        jnz     dp2_blood
        jmp     dp2_bloodend
dp2_blood:
        call    dis_p2_blood
        dec     p2_losenow
        dec     p2_blood
        test    p2_losenow,0ffh
        jz      dp2_bloodend
        call    dis_p2_blood
        dec     p2_losenow
        dec     p2_blood
dp2_bloodend:
        ret
;
dis_p1_blood:
        mov     bx,alloc0
        add     bx,1000h
        mov     es,bx
        mov     di,8000h+4303           ;4306 = 13*320+146
        sub     di,p1_blood
        mov     bx,di
        mov     al,173
        mov     cx,6
fp1_blood:
        stosb
        add     bx,320
        mov     di,bx
        loop    fp1_blood
        ret
;
dis_p2_blood:
        mov     ax,alloc0
        add     ax,1000h
        mov     es,ax
        mov     di,8000h+4336
        add     di,p2_blood
        mov     bx,di
        mov     al,173
        mov     cx,6
fp2_blood:
        stosb
        add     bx,320
        mov     di,bx
        loop    fp2_blood
        ret
d_blood ENDP
;
f_blood PROC    NEAR
        mov     bx,alloc0
        add     bx,1000h
        mov     es,bx
        mov     di,8000h+4303           ;4306 = 13*320+146
        sub     di,p1_blood
        mov     bx,di
        mov     al,174
        mov     cx,6
f1_blood:
        stosb
        add     bx,320
        mov     di,bx
        loop    f1_blood
;
        mov     ax,alloc0
        add     ax,1000h
        mov     es,ax
        mov     di,8000h+4336
        add     di,p2_blood
        mov     bx,di
        mov     al,174
        mov     cx,6
f2_blood:
        stosb
        add     bx,320
        mov     di,bx
        loop    f2_blood
        ret
f_blood ENDP
;  =================================
;  =  FILL SHADOW                  =
;  =================================
f_shadow        PROC    NEAR
        xor     bx,bx
        mov     es,alloc1
        mov     di,0de80h
        test    p1_direct,0ffh
        jz      fp1shadow1
        mov     ax,p1_subx
        sub     ax,003dh        ;shadow width
        jmp     fp1shadow2
fp1shadow1:
        mov     ax,p1_x
fp1shadow2:
        add     al,p1_px
        adc     ah,0
        call    fshadowmain
;  fill p2 shadow
        xor     bx,bx
        mov     es,alloc1
        mov     di,0de80h       ;calc score board addr
        test    p2_direct,0ffh
        jz      fp2shadow1
        mov     ax,p2_subx
        sub     ax,003dh        ;shadow width
        jmp     fp2shadow2
fp2shadow1:
        mov     ax,p2_x
fp2shadow2:
        add     al,p2_px
        adc     ah,0
fshadowmain:
        push    ax
        sub     ax,scr_x
        test    ax,8000h
        jz      shadow1_xin
;shadow1_xout:
        xor     ax,0ffffh
        inc     ax
        mov     bl,al
        mov     ax,0
shadow1_xin:
        add     di,ax
        mov     r0,di
        pop     ax
        add     ax,003dh
        mov     cx,scr_x
        add     cx,319
        sub     cx,ax
        jnc     shadow1_xrtin
        xor     cx,0ffffh
        inc     cx
        mov     bh,cl
shadow1_xrtin:
;
        mov     si,offset sdbuf
        cld
        xor     cx,cx
        lodsw
        mov     cl,al
        mov     dl,ah
        sub     cl,bl
        sbb     cl,bh
        test    cl,80h
        jz      fswx00
        jmp     fp1shadowend
fswx00:
        mov     al,bh
        xor     bh,bh
        add     si,bx
        add     bl,al
        adc     bh,0
        mov     r1,cx
fsw00:  lodsb
        cmp     al,0
        jz      fsw10
;
        or      al,0a0h
        stosb
        jmp     fsw20
fsw10:  inc     di
fsw20:  loop    fsw00
        mov     cx,r1
        mov     di,r0
        add     si,bx
        mov     ax,es
        add     ax,14h
        mov     es,ax
        dec     dl
        jnz     fsw00
fp1shadowend:
        ret
f_shadow        ENDP
;  =================================
;  =  FILL MAN                     =
;  =================================
f_man           PROC    NEAR
        test    priority,0ffh
        jnz     fmanp2
        call    f_man2
        call    f_man1
        jmp     fman99
fmanp2: call    f_man1
        call    f_man2
fman99: call    f_air
        ret
f_man           ENDP
;
f_man1          PROC    NEAR
        push    ds
        mov     es,alloc1
        xor     dx,dx
        mov     bx,dx
        mov     ax,p1_txty
        test    ax,8000h
        jz      man1_yin
        xor     ax,0ffffh
        inc     ax
        mov     dl,al
        mov     ax,0
man1_yin:
        push    dx
        mov     di,320
        mul     di
        mov     di,ax
        pop     dx
        mov     ax,p1_txtx
        sub     ax,scr_x
;
        test    ax,8000h
        jz      man1_xin
man1_xout:
        xor     ax,0ffffh
        inc     ax
        mov     bl,al
        mov     ax,0
man1_xin:
        add     di,ax
        mov     ax,p1_txtx
        add     al,p1_width
        adc     ah,0
        mov     cx,scr_x
        add     cx,319
        sub     cx,ax
        jnc     man1_xrtin
        xor     cx,0ffffh
        add     cx,1
        mov     bh,cl
man1_xrtin:
        push    bx
        xor     bx,bx
        mov     bl,p1_sprite
        shl     bx,1
        shl     bx,1
;
        mov     dh,p1_type
        and     dh,10h
        add     dh,0c0h
        mov     cl,p1_attr
        mov     al,p1_direct
        and     al,40h
        xor     cl,al
        mov     ax,[bx+offset p1_ptrbuf]
        mov     si,[bx+2+offset p1_ptrbuf]
        add     ax,p1_ptr
        mov     ds,ax
        pop     bx
        test    cl,040h
        jnz     f1manh
        test    cl,80h
        jnz     f1manv
        call    putbuf1
        jmp     f1manend
f1manv:
        call    putbuf3
        jmp     f1manend
f1manh:
        test    cl,80h
        jnz     f1manhv
        call    putbuf2
        jmp     f1manend
f1manhv:
        call    putbuf4
f1manend:
        pop     ds
        test    p1_picno,0ffh
        jz      f1manend2
        test    p1_type,BIT3
        jnz     f1manend1
        mov     p1_picno,0
        jmp     f1manend2
f1manend1:
        call    f_subman1
f1manend2:
        ret
f_man1          ENDP
;
f_subman1       PROC    NEAR
        push    ds
        mov     es,alloc1
        xor     ax,ax
        mov     dx,ax
        mov     bx,ax
        mov     al,p1_picy
        sub     ax,p1_y
        jnc     pic1_yin
        xor     ax,0ffffh
        inc     ax
        mov     dl,al
        mov     ax,0
pic1_yin:
        push    dx
        mov     di,320
        mul     di
        mov     di,ax
        pop     dx
        test    p1_direct,0ffh
        jnz     pic1_rt
        mov     ax,p1_x
        add     al,p1_picx
        adc     ah,0
        jmp     pic1_xover
pic1_rt:
        xor     ax,ax
        mov     al,p1_px
        shl     ax,1
        add     ax,p1_subx
        sub     al,p1_picx
        sbb     ah,0
        sub     al,p1_picwid
        sbb     ah,0
pic1_xover:
;
        push    ax
        sub     ax,scr_x
;
        jc      pic1_xout
        test    ax,8000h
        jz      pic1_xin
pic1_xout:
        xor     ax,0ffffh
        inc     ax
        mov     bl,al
        mov     ax,0
pic1_xin:
        add     di,ax
        pop     ax
        add     al,p1_picwid
        adc     ah,0
        mov     cx,scr_x
        add     cx,319
        sub     cx,ax
        jnc     pic1_xrtin
        xor     cx,0ffffh
        add     cx,1
        mov     bh,cl
pic1_xrtin:
        push    bx
;
        xor     bx,bx
        mov     bl,p1_picno
        shl     bx,1
        shl     bx,1
;
        mov     dh,p1_pictype
        and     dh,10h
        add     dh,0c0h
        mov     cl,p1_picattr
        mov     al,p1_direct
        and     al,40h
        xor     cl,al
        mov     ax,[bx+offset p1_ptrbuf]
        mov     si,[bx+2+offset p1_ptrbuf]
        add     ax,p1_ptr
        mov     ds,ax
        pop     bx
        test    cl,040h
        jnz     f1pich
        test    cl,80h
        jnz     f1picv
        call    putbuf1
        jmp     f1picend
f1picv:
        call    putbuf3
        jmp     f1picend
f1pich:
        test    cl,80h
        jnz     f1pichv
        call    putbuf2
        jmp     f1picend
f1pichv:
        call    putbuf4
f1picend:
        pop     ds
        ret
f_subman1       ENDP
;
f_man2          PROC    NEAR
        push    ds
        mov     es,alloc1
        xor     dx,dx
        mov     bx,dx
        mov     ax,p2_txty
        test    ax,8000h
        jz      man2_yin
        xor     ax,0ffffh
        inc     ax
        mov     dl,al
        mov     ax,0
man2_yin:
        push    dx
        mov     di,320
        mul     di
        mov     di,ax
        pop     dx
        mov     ax,p2_txtx
        sub     ax,scr_x
;
        test    ax,8000h
        jz      man2_xin
man2_xout:
        xor     ax,0ffffh
        inc     ax
        mov     bl,al
        mov     ax,0
man2_xin:
        add     di,ax
        mov     ax,p2_txtx
        add     al,p2_width
        adc     ah,0
        mov     cx,scr_x
        add     cx,319
        sub     cx,ax
        jnc     man2_xrtin
        xor     cx,0ffffh
        add     cx,1
        mov     bh,cl
man2_xrtin:
        push    bx
        xor     bx,bx
        mov     bl,p2_sprite
        shl     bx,1
        shl     bx,1
;
        mov     dh,p2_type
        and     dh,10h
        add     dh,0e0h
        mov     cl,p2_attr
        mov     al,p2_direct
        and     al,40h
        xor     cl,al
        mov     ax,[bx+offset p2_ptrbuf]
        mov     si,[bx+2+offset p2_ptrbuf]
        add     ax,p2_ptr
        mov     ds,ax
        pop     bx
        test    cl,040h
        jnz     f2manh
        test    cl,80h
        jnz     f2manv
        call    putbuf1
        jmp     f2manend
f2manv:
        call    putbuf3
        jmp     f2manend
f2manh:
        test    cl,80h
        jnz     f2manhv
        call    putbuf2
        jmp     f2manend
f2manhv:
        call    putbuf4
f2manend:
        pop     ds
        test    p2_picno,0ffh
        jz      f2manend2
        test    p2_type,BIT3
        jnz     f2manend1
        mov     p2_picno,0
        jmp     f2manend2
f2manend1:
        call    f_subman2
f2manend2:
        ret
f_man2          ENDP
;
f_subman2       PROC    NEAR
        push    ds
        mov     es,alloc1
        xor     ax,ax
        mov     dx,ax
        mov     bx,ax
        mov     al,p2_picy
        sub     ax,p2_y
        jnc     pic2_yin
        xor     ax,0ffffh
        inc     ax
        mov     dl,al
        mov     ax,0
pic2_yin:
        push    dx
        mov     di,320
        mul     di
        mov     di,ax
        pop     dx
        test    p2_direct,0ffh
        jnz     pic2_rt
        mov     ax,p2_x
        add     al,p2_picx
        adc     ah,0
        jmp     pic2_xover
pic2_rt:
        xor     ax,ax
        mov     al,p2_px
        shl     ax,1
        add     ax,p2_subx
        sub     al,p2_picx
        sbb     ah,0
        sub     al,p2_picwid
        sbb     ah,0
pic2_xover:
;
        push    ax
        sub     ax,scr_x
;
        jc      pic2_xout
        test    ax,8000h
        jz      pic2_xin
pic2_xout:
        xor     ax,0ffffh
        inc     ax
        mov     bl,al
        mov     ax,0
pic2_xin:
        add     di,ax
        pop     ax
        add     al,p2_picwid
        adc     ah,0
        mov     cx,scr_x
        add     cx,319
        sub     cx,ax
        jnc     pic2_xrtin
        xor     cx,0ffffh
        add     cx,1
        mov     bh,cl
pic2_xrtin:
        push    bx
;
        xor     bx,bx
        mov     bl,p2_picno
        shl     bx,1
        shl     bx,1
;
        mov     dh,p2_pictype
        and     dh,10h
        add     dh,0e0h
        mov     cl,p2_picattr
        mov     al,p2_direct
        and     al,40h
        xor     cl,al
        mov     ax,[bx+offset p2_ptrbuf]
        mov     si,[bx+2+offset p2_ptrbuf]
        add     ax,p2_ptr
        mov     ds,ax
        pop     bx
        test    cl,040h
        jnz     f2pich
        test    cl,80h
        jnz     f2picv
        call    putbuf1
        jmp     f2picend
f2picv:
        call    putbuf3
        jmp     f2picend
f2pich:
        test    cl,80h
        jnz     f2pichv
        call    putbuf2
        jmp     f2picend
f2pichv:
        call    putbuf4
f2picend:
        pop     ds
        ret
f_subman2       ENDP
;
f_air   PROC    NEAR
        test    p1_airflag,0ffh
        jnz     p1_f_air
        jmp     p1_f_air_end
p1_f_air:
        push    ds
        mov     es,alloc1
        xor     ax,ax
        mov     dx,ax
        mov     ax,p1_airtxty
        mov     di,320
        mul     di
        mov     di,ax
        mov     ax,p1_airtxtx
        sub     ax,scr_x
        add     di,ax
        xor     bx,bx
        mov     bl,p1_airsprite
        shl     bx,1
        shl     bx,1
;
        mov     dh,p1_airtype
        and     dh,10h
        add     dh,0c0h
        mov     cl,p1_airattr
        mov     al,p1_airdir
        and     al,40h
        xor     cl,al
        mov     ax,[bx+offset p1_ptrbuf]
        mov     si,[bx+2+offset p1_ptrbuf]
        add     ax,p1_ptr
        mov     ds,ax
        xor     bx,bx
        test    cl,040h
        jnz     f1airh
        test    cl,80h
        jnz     f1airv
        call    putbuf1
        jmp     f1airend
f1airv:
        call    putbuf3
        jmp     f1airend
f1airh:
        test    cl,80h
        jnz     f1airhv
        call    putbuf2
        jmp     f1airend
f1airhv:
        call    putbuf4
f1airend:
        pop     ds
p1_f_air_end:
        test    p2_airflag,0ffh
        jnz     p2_f_air
        jmp     p2_f_air_end
p2_f_air:
        push    ds
        mov     es,alloc1
        xor     ax,ax
        mov     dx,ax
        mov     ax,p2_airtxty
        mov     di,320
        mul     di
        mov     di,ax
        mov     ax,p2_airtxtx
        sub     ax,scr_x
        add     di,ax
        xor     bx,bx
        mov     bl,p2_airsprite
        shl     bx,1
        shl     bx,1
;
        mov     dh,p2_airtype
        and     dh,10h
        add     dh,0e0h
        mov     cl,p2_airattr
        mov     al,p2_airdir
        and     al,40h
        xor     cl,al
        mov     ax,[bx+offset p2_ptrbuf]
        mov     si,[bx+2+offset p2_ptrbuf]
        add     ax,p2_ptr
        mov     ds,ax
        xor     bx,bx
        test    cl,040h
        jnz     f2airh
        test    cl,80h
        jnz     f2airv
        call    putbuf1
        jmp     f2airend
f2airv:
        call    putbuf3
        jmp     f2airend
f2airh:
        test    cl,80h
        jnz     f2airhv
        call    putbuf2
        jmp     f2airend
f2airhv:
        call    putbuf4
f2airend:
        pop     ds
        test    p1_airflag,0ffh
        jz      p2_f_air_end
        cmp     p1_aircnt,2
        jnc     p2_f_air_end
        mov     ax,p1_airtxtx
        sub     ax,p2_airtxtx
        cwd
        xor     ax,dx
        sub     ax,dx
        cmp     ax,p1_airwid
        ja      p2_f_air_end
        mov     p1_aircnt,2
        mov     p2_aircnt,2
        mov     p1_airstop,0
        mov     p2_airstop,0
p2_f_air_end:
        ret
f_air   ENDP
;  =================================
;  =  PUT BUFFER                   =
;  =================================
putbuf1 PROC    NEAR
        cld
        lodsw                   ;get wid and len
        mov     cl,al
        inc     cl
        shr     cl,1
        test    dl,0ffh
        jz      ptf010
        sub     ah,dl
        jnc     ptf000
        ret
ptf000:
        push    ax
        mov     al,cl
        mul     dl
        add     si,ax
        pop     ax
ptf010:
        mov     ch,ah
        inc     bl
        shr     bl,1
        inc     bh
        shr     bh,1
        sub     cl,bl
        sbb     cl,bh
        test    cl,0ffh
        jz      ptf015
        test    cl,80h
        jz      ptf020
ptf015: ret
ptf020:
        mov     al,bh
        xor     bh,bh
        add     si,bx
        add     bl,al
        adc     bh,0
        push    di
        push    cx
fmn00:  lodsb
        test    al,0ffh
        jz      fmn10
        mov     ah,al
        and     ah,0f0h
        jz      fmn05
        shr     ah,1
        shr     ah,1
        shr     ah,1
        shr     ah,1
        or      ah,dh
        xchg    al,ah
        stosb
        mov     al,ah
        jmp     fmn07
fmn05:
        inc     di
fmn07:
        and     al,0fh
        jz      fmn12
        or      al,dh
        stosb
        jmp     fmn20
fmn10:  inc     di
fmn12:  inc     di
fmn20:  dec     cl
        jnz     fmn00
        pop     cx
        pop     di
        push    di
;
        add     si,bx
        mov     ax,es
        add     ax,14h
        mov     es,ax
        dec     ch
        push    cx
        jnz     fmn00
        pop     cx
        pop     di
        ret
putbuf1 ENDP
;
putbuf2 PROC    NEAR
        cld
        lodsw
        xor     cx,cx
        mov     cl,al
        inc     cl
        sub     cl,bl
        add     di,cx
        add     cl,bl
        shr     cl,1
        test    dl,0ffh
        jz      ptf210
        sub     ah,dl
        jnc     ptf200
        ret
ptf200:
        push    ax
        mov     al,cl
        mul     dl
        add     si,ax
        pop     ax
ptf210:
        mov     ch,ah
        xor     ax,ax
        inc     bh
        mov     al,bh
        sub     di,ax
        inc     bl
        shr     bl,1
        shr     bh,1
        sub     cl,bl
        sbb     cl,bh
        test    cl,0ffh
        jz      ptf215
        test    cl,80h
        jz      ptf220
ptf215: ret
ptf220:
        mov     al,bl
        mov     bl,bh
        xor     bh,bh
        add     si,bx
        add     bl,al
        adc     bh,0
        push    di
        push    cx
fmn200:
        cld
        lodsb
        std
        test    al,0ffh
        jz      fmn210
        mov     ah,al
        and     ah,0f0h
        jz      fmn205
        shr     ah,1
        shr     ah,1
        shr     ah,1
        shr     ah,1
        or      ah,dh
        xchg    al,ah
        stosb
        mov     al,ah
        jmp     fmn207
fmn205:
        dec     di
fmn207:
        and     al,0fh
        jz      fmn212
        or      al,dh
        stosb
        jmp     fmn220
fmn210: dec     di
fmn212: dec     di
fmn220: dec     cl
        jnz     fmn200
        pop     cx
        pop     di
        push    di
;
        add     si,bx
        mov     ax,es
        add     ax,14h
        mov     es,ax
        dec     ch
        push    cx
        jnz     fmn200
        cld
        pop     cx
        pop     di
        ret
putbuf2 ENDP
;
putbuf3 PROC    NEAR
        cld
        lodsw                   ;get wid and len
        mov     cl,al
        inc     cl
        shr     cl,1
        test    dl,0ffh
        jz      ptf300
        sub     ah,dl
        jnc     ptf300
        ret
ptf300:
        push    ax
        push    bx
        push    dx
        mov     al,ah
        xor     ah,ah
        mov     bx,320
        mul     bx
        add     di,ax
        pop     dx
        pop     bx
        pop     ax
ptf310:
        mov     ch,ah
;
        inc     bl
        shr     bl,1
        inc     bh
        shr     bh,1
        sub     cl,bl
        sbb     cl,bh
        test    cl,0ffh
        jz      ptf315
        test    cl,80h
        jz      ptf320
ptf315: ret
ptf320:
        mov     al,bh
        xor     bh,bh
        add     si,bx
        add     bl,al
        adc     bh,0
        push    di
        push    cx
fmn300: lodsb
        test    al,0ffh
        jz      fmn310
;
        mov     ah,al
        and     ah,0f0h
        jz      fmn305
        shr     ah,1
        shr     ah,1
        shr     ah,1
        shr     ah,1
        or      ah,dh
        xchg    al,ah
        stosb
        mov     al,ah
        jmp     fmn307
fmn305:
        inc     di
fmn307:
        and     al,0fh
        jz      fmn312
        or      al,dh
        stosb
        jmp     fmn320
fmn310: inc     di
fmn312: inc     di
fmn320: dec     cl
        jnz     fmn300
        pop     cx
        pop     di
        push    di
;
        add     si,bx
        mov     ax,es
        sub     ax,14h
        mov     es,ax
        dec     ch
        push    cx
        jnz     fmn300
        pop     cx
        pop     di
        ret
putbuf3 ENDP
;
putbuf4 PROC    NEAR
        cld
        lodsw
        xor     cx,cx
        mov     cl,al
        inc     cl
        sub     cl,bl
        add     di,cx
        add     cl,bl
        shr     cl,1
        test    dl,0ffh
        jz      ptf400
        sub     ah,dl
        jnc     ptf400
        ret
ptf400:
        push    ax
        push    bx
        push    dx
        mov     al,ah
        xor     ah,ah
        mov     bx,320
        mul     bx
        add     di,ax
        pop     dx
        pop     bx
        pop     ax
ptf410:
        mov     ch,ah
;
        xor     ax,ax
        inc     bh
        mov     al,bh
        sub     di,ax
        inc     bl
        shr     bl,1
        shr     bh,1
        sub     cl,bl
        sbb     cl,bh
        test    cl,0ffh
        jz      ptf415
        test    cl,80h
        jz      ptf420
ptf415: ret
ptf420:
        mov     al,bh
        xor     bh,bh
        add     si,bx
        add     bl,al
        adc     bh,0
        push    di
        push    cx
fmn400:
        cld
        lodsb
        std
        test    al,0ffh
        jz      fmn410
        mov     ah,al
        and     ah,0f0h
        jz      fmn405
        shr     ah,1
        shr     ah,1
        shr     ah,1
        shr     ah,1
        or      ah,dh
        xchg    al,ah
        stosb
        mov     al,ah
        jmp     fmn407
fmn405:
        dec     di
fmn407:
        and     al,0fh
        jz      fmn412
        or      al,dh
        stosb
        jmp     fmn420
fmn410: dec     di
fmn412: dec     di
fmn420: dec     cl
        jnz     fmn400
        pop     cx
        pop     di
        push    di
;
        add     si,bx
        mov     ax,es
        sub     ax,14h
        mov     es,ax
        dec     ch
        push    cx
        jnz     fmn400
        cld
        pop     cx
        pop     di
        ret
putbuf4 ENDP
;  =================================
;  =  PATH CONTROL                 =
;  =================================
pth_control     PROC    NEAR
        test    p1_stop,0ffh
        jz      pthp1enter
        jmp     p1_vector
pthp1enter:
        test    p1_forcenum,0ffh
        jnz     passp1hit
        mov     p1_hitflg,0
passp1hit:
        xor     bx,bx
        mov     bl,p1_pthno
        shl     bx,1
        mov     ax,[bx+offset p1_pth]
        mov     si,offset p1_pth
        add     si,ax
        xor     ax,ax
        mov     al,p1_pthcnt
        mov     cl,3
        shl     ax,cl
        add     si,ax
        mov     di,offset p1_type
        mov     ax,ds
        mov     es,ax
        mov     cx,08
        cld
        rep     movsb
        mov     al,p1_sprite
        mov     p1_sprbak,al
        test    p1_type,BIT7
        jz      eraseforcep1
        mov     p1_forcenum,0
eraseforcep1:
        test    p1_type,0ffh            ;end this path
        jnz     p1_notend
        test    p2_aitype,BIT7
        jz      noclearp2ai
        mov     p2_aitype,0
noclearp2ai:
        xor     ax,ax
        mov     al,p1_pthno
        mov     si,ax
        mov     al,nextpath[si]
        cmp     al,13
        jnz     chkp1down2
        test    p1_die,0ffh
        jnz     chkp1down
        test    p1_sleepcnt,80h
        jz      chkp1down2
chkp1down:
        mov     al,12
        jmp     chkp1die
chkp1down2:
        test    p1_die,0ffh
        jz      chkp1die
        cmp     al,14
        jnz     chkp1die
        dec     p1_pthcnt
        mov     p1_die,80h
        jmp     pthp1enter
chkp1die:
        mov     p1_pthno,al
        mov     p1_forcenum,0
        mov     p1_pthcnt,0
        test    al,0ffh
        jz      chkp1die2
        jmp     pthp1enter
chkp1die2:
        test    p1_sleepcnt,80h
        jz      chkp1sleep
        mov     p1_sleepcnt,0
        mov     p1_pthno,15
        jmp     pthp1enter
chkp1sleep:
        mov     p1_type,BIT1
        call    joy_control_p1
        jmp     pthp1enter
p1_notend:                              ;get sub picture data
        test    p1_type,BIT3
        jz      p1_nosubpic
        mov     cx,06
        mov     di,offset p1_pictype
        rep     movsb
        test    p1_pictype,BIT3
        jz      p1_nosubpic
        mov     p1_picno,0
        mov     p1_airflag,1
        mov     p2_aitype,0             ;force p2 check air attack
p1_nosubpic:
        mov     al,p1_stop
        mov     p1_attr,al
        and     p1_stop,03fh
        mov     p1_veccnt,0
        call    sprite_info1
        test    p1_forcenum,0ffh
        jz      p1_notend2
        mov     dl,p1_forcecnt
        call    set_p1_force
        mov     p1_forcecnt,dl
p1_notend2:
        test    p1_direct,0ffh
        jz      p1vet
        mov     al,p1_levelx
        sub     al,p1_px
        cbw
        shl     ax,1
        add     p1_subx,ax
p1vet:  mov     al,p1_px
        mov     p1_levelx,al
p1_vector:
        test    p1_direct,0ffh
        jz      p1_noshake
        mov     ax,p1_subx
        sub     al,p1_width
        sbb     ah,0
        mov     p1_x,ax
        jmp     p1_noshake2
p1_noshake:
        mov     ax,p1_x
        add     al,p1_width
        adc     ah,0
        mov     p1_subx,ax
p1_noshake2:
        xor     ax,ax
        mov     al,p1_px
        add     ax,p1_x
        mov     p1_txtx,ax
        mov     p1_move,ax
        mov     p1_pretx,ax
        test    ax,8000h
        jz      p1_noshake3
        mov     p1_pretx,0
p1_noshake3:
        test    p1_veccnt,0ffh
        jz      p1_noshake4
        jmp     p1_vecover
p1_noshake4:
        test    p1_die,80h
        jz      p1_noshake5
        jmp     p1_vecover
p1_noshake5:
        test    p1_hitflg,0ffh
        jnz     p1nooverflow
        mov     al,p1_xvec
        xor     al,p1_direct
        sub     al,p1_direct
p1_noneed:
        cbw
        add     p1_move,ax
        test    p1_move,8000h
        jz      p1nooverflow
        mov     ax,p1_txtx
        test    ax,8000h
        jnz     p1_noneed2
        mov     p1_move,0
        jmp     p1nooverflow
p1_noneed2:                     ;both are negative
        cmp     ax,p1_move
        jna     p1nooverflow
        mov     p1_move,ax
p1nooverflow:
        mov     ax,p1_move
        add     al,p1_width
        adc     ah,0
        cmp     ax,479
        jna     p1nooverflow2
        mov     ax,p1_txtx
        add     al,p1_width
        adc     ah,0
        cmp     ax,479
        ja      p1nooverflow1
        mov     ax,479
        sub     al,p1_width
        sbb     ah,0
        mov     p1_move,ax
        jmp     p1nooverflow2
p1nooverflow1:
        mov     ax,p1_txtx
        cmp     ax,p1_move
        ja      p1nooverflow2
        mov     p1_move,ax
p1nooverflow2:
        mov     al,p1_yvec
        cbw
        add     p1_y,ax
        test    p1_y,8000h
        jz      p1_vecover
        mov     p1_y,0
p1_vecover:
        test    p1_hitback,0ffh
        jz      p1_hitproc2
        inc     p1_hitback
        cmp     p1_hitback,BACK_REP
        jna     p1_hit00
        mov     p1_hitback,0
p1_hit00:
        mov     al,BACK_RATE
        xor     al,p1_direct
        sub     al,p1_direct
        cbw
        add     ax,p1_txtx
        mov     p1_move,ax
        test    p1_move,8000h
        jz      p1_hitproc
        mov     ax,p1_txtx
        mov     p1_move,ax
        test    ax,8000h
        jnz     p1_hitproc
        mov     p1_move,0
p1_hitproc:
        mov     ax,p1_move
        add     al,p1_width
        adc     ah,0
        cmp     ax,479
        jna     p1_hitproc2
        mov     ax,p1_txtx
        mov     p1_move,ax
        mov     ax,p1_txtx
        add     al,p1_width
        adc     ah,0
        cmp     ax,479
        ja      p1_hitproc2
        mov     ax,479
        sub     al,p1_width
        sbb     ah,0
        mov     p1_move,ax
p1_hitproc2:
        inc     p1_veccnt
        mov     al,p1_veccnt
        cmp     al,p1_vecdelay
        jc      pthp1over
        mov     p1_veccnt,0
pthp1over:
        test    p1_forcenum,0ffh
        jz      pthp1over2
        dec     p1_forcecnt
        jnz     pthp1over2
        inc     p1_forceno
        call    set_p1_force
pthp1over2:
        dec     p1_stop
        jnz     p1_chkair
        test    p1_type,BIT6
        jz      pthp1over4
        test    p1_y,0ffffh
        jz      pthp1over4
        inc     p1_stop
        jmp     p1_chkair
pthp1over4:
        test    p1_type,BIT3
        jz      pthp1over6
        inc     p1_pthcnt
pthp1over6:
        inc     p1_pthcnt
; p1 air attack
p1_chkair:
        test    p1_airflag,0ffh
        jnz     p1_air_init
        jmp     p1_air_end
p1_air_init:
        test    p1_airflag,80h
        jnz     p1_air_main
        xor     ax,ax
        mov     al,p1_picy
        mov     al,p1_airsuby
        sub     ax,p1_y
        mov     p1_airtxty,ax
        test    p1_direct,0ffh
        jnz     p1_air_init_rt
        mov     ax,p1_x
        add     al,p1_picx
        adc     ah,0
        jmp     p1_air_init_end
p1_air_init_rt:
        xor     ax,ax
        mov     al,p1_px
        shl     ax,1
        add     ax,p1_subx
        sub     al,p1_picx
        sbb     ah,0
        sub     al,p1_picwid
        sbb     ah,0
p1_air_init_end:
        mov     p1_airtxtx,ax
        mov     p1_aircnt,0
        mov     p1_airstop,0
        mov     al,p1_picx
        mov     p1_airsubx,al
        mov     al,p1_direct
        mov     p1_airdir,al
        xor     ax,ax
        mov     al,p1_picwid
        mov     p1_airwid,ax
        add     al,p1_picx
        adc     ah,0
        mov     p1_airwid2,ax
        or      p1_airflag,80h
p1_air_main:
        test    p1_airstop,0ffh
        jz      p1airpthread
        jmp     p1_airvector
p1airpthread:
        mov     bx,78
        mov     ax,[offset p1_pth+bx]   ;78 = path 39 * 2
        mov     si,offset p1_pth
        add     si,ax
        xor     ax,ax
        mov     al,p1_aircnt
        mov     cl,3
        shl     ax,cl
        add     si,ax
        mov     di,offset p1_airtype
        mov     ax,ds
        mov     es,ax
        mov     cx,8
        cld
        rep     movsb
        test    p1_airtype,0ffh
        jnz     p1_airinitdata
        mov     p1_airflag,0
        jmp     p1_air_end
p1_airinitdata:
        mov     al,p1_airstop
        mov     p1_airattr,al
        and     p1_airstop,3fh
        mov     p1_airveccnt,0
        mov     al,p1_airy
        mov     ah,al
        sub     al,p1_airsuby
        mov     p1_airsuby,ah
        cbw
        add     p1_airtxty,ax
        test    p1_airdir,0ffh
        jnz     p1_airvector
        mov     al,p1_airx
        mov     ah,al
        sub     al,p1_airsubx
        mov     p1_airsubx,ah
        cbw
        add     p1_airtxtx,ax
p1_airvector:
        test    p1_airveccnt,0ffh
        jnz     p1_airvecover
        mov     al,p1_airvecx
        xor     al,p1_airdir
        sub     al,p1_airdir
        cbw
        add     p1_airtxtx,ax
        mov     ax,p1_airtxtx
        test    ax,8000h
        jnz     p1_airout
        sub     ax,scr_x
        jc      p1_airout
        add     ax,p1_airwid
        cmp     ax,320
        jna     p1_airvecover
p1_airout:
        mov     p1_airflag,0
        jmp     p1_air_end
p1_airvecover:
        inc     p1_airveccnt
        mov     al,p1_airveccnt
        cmp     al,p1_airdelay
        jc      p1_air_over
        mov     p1_airveccnt,0
p1_air_over:
        dec     p1_airstop
        jnz     p1_air_end
        test    p1_aircnt,0feh
        jnz     p1_air_noctrl
        xor     p1_aircnt,01h
        jmp     p1_air_end
p1_air_noctrl:
        inc     p1_aircnt
p1_air_end:
;
pthp2ctrl:
        test    p2_stop,0ffh
        jz      pthp2enter
        jmp     p2_vector
pthp2enter:
        test    p2_forcenum,0ffh
        jnz     passp2hit
        mov     p2_hitflg,0
passp2hit:
        xor     bx,bx
        mov     bl,p2_pthno
        shl     bx,1
        mov     ax,[bx+offset p2_pth]
        mov     si,offset p2_pth
        add     si,ax
        xor     ax,ax
        mov     al,p2_pthcnt
        mov     cl,3
        shl     ax,cl
        add     si,ax
        mov     di,offset p2_type
        mov     ax,ds
        mov     es,ax
        mov     cx,08
        cld
        rep     movsb
        mov     al,p2_sprite
        mov     p2_sprbak,al
        test    p2_type,BIT7
        jz      eraseforcep2
        mov     p2_forcenum,0
eraseforcep2:
        test    p2_type,0ffh
        jnz     p2_notend
        test    p1_aitype,BIT7
        jz      noclearp1ai
        mov     p1_aitype,0
noclearp1ai:
        xor     ax,ax
        mov     al,p2_pthno
        mov     si,ax
        mov     al,nextpath[si]
        cmp     al,13
        jnz     chkp2down2
        test    p2_die,0ffh
        jnz     chkp2down
        test    p2_sleepcnt,80h
        jz      chkp2down2
chkp2down:
        mov     al,12
        jmp     chkp2die
chkp2down2:
        test    p2_die,0ffh
        jz      chkp2die
        cmp     al,14
        jnz     chkp2die
        dec     p2_pthcnt
        mov     p2_die,80h
        jmp     pthp2enter
chkp2die:
        mov     p2_pthno,al
        mov     p2_forcenum,0
        mov     p2_pthcnt,0
        test    al,0ffh
        jz      chkp2die2
        jmp     pthp2enter
chkp2die2:
        test    p2_sleepcnt,80h
        jz      chkp2sleep
        mov     p2_sleepcnt,0
        mov     p2_pthno,15
        jmp     pthp2enter
chkp2sleep:
        mov     p2_type,BIT1
        call    joy_control_p2
        jmp     pthp2enter
p2_notend:
        test    p2_type,BIT3
        jz      p2_nosubpic
        mov     cx,06
        mov     di,offset p2_pictype
        rep     movsb
        test    p2_pictype,BIT3
        jz      p2_nosubpic
        mov     p2_picno,0
        mov     p2_airflag,1
        mov     p1_aitype,0             ;if air attack set clear aitype
p2_nosubpic:
        mov     al,p2_stop
        mov     p2_attr,al
        and     p2_stop,03fh
        mov     p2_veccnt,0
        call    sprite_info2
        test    p2_forcenum,0ffh
        jz      p2_notend2
        mov     dl,p2_forcecnt
        call    set_p2_force
        mov     p2_forcecnt,dl
p2_notend2:
        test    p2_direct,0ffh
        jz      p2vet
        mov     al,p2_levelx
        sub     al,p2_px
        cbw
        shl     ax,1
        add     p2_subx,ax
p2vet:  mov     al,p2_px
        mov     p2_levelx,al
p2_vector:
        test    p2_direct,0ffh
        jz      p2_noshake
        mov     ax,p2_subx
        sub     al,p2_width
        sbb     ah,0
        mov     p2_x,ax
        jmp     p2_noshake2
p2_noshake:
        mov     ax,p2_x
        add     al,p2_width
        adc     ah,0
        mov     p2_subx,ax
p2_noshake2:
        xor     ax,ax
        mov     al,p2_px
        add     ax,p2_x
        mov     p2_txtx,ax
        mov     p2_move,ax
        mov     p2_pretx,ax
        test    ax,8000h
        jz      p2_noshake3
        mov     p2_pretx,0
p2_noshake3:
        test    p2_veccnt,0ffh
        jz      p2_noshake4
        jmp     p2_vecover
p2_noshake4:
        test    p2_die,80h
        jz      p2_noshake5
        jmp     p2_vecover
p2_noshake5:
        test    p2_hitflg,0ffh
        jnz     p2nooverflow
        mov     al,p2_xvec              ;get p2_xvec reverse
        xor     al,p2_direct
        sub     al,p2_direct
p2_noneed:
        cbw
        add     p2_move,ax
        test    p2_move,8000h
        jz      p2nooverflow
        mov     ax,p2_txtx
        test    ax,8000h
        jnz     p2_noneed2
        mov     p2_move,0
        jmp     p2nooverflow
p2_noneed2:
        cmp     ax,p2_move
        jna     p2nooverflow
        mov     p2_move,ax
p2nooverflow:
        mov     ax,p2_move
        add     al,p2_width
        adc     ah,0
        cmp     ax,479
        jna     p2nooverflow2
        mov     ax,p2_txtx
        add     al,p2_width
        adc     ah,0
        cmp     ax,479
        ja      p2nooverflow1
        mov     ax,479
        sub     al,p2_width
        sbb     ah,0
        mov     p2_move,ax
        jmp     p2nooverflow2
p2nooverflow1:
        mov     ax,p2_txtx
        cmp     ax,p2_move
        ja      p2nooverflow2
        mov     p2_move,ax
p2nooverflow2:
        mov     al,p2_yvec
        cbw
        add     p2_y,ax
        test    p2_y,8000h
        jz      p2_vecover
        mov     p2_y,0
p2_vecover:
        test    p2_hitback,0ffh
        jz      p2_hitproc2
        inc     p2_hitback
        cmp     p2_hitback,BACK_REP
        jna     p2_hit00
        mov     p2_hitback,0
p2_hit00:
        mov     al,BACK_RATE
        xor     al,p2_direct
        sub     al,p2_direct
        cbw
        add     ax,p2_txtx
        mov     p2_move,ax
        test    p2_move,8000h
        jz      p2_hitproc
        mov     ax,p2_txtx
        mov     p2_move,ax
        test    ax,8000h
        jnz     p2_hitproc
        mov     p2_move,0
p2_hitproc:
        mov     ax,p2_move
        add     al,p2_width
        adc     ah,0
        cmp     ax,479
        jna     p2_hitproc2
        mov     ax,p2_txtx
        mov     p2_move,ax
        mov     ax,p2_txtx
        add     al,p2_width
        adc     ah,0
        cmp     ax,479
        ja      p2_hitproc2
        mov     ax,479
        sub     al,p2_width
        sbb     ah,0
        mov     p2_move,ax
p2_hitproc2:
        inc     p2_veccnt
        mov     al,p2_veccnt
        cmp     al,p2_vecdelay
        jc      pthp2over
        mov     p2_veccnt,0
pthp2over:
        test    p2_forcenum,0ffh
        jz      pthp2over2
        dec     p2_forcecnt
        jnz     pthp2over2
        inc     p2_forceno
        call    set_p2_force
pthp2over2:
        dec     p2_stop
        jnz     p2_chkair
        test    p2_type,BIT6
        jz      pthp2over4
        test    p2_y,0ffffh
        jz      pthp2over4
        inc     p2_stop
        jmp     p2_chkair
pthp2over4:
        test    p2_type,BIT3
        jz      pthp2over6
        inc     p2_pthcnt
pthp2over6:
        inc     p2_pthcnt
; p2 air attack
p2_chkair:
        test    p2_airflag,0ffh
        jnz     p2_air_init
        jmp     p2_air_end
p2_air_init:
        test    p2_airflag,80h
        jnz     p2_air_main
        xor     ax,ax
        mov     al,p2_picy
        mov     p2_airsuby,al
        sub     ax,p2_y
        mov     p2_airtxty,ax
        test    p2_direct,0ffh
        jnz     p2_air_init_rt
        mov     ax,p2_x
        add     al,p2_picx
        adc     ah,0
        jmp     p2_air_init_end
p2_air_init_rt:
        xor     ax,ax
        mov     al,p2_px
        shl     ax,1
        add     ax,p2_subx
        sub     al,p2_picx
        sbb     ah,0
        sub     al,p2_picwid
        sbb     ah,0
p2_air_init_end:
        mov     p2_airtxtx,ax
        mov     p2_aircnt,0
        mov     p2_airstop,0
        mov     al,p2_picx
        mov     p2_airsubx,al
        mov     al,p2_direct
        mov     p2_airdir,al
        xor     ax,ax
        mov     al,p2_picwid
        mov     p2_airwid,ax
        add     al,p2_picx
        adc     ah,0
        mov     p2_airwid2,ax
        or      p2_airflag,80h
p2_air_main:
        test    p2_airstop,0ffh
        jz      p2airpthread
        jmp     p2_airvector
p2airpthread:
        mov     bx,78
        mov     ax,[offset p2_pth+bx]   ;78 = path 39 * 2
        mov     si,offset p2_pth
        add     si,ax
        xor     ax,ax
        mov     al,p2_aircnt
        mov     cl,3
        shl     ax,cl
        add     si,ax
        mov     di,offset p2_airtype
        mov     ax,ds
        mov     es,ax
        mov     cx,8
        cld
        rep     movsb
        test    p2_airtype,0ffh
        jnz     p2_airinitdata
        mov     p2_airflag,0
        jmp     p2_air_end
p2_airinitdata:
        mov     al,p2_airstop
        mov     p2_airattr,al
        and     p2_airstop,3fh
        mov     p2_airveccnt,0
        mov     al,p2_airy
        mov     ah,al
        sub     al,p2_airsuby
        mov     p2_airsuby,ah
        cbw
        add     p2_airtxty,ax
        test    p2_airdir,0ffh
        jnz     p2_airvector
        mov     al,p2_airx
        mov     ah,al
        sub     al,p2_airsubx
        mov     p2_airsubx,ah
        cbw
        add     p2_airtxtx,ax
p2_airvector:
        test    p2_airveccnt,0ffh
        jnz     p2_airvecover
        mov     al,p2_airvecx
        xor     al,p2_airdir
        sub     al,p2_airdir
        cbw
        add     p2_airtxtx,ax
        mov     ax,p2_airtxtx
        test    ax,8000h
        jnz     p2_airout
        sub     ax,scr_x
        jc      p2_airout
        add     ax,p2_airwid
        cmp     ax,320
        jna     p2_airvecover
p2_airout:
        mov     p2_airflag,0
        jmp     p2_air_end
p2_airvecover:
        inc     p2_airveccnt
        mov     al,p2_airveccnt
        cmp     al,p2_airdelay
        jc      p2_air_over
        mov     p2_airveccnt,0
p2_air_over:
        dec     p2_airstop
        jnz     p2_air_end
        test    p2_aircnt,0feh
        jnz     p2_air_noctrl
        xor     p2_aircnt,01h
        jmp     p2_air_end
p2_air_noctrl:
        inc     p2_aircnt
p2_air_end:
pthp2end:
        ret
pth_control     ENDP
;  =================================
;  =  SET CONTROL EQUIPMENT        =
;  =================================
set_control     PROC    NEAR
        mov     ax,p2_man
        mov     scene_no,ax
        xor     ax,ax
        mov     p1_win,ax
        mov     p2_win,ax
        mov     round,ax
        mov     who_win,ax
        mov     timecnt,ax
        mov     round_flag,ax
        ret
set_control     ENDP
;  =================================
;  = change keyboard vector        =
;  =================================
keyinit         PROC    NEAR
        cli
        push    ds
        push    cs
        mov     al,09h
        mov     ah,35h
        int     21h
        mov     ax,es
        mov     ds,ax
        mov     dx,bx
        mov     al,60h
        mov     ah,25h
        int     21h

        pop     ds
        mov     dx,offset keyentry
        mov     al,09h
        mov     ah,25h
        int     21h

        pop     ds
        sti
        ret
keyinit         ENDP
;
keyentry        PROC    NEAR
        push    ds
        push    si
        push    cx
        push    ax
        mov     ax,@data
        mov     ds,ax
        in      al,60h
        push    ax
        in      al,61h
        or      al,80h
        out     61h,al
        and     al,7fh
        out     61h,al
        pop     ax
;
        cmp     al,3bh
        jnz     key2
        mov     escape,01h
key2:
        cmp     al,01
        jnz     key3
        mov     escape,080h
key3:
;
        cmp     al,3ch          ; F2
        jnz     key4
        call    test_joy
dkey:   call    read_joy
        test    joyvalue,BUT_A
        jz      dkey
key4:
        cmp     main_select,4
        jnz     key5
        mov     escape,080h
key5:
;
        call    chk_key1
        call    chk_key2
        mov     al,20h
        out     20h,al
        pop     ax
        pop     cx
        pop     si
        pop     ds
        iret
;
chk_key1:
        mov     si,0
cky100:
        cmp     key1table[si],al
        jz      key1bomb
        inc     si
        cmp     si,8
        jnz     cky100
cky110: cmp     al,0fh
        jnz     cky120
        or      key1val,BUT_A
        jmp     cky199
cky120: cmp     al,8fh
        jnz     cky130
        and     key1val,0efh
        jmp     cky199
cky130: cmp     al,2ah
        jnz     cky140
        or      key1val,BUT_B
        jmp     cky199
cky140: cmp     al,0aah
        jnz     cky150
        and     key1val,0dfh
        jmp     cky199
cky150:
        test    al,80h
        jz      cky199
        mov     ah,al
        and     ah,key1last
        cmp     ah,key1last
        jnz     cky199
        and     key1val,0f0h
        mov     key1last,00h
        jmp     cky199
key1bomb:
        mov     key1last,al
        and     key1val,0f0h
        mov     al,arrortable[si]
        or      key1val,al
cky199:
        ret
;
chk_key2:
        mov     si,0
cky200:
        cmp     key2table[si],al
        jz      key2bomb
        inc     si
        cmp     si,8
        jnz     cky200
cky210: cmp     al,1ch
        jnz     cky220
        or      key2val,BUT_A
        jmp     cky299
cky220: cmp     al,9ch
        jnz     cky230
        and     key2val,0efh
        jmp     cky299
cky230: cmp     al,36h
        jnz     cky240
        or      key2val,BUT_B
        jmp     cky299
cky240: cmp     al,0b6h
        jnz     cky250
        and     key2val,0dfh
        jmp     cky299
cky250:
        test    al,80h
        jz      cky299
        mov     ah,al
        and     ah,key2last
        cmp     ah,key2last
        jnz     cky299
        and     key2val,0f0h
        mov     key2last,00h
        jmp     cky299
key2bomb:
        mov     key2last,al
        and     key2val,0f0h
        mov     al,arrortable[si]
        or      key2val,al
cky299:
        ret
keyentry        ENDP
;
keyundo         PROC    NEAR
        cli
        push    ds
        push    es
        push    dx
        push    di
        xor     ax,ax
        mov     ds,ax
        les     di,ds:[60h*4]
        mov     ax,es
        mov     ds:[09h*4+2],ax
        mov     ds:[09h*4],di
        pop     di
        pop     dx
        pop     es
        pop     ds
        sti
        ret
keyundo         ENDP
;  =================================
;  =  Check attack                 =
;  =================================
check_attack    PROC    NEAR
        mov     p1_hitnum,0
        mov     p2_hitnum,0
;
        xor     bx,bx
        mov     bl,p1_sprite
        shl     bx,1
        shl     bx,1
        mov     al,p1_type
        and     al,01h
        add     bl,al
        mov     al,[bx+offset p1_atrbuf+72]
        mov     p1_atkatt,al
        mov     al,[bx+offset p1_atrbuf+74]
        mov     p1_atkatk,al
        xor     bx,bx
        mov     bl,p2_sprite
        shl     bx,1
        shl     bx,1
        mov     al,p2_type
        and     al,01h
        add     bl,al
        mov     al,[bx+offset p2_atrbuf+72]
        mov     p2_atkatt,al
        mov     al,[bx+offset p2_atrbuf+74]
        mov     p2_atkatk,al
;
        test    p1_hitflg,0ffh
        jz      p1_check_attack
        jmp     p1_attack_end
p1_check_attack:
        test    p2_atkatt,BIT6
        jz      p1_check_attack2
        jmp     p1_airattack_end
p1_check_attack2:
        test    p1_atkatt,80h
        jnz     p1_attack_main
        jmp     p1_attack_end
p1_attack_main:
        xor     bx,bx
        mov     bl,p1_atkatt
        and     bl,0fh
        shl     bx,1
        shl     bx,1
        shl     bx,1
        xor     ax,ax
        mov     al,[bx+offset p1_atrbuf+72*5]
        mov     r0,ax
        mov     al,[bx+offset p1_atrbuf+72*5+1]
        mov     r2,ax
        mov     al,[bx+offset p1_atrbuf+72*5+2]
        mov     r1,ax
        mov     al,[bx+offset p1_atrbuf+72*5+3]
        mov     r3,ax
        test    p1_direct,0ffh
        jnz     p1_attack_rt
        mov     ax,p1_x
        add     ax,r0
        mov     r0,ax
        add     r1,ax
        jmp     p1_attack_ok
p1_attack_rt:
        xor     ax,ax
        mov     al,p1_px
        shl     ax,1
        add     ax,p1_subx
        sub     ax,r0
        sbb     ax,r1
        add     r1,ax
        cwd
        xor     dx,0ffffh
        and     ax,dx
        mov     r0,ax
p1_attack_ok:
        test    r0,8000h
        jz      p1_redefx1
        mov     r0,0
p1_redefx1:
        test    r1,8000h
        jz      p1_redefx2
        mov     r1,0
p1_redefx2:
        mov     ax,r2
        sub     ax,p1_y
        mov     r2,ax
        add     r3,ax                   ;calc y position over
        mov     ax,p2_pretx
        cmp     ax,r1
        jna     p1_attack_ok2
        jmp     p1_attack_end
p1_attack_ok2:
        mov     ax,p2_subx
        add     al,p2_px
        adc     ah,0
        cmp     ax,r0
        ja      p1_attack_ok3
        jmp     p1_attack_end           ;check x position over
p1_attack_ok3:
        mov     ax,r3
        cmp     ax,p2_txty
        ja      p1_attack_ok4
        jmp     p1_attack_end
p1_attack_ok4:
        mov     ax,r2
        mov     cx,p2_txty
        add     cl,p2_length
        adc     ch,0
        cmp     ax,cx
        jna     p1_attack_con
        jmp     p1_attack_end
p1_attack_con:
        inc     p1_hitflg
        mov     p1_light,1
        mov     ax,p1_txtx
        test    ax,8000h
        jz      p1lightok1
        mov     ax,0
p1lightok1:
        mov     p1_lightx,ax
        mov     cx,p2_txtx
        test    cx,8000h
        jz      p1lightok2
        mov     cx,0
p1lightok2:
        cmp     ax,cx
        ja      setp1lightx
        mov     p1_lightx,cx
setp1lightx:
        sub     p1_lightx,8
        mov     ax,p1_txty
        mov     p1_lighty,ax
        cmp     ax,p2_txty
        ja      setp1lighty
        mov     ax,p2_txty
        mov     p1_lighty,ax
setp1lighty:
        test    p1_lighty,8000h
        jz      setp1lighty2
        mov     p1_lighty,0
setp1lighty2:
        mov     p2_hitback,1
        test    p2_direct,0ffh
        jnz     p1_chkp2hitrt
        mov     ax,p2_txtx
        jmp     p1_chkp2hitmain
p1_chkp2hitrt:
        xor     ax,ax
        mov     al,p2_width
        add     ax,p2_txtx
        sub     ax,480
p1_chkp2hitmain:
        cwd
        xor     ax,dx
        sub     ax,dx
        cmp     ax,BACK_REF
        ja      p1_chkp2hitend
        mov     p1_hitback,1
p1_chkp2hitend:
        push    bx
        mov     ax,0
        call    check_p2_lose
        pop     bx
        mov     al,[bx+offset p1_atrbuf+72*5+4]
        test    al,0ffh
        jnz     p1_set_atkact
        mov     ax,p2_losenow
        cmp     ax,p2_blood
        jnc     p1_atkact_fix
        cmp     p2_pthno,7
        jz      p1_atkact_end
        cmp     p2_pthno,8
        jz      p1_atkact_end
p1_atkact_fix:
        mov     al,[bx+offset p1_atrbuf+72*5+5]
        jmp     p1_atkact_end
p1_set_atkact:
        test    p2_sleepcnt,80h
        jnz     p1_attack_ok5
        mov     bx,p2_losenow
        cmp     bx,p2_blood
        jc      p1_attack_ok6
p1_attack_ok5:
        mov     al,12
        jmp     p1_atkact_end
p1_attack_ok6:
        xor     bx,bx
        mov     bl,p2_sprite
        shl     bx,1
        shl     bx,1
        mov     ah,[bx+offset p2_atrbuf+72]
;
        mov     bx,p2_y
        mov     dl,p2_pthno
        mov     cx,p1_light
        call    check_hitted_act
        mov     p1_light,cx
p1_atkact_end:
        test    al,0ffh
        jz      p1_attack_end
        mov     p2_hitnum,al
p1_attack_end:
;  check air attack
        test    p1_airflag,0ffh
        jnz     p1_airattack_chk
        jmp     p1_airattack_end
p1_airattack_chk:
        xor     bx,bx
        mov     bl,p1_airsprite
        shl     bx,1
        shl     bx,1
        mov     al,p1_airtype
        and     al,01h
        add     bl,al
        mov     al,[bx+offset p1_atrbuf+72]
        test    al,80h
        jnz     p1_airattack_main
        jmp     p1_airattack_end
p1_airattack_main:
        xor     bx,bx
        mov     bl,al
        and     bl,0fh
        shl     bx,1
        shl     bx,1
        shl     bx,1
        xor     ax,ax
        mov     al,[bx+offset p1_atrbuf+72*5]
        mov     r0,ax
        mov     al,[bx+offset p1_atrbuf+72*5+1]
        mov     r2,ax
        mov     al,[bx+offset p1_atrbuf+72*5+2]
        mov     r1,ax
        mov     al,[bx+offset p1_atrbuf+72*5+3]
        mov     r3,ax
;
        test    p1_airdir,0ffh
        jz      p1_airattack_000
        mov     ax,p1_airwid2
        sub     ax,r0
        sbb     ax,r1
        add     ax,p1_airtxtx
        jmp     p1_airattack_010
p1_airattack_000:
        mov     ax,r0
        sub     al,p1_airx
        sbb     ah,0
        add     ax,p1_airtxtx
p1_airattack_010:
        test    ax,8000h
        jz      p1_airattack_main2
        mov     ax,0
p1_airattack_main2:
        mov     r0,ax
        add     r1,ax
;
        mov     ax,r2
        sub     al,p1_airy
        sbb     ah,0
        add     ax,p1_airtxty
        mov     r2,ax
        add     r3,ax                   ;calc y position over
        mov     ax,p2_pretx
        cmp     ax,r1
        jna     p1_airattack_ok2
        jmp     p1_airattack_end
p1_airattack_ok2:
        mov     ax,p2_subx
        add     al,p2_px
        adc     ah,0
        cmp     ax,r0
        jnc     p1_airattack_ok3
        jmp     p1_airattack_end           ;check x position over
p1_airattack_ok3:
        mov     ax,r3
        cmp     ax,p2_txty
        ja      p1_airattack_ok4
        jmp     p1_airattack_end
p1_airattack_ok4:
        mov     ax,r2
        mov     cx,p2_txty
        add     cl,p2_length
        adc     ch,0
        sub     cx,Y_OFFDOWN
        cmp     ax,cx
        ja      p1_airattack_end
;  hit !
        mov     p2_hitback,1
        mov     p1_airstop,0
        mov     p1_aircnt,2
        push    bx
        mov     ax,39
        call    check_p2_lose
        pop     bx
        mov     al,[bx+offset p1_atrbuf+72*5+4]
        test    al,0ffh
        jnz     p1_set_airatkact
        mov     al,[bx+offset p1_atrbuf+72*5+5]
        jmp     p1_airatkact_end
p1_set_airatkact:
        mov     bx,p2_losenow
        cmp     bx,p2_blood
        jc      p1_airattack_ok6
        mov     al,12
        jmp     p1_airatkact_end
p1_airattack_ok6:
        xor     bx,bx
        mov     bl,p2_sprite
        shl     bx,1
        shl     bx,1
        mov     ah,[bx+offset p2_atrbuf+72]
;
        mov     bx,p2_y
        mov     dl,p2_pthno
        call    check_hitted_act
p1_airatkact_end:
        test    al,0ffh
        jz      p1_airattack_end
        mov     p2_hitnum,al
p1_airattack_end:
;
        test    p2_hitflg,0ffh
        jz      p2_check_attack
        jmp     p2_attack_end
p2_check_attack:
        test    p1_atkatt,BIT6
        jz      p2_check_attack2
        jmp     p2_airattack_end
p2_check_attack2:
        test    p2_atkatt,80h
        jnz     p2_attack_main
        jmp     p2_attack_end
p2_attack_main:
        xor     bx,bx
        mov     bl,p2_atkatt
        and     bl,0fh
        shl     bx,1
        shl     bx,1
        shl     bx,1
        xor     ax,ax
        mov     al,[bx+offset p2_atrbuf+72*5]
        mov     r0,ax
        mov     al,[bx+offset p2_atrbuf+72*5+1]
        mov     r2,ax
        mov     al,[bx+offset p2_atrbuf+72*5+2]
        mov     r1,ax
        mov     al,[bx+offset p2_atrbuf+72*5+3]
        mov     r3,ax
        test    p2_direct,0ffh
        jnz     p2_attack_rt
        mov     ax,p2_x
        add     ax,r0
        mov     r0,ax
        add     r1,ax
        jmp     p2_attack_ok
p2_attack_rt:
        xor     ax,ax
        mov     al,p2_px
        shl     ax,1
        add     ax,p2_subx
        sub     ax,r0
        sbb     ax,r1
        add     r1,ax
        cwd
        xor     dx,0ffffh
        and     ax,dx
        mov     r0,ax
p2_attack_ok:
        test    r0,8000h
        jz      p2_redefx1
        mov     r0,0
p2_redefx1:
        test    r1,8000h
        jz      p2_redefx2
        mov     r1,0
p2_redefx2:
        mov     ax,r2
        sub     ax,p2_y
        mov     r2,ax
        add     r3,ax                   ;calc y position over
        mov     ax,p1_pretx
        cmp     ax,r1
        jna     p2_attack_ok2
        jmp     p2_attack_end
p2_attack_ok2:
        mov     ax,p1_subx
        add     al,p1_px
        adc     ah,0
        cmp     ax,r0
        ja      p2_attack_ok3
        jmp     p2_attack_end           ;check x position over
p2_attack_ok3:
        mov     ax,r3
        cmp     ax,p1_txty
        ja      p2_attack_ok4
        jmp     p2_attack_end
p2_attack_ok4:
        mov     ax,r2
        mov     cx,p1_txty
        add     cl,p1_length
        adc     ch,0
        cmp     ax,cx
        jna     p2_attack_con
        jmp     p2_attack_end
p2_attack_con:
        inc     p2_hitflg
        mov     p2_light,1
        mov     ax,p1_txtx
        test    ax,8000h
        jz      p2lightok1
        mov     ax,0
p2lightok1:
        mov     p2_lightx,ax
        mov     cx,p2_txtx
        test    cx,8000h
        jz      p2lightok2
        mov     cx,0
p2lightok2:
        cmp     ax,cx
        ja      setp2lightx
        mov     p2_lightx,cx
setp2lightx:
        sub     p2_lightx,8
        mov     ax,p1_txty
        mov     p2_lighty,ax
        cmp     ax,p2_txty
        ja      setp2lighty
        mov     ax,p2_txty
        mov     p2_lighty,ax
setp2lighty:
        test    p2_lighty,8000h
        jz      setp2lighty2
        mov     p2_lighty,0
setp2lighty2:
        mov     p1_hitback,1
        test    p1_direct,0ffh
        jnz     p2_chkp1hitrt
        mov     ax,p1_txtx
        jmp     p2_chkp1hitmain
p2_chkp1hitrt:
        xor     ax,ax
        mov     al,p1_width
        add     ax,p1_txtx
        sbb     ax,480
p2_chkp1hitmain:
        cwd
        xor     ax,dx
        sub     ax,dx
        cmp     ax,BACK_REF
        ja      p2_chkp1hitend
        mov     p2_hitback,1
p2_chkp1hitend:
        push    bx
        mov     ax,0
        call    check_p1_lose
        pop     bx
        mov     al,[bx+offset p2_atrbuf+72*5+4]
        test    al,0ffh
        jnz     p2_set_atkact
        mov     ax,p1_losenow
        cmp     ax,p1_blood
        jnc     p2_atkact_fix
        cmp     p1_pthno,7
        jz      p2_atkact_end
        cmp     p1_pthno,8
        jz      p2_atkact_end
p2_atkact_fix:
        mov     al,[bx+offset p2_atrbuf+72*5+5]
        jmp     p2_atkact_end
p2_set_atkact:
        test    p1_sleepcnt,80h
        jnz     p2_attack_ok5
        mov     bx,p1_losenow
        cmp     bx,p1_blood
        jc      p2_attack_ok6
p2_attack_ok5:
        mov     al,12
        jmp     p2_atkact_end
p2_attack_ok6:
        xor     bx,bx
        mov     bl,p1_sprite
        shl     bx,1
        shl     bx,1
        mov     ah,[bx+offset p1_atrbuf+72]
;
        mov     bx,p1_y
        mov     dl,p1_pthno
        mov     cx,p2_light
        call    check_hitted_act
        mov     p2_light,cx
p2_atkact_end:
        test    al,0ffh
        jz      p2_attack_end
        mov     p1_hitnum,al
p2_attack_end:
;  check p2 air attack
        test    p2_airflag,0ffh
        jnz     p2_airattack_chk
        jmp     p2_airattack_end
p2_airattack_chk:
        xor     bx,bx
        mov     bl,p2_airsprite
        shl     bx,1
        shl     bx,1
        mov     al,p2_airtype
        and     al,01h
        add     bl,al
        mov     al,[bx+offset p2_atrbuf+72]
        test    al,80h
        jnz     p2_airattack_main
        jmp     p2_airattack_end
p2_airattack_main:
        xor     bx,bx
        mov     bl,al
        and     bl,0fh
        shl     bx,1
        shl     bx,1
        shl     bx,1
        xor     ax,ax
        mov     al,[bx+offset p2_atrbuf+72*5]
        mov     r0,ax
        mov     al,[bx+offset p2_atrbuf+72*5+1]
        mov     r2,ax
        mov     al,[bx+offset p2_atrbuf+72*5+2]
        mov     r1,ax
        mov     al,[bx+offset p2_atrbuf+72*5+3]
        mov     r3,ax
;
        test    p2_airdir,0ffh
        jz      p2_airattack_000
        mov     ax,p2_airwid2
        sub     ax,r0
        sbb     ax,r1
        add     ax,p2_airtxtx
        jmp     p2_airattack_010
p2_airattack_000:
        mov     ax,r0
        sub     al,p2_airx
        sbb     ah,0
        add     ax,p2_airtxtx
p2_airattack_010:
        test    ax,8000h
        jz      p2_airattack_main2
        mov     ax,0
p2_airattack_main2:
        mov     r0,ax
        add     r1,ax
;
        mov     ax,r2
        sub     al,p2_airy
        sbb     ah,0
        add     ax,p2_airtxty
        mov     r2,ax
        add     r3,ax                   ;calc y position over
        mov     ax,p1_pretx
        cmp     ax,r1
        jna     p2_airattack_ok2
        jmp     p2_airattack_end
p2_airattack_ok2:
        mov     ax,p1_subx
        add     al,p1_px
        adc     ah,0
        cmp     ax,r0
        jnc     p2_airattack_ok3
        jmp     p2_airattack_end           ;check x position over
p2_airattack_ok3:
        mov     ax,r3
        cmp     ax,p1_txty
        ja      p2_airattack_ok4
        jmp     p2_airattack_end
p2_airattack_ok4:
        mov     ax,r2
        mov     cx,p1_txty
        add     cl,p1_length
        adc     ch,0
        sub     cx,Y_OFFDOWN
        cmp     ax,cx
        ja      p2_airattack_end
;  hit !
        mov     p1_hitback,1
        mov     p2_airstop,0
        mov     p2_aircnt,2
        push    bx
        mov     ax,39
        call    check_p1_lose
        pop     bx
        mov     al,[bx+offset p2_atrbuf+72*5+4]
        test    al,0ffh
        jnz     p2_set_airatkact
        mov     al,[bx+offset p2_atrbuf+72*5+5]
        jmp     p2_airatkact_end
p2_set_airatkact:
        mov     bx,p1_losenow
        cmp     bx,p1_blood
        jc      p2_airattack_ok6
        mov     al,12
        jmp     p2_airatkact_end
p2_airattack_ok6:
        xor     bx,bx
        mov     bl,p1_sprite
        shl     bx,1
        shl     bx,1
        mov     ah,[bx+offset p1_atrbuf+72]
;
        mov     bx,p1_y
        mov     dl,p1_pthno
        call    check_hitted_act
p2_airatkact_end:
        test    al,0ffh
        jz      p2_airattack_end
        mov     p1_hitnum,al
p2_airattack_end:
;
        mov     al,p1_hitnum
        test    al,0ffh
        jz      p1_exchg_end
        call    setp1path
        mov     p1_type,0
        mov     al,p2_direct
        xor     al,0ffh
        mov     p1_direct,al
        mov     j1_trg,0
        mov     p1_aitype,0
        mov     p2_sleep,0
        mov     p2_sleepcnt,0
p1_exchg_end:
        mov     al,p2_hitnum
        test    al,0ffh
        jz      p2_exchg_end
        call    setp2path
        mov     p2_type,0
        mov     al,p1_direct
        xor     al,0ffh
        mov     p2_direct,al
        mov     j2_trg,0
        mov     p2_aitype,0
        mov     p1_sleep,0
        mov     p1_sleepcnt,0
p2_exchg_end:
        ret
check_attack    ENDP
;
check_hitted_act        PROC    NEAR
chk_hit_act00:
        cmp     al,01h
        jnz     chk_hit_act10
        test    bx,0ffffh
        jz      chk_hit_act001
        mov     al,13
        jmp     chk_hit_end
chk_hit_act001:
        mov     al,0
        cmp     dl,7
        jz      chk_hit00_end
        cmp     dl,8
        jz      chk_hit00_end
        mov     al,10
        test    ah,20h
        jz      chk_hit00_end
        mov     al,9
chk_hit00_end:
        jmp     chk_hit_end
chk_hit_act10:
        cmp     al,02h
        jnz     chk_hit_act20
        test    bx,0ffffh
        jz      chk_hit_act15
        mov     al,13
        jmp     chk_hit_end
chk_hit_act15:
        mov     al,0
        cmp     dl,7
        jz      chk_hit10_end
        cmp     dl,8
        jz      chk_hit10_end
        mov     al,11
        test    ah,20h
        jz      chk_hit10_end
        mov     al,9
chk_hit10_end:
        jmp     chk_hit_end
chk_hit_act20:
        cmp     al,03h
        jnz     chk_hit_act30
        test    bx,0ffffh
        jz      chk_hit_act25
        mov     al,0
        mov     cx,0
        jmp     chk_hit_end
chk_hit_act25:
        mov     al,0
        cmp     dl,8
        jz      chk_hit20_end
        mov     al,11
        test    ah,20h
        jz      chk_hit20_end
        mov     al,9
chk_hit20_end:
        jmp     chk_hit_end
chk_hit_act30:
        cmp     al,04h
        jnz     chk_hit_act40
        test    bx,0ffffh
        jz      chk_hit_act35
        mov     al,13
        jmp     chk_hit_end
chk_hit_act35:
        mov     al,0
        cmp     dl,7
        jz      chk_hit30_end
        mov     al,10
        test    ah,20h
        jz      chk_hit30_end
        mov     al,9
chk_hit30_end:
        jmp     chk_hit_end
chk_hit_act40:
        cmp     al,05h
        jnz     chk_hit_act50
        test    bx,0ffffh
        jz      chk_hit_act045
        mov     al,12
        jmp     chk_hit_end
chk_hit_act045:
        mov     al,0
        cmp     dl,7
        jz      chk_hit40_end
        cmp     dl,8
        jz      chk_hit40_end
        mov     al,10
        test    ah,20h
        jz      chk_hit40_end
        mov     al,9
chk_hit40_end:
        jmp     chk_hit_end
chk_hit_act50:
        cmp     al,06h
        jnz     chk_hit_act60
        test    bx,0ffffh
        jz      chk_hit_act55
        mov     al,12
        jmp     chk_hit_end
chk_hit_act55:
        mov     al,0
        cmp     dl,7
        jz      chk_hit50_end
        mov     al,10
        test    ah,20h
        jz      chk_hit50_end
        mov     al,9
chk_hit50_end:
        jmp     chk_hit_end
chk_hit_act60:
        cmp     al,07h
        jnz     chk_hit_act70
        test    bx,0ffffh
        jz      chk_hit_act65
        mov     al,0
        mov     cx,0
        jmp     chk_hit_end
chk_hit_act65:
        mov     al,0
        cmp     dl,8
        jz      chk_hit60_end
        mov     al,12
chk_hit60_end:
        jmp     chk_hit_end
chk_hit_act70:
chk_hit_end:
        ret
check_hitted_act        ENDP
;
check_p2_lose   PROC    NEAR
        xor     bx,bx
        mov     bl,p2_sprite
        shl     bx,1
        shl     bx,1
        mov     ch,[bx+offset p2_atrbuf+74]
        and     ch,0fh
;
        mov     bx,ax
        test    ax,0ffffh
        jnz     chkp2lose
        xor     bx,bx
        mov     bl,p1_pthno
        test    p1_forcenum,0ffh
        jz      chkp2lose
        mov     bl,p1_forcenum
chkp2lose:
        sub     bx,26
        jnc     chkp2lose2
        mov     bx,0
chkp2lose2:
        mov     cl,4
        shl     bx,cl
        add     bl,ch
        adc     bh,0
        xor     cx,cx
        mov     cl,[bx+offset p1_atrbuf+72*5+16*8]
        add     p2_sleep,cx
        add     cx,p2_losenow
        mov     p2_losenow,cx
        cmp     cx,p2_blood
        jc      p1_set_atkact2
        mov     cx,p2_blood
        mov     p2_losenow,cx
        mov     p2_die,01h
p1_set_atkact2:
        mov     p2_sleepcnt,0
        cmp     p2_sleep,40
        jna     p1_set_atkact4
        mov     p2_sleepcnt,80h
        mov     p2_sleep,0
p1_set_atkact4:
        ret
check_p2_lose   ENDP
;
check_p1_lose   PROC    NEAR
        xor     bx,bx
        mov     bl,p1_sprite
        shl     bx,1
        shl     bx,1
        mov     ch,[bx+offset p1_atrbuf+74]
        and     ch,0fh
;
        mov     bx,ax
        test    ax,0ffffh
        jnz     chkp1lose
        xor     bx,bx
        mov     bl,p2_pthno
        test    p2_forcenum,0ffh
        jz      chkp1lose
        mov     bl,p2_forcenum
chkp1lose:
        sub     bx,26
        jnc     chkp1lose2
        mov     bx,0
chkp1lose2:
        mov     cl,4
        shl     bx,cl
        add     bl,ch
        adc     bh,0
        xor     cx,cx
        mov     cl,[bx+offset p2_atrbuf+72*5+16*8]
        add     p1_sleep,cx
        add     cx,p1_losenow
        mov     p1_losenow,cx
        cmp     cx,p1_blood
        jc      p2_set_atkact2
        mov     cx,p1_blood
        mov     p1_losenow,cx
        mov     p1_die,01h
p2_set_atkact2:
        mov     p1_sleepcnt,0
        cmp     p1_sleep,40
        jna     p2_set_atkact4
        mov     p1_sleepcnt,80h
        mov     p1_sleep,0
p2_set_atkact4:
        ret
check_p1_lose   ENDP
;  =================================
;  =  Delay time                   =
;  =================================
D_hitted        PROC    NEAR
        mov     al,p1_die
        or      al,p2_die
        test    al,0ffh
        jz      dhit5
        test    escape,0ffh
        jnz     dhit9
        mov     cx,3
        call    Delay
        jmp     dhit9
dhit5:
        mov     ax,p1_light
        or      ax,p2_light
        test    ax,0ffffh
        jz      dhit9
        cmp     ax,0002h
        jnz     dhit9
        mov     cx,5
        call    Delay
dhit9:  ret
D_hitted        ENDP
;
local_proc      PROC    NEAR
        call    pth_control
        call    set_scrxy
        call    f_sbuf2
        call    f_shadow
        call    f_man
        call    f_board
        call    f_round
        call    f_scr2
        ret
local_proc      ENDP
;
;  =================================
;  =  MAIN PROGRAM                 =
;  =================================
fight_main      PROC    NEAR
        call    show_load
        call    set_pointer
        call    set_control
;
        call    keyinit
        call    chkdelay
round_again:
        call    R_ai
        call    R_bgn
        call    S_board
        call    R_shadow
        call    R_man
        call    R_path
        call    far ptr _midi_stop
        mov     si,10
        call    load_music
        call    play_music
        mov     al,1
        call    mkfade
        VOFF
        call    initial_vga
        call    initial_local_var
        call    initial_var
        mov     timecnt,4
        mov     round_flag,0
        call    f_color
        call    f_second
        call    local_proc
        VON
predo:
        call    local_proc
        cmp     round_flag,0ffffh
        jnz     predo
        mov     round_flag,0
        mov     timecnt,0
doagain:
        inc     counter
        call    random_gen
        call    read_ctl
        call    check_attack
        call    attack_control
        call    joy_control_p1
        call    joy_control_p2
        call    d_blood
        call    pth_control
        call    set_scrxy
        call    f_sbuf
        call    f_shadow
        call    f_man
        call    shake_man
        call    f_second
        call    f_board
        call    f_scr2
        call    D_hitted
        test    second,0ffffh
        jz      round_over
        mov     al,p1_die
        or      al,p2_die
        cmp     al,080h
        jz      round_over
        mov     al,escape
        test    al,080h
        jz      condo
        jmp     notdo
condo:
        test    al,0fh
        jz      nofull
        cmp     p1_blood,7ch
        jz      nofullp1
        inc     p1_blood
nofullp1:
        cmp     p2_blood,7ch
        jz      nofullp2
        inc     p2_blood
nofullp2:
        call    f_blood
        mov     ax,p1_blood
        and     ax,p2_blood
        cmp     ax,7ch
        jnz     nofull
        mov     p1_die,0
        mov     p2_die,0
        mov     escape,0
nofull:
        mov     cx,delaycnt
slow:   loop    slow
        jmp     doagain
round_over:
        cmp     main_select,4
        jnz     notdemofight
        jmp     notdo
notdemofight:
        call    round_end_proc
        call    d_blood
        call    pth_control
        call    set_scrxy
        call    f_sbuf
        call    f_shadow
        call    f_man
        call    f_board
        call    f_scr2
        cmp     round_flag,0ffffh
        jnz     round_over
        call    S_board
        call    f_board
        call    f_scr2
        call    f_control
        mov     cx,0ffffh
waittime:
        loop    waittime
        inc     round
        mov     who_win,0
        cmp     p1_win,2
        jnc     notdo
        mov     who_win,1
        cmp     p2_win,2
        jnc     notdo
        cmp     round,10
        jnc     notdo
        jmp     round_again
notdo:
        mov     cx,18
        call    Delay
        call    keyundo
exit:
        mov     al,1
        call    mkfade
        call    reset_vga
        ret
fight_main      ENDP
;
_TEXT   ENDS
        END
