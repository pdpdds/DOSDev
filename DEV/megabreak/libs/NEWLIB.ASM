.386P
.MODEL FLAT


_TEXT SEGMENT DWORD PUBLIC 'CODE'
	ASSUME	cs:_TEXT, ds:_DATA

; Functions for mode 13h

; Function-names

   public mode_13h
   public text_mode
   public put_block
   public bput_block
   public plot
   public h_line
   public wait_vbl
   public clear_screen
   public clear_screen_c
   public draw_light
   public put_sprite
   public bput_sprite
   public draw_box
   public bdraw_box
   public screen_copy
   public block_copy
   public block_copy_b
   public cput_block
   public anti_alias
;   public BG_block_to_buffer
;   public BG_buffer_to_screen


; Functions for mode-x

; Function-names

   public mode400
   public xplot
   public xput_block


; Konstanter

BG_BUFFER_WIDTH         EQU     960
BG_BUFFER_HEIGHT        EQU     600


; Variabler

clipart         dd      0                       ;    :)
pixoff          dd      200     DUP     (5)     ; Precalculated offsets

; ********************************************************************
; void mode_13h();
;
; Sets the screen mode to 320x200x256 mode (MCGA).
; ********************************************************************

mode_13h   PROC   NEAR

mov    eax,0013h
int    10h
;lea    ebx, pixoff
;mov    edx, 0
;mov    ecx, 0

;offset_next:

;mov    [ebx], edx
;add    ebx, 4
;add    edx, 320
;inc    ecx

;cmp    ecx, 200
;jl     offset_next

ret

mode_13h   ENDP

; ********************************************************************
; void text_mode();
;
; Sets the screen mode to normal text-mode.
; ********************************************************************

text_mode   PROC   NEAR

mov    eax,0003h
int    10h

ret

text_mode   ENDP

; ********************************************************************
; void put_block(long x, long y, long xlen, long ylen, byte *imgpnt)
;
; Blits a graphics-block to the screen.
; ********************************************************************

put_block    PROC NEAR

push    ebp
mov     ebp,esp

ARG     X:DWORD, Y:DWORD, XLEN:DWORD, YLEN:DWORD, IMGPNT:DWORD

; Kopier parametre..
mov     eax,[Y]
mov     ebx,[X]
mov     esi,IMGPNT

mov     edi,0A0000h          ; Video-mem...
mov     ecx,eax
shl     eax,8
shl     ecx,6
add     eax,ecx             ; Legg sammen begge Y-ene
add     eax,ebx             ; Legg til X
add     edi,eax             ; Ferdig koordinat ligger nå i EDI
mov     edx,320             ; Kalkuler mengde som må legges til EDI på
mov     ebx,[XLEN]          ; slutten av hver linje.
sub     edx,ebx             ; Mengde ligger i EDX
mov     eax,[YLEN]          ; Legg vertikal teller i EAX

next_line:

mov     ecx,ebx             ; Legg linje-teller i ECX
rep     movsb               ; Tegn opp en linje..
dec     eax                 ; En linje mindre..
add     edi,edx             ; Legg til slik at neste linje begynner under..
or      eax,eax             ; Vertikal teller 0 ennå?
jnz     next_line           ; Nei - gå til neste linje

mov     esp, ebp            ; Ja - ferdig
pop     ebp

ret

put_block    ENDP

; ********************************************************************
; void bput_block(long x, long y, long xlen, long ylen, byte *imgpnt, byte *scr)
;
; Blits a graphics-block to a virtual screen
; ********************************************************************

bput_block    PROC NEAR

push    ebp
mov     ebp,esp

ARG     X:DWORD, Y:DWORD, XLEN:DWORD, YLEN:DWORD, IMGPNT:DWORD, SCR:DWORD

; Kopier parametre..
mov     eax,[Y]
mov     ebx,[X]
mov     esi,IMGPNT

mov     edi,SCR          ; Video-mem...
mov     ecx,eax
shl     eax,8
shl     ecx,6
add     eax,ecx             ; Legg sammen begge Y-ene
add     eax,ebx             ; Legg til X
add     edi,eax             ; Ferdig koordinat ligger nå i EDI
mov     edx,320             ; Kalkuler mengde som må legges til EDI på
mov     ebx,[XLEN]          ; slutten av hver linje.
sub     edx,ebx             ; Mengde ligger i EDX
mov     eax,[YLEN]          ; Legg vertikal teller i EAX

next_line5:

mov     ecx,ebx             ; Legg linje-teller i ECX
rep     movsb               ; Tegn opp en linje..
dec     eax                 ; En linje mindre..
add     edi,edx             ; Legg til slik at neste linje begynner under..
or      eax,eax             ; Vertikal teller 0 ennå?
jnz     next_line5          ; Nei - gå til neste linje

mov     esp, ebp            ; Ja - ferdig
pop     ebp

ret

bput_block    ENDP


; ******************************************************************
; void plot(long X, long Y, long colour);
;
; Plots a pixel on the screen.
; ******************************************************************

plot    PROC NEAR

push    ebp
mov     ebp,esp

; Parametre
ARG     X:DWORD, Y:DWORD, COLOUR:DWORD

mov     eax,[Y]
mov	ebx,[X]
mov	edx,[COLOUR]

mov     edi, 0a0000h
shl     eax, 6
add     edi, eax
shl     eax, 1
shl     eax, 1
add     edi, eax
add     edi, ebx

;shl     eax, 2
;mov     edi, 0a0000h
;add     eax, pixoff
;add     edi, [eax]

mov     BYTE PTR [edi], dl  ; Draw the pixel at the desired location.

mov     esp, ebp
pop     ebp

ret

plot   ENDP

; ******************************************************************
; void h_line(int x, int y, int x2, byte colour);
;
; Plots a pixel on the screen.
; ******************************************************************

h_line    PROC NEAR

push    ebp
mov     ebp,esp

; Parametre
ARG     X:DWORD, Y:DWORD, X2:DWORD, COLOUR:BYTE

mov     eax,[Y]
mov	ebx,[X]
mov     ecx,[X2]
mov     dl,[COLOUR]

shl     eax, 6
mov     edi, eax
shl     eax, 2
add     edi, eax
sub     ecx, ebx
inc     ecx
add     edi, 0a0000h
add     edi, ebx

line_next:

mov     BYTE PTR [edi], dl
inc     edi
dec     ecx
cmp     ecx, 0
jnz     line_next

mov     esp, ebp
pop     ebp

ret

h_line   ENDP


; ************************************************************
; void wait_vbl();
;
; Funksjonen venter pσ VBL..
; ************************************************************

wait_vbl	PROC	NEAR

        mov  dx,03DAh
loop1:
	in   al,dx
	test al,8
	jnz  loop1
loop2:
	in   al,dx
	test al,8
	jz   loop2
	
	ret

wait_vbl	ENDP

; ***************************************************************
; void clear_screen();
;
; Funksjonen clear'er skjermen. Den tegner 4 pix'ler hver gang.
; Den bruker i tillegg roop-unrolling x2...
; ***************************************************************

clear_screen	PROC	NEAR

	mov   edi,0a0000h
        mov   ecx,8000          ; 64000/4 (4 pixels samtidig) / 2 (dobbel loop-unrolling)
        mov   eax,00000000h    ; Farge = bare svart
next_pixel:
	mov   [edi], eax
	add   edi,4
	mov   [edi], eax
	add   edi,4
	dec   ecx
	or    ecx,ecx
	jnz   next_pixel

	ret

clear_screen	ENDP

; **********************************************************************
; void clear_screen_c(unsigned long colour);
;
; Clear'er skjermen til en spesifisert farge. Den tegner 4 pix'ler om 
; gangen. Parameter mσ inneholde en blokk med farge (f.x. 0FFFFFFFFh)
; **********************************************************************

clear_screen_c	PROC	NEAR

	ARG   COLOUR:DWORD

	push    ebp
	mov     ebp,esp

	mov   edi,0a0000h
        mov   ecx,8000          ; 64000/4 (4 pixels samtidig) / 2 (dobbel loop-unrolling)
        mov   eax,[COLOUR]      ; Farge = spesifisert i parameter
next:
	mov   [edi], eax
	add   edi,4
	mov   [edi], eax
	add   edi,4
	dec   ecx
	or    ecx,ecx
        jnz   next

	mov     esp, ebp
	pop     ebp

	ret

clear_screen_c	ENDP

; *******************************************************************
; void draw_box(long x, long y, long xlen, long ylen, long colour);
;
; Funksjonen tegner en boks etter fargen spesifisert i colour..
; *******************************************************************

draw_box	PROC	NEAR
	
	push  ebp
	mov   ebp, esp

	ARG X:DWORD, Y:DWORD, XLEN:DWORD, YLEN:DWORD, COLOUR:DWORD

	mov     eax,[Y]
	mov	ebx,[X]

	mov     edi,0A0000h	; Video-mem..
	mov     ecx,eax		; Calculate pixel-position..
	shl     eax,8                     
	shl     ecx,6                     
	add     eax,ecx                   
	add     eax,ebx                   
	add     edi,eax
	mov	ebx,320		; Kalkuler mengde som må legges til DI på
	mov	edx,[XLEN]
	sub	ebx,edx
	mov	eax,[YLEN]	; Legg vertikal teller i AX
	mov	edx,[COLOUR]
next_line2:
	mov	ecx,[XLEN]	; Horisontal teller i CX
next2:
        mov     [edi],dl        ; Legg en byte til EDI
	dec	ecx
	inc	edi
	or	ecx,ecx
        jnz     next2
	dec	eax
	add	edi,ebx		; Legg til slik at neste linje begynner
	or	eax,eax		; Er vertikal teller 0 ennå?
        jnz     next_line2

	mov   esp, ebp
	pop   ebp

	ret

draw_box	ENDP

; *******************************************************************
; void bdraw_box(long x, long y, long xlen, long ylen, long colour, byte *scr);
;
; Funksjonen tegner en boks etter fargen spesifisert i colour..
; *******************************************************************

bdraw_box        PROC    NEAR
	
	push  ebp
	mov   ebp, esp

        ARG X:DWORD, Y:DWORD, XLEN:DWORD, YLEN:DWORD, COLOUR:DWORD, SCR:DWORD

	mov     eax,[Y]
	mov	ebx,[X]

        mov     edi,SCR
	mov     ecx,eax		; Calculate pixel-position..
	shl     eax,8                     
	shl     ecx,6                     
	add     eax,ecx                   
	add     eax,ebx                   
	add     edi,eax
	mov	ebx,320		; Kalkuler mengde som må legges til DI på
	mov	edx,[XLEN]
	sub	ebx,edx
	mov	eax,[YLEN]	; Legg vertikal teller i AX
	mov	edx,[COLOUR]
next_line4:
	mov	ecx,[XLEN]	; Horisontal teller i CX
next4:
        mov     [edi],dl        ; Legg en byte til EDI
	dec	ecx
	inc	edi
	or	ecx,ecx
        jnz     next4
	dec	eax
	add	edi,ebx		; Legg til slik at neste linje begynner
	or	eax,eax		; Er vertikal teller 0 ennå?
        jnz     next_line4

	mov   esp, ebp
	pop   ebp

	ret

bdraw_box        ENDP

; ********************************************************************
; void screen_copy(byte *dest, byte *src);
;
; Copies the contents of one screen to another screen (64K)
; ********************************************************************

screen_copy    PROC NEAR

        push    ebp
        mov     ebp,esp

        ARG     DEST:DWORD, SRC:DWORD

        mov     esi, SRC
        mov     edi, DEST
        mov     ecx, 16000

next__line:

        rep movsd

        mov     esp, ebp
        pop     ebp

        ret

screen_copy     ENDP

; *************************************************************************
; void draw_light(long X, long Y, long xlen, long ylen, byte *lightptr,
;                 byte *imgptr);
;
; Denne funksjonen tegner et lys utifra bildet i lightptr. Lyset kan ha 10
; forskjellige intensiteter, og kan skape stilige effekter....
; *************************************************************************

xcounter        dd      ?
ycounter        dd      ?

draw_light      PROC    NEAR

        push  ebp
        mov   ebp, esp

ARG X:DWORD, Y:DWORD, XLEN:DWORD, YLEN:DWORD, LIGHTPTR:DWORD, IMGPTR:DWORD

        mov   esi, LIGHTPTR
        mov   ebx, IMGPTR
        mov   edi, 0a0000h

; *** Kalkuler start-koordinat i virtuell skjerm: IMGPTR
        mov     eax,[Y]
        mov     edx,[X]

        mov     ecx,eax
        shl     eax,8
        shl     ecx,6
        add     eax,ecx
        add     eax,edx                 ; Legg til X - ferdig koordinat i EAX
        add     ebx,eax                 ; Ferdig koordinat ligger nå i EBX

        add     edi,eax                 ; Legg det samme koordinatet i EDI

; *** Initialiser hoved-loop ***

        mov     edx,[YLEN]

bnext_line:
        mov     ecx,[XLEN]
        mov     [ycounter], edx         ; Spar vertikal teller i YCOUNTER
bnext_pixel:
        mov     [xcounter], ecx         ; Spar horisontal teller i XCOUNTER

; *** Hent pixel-farger fra source og dest.
        mov     cl,[esi]        ; Pixel-farge fra lys-data
        inc     esi             ; Gjør klar for neste pixel..
        mov     dl,[ebx]        ; Pixel-farge fra bilde-data
        inc     ebx             ; Gjør klar for neste pixel
        or      cl,cl           ; Hvis pixel-farge fra lys-data er 0..
        jz      bskip           ; ..så hopp over dette koordinatet.
        or      dl,dl           ; Hvis pixel-farge fra bilde-data er 0..
        jz      bskip           ; ..så hopp over dette koordinatet.
        xor     eax,eax         ; Sett EAX til 0
        mov     al,dl           ; Legg pixel-farge fra bilde-data i al
        mov     dl,11           ; Skal deles med 10...
        div     dl              ; Svar på divisjon ligger nå i AL
        mul     dl              ; * med 10 for å få start på farge-blokk
        add     al,cl           ; Legg sammen start-farge med aktuell nyanse

        mov     [edi], al       ; Plot denne fargen på skjermen..
bskip:
        inc     edi             ; Neste pixel...
        mov     ecx, [xcounter] ; Få fram horisontal teller i ECX
        dec     ecx             ; En pixel mindre...
        or      ecx,ecx         ; Er horisontal teller 0 ennå?
        jnz     bnext_pixel     ; Nei - gå til neste pixel

        mov     eax,320         ; Kalkuler mengde som må legges til EDI
        mov     edx,[XLEN]      ; ved linjeskift..
        sub     eax,edx         ; Mengde ligger nå i EAX

        mov     edx, [ycounter] ; Få fram vertikal teller i EDX
        dec     edx             ; Ja - sjekk etter neste linje
        add     edi,eax         ; Neste linje i skjerm-minne
        add     ebx,eax         ; Neste linje i virtuell skjerm
        or      edx,edx         ; Er vertikal teller 0 ennå?
        jnz     bnext_line      ; Nei - gå til neste linje

        mov   esp, ebp          ; Ja - gå ut
        pop   ebp

        ret

draw_light      ENDP

; ************************************************************************
; void put_sprite(int x, int y, int xlen, int ylen, byte *imgpnt)
;
; Denne funksjonen blitter en block til skjermen, men pix'ler med fargen
; 0 blir ikke tegnet, slik at bildet blir transparent.
; ************************************************************************

put_sprite      PROC    NEAR

        push    ebp
        mov     ebp,esp

        ARG     X:DWORD, Y:DWORD, XLEN:DWORD, YLEN:DWORD, IMGPNT:DWORD

        ; Kopier parametre..
        mov     eax,[Y]
        mov     ebx,[X]
        mov     esi,IMGPNT

        mov     edi,0A0000h             ; Video-mem...
        mov     ecx,eax
        shl     eax,8
        shl     ecx,6
        add     eax,ecx                 ; Legg sammen begge Y-ene
        add     eax,ebx                 ; Legg til X
        add     edi,eax                 ; Ferdig koordinat ligger nå i EDI
        mov     edx,320                 ; Kalkuler mengde som må legges til EDI på
        mov     ebx,[XLEN]              ; slutten av hver linje.
        sub     edx,ebx                 ; Mengde ligger i EDX
        mov     eax,[YLEN]              ; Legg vertikal teller i EAX
bloop1:
        mov     ecx,[XLEN]              ; Sett counter til horisontal lengde.
bloop2:
        mov  bl,[esi]                   ; Få en byte fra SI
        or   bl,bl                    ; Er den 0?
        je   skip_pixel                 ; OK - Skip den
        mov  [edi],bl                   ; Nei - Tegn den
        inc  edi
        inc  esi
        dec  ecx                        ; En pixel tegnet...
        or   ecx,ecx                    ; Er denne linjen ferdig ennå?
        jne  bloop2                      ; Nei - kjør gjennom igjen...
        add  edi,edx                    ; Legg til slik at neste linje starter under...
        dec  eax                        ; Ja - en linje ferdig...
        or   eax,eax                    ; Er Vertikal teller null enda?
        jne  bloop1                      ; Nei - kjør loop igjen
        jmp  bend                        ; Ja - avslutt
skip_pixel:
        inc  edi                        ; Hopp over dette koordinatet
        inc  esi
        dec  ecx                        ; En pixel "tegnet"..
        or   ecx,ecx                    ; Er horisontal teller 0?
        jne  bloop2                      ; Hopp tilbake hvis linje ikke er
                                        ; ferdig tegnet..
        add  edi,edx                    ; Legg til slik at neste linje starter under..
        or   eax,eax                    ; Er vertikal teller 0?
        dec  eax                        ; En linje mindre..
        jnz  bloop1                      ; Hvis den ikke er 0, initialiser
                                        ; teller på nytt.
bend:
        mov   esp, ebp                  ; gå ut
        pop   ebp

        ret

put_sprite      ENDP

; ************************************************************************
; void bput_sprite(int x, int y, int xlen, int ylen, byte *imgpnt, byte *buffer)
;
; Denne funksjonen blitter en block til en buffer, men pix'ler med fargen
; 0 blir ikke tegnet, slik at bildet blir transparent.
; ************************************************************************

bput_sprite      PROC    NEAR

        push    ebp
        mov     ebp,esp

        ARG     X:DWORD, Y:DWORD, XLEN:DWORD, YLEN:DWORD, IMGPNT:DWORD, BUF:DWORD

        ; Kopier parametre..
        mov     eax,[Y]
        mov     ebx,[X]
        mov     esi,IMGPNT

        mov     edi,BUF             ; Video-mem...
        mov     ecx,eax
        shl     eax,8
        shl     ecx,6
        add     eax,ecx                 ; Legg sammen begge Y-ene
        add     eax,ebx                 ; Legg til X
        add     edi,eax                 ; Ferdig koordinat ligger nå i EDI
        mov     edx,320                 ; Kalkuler mengde som må legges til EDI på
        mov     ebx,[XLEN]              ; slutten av hver linje.
        sub     edx,ebx                 ; Mengde ligger i EDX
        mov     eax,[YLEN]              ; Legg vertikal teller i EAX
bbloop1:
        mov     ecx,[XLEN]              ; Sett counter til horisontal lengde.
bbloop2:
        mov  bl,[esi]                   ; Få en byte fra SI
        or   bl,bl                    ; Er den 0?
        je   bskip_pixel                 ; OK - Skip den
        mov  [edi],bl                   ; Nei - Tegn den
        inc  edi
        inc  esi
        dec  ecx                        ; En pixel tegnet...
        or   ecx,ecx                    ; Er denne linjen ferdig ennå?
        jne  bbloop2                      ; Nei - kjør gjennom igjen...
        add  edi,edx                    ; Legg til slik at neste linje starter under...
        dec  eax                        ; Ja - en linje ferdig...
        or   eax,eax                    ; Er Vertikal teller null enda?
        jne  bbloop1                      ; Nei - kjør loop igjen
        jmp  bbend                        ; Ja - avslutt
bskip_pixel:
        inc  edi                        ; Hopp over dette koordinatet
        inc  esi
        dec  ecx                        ; En pixel "tegnet"..
        or   ecx,ecx                    ; Er horisontal teller 0?
        jne  bbloop2                      ; Hopp tilbake hvis linje ikke er
                                        ; ferdig tegnet..
        add  edi,edx                    ; Legg til slik at neste linje starter under..
        or   eax,eax                    ; Er vertikal teller 0?
        dec  eax                        ; En linje mindre..
        jnz  bbloop1                      ; Hvis den ikke er 0, initialiser
                                        ; teller på nytt.
bbend:
        mov   esp, ebp                  ; gå ut
        pop   ebp

        ret

bput_sprite      ENDP

; ********************************************************************
; void block_copy(int srcx, int srcy, int scrxlen, int srcylen,
;                 byte *srcimg, int destx, int desty);
;
; Copies a specified area from a buffer to a specified location on the
; screen.
; ********************************************************************

block_copy      PROC NEAR

push    ebp
mov     ebp,esp

ARG     SRCX:DWORD, SRCY:DWORD, SRCXLEN:DWORD, SRCYLEN:DWORD, SRCIMG:DWORD, DESTX:DWORD, DESTY:DWORD

; Kalkuler koordinater for virtuell skjerm (SRCIMG)
mov     esi,SRCIMG
mov     ecx,[SRCY]
shl     ecx,6
mov     eax,[SRCY]
shl     eax,8
add     eax,ecx
add     eax,[SRCX]
add     esi,eax

; Kalkuler koordinater for skjerm-minne
mov     eax,[DESTY]
mov     edi,0A0000h         ; Video-minne...
mov     ecx,eax
shl     eax,8
shl     ecx,6
add     eax,ecx             ; Legg sammen begge Y-ene
add     eax,[DESTX]         ; Legg til X
add     edi,eax             ; Ferdig koordinat ligger nå i EDI

; Kalkuler mengde som må legges til hver av skjermene ved linjeskift
mov     edx,320             ; Kalkuler mengde som må legges til EDI på
mov     ebx,[SRCXLEN]       ; slutten av hver linje.
sub     edx,ebx             ; Mengde ligger i EDX
mov     eax,[SRCYLEN]       ; Legg vertikal teller i EAX

next_line6:

mov     ecx,ebx             ; Legg linje-teller i ECX
rep     movsb               ; Tegn opp en linje..
add     esi, edx            ; Legg til 'linjeskift' på buffer
dec     eax                 ; En linje mindre..
add     edi,edx             ; Legg til 'linjeskift' på skjerm
or      eax,eax             ; Vertikal teller 0 ennå?
jnz     next_line6          ; Nei - gå til neste linje

mov     esp, ebp            ; Ja - ferdig
pop     ebp

ret

block_copy    ENDP

; ********************************************************************
; void block_copy_b(int srcx, int srcy, int scrxlen, int srcylen,
;                 byte *srcimg, int destx, int desty, byte *destimg);
;
; Copies a specified area from a buffer to a specified location to
; another buffer.
; ********************************************************************

block_copy_b      PROC NEAR

push    ebp
mov     ebp,esp

ARG     SRCX:DWORD, SRCY:DWORD, SRCXLEN:DWORD, SRCYLEN:DWORD, SRCIMG:DWORD, DESTX:DWORD, DESTY:DWORD, DESTIMG:DWORD

; Kalkuler koordinater for virtuell skjerm (SRCIMG)
mov     esi,SRCIMG
mov     ecx,[SRCY]
shl     ecx,6
mov     eax,[SRCY]
shl     eax,8
add     eax,ecx
add     eax,[SRCX]
add     esi,eax

; Kalkuler koordinater for skjerm-minne
mov     eax,[DESTY]
mov     edi,DESTIMG         ; Video-minne...
mov     ecx,eax
shl     eax,8
shl     ecx,6
add     eax,ecx             ; Legg sammen begge Y-ene
add     eax,[DESTX]         ; Legg til X
add     edi,eax             ; Ferdig koordinat ligger nå i EDI

; Kalkuler mengde som må legges til hver av skjermene ved linjeskift
mov     edx,320             ; Kalkuler mengde som må legges til EDI på
mov     ebx,[SRCXLEN]       ; slutten av hver linje.
sub     edx,ebx             ; Mengde ligger i EDX
mov     eax,[SRCYLEN]       ; Legg vertikal teller i EAX

next_line7:

mov     ecx,ebx             ; Legg linje-teller i ECX
rep     movsb               ; Tegn opp en linje..
add     esi, edx            ; Legg til 'linjeskift' på buffer
dec     eax                 ; En linje mindre..
add     edi,edx             ; Legg til 'linjeskift' på skjerm
or      eax,eax             ; Vertikal teller 0 ennå?
jnz     next_line7          ; Nei - gå til neste linje

mov     esp, ebp            ; Ja - ferdig
pop     ebp

ret

block_copy_b    ENDP

; ********************************************************************
; void cput_block(long x, long y, long xlen, long ylen, byte *imgpnt)
;
; Blits a graphics-block to the screen w/ clipping.
; ********************************************************************

cput_block    PROC NEAR

push    ebp
mov     ebp,esp

ARG     X:DWORD, Y:DWORD, XLEN:DWORD, YLEN:DWORD, IMGPNT:DWORD

; Kopier parametre..
mov     eax,[Y]
mov     ebx,[X]
mov     esi,IMGPNT

; Finn ut om block'en må 'clippes'
cmp     ebx, 319        ; Sjekk om block er utenfor skjermen
ja      cend
cmp     eax, 199
ja      cend
; Hvis ikke, finn ut om hele block skal tegnes
add     ebx, [XLEN]
add     eax, [YLEN]
cmp     ebx, 319
jle     normal_put
cmp     eax, 199
jle     normal_put
sub     ebx, 319
sub     eax, 199
mov     [clipart], 0
cmp     ebx, 0
ja      new_xlen
cmp     eax, 0
ja      new_ylen
jmp     normal_put

new_xlen:

sub     [XLEN], ebx
mov     [clipart], ebx
jmp     normal_put

new_ylen:

sub     [YLEN], eax

normal_put:

mov     eax,[Y]
mov     [Y], ebx
mov     ebx,[X]

mov     edi,0A0000h          ; Video-mem...
mov     ecx,eax
shl     eax,8
shl     ecx,6
add     eax,ecx             ; Legg sammen begge Y-ene
add     eax,ebx             ; Legg til X
add     edi,eax             ; Ferdig koordinat ligger nå i EDI
mov     edx,320             ; Kalkuler mengde som må legges til EDI på
mov     ebx,[XLEN]          ; slutten av hver linje.
sub     edx,ebx             ; Mengde ligger i EDX
mov     eax,[YLEN]          ; Legg vertikal teller i EAX

cnext_line:

mov     ecx,ebx             ; Legg linje-teller i ECX
rep     movsb               ; Tegn opp en linje..
dec     eax                 ; En linje mindre..
add     edi,edx             ; Legg til slik at neste linje begynner under..
add     esi,[clipart]
or      eax,eax             ; Vertikal teller 0 ennå?
jnz     cnext_line          ; Nei - gå til neste linje

cend:

mov     esp, ebp            ; Ja - ferdig
pop     ebp

ret

cput_block    ENDP


; Mode-X functions:

; ********************************************************************
; void mode400();
;
; Sets the 320x400x256 screen mode
; ********************************************************************

mode400         PROC NEAR

mov     ax, 0013h
int     10h

mov     dx, 3c4h
mov     al, 4
out     dx, al
inc     dx
in      al, dx
and     al, not 08h
or      al, 04h
out     dx, al
mov     dx, 3ceh
mov     al, 5
out     dx, al
inc     dx
in      al, dx
and     al, not 10h
out     dx, al
dec     dx
mov     al, 6
out     dx, al
inc     dx
in      al, dx
and     al, not 02h
out     dx, al

mov     dx, 3c4h
mov     ax, (0fh shl 8) + 2
;out     dx, ax
out     dx, al
inc     dx
xchg    ah, al
out     dx, al
dec     dx
xchg    ah, al

mov     edi, 0a0000h
sub     eax, eax
mov     cx, 8000h
cld
rep     stosw           ; Slett video-minne

mov     dx, 3d4h
mov     al, 9
out     dx, al
inc     dx
in      al, dx
and     al, not 1fh
;out     dx, ax
out     dx, al
inc     dx
xchg    ah, al
out     dx, al
dec     dx
xchg    ah, al
dec     dx

mov     al, 14h
out     dx, al
inc     dx
in      al, dx
and     al, not 40h
out     dx, al
dec     dx
mov     al, 17h
out     dx, al
inc     dx
in      al, dx
or      al, 40h
out     dx, al

ret

mode400         ENDP

; ********************************************************************
; void xplot(int x, int y, byte colour);
;
; Plots a Mode-X pixel
; ********************************************************************

xplot   PROC NEAR

push    ebp
mov     ebp,esp

ARG     X:DWORD, Y:DWORD, COLOUR:BYTE

; Kopier parametre
mov     ecx, [X]
mov     edx, [Y]

mov     edi, 0a0000h
mov     eax, 80         ; 320/4: Lengde på hvert bit-plan
mul     edx             ; Pek til riktig rad
mov     ebx, ecx
shr     ebx, 1
shr     ebx, 1          ; Del X-koordinat med 4 for å få riktig adresse
add     ebx, eax        ; Legg til Y-adresse
add     edi, ebx        ; Legg dette sammen i EDI
and     cl, 3           ; Samme som Xcoord % 4 for å finne riktig bit-plan
mov     ah, 1           ; Legg en bit i ah
shl     ah, cl          ; Shift denne slik at den tilsvarer riktig bit-plan
mov     al, 2           ; Register 2 i SC-Index for å skifte bit-plan
mov     dx, 3c4h        ; SC-Index i DX
out     dx, ax          ; Skift til riktig bit-plan

; Plot til slutt pixel på skjermen
mov     bl, [COLOUR]
mov     BYTE PTR [edi], bl

mov     esp, ebp
pop     ebp

ret

xplot   ENDP

; ********************************************************************
; void xput_block(int x, int y, int xlen, int ylen, int *imgpnt);
;
; Writes a normal graphics-block to the screen
; This function would benefit greatly from specially stored picture-data
; with the information for each bit-plane stored separately, allowing for
; use of rep movsd.
; ********************************************************************

xput_block      PROC NEAR

push    ebp
mov     ebp,esp

ARG     X:DWORD, Y:DWORD, XLEN:DWORD, YLEN:DWORD, IMGPNT:DWORD

; Kopier parametre
mov     ecx, [X]
mov     edx, [Y]
mov     esi, IMGPNT

mov     edi, 0a0000h
mov     eax, 80         ; 320/4: Lengde på hvert bit-plan
mul     edx             ; Pek til riktig rad
mov     ebx, ecx
shr     ebx, 1
shr     ebx, 1          ; Del X-koordinat med 4 for å få riktig adresse
add     edi, eax        ; Legg til Y-adresse
add     edi, ebx        ; Legg til X-adresse
mov     edx, 4          ; Hold 'øye' med hvor mange bit-plan som er tegnet

; Finn start bit-plan

switch_bit_plane:

push    ecx             ; Spar X-koordinatet
and     cl, 3           ; Samme som Xcoord % 4 for å finne riktig bit-plan
mov     ah, 1           ; Legg en bit i ah
shl     ah, cl          ; Shift denne slik at den tilsvarer riktig bit-plan
mov     al, 2           ; Register 2 i SC-Index for å skifte bit-plan
push    edx             ; Spar bit-plan count
mov     dx, 3c4h        ; SC-Index i DX
out     dx, ax          ; Skift til riktig bit-plan

draw_to_plane:

mov     eax, [YLEN]

xnext_line:

mov     ecx, [XLEN]
shr     ecx, 1
shr     ecx, 1          ; Fordi vi har 4 bit-plan må lengden fordeles

xnext_pixel:

mov     bl, BYTE PTR [esi]
mov     BYTE PTR [edi], bl      ; Tegn til skjermen
add     esi, 4                  ; Hopp til neste pixel som skal på dette plan
dec     ecx
or      ecx, ecx                ; Horisontal teller 0 ennå?
jnz     xnext_pixel             ; Nei - tegn en pixel til
dec     eax
or      eax, eax                ; Vertikal teller 0 ennå?
jnz     xnext_line              ; Nei - tegn en linje til
pop     edx                     ; Få fram igjen bit-plan count
dec     edx
or      edx, edx                ; Er alle bit-plan tegnet?
jz      endxput
pop     ecx
inc     ecx
jmp     switch_bit_plane

endxput:

pop     ecx

mov     esp, ebp
pop     ebp

ret

xput_block      ENDP



anti_alias      PROC NEAR

push    ebp
mov     ebp,esp

ARG     X:DWORD, Y:DWORD, XLEN:DWORD, YLEN:DWORD, SCRPNT:DWORD

; Kalkuler startpunkt
mov     eax, [Y]
mov     ebx, [X]
mov     ecx, eax
shl     eax, 8
shl     ecx, 6
mov     edi, scrpnt
add     eax, ecx
add     eax, ebx
add     edi, eax        ; Ferdig koord i EDI

mov     eax, [YLEN]

alias_next_line:

mov     ecx, [XLEN]

alias_next_pixel:

mov     ebx, 0
sub     edi, 321
add     bl, [edi]
inc     edi
add     bl, [edi]
inc     edi
add     bl, [edi]
add     edi, 318
add     bl, [edi]
add     edi, 2
add     bl, [edi]
add     edi, 318
add     bl, [edi]
inc     edi
add     bl, [edi]
inc     edi
add     bl, [edi]
sub     edi, 321

shr     bl, 3          ; Del med 8..
mov     BYTE PTR [edi], bl

inc     edi
dec     ecx
cmp     ecx, 0
jnz     alias_next_pixel
mov     ebx, 320
sub     ebx, [XLEN]
add     edi, ebx
dec     eax
cmp     eax, 0
jnz     alias_next_line

mov     esp, ebp
pop     ebp

ret

anti_alias      ENDP

; ********************************************************************
; void BG_tile_to_buffer(byte *tile, int destx, int desty, byte *buffer)
;                 byte *srcimg, int destx, int desty, byte *destimg);
;
; Copies a specified area from a buffer to a specified location to
; another buffer.
; ********************************************************************



_TEXT ENDS

   END
