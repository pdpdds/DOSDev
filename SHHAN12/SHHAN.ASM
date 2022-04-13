public _shhan
public _shget
public _shset
public _shmode
_TEXT SEGMENT BYTE public 'CODE'
ASSUME CS:_TEXT
start:
;************************************************************
;¡°ã°—¡ ûÅÀ• gd,gm∑aù° —e∏Å∑Å ãaúÅœ¢ óaúa∑°§·µ¡ °°óaüi set–eîa.
;gd,gm∑i CµA¨· ¨a∂w–aìe §Â—°µ¡ ∑©√°Ø°≈v∑a£aù° Àb•i–e îaüe
;¿·ü°àa ¶âœ©∂a–aîa.
_shset proc far
push bp       ;
mov bp,sp     ; ùAª°Øa»· îÅœ°
push es       ;
push ds       ;
push si       ;
push di       ;
mov ax,ss:[bp+6]   ;
mov cs:video,ax    ;
mov ax,ss:[bp+8]   ; stack∑Å ∑•ÆÅüi §h¥a videoµ¡ modeüi set
mov cs:mode,ax     ;
pop di          ;
pop si          ;ùAª°Øa»· •¢äÅ,ü°»Â
pop ds          ;
pop es          ;
pop bp          ;
RET             ;
_shset endp
;*****************************************************************
;–eãi ∑≥ùbØ°∑Å ∏aÃe∑i §aéÖîa.
;modeàa 0∑°°e –eãi ∏aÃe,1∑°°e µw¢Ö∏aÃe∑°îa.
_shmode proc far
push bp
mov bp,sp
push es
push ds
push si
push di
mov ax,ss:[bp+6]   ;
mov cs:nowjapan,al ;
pop di          ;
pop si          ;ùAª°Øa»· •¢äÅ,ü°»Â
pop ds          ;
pop es          ;
pop bp          ;
RET             ;
_shmode endp

;********************************************************************
;–eãi 16*16 ¢Ö∏aµi ¬âùb
;∑≥ùb: C µA shhan(int x,int y,char *string,int forecolor
;                  ,int bkcolor,int attr,int put);
;x,y      ¢Ö∏aàa ¬âùbñI π¡Œa.ãaúÅœ¢ π¡Œa∑°îa.
;string   ¬âùbñI ¢Ö∏aµi∑Å Õ°∑•»·
;forecolor,bkcolor   ¢Ö∏a¨Çâ¡ §Åâw¨Ç
;attr     ¢Ö∏a∑Å ≠¢¨˜ ª°∏˜
;put      —¡°e imageµ¡∑Å ß°Àaµe¨e π∑üA.0:COPY,1:XOR,2:OR,3:AND,4:NOT
_shhan PROC far
push bp
mov bp,sp
push es
push ds
push si
push di
mov ax,ss:[bp+6]   ;
mov cs:x,ax        ;
mov ax,ss:[bp+8]   ; stack∑Å ∑•ÆÅüi §h¥a •eÆÅµA ∏·∏w
mov cs:y,ax        ;
lds si,ss:[bp+10]  ;
MOV CS:STRINGBUF,SI;
mov ax,ss:[bp+14]  ;
mov cs:fcolor,ax   ;
mov ax,ss:[bp+16]  ;
mov cs:bcolor,ax   ;
mov ax,ss:[bp+18]  ;
mov cs:attr,ax     ;
mov ax,ss:[bp+20]  ;
mov cs:put,ax      ;
mov ax,0b000h        ; esüi vram§Âª°ù°
cmp cs:video,9       ; HGC°e b000,VGA°e a000
jnz m05              ;
mov ax,0a000h        ;
m05:                 ;
mov es,ax            ;
MOV AX,CS:X         ; ∂·√°µ¡ Ø°∏b§Âª° †x¬Ö “Å
MOV CS:MUNX,AX      ; MUNJANGJAE—°¬â
MOV AX,CS:Y         ;
MOV CS:MUNY,AX      ;
MOV AX,CS:STRINGBUF ;
CALL MUNJANGJAE     ;
m04:
pop di          ;
pop si          ;ùAª°Øa»· •¢äÅ,ü°»Â
pop ds          ;
pop es          ;
pop bp          ;
RET             ;
X Dw ?
Y Dw ?
attr dw ?
put dw ?
fcolor dw ?
bcolor dw ?
video dw 9
mode dw 0
STRINGBUF DW ?
buf1 db 128 dup (?)
_shhan ENDP
;*************************************************************
;¢Ö∏w ∏˜ùi ûÅÀ•
MUNJANGJAE PROC NEAR
MOV BP,AX
MOV SI,CS:STRINGBUF   ;
MOV AX,CS:X           ; ¬âùb ∂·√° ∏˜–q
MOV CS:MUNX,AX        ; §·Ã· Ø°∏b∂·√°µA¨· —e∂·√°åaª°∑Å
MOV AX,CS:Y           ; °A°°ü° §Âª°¿aüi äÅ–Å *8–Å¨·
MOV CS:MUNY,AX        ; Xπ¡Œa ∂·√°µA î·–q
MOV AX,BP             ;
SUB AX,SI             ;
SHL AX,1              ;
SHL AX,1              ;
SHL AX,1              ;
ADD CS:MUNX,AX        ;
MOV SI,BP

m02:MOV AX,DS:[SI]   ;
CMP AL,0             ; 0àa êaµ°°e π∑ûa •¢ä·
JNZ M03              ;
;mov al,20h
;call hexeng
;add cs:munx,8
;mov al,20h
;call hexeng
ret
M03:TEST AL,80H     ;
JZ M01              ; –eãi 1 ¢Ö∏a ¬âùb
CALL HEXHAN         ;
add CS:MUNX,16      ;
INC SI              ;
INC SI              ;
JMP M02             ;
M01:CALL HEXENG   ;
add cs:MUNx,8     ;  µw¢Ö £π ã°—° 1 ¢Ö∏a ¬âùb
INC SI            ;
JMP M02           ;

MUNX DW ?
MUNY DW ?
MUNJANGJAE ENDP

;ß°ó°µ° °A°°ü°∑Å §Âª° äÅ–q.HGC∑• âw∂Åµ¡ VGA∑• âw∂Åìe àbàb
;§Âª° âÅ¨e â∑Ø¢∑° îaüq
vaddr proc near
       mov ax,cs:video
       cmp ax,9
       jnz vaddr01
       mov ax,cs:MUNy    ; VGA ∑Å §Âª° âÅ¨e
       mov cx,50h        ; y*50h+x/8
       mul cx
       push ax
       mov ax,cs:MUNx
       mov cl,3
       shr ax,cl
       pop cx
       add ax,cx
       mov di,ax
       ret
vaddr01:mov ax,cs:MUNy  ;; HGC∑Å §Âª° âÅ¨e
       and ax,0003h      ;   (y mod 4)*2000h
       mov dx,2000h      ;
       mul dx            ;
       push ax          ;;
       mov ax,cs:MUNy ;;
       shr ax,1        ; (y/4)*50h
       shr ax,1        ;
       mov cl,50h      ;
       cmp cs:mode,00
       jnz vaddr02
       add cl,0ah
vaddr02:mul cl          ;
       push ax        ;;
       mov ax,cs:MUNx  ;;
       mov cl,3         ; x/8
       shr ax,cl       ;;
       pop cx       ;
       add ax,cx    ; ∂·∑Å ≠A àt∑i –s¨e–Å¨· diµA îÅ∑≥
       pop cx       ;
       add ax,cx    ;
       mov di,ax    ;
       RET          ;
vaddr endp
;******************************************************
; §·Ã·µA –eãi –e ¢Ö∏aüi π°ü≥
HEXHAN proc near
PUSH SI
mov bx,offset buf1  ;
mov cx,10h          ; ∫ó¨˜∑° ¥Ùìe âw∂Åüi â°ùa–Å¨·
h11:mov word ptr cs:[bx],0   ; §·Ã·üi £°ü° ª°∂Öîa.
inc bx              ;
inc bx              ;
loop h11            ;

MOV CS:HEXHANAX,AX
AND AX,0E003H             ;;
cmp ax,0                   ; ∫ó¨˜∑° ¥Ù∑a°e ∫ó¨˜∑e ¬âùb ¥e–q
jz h09                     ;
MOV CL,5                   ;
xchg al,ah                 ;
SHR AX,CL                  ; ∫ó¨˜ Ø°∏b§Âª° äÅ–q
MOV Cl,6                   ;
MUL Cl                     ;
ADD AX,OFFSET SIJACTABLE+2 ;
MOV SI,AX                 ;;
MOV SI,CS:[SI]
ADD SI,OFFSET HANFONT
MOV AX,CS:HEXHANAX   ;;
AND AX,01F00H         ;  §h√±∑° ∑∂∑a°e 20H î·–q
CMP AX,0100h          ;
JZ H01                ;
cmp ax,0
jz h01
ADD SI,20H           ;;
H01:MOV CX,10H     ;;
mov di,offset buf1  ;
H02:MOV AX,CS:[SI]  ;  ∫ó¨˜ ¬âùb
MOV cs:[DI],AX      ;
INC SI              ;
INC SI              ;
INC DI
INC DI
LOOP H02            ;
h09:
MOV AX,CS:HEXHANAX          ;;
AND AX,0E003H
MOV CL,5                     ; ∫ó¨˜ ∏˜•° …A∑°ßi ¿y∑q
xchg al,ah
SHR AX,CL                    ;
shl ax,1
ADD AX,OFFSET JUNGINFOTABLE  ;
MOV SI,AX                   ;;
MOV AL,CS:[SI]   ;;
MOV BP,0          ;  BPµA π∑¨˜ + ÆÅ
TEST AL,08H       ;
JZ H03            ;
MOV BP,20H       ;;
H03:MOV AX,CS:HEXHANAX   ;;
AND AX,01F00H             ; DXµA ¡°¨˜ + ÆÅ
CMP AX,0100h                  ;
JZ H04                    ;
cmp ax,0
jz h04
INC SI                    ;
H04:MOV AL,CS:[SI]        ;
and al,07h
MOV CL,20H                ;
MUL CL                    ;
MOV DX,AX                ;;
MOV AX,CS:HEXHANAX          ;;
AND AX,007CH                 ;
cmp ax,0
jz h10
MOV CL,0AH                   ;
xchg al,ah
SHR AX,CL                    ;  ¡°¨˜ Ø°∏b§Âª° ¿y∑q
MOV Cl,06                    ;
MUL Cl                       ;
ADD AX,OFFSET SIJACTABLE     ;
MOV SI,AX                    ;
mov si,cs:[si]
ADD SI,DX                   ;;
ADD SI,OFFSET HANFONT
MOV CX,10H
mov di,offset buf1      ;
H05:MOV AX,CS:[SI]      ;
OR cs:[DI],AX           ; ¡°¨˜OR
INC SI                  ;
INC SI
INC DI
INC DI                  ;
LOOP H05                ;
h10:
MOV AX,CS:HEXHANAX         ;;
AND AX,01F00H               ; π∑¨˜ ¥Ù∑a°e RET
CMP AX,0100h                    ;
JZ H06                     ;;
cmp ax,0
jz h06
xchg al,ah
MOV Cl,6                     ;;
MUL Cl                        ;  π∑¨˜ Ø°∏b§Âª° ¿y∑q
ADD AX,OFFSET SIJACTABLE+4    ;
MOV SI,AX                     ;
mov si,cs:[si]
ADD SI,BP                    ;;
ADD SI,OFFSET HANFONT
MOV CX,10h           ;;
mov di,offset buf1    ;
H07:MOV AX,CS:[SI]    ;π∑¨˜ OR
OR cS:[DI],AX         ;
INC SI                ;
INC SI
INC DI
INC DI                ;
LOOP H07             ;;
h06:
Call vaddr
mov cx,10H           ;
mov si,offset buf1   ;  –eãi 1¢Ö∏aüi —¡°e¨wµA ¬âùb
h08:mov al,cs:[si]   ;
push cx              ;
call writevram       ;
inc si               ;
inc di               ;
mov al,cs:[si]       ;
call writevram       ;
inc si               ;
pop cx               ;
dec di               ;
call nextdi          ;
loop h08             ;
POP SI
RET
HEXHANAX DW ?
hexhan endp
; ************************************************
; µw¢Ö 1 ¢Ö∏aüi §·Ã·µA π°ü≥

HEXENG proc near
PUSH SI        ;
SUB AL,20H            ;
MOV CL,10H            ;  µw¢Ö £π ã°—° ¬âùb
MUL CL                ;
ADD AX,OFFSET ENGFONT ;
MOV SI,AX             ;
call vaddr
mov cx,10h
E01:MOV AL,CS:[SI]    ;
push cx
call writevram        ;
pop cx
inc si                ;
CALL NEXTDI           ;
LOOP E01              ;
POP SI                ;
RET                   ;
hexeng endp

;video ram∑aù° 1byteüi ¬âùb. HGCµ¡ VGA∑• âw∂Åüi äÅ¶Ö–Å¨· ¬âùb–q
;VGA∑• âw∂Å ãi∏a ¶Å¶Ö∑Å ïA∑°»aù° ∏Ââw¨Ç∑aù° †Â∏· ¬âùb∑i –aâ° ∑°
;ïA∑°»aüi §e∏ÂØ°≈a §Åâw¨Ç∑aù° –e§Âî· ¬âùb–aµa¥° –eîa.
writevram proc near
cmp cs:video,9    ; HGC°e îeÆÖ–aâA ∑°£°ª°üi —¡°e∑aù° ∏Â≠∑
jz wv01           ;
mov es:[di],al    ;
ret               ;
wv01:
mov bl,al      ;;
mov dx,3ceh     ; bit mask ∏˜∑Å
mov al,08       ;
mov ah,bl       ;
out dx,ax      ;;
mov dx,3c4h       ;
out dx,ax         ;
mov dx,03c4h      ; latch reset
mov ax,0f02h      ;
out dx,ax         ;
xor al,al         ;
and es:[di],al    ;
inc dx          ;
mov ax,cs:fcolor ; map mask µA ƒiúa∏˜∑Å
and al,0fh
out dx,al       ;
mov al,0ffh       ; ãi∏a¶Å¶Ö ¬âùb
mov es:[di],al    ;

xor bl,0ffh    ; §Åâw¨Ç¬âùb∑i ∂·–Å data§e∏Â
mov dx,3ceh       ;
mov al,08         ; bit mask ∏˜∑Å
mov ah,bl         ;
out dx,ax         ;
mov dx,3c4h     ;
out dx,ax       ;
mov dx,03c4h    ; latch reset
mov ax,0f02h    ;
out dx,ax       ;
xor al,al       ;
and es:[di],al  ;
inc dx            ;
mov ax,cs:bcolor   ; map mask µA ƒiúa∏˜∑Å
and al,0fh
out dx,al         ;
mov al,0ffh     ; §Åâw¶Å¶Ö ¬âùb
mov es:[di],al  ;
mov dx,3ceh
mov ax,0ff08h
out dx,ax
mov dx,3c4h
mov ax,0f02h
out dx,ax
ret
writevram endp

;—¡°e¨w∑Å §aù° ¥aúÅ úa∑•∑Å §Âª°üi äÅ–q
NEXTDI proc near
cmp cs:video,9  ;
jnz nextdi02    ; VGA°e îeÆÖ”° 50Hüi î·–q
add di,50h      ;
ret
nextdi02:
ADD DI,2000H
CMP DI,8000H
JAE NEXTDI01
RET
NEXTDI01:SUB DI,8000H
ADD DI,50H
cmp cs:mode,00
jnz nextdi03
add di,0ah
nextdi03:
RET
nextdi endp
;*****************************************
;¢Ö∏aµi ∑≥ùb–qÆÅ,mitµA¨· àaπaµ• ûÅÀ•∑i •e—w–aµvîa.
_shget proc far
push bp
mov bp,sp
push es
push ds
push si
push di
mov ax,ss:[bp+6]   ;
mov cs:x,ax        ;
mov ax,ss:[bp+8]   ; stack∑Å ∑•ÆÅüi §h¥a •eÆÅµA ∏·∏w
mov cs:y,ax        ;
lds si,ss:[bp+10]  ;
MOV CS:STRINGBUF,SI;
mov cs:nowaddress,si;
mov ax,ss:[bp+14]  ;
mov cs:fcolor,ax   ;
mov ax,ss:[bp+16]  ;
mov cs:bcolor,ax   ;
mov ax,ss:[bp+18]  ;
mov cs:limit,ax    ;
mov ax,ss:[bp+20]  ;
mov cs:keep,ax     ;
mov ax,0b000h        ; esüi vram§Âª°ù°
cmp cs:video,9       ; HGC°e b000,VGA°e a000
jnz here             ;
mov ax,0a000h        ;
here:                ;
mov es,ax            ;
mov ax,cs:keep
cmp cs:keep,01
jz keep01
mov si,cs:stringbuf     ;
mov cx,cs:limit         ; keepàa 0∑©âw∂Å
keep02:                 ; §·Ã·üi °∑òw ª°∂ë
mov byte ptr ds:[si],0  ;
inc si                  ;
loop keep02             ;
jmp main
keep01:mov ax,cs:stringbuf ;  keepàa 1∑© âw∂Å
call munjangjae            ;  ¢Ö∏aµi∑i ¬âùb–Å∫ë
mov bx,cs:stringbuf            ;
mov cx,bx                      ;
add cx,cs:limit                ; ¢Ö∏aµi è{∑Å 0∑Å ¿x∑q
keep04:cmp byte ptr ds:[bx],0  ;
jz keep03                      ;
inc bx                         ;
jmp keep04                     ;
keep03:                    ;
mov byte ptr ds:[bx],0     ;
cmp bx,cx                  ; ∑°“Å ¢Ö∏aµi∑Å è{åaª° 0∑aù° ¿Å∂ë
jz keep05                  ; limit∏ÒàÒ∑i ∂·–e ∫Öß°
inc bx                     ;
jmp keep03                 ;
keep05:                    ;
jmp endkey

MAIN:
mov cs:mainsp,sp
key02:
mov ax,cs:count   ;
inc ax            ; ƒ·¨· ƒa∂ÖÀa ªwàa
mov cs:count,ax   ;
cmp cs:count,3000h  ;
jnz key02_1         ; 3000h§Â∑i —A¥aü°â° ƒ·¨·üi –e§Â
mov cs:count,0      ; §e∏ÂØ°«•îa.
call cursor         ;
key02_1:
MOV AH,1        ; «°•°óa π°¨a.îÅã°∫ó∑• «°àa ∑∂∑a°e
INT 16H         ; ƒ·¨· ûÅÀ•∑i è{êë
jz key02       ;
cmp cs:cursorflag,01  ;
jz key03              ; ƒ·¨·àa ≈aπa ∑∂ìe ¨w»Å°e èeîa.
call cursor           ; îa∑qµA §aù° ƒ·¨·àa •°∑°ï°ù¢ –aã°
key03:                ; ∂·–Å ƒa∂ÖÀaüi π°∏b–eîa.
mov cs:count,2fffh    ;
mov ah,0             ;
int 16h              ; scancodeµ¡ shift¨w»Åüi ∑™¥·
MOV CS:SCANCODE,AX   ; àb •eÆÅµA ∏·∏w
MOV AH,02            ;
INT 16H              ;
MOV CS:SCANSHIFT,AX  ;

MOV AX,CS:SCANCODE
CMP AH,01     ; ESC
JNZ MAIN04
JMP ESCKEY
MAIN04:
CMP AH,1CH    ; RETURN
JNZ MAIN05
call nowhantest
pop di          ;
pop si          ;ùAª°Øa»· •¢äÅ,ü°»Â
pop ds          ;
pop es          ;
pop bp          ;
RET             ;
MAIN05:
CMP AH,39H    ; SPACE
JNZ MAIN06
JMP space
MAIN06:
CMP AH,0EH    ; BS
JNZ MAIN07
JMP BS
MAIN07:
cmp ah,4bh    ; left
jnz main08
jmp left
main08:
cmp ah,4dh    ; right
jnz main09
jmp right
main09:
cmp ah,53h    ;del
jnz main10
jmp del
main10:
cmp ah,47h    ;home
jnz main10_1
jmp home
main10_1:
cmp ah,4fh    ;end
jnz main10_2
jmp endkey
main10_2:
;—e∏Å ¨Â»ÇñA¥· ∑∂ìe ∏aÃe ¿·ü° ûÅÀ•∑aù° •°êë
MOV AL,CS:NOWJAPAN
CMP AL,0
JNZ MAIN12
JMP HANAUTO
MAIN12:
JMP ENGLAUTO
SCANCODE DW ?
SCANSHIFT DW ?
NOWJAPAN DB 0
HANJAE DB ?
HANTEST DB ?
NOWADDRESS DW ?
mainsp dw ?
limit dw ?
keep dw ?
_shget endp
;********************************************
;ƒ·¨· ¿·ü° ûÅÀ•
;—e∏Å ∂·√°∑Å —e∏Å ƒiúÒ∑aù° §e∏ÂØ°«•îa.
cursor proc near
mov ax,cs:x           ;
mov cs:munx,ax        ;
mov ax,cs:y           ;
mov cs:muny,ax        ; ƒ·¨·∑Å ∂·√° äÅ–q
mov ax,cs:nowaddress  ; —e∏Å °A°°ü°∑Å offset∑i 8ù° â≥–Å
mov bx,cs:stringbuf   ; xπ¡ŒaµA î·–q
sub ax,bx             ;
shl ax,1              ;
shl ax,1              ;
shl ax,1              ;
add cs:munx,ax        ;
mov cs:column,01        ;
mov bx,cs:nowaddress
test word ptr ds:[bx],0080h      ; ¢Ö∏a∑Å ƒiúÒ∑i äÅ–q
jz cursor01             ;
mov cs:column,02        ;
cursor01:
call vaddr
mov cx,10h
cursor03:
call xorbit        ; 1 byte §e∏Â
cmp cs:column,01
jz cursor02
inc di            ;
call xorbit       ; 2 ƒiúÒ∑°°e îa∑q §a∑°Àaï° §e∏Â
dec di            ;
cursor02:
call nextdi     ; 16“A §e•¢
loop cursor03   ;
xor cs:cursorflag,01    ;flag§e∏Â
ret
count dw ?
cursorflag db ?
column db ?
cursor endp
;di§Âª°∑Å 1 bitüi §e∏Â Ø°«±
xorbit proc near
cmp cs:video,07   ;
jnz xorbit01      ; –·«Iü°Øa°e îeÆÖ”° §e∏Â
mov al,es:[di]    ; Ø°«°â° return
xor al,0ffh       ;
mov es:[di],al    ;
ret               ;
xorbit01:

mov ax,0102h      ; map mask∏˜∑Å
mov dx,03c4h      ; 0§Â plane†e àÅ§w
out dx,ax         ;
mov ax,0004h     ;
mov dx,03ceh     ; 0§Â plane∑i ∑™∑q
out dx,ax        ;
mov al,es:[di]   ;
push ax;       ;
mov ah,0ffh    ;
mov al,08h     ; §e∏Âàt∑i 0§Â planeµA ã°ù¢
mov dx,03ceh   ;
out dx,ax      ;
pop ax         ;
xor al,0ffh    ;
mov es:[di],al ;

mov ax,0202h      ; map mask∏˜∑Å
mov dx,03c4h      ; 1§Â plane†e àÅ§w
out dx,ax         ;
mov ax,0104h     ;
mov dx,03ceh     ; 1§Â plane∑i ∑™∑q
out dx,ax        ;
mov al,es:[di]   ;
push ax;       ;
mov ah,0ffh    ;
mov al,08h     ; §e∏Âàt∑i 0§Â planeµA ã°ù¢
mov dx,03ceh   ;
out dx,ax      ;
pop ax         ;
xor al,0ffh    ;
mov es:[di],al ;

mov ax,0402h      ; map mask∏˜∑Å
mov dx,03c4h      ; 2§Â plane†e àÅ§w
out dx,ax         ;
mov ax,0204h     ;
mov dx,03ceh     ; 2§Â plane∑i ∑™∑q
out dx,ax        ;
mov al,es:[di]   ;
push ax;       ;
mov ah,0ffh    ;
mov al,08h     ; §e∏Âàt∑i 0§Â planeµA ã°ù¢
mov dx,03ceh   ;
out dx,ax      ;
pop ax         ;
xor al,0ffh    ;
mov es:[di],al ;

mov ax,0802h      ; map mask∏˜∑Å
mov dx,03c4h      ; 3§Â plane†e àÅ§w
out dx,ax         ;
mov ax,0304h     ;
mov dx,03ceh     ; 3§Â plane∑i ∑™∑q
out dx,ax        ;
mov al,es:[di]   ;
push ax;       ;
mov ah,0ffh    ;
mov al,08h     ; §e∏Âàt∑i 0§Â planeµA ã°ù¢
mov dx,03ceh   ;
out dx,ax      ;
pop ax         ;
xor al,0ffh    ;
mov es:[di],al ;
mov ax,0f02h
mov dx,03c4h
out dx,ax
ret
xorbit endp







;–eƒe ¥|∑Å °A°°ü°ù° ∑°ï∑.bxµA §Âª°àt return
premem proc near
mov bx,cs:stringbuf
mov dx,bx
premem03:
inc dx
test word ptr ds:[bx],0080h
jz premem01
inc dx
premem01:
cmp dx,cs:nowaddress
jz premem02
mov bx,dx
jmp premem03
premem02:
ret
premem endp
;–eƒe π¡√b∑aù° ∑°ï∑
left proc near
mov byte ptr cs:hantest,0   ; …AØaÀa ãqª°,∏Å∑≥ùb –·àa
mov byte ptr cs:hanjae,01   ;
mov si,cs:nowaddress     ;
cmp si,cs:stringbuf      ; —e∏Å §·Ã·∑Å ¿·∑q∑°°e ãaê∑
jnz left01               ;       return
jmp main                 ;
left01:
call premem                  ;  –eƒe ¥| °A°°ü°üi ¿x¥a
mov cs:nowaddress,bx         ;  àóØ•
jmp main                     ;
left endp
;–eƒe ∂Å√b∑aù° ∑°ï∑
right proc near
mov byte ptr cs:hantest,0  ;…AØaÀa ãqª°,∏Å∑≥ùb –·àa
mov byte ptr cs:hanjae,01
mov si,cs:nowaddress
cmp word ptr ds:[si],0  ;
jnz right02             ; §·Ã·∑Å è{∑°°e return
jmp main                ;
right02:
mov ax,01                   ;
test word ptr ds:[si],0080h ; —e∏Å∑Å §Âª°üi ƒiúÒÆÅ†e«q
jz right01                  ; î·–q
inc ax                      ;
right01:add si,ax           ;
mov cs:nowaddress,si   ; memory àóØ•
jmp main               ;
right endp
;ƒ·¨·üi ¿ıƒe∑aù° µ´ã±
home proc near
mov byte ptr cs:hantest,0
mov byte ptr cs:hanjae,01
mov si,cs:stringbuf
mov cs:nowaddress,si
jmp main
home endp
;ƒ·¨·üi †aª°†b ƒe∑aù° µ´ã±
endkey proc near
mov byte ptr cs:hantest,0
mov byte ptr cs:hanjae,01
endkey03:cmp word ptr ds:[si],0  ;
jnz endkey01                     ; —e∏Å ∂·√°∑Å °A°°ü°àt∑°
mov cs:nowaddress,si             ; 0∑°°e return
jmp main                         ;
endkey01:
mov ax,01                     ;
test word ptr ds:[si],0080h   ; ƒiúÒÆÅ†e«q §Âª°üi î·–q
jz endkey02                   ;
inc ax                        ;
endkey02:add si,ax            ;
jmp endkey03
endkey endp


SPACE PROC NEAR
        MOV AX,CS:SCANSHIFT  ;;
        AND AL,03             ;SHIFT+SPACEìe ∏aÃe •eâw
        CMP AL,0              ;
        JZ SPACE01            ;
        MOV AL,CS:NOWJAPAN    ;
        AND AL,01             ;
        XOR AL,01             ;
        MOV CS:NOWJAPAN,AL    ;
        JMP MAIN             ;;
SPACE01:MOV AL,CS:HANTEST          ;
        CMP AL,0                   ; –eãi TEST
        JZ SPACE02                 ;
        CALL NOWHANTEST            ;
        MOV BYTE PTR CS:HANTEST,0  ;
SPACE02:MOV BYTE PTR CS:HANJAE,01
        MOV DI,CS:NOWADDRESS    ;
        CALL MEMORYINS          ; MEMORYã°ù¢ “Å ∏˜ùi
        MOV BX,CS:NOWADDRESS    ;
        MOV AX,20H              ;
        MOV dS:[BX],Al          ;
        MOV AX,CS:NOWADDRESS    ;
        CALL MUNJANGJAE         ;
        MOV BX,OFFSET NOWADDRESS ; MEMORY+=2
        ADD WORD PTR CS:[BX],01  ;
        JMP MAIN
SPACE ENDP
;–e¢Ö∏a ¨bπA
del proc near
mov di,cs:nowaddress
mov ax,ds:[di]
test ax,0080h         ;
jz del01              ; –eãi∑°°e °A°°ü° ñÅƒe ¨bπA
push di               ; µw¢Ö∑°°e –eƒe ¨bπA
call memorydel        ;
pop di                ;
del01:call memorydel  ;
mov ax,cs:nowaddress      ;
call munjangjae           ;  ¢Ö∏w ∏˜ùw £π flag  set
mov al,20h
call hexeng
add cs:munx,8
mov al,20h
call hexeng
mov byte ptr cs:hantest,0 ;
mov byte ptr cs:hanjae,01 ;
jmp main
del endp

BS PROC NEAR
mov si,cs:nowaddress      ; §·Ã·∑Å ¿·∑q∑°°e ret
cmp si,cs:stringbuf       ;
jnz BS01
jmp main

bs01:
mov si,cs:nowaddress
mov si,ds:[si]
test si,0080h
jz bs02
mov al,cs:hanjae
cmp al,0
jnz bs02
jmp del
bs02:call premem           ; –eƒe ¥|∑aù° ∑°ï∑ “Å del
mov cs:nowaddress,bx  ;
jmp del              ;
BS ENDP
ESCKEY PROC NEAR
jmp main
ESCKEY ENDP
RETURN PROC NEAR
ret
RETURN ENDP

NOWHANTEST PROC NEAR
mov bx,cs:nowaddress ;
mov bx,ds:[bx]       ;b7 ∑° 1 ∑°°e –eãi.
and bx,0080h         ;–eãi∑° ¥aì°°e ãaê∑ ï©¥aàeîa.
cmp bx,0080h         ;
jz nowhantest01      ;
ret                  ;
nowhantest01:mov bx,cs:nowaddress
mov ax,ds:[bx]    ;
and ax,1f00h      ;π∑¨˜∑° ¥Ù∑a°e π∑¨˜ ∏aü°µA
cmp ax,0000       ;  null codeüi ¨s∑≥Ø°≈a∫Öîa.
jnz nowhantest02  ;
mov ax,0100h      ;
or ds:[bx],ax     ;
nowhantest02:mov bx,offset nowaddress  ;°A°°ü°üi îa∑q∏aü°ù°
add word ptr cs:[bx],02                ;
ret
NOWHANTEST ENDP
;DI §Âª°¶Å»· 0∑° êaµ©òÅåaª° °A°°ü° –eƒe ¨s∑≥,
MEMORYINS PROC NEAR
call strlen             ;
mov ax,cs:stringbuf     ; ¢Ö∏aµi è{∑° limit•°îa à{à·êa
add ax,cs:limit         ; «a°e ¨s∑≥–i ÆÅ ¥Ù∑q
dec ax
cmp ax,bx               ;
ja memory03             ;
mov sp,mainsp           ;
 jmp main               ;
 memory03:              ;
 MOV AL,DS:[DI]
 MOV BYTE PTR DS:[DI],0
MEMORYINS02:INC DI
MOV AH,DS:[DI]
CMP AH,0
JNZ MEMORYINS01
MOV DS:[DI],AL
RET
MEMORYINS01:MOV DS:[dI],AL
MOV AL,AH
JMP MEMORYINS02
MEMORYINS ENDP
memory2ins proc near
call strlen           ;
mov ax,cs:stringbuf   ;
add ax,cs:limit       ; ¢Ö∏aµi è{∑° limit-1•°îa à{à·êa
dec ax                ; «a°e ¨s∑≥ –i ÆÅ ¥Ù∑q
dec ax
cmp ax,bx             ;
ja memory203           ;
mov sp,mainsp         ;
 jmp main             ;
 memory203:            ;
push di
call memoryins
pop di
call memoryins
ret
memory2ins endp
;di§Âª°¶Å»· °A°°ü° –eƒe ¨bπA
memorydel proc near
inc di
memorydel02:
mov al,ds:[di]
cmp al,0         ;
jnz memorydel01  ; ¢Ö∏aµi∑Å è{∑°°e †aª°†b dataã°ù¢ “Å
dec di           ;     return
mov ds:[di],al   ;
ret              ;
memorydel01:dec di ;
mov ds:[di],al     ; dataã°ù¢“Å loop
add di,2           ;
jmp memorydel02    ;
memorydel endp
memory2del proc near    ;  °A°°ü° ñÅƒe ¨bπA
push di                 ;  îeÆÖ”° memorydel∑i ñÅ§Â —°¬â
call memorydel
pop di
call memorydel
ret
memory2del endp
;¢Ö∏aµi∑Å è{∑i ¿x¥a∫Öîa.
strlen proc near
mov bx,cs:stringbuf
strlen02:
cmp byte ptr ds:[bx],0
jz strlen01
inc bx
jmp strlen02
strlen01:ret
strlen endp


;**********************************************
;hanauto
;**********************************************
hanauto PROC NEAR
        call numbauto    ;∂Å¨Â Æï∏a∑•ª° ••îa.
        mov cs:hantest,01
        mov al,cs:hanjae    ;;
        cmp al,0             ;   
        jz hanautojp01       ;–eãi ∏Å¬âùb ¨w»Å°e °A°°ü° ñÅƒe ñ·ù°
        mov di,cs:nowaddress ;
        call memory2ins      ;
        mov cs:hanjae,0     ;;
hanautojp01:
            mov si,cs:nowaddress
            mov si,ds:[si]  ;;
            mov ax,007ch     ;
            and ax,si        ;¡°¨˜∑° ¥Ù∑a°e ¡°¨˜ ûÅÀ•∑aù°
            cmp ax,0         ;
            jnz hanautojp02  ;
            call cho         ;
            jmp hanautojp03 ;;
hanautojp02:
            mov ax,0e003h   ;;
            and ax,si        ;∫ó¨˜∑° ¥Ù∑a°e ∫ó¨˜ ûÅÀ•∑aù°
            cmp ax,0         ;
            jNZ hanautojp04  ;
            call jung        ;
            jmp hanautojp03 ;;
hanautojp04:mov ax,1f00h      ;;
            and ax,si          ;π∑¨˜∑° ¥Ù∑a°e •¢°°∑q ûÅÀ•∑aù°
            cmp ax,0           ;
            jnz hanautojp05    ;        
            call bokmo         ;
            jmp hanautojp03   ;;
hanautojp05:call boksae  ;¡°,∫ó,π∑¨˜∑° îa ∑∂∑a°e •¢∏a∑q/¨Åãi∏a ûÅÀ•
hanautojp03:jmp main
HANAUTO ENDP
;************************************
;¡°¨˜ûÅÀ•
cho PROC NEAR
            call motest ;;
            cmp al,0         ;°°∑q∑°°e °°∑q†e ¬âùb
            jnz chojp01      ;
            jmp chojp02     ;;
chojp01:mov ax,cs:scanshift   ;;
            and al,03          ;shiftàa íâùaª°ª° ¥g¥v∑a°e 03∑aù°
            cmp al,0           ; 
            jz chojp03        ;; 
BACHIMJP02:mov ax,cs:scancode                 ;;•¢∏a∑q ¿·ü°
            cmp ah,13h                          ;
            jnz chojp04                 
            mov si,offset hancodetable+0e0h
            jmp chojp05
chojp04:cmp ah,12h
            jNZ chojp06
            mov si,offset hancodetable+0e4h
            jmp chojp05
chojp06:cmp ah,10h
            jnz chojp07
            mov si,offset hancodetable+0e8h
            jmp chojp05
chojp07:cmp ah,14h
            jnz chojp08
            mov si,offset hancodetable+0ech
            jmp chojp05
chojp08:cmp ah,11h
            jnz chojp09
            mov si,offset hancodetable+0f0h   ;
            jmp chojp05                      ;;
chojp09:
chojp03:mov ax,cs:scancode             ;;
            mov al,ah                   ;si=scan*4+hancodetable
            mov ah,0                    ;
            mov cl,04                   ;
            mul cl                      ;
            add ax,offset hancodetable  ;
            mov si,ax                  ;; 
chojp05:    mov ax,cs:[si]        ;
            mov bx,cs:nowaddress  ;Memoey OR
            or ds:[bx],ax         ;
            mov ax,cs:nowaddress     ;¢Ö∏w ∏˜ùi
            call mUnjangjae          ;
            RET
chojp02:    mov ax,cs:scancode         ;; 
            mov al,ah                   ;si=scan*4+hancodetable
            mov cl,04                   ;
            mul cl                      ;
            add ax,offset hancodetable  ;
            mov si,ax                  ;;
            mov ax,cs:[si]       ;;
            mov bx,cs:nowaddress  ;¡°¨˜∑e nullù° –aâ° ∫ó¨˜†e
            or ax,0084h           ;     Memory OR
            or ds:[bx],ax        ;;
            mov ax,cs:nowaddress     ;¢Ö∏w ∏˜ùi
            call munjangjae          ;
CHO ENDP
;******************************8
;∫ó¨˜ ûÅÀ•
jung PROC NEAR
            call motest    ;
            cmp al,0       ;°°∑q ¥aì°°e 02ù°
            jz jungjp01    ;
            jmp jungjp02   ;
jungjp01:   mov ax,cs:scanshift   ;;
            and al,03              ;shift¥eíâùaπv∑a°e 03∑aù°
            cmp al,0               ;
            jz jungjp03           ;;
            mov ax,cs:scancode
            cmp ah,18h
            jnz jungjp04
            mov si,offset hancodetable+0f8h
            jmp jungjp05
jungjp04:   cmp ah,19h
            jnz jungjp06
            mov si,offset hancodetable+0fch
            jmp jungjp05
jungjp06:   cmp ah,25h
            jnz jungjp07
            mov si,offset hancodetable+0a0h
            jmp jungjp05
jungjp07:
jungjp03:   mov ax,cs:scancode         ;;
            mov al,ah                   ;SI=scan*4+hancodetable
            mov ah,0                    ;
            mov cl,04                   ;
            mul cl                      ;
            add ax,offset hancodetable  ;
            mov si,ax                  ;; 
jungjp05:   mov ax,cs:[si]        ;
            mov bx,cs:nowaddress  ;Memory OR
            or ds:[bx],ax         ;
            mov ax,cs:nowaddress      ;¢Ö∏w∏˜ùi
            call munjangjae           ;
            RET
jungjp02:   mov bx,cs:nowaddress
            mov bx,ds:[bx]
            mov ax,cs:scancode
            cmp ah,10h    ;
            jnz jungjp08  ; ®A
            cmp bx,00a4h  ;
            jnz jungjp08  ;
            mov ax,00a8h  ;
            jmp jungjp09  ;
jungjp08:   cmp ah,11h       ;
            jnz jungjp10     ; ºA
            cmp bx,00b8h     ;
            jnz jungjp10     ;
            mov ax,00bch     ;
            jmp jungjp09     ;
jungjp10:   cmp ah,12h    ;
            jnz jungjp11  ; òA
            cmp bx,0094h  ;
            jnz jungjp11  ;
            mov ax,0098h  ;
            jmp jungjp09  ;
jungjp11:   cmp ah,13h       ;
            jnz jungjp12     ; åA
            cmp bx,0088h     ;
            jnz jungjp12     ;
            mov ax,008ch     ;
            jmp jungjp09     ;
jungjp12:   cmp ah,14h    ;
            jnz jungjp13  ; ∞A
            cmp bx,00ach  ;
            jnz jungjp13  ;
            mov ax,00b0h  ;
            jmp jungjp09  ;
jungjp13:   jmp jungjp14
jungjp09:   mov bx,cs:nowaddress  ;;
            mov ds:[bx],ax         ;∂•úÅ∑Å ¡°¨˜ ¥Ù¥Åâ° ∞w∏a∑q∑aù°
            mov ax,cs:nowaddress   ;
            call munjangjae       ;;
            ret
jungjp14:   cmp bx,00c4h  ;
            jz jungjp15   ; ƒA ¥aì°°e 16∑aù°
            jmp jungjp16  ;
jungjp15:   cmp ah,10h      ;
            jnz jungjp17    ; ®A
            mov ax,00a8h    ;
            jmp jungjp18    ;
jungjp17:   cmp ah,11h    ;
            jnz jungjp19  ; ºA
            mov ax,00bch  ;
            jmp jungjp18  ;
jungjp19:   cmp ah,12       ;
            jnz jungjp20    ; òA
            mov ax,0098h    ;
            jmp jungjp18    ;
jungjp20:   cmp ah,13h    ;
            jnz jungjp21  ; åA
            mov ax,008ch  ;
            jmp jungjp18  ;
jungjp21:   cmp ah,14h      ;
            jnz jungjp22    ; ∞A
            mov ax,00b0H    ;
jungjp18:   jmp jungjp09    ;
jungjp22:   jmp jungjp16
jungjp16:   mov bx,Offset nowaddress  ;Memory +=2
            add word pTR cs:[bx],02   ;
            mov di,cs:nowaddress  ; ¨Åãi∏a †eói ∏aü° †eók
            call memory2ins       ;
            mov di,cs:nowaddress
            mov ax,cs:scancode
            mov al,ah                  ;;
            mov cl,04                   ;SI=scan*4+hancodetable
            mul cl                      ;
            add ax,offset hancodetable  ;
            mov si,ax                  ;;
            mov ax,cs:[si]        ;
            mov bx,cs:nowaddress  ;Memory or
            or ds:[bx],ax         ;
            mov ax,cs:nowaddress  ; ¢Ö∏w ∏˜ùi
            call munjangjae       ;
JUNG ENDP
;***************************
;•¢°°∑q ¿·ü°
bokmo PROC NEAR
            call motest    ;
            cmp al,01      ;°°∑q∑°°e °°∑q ¿·ü°
            jnz bokmojp01  ;∏a∑q∑°°e §h√± ¿·ü°
            jmp bachim     ;
bokmojp01:  mov bx,cs:nowaddress
            mov bx,ds:[bx]
            and bx,0e003h
            mov Cl,BYTE PTR cs:scancode+1
            mov si,offset bokmotable
bokmojp05:  mov al,cs:[si]
            cmp al,0
            jnz bokmojp02
            jmp bokmojp03
bokmojp02:  cmP cl,al      ;
            jz bokmojp04   ; ØaƒÖ≈°óaàa ∑©√°–i òÅåaª° àÒ¨Ç
            add si,0008    ;
            jmp bokmojp05  ;
bokmojp04:  inc si           ;
            mov ax,cs:[si]   ; ∫ó¨˜≈°óaàa ∑©√°–i òÅåaª° àÒ¨Ç
            cmp bx,ax        ;
            jz bokmojp06     ;
            add si,0007      ;
            jmp bokmoJP05    ;
bokmoJP06:  mOv bx,cs:nowaddress ;;
            mov ax,ds:[bx]        ; —e∏Å∑Å ∫ó¨˜∑i ¨Åù°π°ü≥ñE
            and ax,01ffch         ;      •¢°°∑q∑aù° §aéë
            inc si                ;
            inc si                ;
            mov si,cs:[si]        ;
            or ax,si              ;
            mov ds:[bx],ax       ;;
            mov ax,cs:nowaddress      ;
            call munjangjae           ;¢Ö∏w∏˜ùi “Å •¢ä·
            ret                       ;  
bokmojp03:  mov bx,offset nowaddress    ;Memory+=2
            add word ptr cs:[bx],02     ;
            mov di,cs:nowaddress
            call memory2ins
            jmp chojp02
BOKMO ENDP
;************************
;§h√± ¿·ü°
bachim PROC NEAR
            mov ax,cs:scanshift  ;
            and al,03            ;shiftàa ¥eíâùaπv∑a°e 01ù°
            cmp al,0             ;
            jz bachimjp01        ;
            mov ax,cs:scancode
            mov bx,offset nowaddress ;Memory+=02
            add word ptr cs:[bx],02  ;
            mov di,cs:nowaddress
            call memory2ins
            jmp bachimJP02
bachimjp01: mov ax,cs:scancode       ;;
            mov al,ah                 ;SI=scan*4+jongtable
            mov cl,04                 ;
            mul cl                    ;
            add ax,offset jongtable  ;;
            mov si,ax
            mov ax,cs:[si]       ;Memory OR
            mov bx,cs:nowaddress ; 
            or ds:[bx],ax        ;
            mov ax,cs:nowaddress    ;¢Ö∏w ∏˜ùi
            call munjangjae         ;
            RET
BACHIM ENDP
;**********************************
;§h√±µA •¢∏a∑q∑° ¨˜ü≥ñA°e •¢∏a∑q
;¥aì°°e ¨Åãi∏aù° ï¢ü≥
boksae PROC NEAR
            call motest    ;
            cmp al,01      ;∏a∑q∑°°e 01
            jz boksae01    ;
            jmp boksae02   ;
boksae01:   mov ax,cs:scanshift  ;
            and al,03            ; Shift àa ¥e íâùaπv∑a°e
            cmp al,0             ;       03∑aù°
            jz boksae03          ;
            mov ax,cs:scancode      ;
            cmp ah,10h              ;scan∑° 10µA¨· 14 ¨a∑° ¥aì°°e
            jb boksae03             ;    03∑aù°
            cmp ah,14h              ;
            jz boksae03             ;
            mov bx,offset nowaddress   ;Memory +=2
            add word ptr cs:[bx],02    ;
            mov di,cs:nowaddress    ;§h√±∑° ∑∂ìe ¨w»ÅµA¨· âw∑q∑° ói¥·µ÷∑a£aù°
            call memory2ins         ;Memory¨s∑≥ “Å ¡°¨˜∑aù° êÒã•îa.
            jmp bachimjp02          ;
boksae03:   mov bx,cs:nowaddress ;BXµA π∑¨˜
            mov bx,ds:[bx]       ;Memory¶Åπ¢ ¶Öã°
            and bx,01f00h        ;
            jmp boksae04         ;
boksae17:   mov cl,BYTE PTR cs:scancode+1 ;; 
            mov si,offset bokjatable       ;•¢∏a table àÒ¨Ç
boksae08:   mov al,cs:[si]                 ;0∑°°e 06∑aù°
            cmp al,0                       ;
            jnz boksae05                   ;     
            jmp boksae06                  ;;
boksae05:   cmp cl,al      ;
            jz boksae07    ;scan∑° à{∑iòÅåaª° àÒ¨Ç
            add si,0008h   ;
            jmp boksae08   ;
boksae07:   inc si               ;
            mov ax,cs:[si]       ;¨w∂w≈°óaàa à{∑iòÅåaª° àÒ¨Ç
            cmp bx,ax            ;
            jz boksae09          ;
            add si,0007          ;
            jmp boksae08         ;
boksae09:   mov bx,cs:nowaddress    ;;
            mov ax,ds:[bx]           ;
            and ax,0e0ffh            ;∏Â §h√±∑i ¥Ù¥Åâ° 
            inc si                   ;¨Åù°∂Ö •¢∏a∑q∑aù° îÅ√°
            inc si                   ;
            mov si,cs:[si]           ;
            or ax,si                 ;
            mov ds:[bx],ax          ;;
            mov ax,cs:nowaddress ;
            call munjangjae      ;¢Ö∏w ∏˜ùi “Å ret
            ret                  ;
boksae06:   mov bx,offset nowaddress ;
            add word ptr cs:[bx],02  ;•¢∏a∑q ¨˜ü≥ ¥eñAìe ∏a∑q ∑≥ùbØ°
            mov di,cs:nowaddress     ;îa∑q ãi∏a∑Å ¡°¨˜∑aù° ï¢ü≥–eîa.
            call memory2ins          ;
            jmp chojp03              ;
;************************************
;∏Â¢Ö∏a π∑¨˜â¡ °°∑q∑aù° ¨Åãi∏a †eóq
boksae02:   mov si,offset bokjatable+3 ;
            mov bx,cs:nowaddress       ;BXµA π∑¨˜
            mov bx,ds:[bx]             ;
            and bx,1f00h               ;
            boksae13:mov ax,cs:[si]  ;
            cmp ax,0                 ;
            jnz boksae10             ; •¢∏a∑q ¿y¥aêë ¥Ù∑a°e 11ù°
            jmp boksae11             ;
boksae10:   cmp bx,ax                ;
            jz boksae12              ;
            add si,0008h             ;
            jmp boksae13             ;
boksae12:   dec si                 ;
            dec si                 ;
            mov cx,cs:[si]         ;•¢∏a∑q ¶Ö–Å–Å¨· ¥|∑Å àı †e∑aù° äa¡A
            mov bx,cs:nowaddress   ;
            mov ax,ds:[bx]         ;
            and ax,0e0ffh          ;
            or ax,cx               ;
            mov ds:[bx],ax         ;
            mov bx,offset nowaddress ;
            add word ptr cs:[bx],02  ;Memoryñ·ù° ñÅƒe
            mov di,cs:nowaddress     ;
            call memory2ins          ;
            inc si                ;
            inc si                ;•¢∏a∑q∫ó ñı∏a ¡°¨˜ code
            inc si                ; äÅ–Å¨· Memory write
            inc si                ;
            mov ax,cs:[si]        ;
            mov bx,cs:nowaddress  ;
            mov ds:[bx],ax        ;
            mov ax,cs:nowaddress    ;¢Ö∏w ∏˜ùi “Å ∫ó¨˜ ¬âùb∑aù°
            dec ax                  ;
            dec ax                  ;
            call munjangjae         ;
            jmp jungjp01            ;
;****************************************
;∏Â¢Ö∏a π∑¨˜ + °°∑q∑aù° ¨Åãi∏a äÅ¨˜
boksae11:   mov bx,cs:nowaddress   ;
            mov bx,ds:[bx]         ;BXµA π∑¨˜
            and bx,01f00h          ;
            mov si,offset jongchotable ;
boksae15:   cmp cs:[si],bx             ;π∑¡° …A∑°ßiµA¨· ∏Â¢Ö∏a π∑¨˜∑i
            jz boksae14                ;¡°¨˜∑aù° §aéë
            add si,0004                ; BPµA •°â≈
            jmp boksae15               ;
boksae14:   inc si                     ;
            inc si                     ;
            mov bp,cs:[si]             ;
            mov bx,cs:nowaddress  ;
            mov ax,ds:[bx]        ; ∏Â¢Ö∏a π∑¨˜ ¥Ù¥ë
            and ax,0e0ffh         ; 01(null)ù° †eóq
            mov cx,0100h          ;
            or ax,cx              ;
            mov ds:[bx],ax        ;
            mov bx,offset nowaddress ;
            add word ptr cs:[bx],02  ;Memoryñ·ù° ñÅƒe
            mov di,CS:nowaddress     ;
            call memory2ins          ;
            mov bx,cs:nowaddress  ;
            mov ds:[bx],bp        ;
            mov ax,cs:nowaddress  ; ∫ó¨˜ ¬âùb∑aù°
            dec ax                ;
            dec ax                ;
            call munjangjae       ;
            jmp jungjp01          ;
boksae24:   mov bx,offset nowaddress ;
            add word ptr cs:[bx],02  ; Memoryñ·ù° ñÅƒe
            push ax                  ;
            mov di,cs:nowaddress     ;
            caLL memory2ins          ;
            pop ax                   ;
            mov bx,cs:nowaddress  ;
            mov ds:[bx],ax        ; âw∑q ã°ù¢“Å ¢Ö∏w ∏˜ùi
            moV ax,CS:nowaddress  ;
            dec ax                ;
            dec ax                ;
            call munjangjae       ;
            ret                   ;
;***********************************
;'ƒA'+âw∑q ¿·ü°
boksae04:   cmp bx,1a00h  ;
            jz boksae16   ; §h√±∑° 'ƒA'∑•àa ••îa.
            jmp boksae17  ;
BOKSAE16:   mov ax,cs:SCANCODE
            cmp ah,10h    ;
            jnz boksae18  ; ®A
            mov ax,00a8H  ;
            jmp boksae19  ;
boksae18:   cmp ah,11h     ;
            jnz boksae20   ; ºA
            mov ax,00bch   ;
            jmp boksae19   ;
boksae20:   cmp ah,12h    ;
            jnz boksae21  ; òA
            mov ax,0098h  ;
            jmp boksae19  ;
boksae21:   cmp ah,13h     ;
            jnz boksae22   ; åA
            mov ax,008ch   ;
            jmp boksae19   ;
boksae22:   cmp ah,14h    ;
            jnz boksae23  ; ∞A
            mov ax,00b0h  ;
boksae19:   jmp boksae25  ;
boksae23:   jmp boksae17
boksae25:   mov bx,cs:nowaddress        ;∏Â¢Ö∏a π∑¨˜ ¥Ù¥ë
            and word ptr ds:[bx],0e0ffh ; Memory ¶Åπ¢ ¶Öã°
            jmp SHORT boksae24          ;
BOKSAE ENDP
;*********************************
;∑≥ùbñE scan codeàa °°∑q∑•ª° ∏a∑q∑•ª° π°¨a–Å ∫Öîa.
motest PROC NEAR
            mov ax,cs:scancode
            cmp ah,15h   ;
            jb motest01  ;
            cmp ah,19h   ;
            ja motest01  ; °°∑q §Ò∂· π°¨a
            jmp motest02 ;
motest01:   cmp ah,23h   ;
            jb motest03  ;
            cmp ah,26h   ;
            ja motest03  ;
            jmp motest02 ;
motest03:   cmp ah,30h   ;
            jb motest04  ;
            cmp ah,32h   ;
            ja motest04  ;
            jmp motest02 ;
motest04:   mov al,01 ;∏a∑q∑°°e AL=1
            ret       ;
motest02:   mov al,0  ;°°∑q∑°°e AL=0
            ret       ;
MOTEST ENDP

;**********************************************
;NUMBAUTO
;Æï∏a ∑≥ùbã°. ï¢ü≥∏‚∑aù° ≥a∑°ª° ¥gâ° îaüe ∑≥ùbã° ¨ÂñÅµA¨·
;—°¬â–aµa ¨a∂w
;**********************************************
NUMBAUTO PROC NEAR
        MOV AX,cs:SCANCODE         ;
        MOV AL,AH                   ; ØaƒÖ∑aù°¶Å»·
        MOV CL,04                   ; TABLE§Âª° ¥Ë∑q
        MUL CL                      ;
        ADD AX,OFFSET ENGLCODETABLE ;
        MOV SI,AX                   ;
        MOV AX,cs:[SI] ;
        CMP AL,0FFH    ; TABLE¨w∂·àa FF(Æï∏aŒaØ°) ¥aì°°e RET
        JZ NUMBAUTO01  ;
        RET            ;
NUMBAUTO01:MOV AL,cs:HANTEST;
        CMP AL,0            ; –eãi …AØaÀa
        JZ NUMBAUTO02       ;
        CALL NOWHANTEST     ;
        MOV cs:HANJAE,1     ;
NUMBAUTO02:MOV cs:HANTEST,01
        MOV DI,cs:NOWADDRESS  ;Memory ¨s∑≥
        CALL MEMORYINS       ;
        MOV AX,CS:SCANCODE
        MOV BX,cs:NOWADDRESS    ; ≈°óa∞·ê˝∑q
        MOV dS:[BX],AL          ;
        MOV AX,cs:NOWADDRESS      ;
        CALL MUNJANGJAE           ; ¢Ö∏w ¬âùb “Å Memory ∑°ï∑
        MOV BX,OFFSET NOWADDRESS  ;
        ADD WORD PTR cs:[BX],01   ;
        POP AX ;î·£° ØaƒÇ πAà·.∂•úÅ îaüe ∑≥ùbã°µA¨· —°¬âñA¥·
               ;µ÷∑q∑aù° πAà·–Å¥° –eîa.
        JMP MAIN
        NUMBAUTO ENDP


;**********************************************
;ENGLAUTO
;**********************************************
ENGLAUTO PROC NEAR
        CALL NUMBAUTO  ; ∂Å¨Â Æï∏a∑•ª° †Â∏· ••îa.
        mov al,cs:hantest ;
        cmp al,0          ;
        jz englauto00     ; –eãi …AØaÀa
        call nowhantest   ;
        mov cs:hanjae,1   ;
englauto00:mov hantest,1
        MOV DI,cs:NOWADDRESS  ;Memory ¨s∑≥
        CALL MEMORYINS       ;
        MOV AX,CS:SCANCODE
        MOV BX,cs:NOWADDRESS    ; ≈°óa∞·ê˝∑q
        MOV dS:[BX],AL          ;
        MOV AX,cs:NOWADDRESS      ;
        CALL MUNJANGJAE           ; ¢Ö∏w ¬âùb “Å Memory ∑°ï∑
        MOV BX,OFFSET NOWADDRESS  ;
        ADD WORD PTR cs:[BX],01   ;
        JMP MAIN
ENGLAUTO ENDP


;**************************************************************
;code table
;**************************************************************
             ;–eãi ¡°¨˜â¡ ∫ó¨˜∑Å CODEµ¡ FONT FILEêÅ∑Å ∂·√° OFFSET
             ;¨w∂· 2BYTE:¨w∂w≈°óa   –a∂· 2 BYTE:DATAØ°∏b§Âª°
hancodetable db 000h,000h,000h,000h,000h,000h,000h,000h    ;0,1
             db 000h,000h,000h,000h,000h,000h,000h,000h    ;2,3
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;4,5
             db 001h,0c0h,050h,022h,002h,040h,070h,023h    ;6,7     Å¡ ÇA
             db 001h,0e0h,0e0h,022h,002h,0e0h,0b0h,025h    ;8,9     Å· Ç·
             db 002h,0a0h,090h,024h,002h,0c0h,020h,025h    ;A,V     Ç° Ç¡
             DB 003H,080H,060H,01EH,000H,000H,000H,000H    ;C,D     ÉÅ
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;e,f
             DB 0A4H,000H,0D8H,009H,0B8H,000H,0C0H,00FH    ;10,11   §A ∏A
             DB 094H,000H,0F0H,003H,088H,000H,000H,000H    ;12,13   îA àA
             DB 0ACH,000H,0D0H,00BH,002H,060H,000H,024H    ;14,15   ¨A Ça
             DB 001H,060H,030H,021H,000H,0A0H,080H,01FH    ;16,17   Åa Ä°
             DB 000H,080H,0F0H,01EH,001H,040H,0A0H,020H    ;18,19   ÄÅ ÅA
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;1a,1b
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;1C,1D
             DB 0A0H,000H,0E0H,007H,090H,000H,0F8H,001H    ;1E,1F   †A êA
             DB 0B4H,000H,0C8H,00DH,09CH,000H,0E8H,005H    ;20,21   ¥A úA
             DB 0D0H,000H,098H,019H,001H,0A0H,0B0H,01CH    ;22,23   –A Å°
             DB 000H,0E0H,020H,01CH,000H,060H,090H,01BH    ;24,25   Ä· Äa
             DB 003H,0A0H,0D0H,026H,000H,000H,000H,000H    ;26,27   É°
             DB 003H,0C0H,000H,048H,000H,000H,000H,000H    ;28,29   ¥aúÅ¥a
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;2A,2B
             DB 0C4H,000H,0B0H,013H,0C8H,000H,0A8H,015H    ;2C,2D   ƒA »A
             DB 0C0H,000H,0B8H,011H,0CCH,000H,0A0H,017H    ;2E,2F   ¿A ÃA
             DB 003H,040H,040H,026H,002H,080H,040H,01DH    ;30,31   ÉA ÇÅ
             DB 003H,060H,0D0H,01DH,000H,000H,000H,000H    ;32,33   Éa
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;34,35
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;36,37
             DB 08CH,000H,028H,03EH,098H,000H,008H,046H    ;38,39   åA òA
             DB 0A8H,000H,010H,044H,0B0H,000H,020H,040H    ;3A,3B   ®A ∞A
             DB 0BCH,000H,018H,042H,000H,000H,000H,000H    ;3C,3D   ºA
             DB 000H,0C0H,010H,020H,001H,080H,0C0H,021H    ;3E,3F   Ä¡ ÅÅ
            ;π∑¨˜∑Å CODEµ¡ FONT FILEêÅ∑Å ∂·√° OFFSET∑° ∑∂îa.
jongtable    db 000h,000h,000h,000h,000h,000h,000h,000h    ;0,1
             db 000h,000h,000h,000h,000h,000h,000h,000h    ;2,3
             db 000h,000h,000h,000h,000h,000h,000h,000h    ;4,5
             db 000h,000h,000h,000h,000h,000h,000h,000h    ;6,7
             db 000h,000h,000h,000h,000h,000h,000h,000h    ;8,9
             db 000h,000h,000h,000h,000h,000h,000h,000h    ;a,b
             db 000h,000h,000h,000h,000h,000h,000h,000h    ;c,d
             db 000h,000h,000h,000h,000h,000h,000h,000h    ;e,f
             db 000h,013h,0e0h,034h,000h,018h,018h,039h    ;10,11  §A ∏A
             db 000h,008h,070h,02ch,000h,002h,060h,027h    ;12,13  îA àA
             db 000h,015h,090h,036h,000h,000h,000h,000h    ;14,15  ¨A
             db 000h,000h,000h,000h,000h,000h,000h,000h    ;16,17
             db 000h,000h,000h,000h,000h,000h,000h,000h    ;18,19
             db 000h,000h,000h,000h,000h,000h,000h,000h    ;1a,1b
             db 000h,000h,000h,000h,000h,000h,000h,000h    ;1C,1D
             db 000h,011h,008h,034h,000h,005h,0e8h,029h    ;1E,1F  †A êA
             db 000h,017h,040h,038h,000h,009h,048h,02dh    ;20,21  ¥A úA
             db 000h,01dh,050h,03dh,000h,000h,000h,000h    ;22,23  –A
             db 000h,000h,000h,000h,000h,000h,000h,000h    ;24,25
             db 000h,000h,000h,000h,000h,000h,000h,000h    ;26,27
             db 000h,000h,000h,000h,000h,000h,000h,000h    ;28,29
             db 000h,000h,000h,000h,000h,000h,000h,000h    ;2A,2B
             db 000h,01ah,0c8h,03ah,000h,01bh,0a0h,03bh    ;2C,2D  ƒA »A
             db 000h,019h,0f0h,039h,000h,01ch,078h,03ch    ;2E,2F  ¿A ÃA
             db 000h,000h,000h,000h,000h,000h,000h,000h    ;30,31
             db 000h,003h,038h,028h,000h,004h,010h,029h    ;32,33  ¥„ ¥‰
             db 000h,006h,0c0h,02ah,000h,007h,098h,02bh    ;34,35  ¥Ê ¥Á
             db 000h,00ah,020h,02eh,000h,00bh,0f8h,02eh    ;36,37  ¥Í ¥Î
             db 000h,00ch,0d0h,02fh,000h,00dh,0a8h,030h    ;38,39  ¥Ï ¥Ì
             db 000h,00eh,080h,031h,000h,00fh,058h,032h    ;3A,3B  ¥Ó ¥Ô
             db 000h,010h,030h,033h,000h,014h,0b8h,035h    ;3C,3D  ¥ ¥Ù
             db 000h,016h,068h,037h,000h,000h,000h,000h    ;3E,3F  ¥ˆ
              ;µw¢Ö,Æï∏a,ã°—°µA îÅ–e CODE,OFFSET
englcodetable db 000h,000h,000h,000h,000h,000h,000h,000h    ;0,1
             db 0FFH,031H,060H,000H,0FFH,032H,0C0H,000H    ;2,3  1  2
             db 0FFH,033H,020H,001H,0FFH,034H,080H,001H    ;4,5  3  4
             db 0FFH,035H,0E0H,001H,0FFH,036H,040H,002H    ;6,7  5  6
             db 0FFH,037H,0A0H,002H,0FFH,038H,000H,003H    ;8,9  7  8
             db 0FFh,039h,060h,003h,0FFh,030h,0C0h,003h    ;a,b  9  0
             DB 0FFH,02DH,080H,007H,0FFH,03DH,020H,004H    ;c,d  -  =
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;e,f
             DB 000H,051H,000H,006H,000H,057H,040H,008H    ;10,11  Q W
             DB 000H,045H,080H,001H,000H,052H,060H,006H    ;12,13  E R
             DB 000H,054h,020h,007h,000h,059h,000h,009h    ;14,15  T Y
             db 000h,055h,080h,007h,000h,049h,000h,003h    ;16,17  U I
             db 000h,04fh,040h,005h,000h,050h,0a0h,005h    ;18,19  O P
             db 0FFH,05BH,0A0H,005H,0FFH,05DH,000H,006H    ;1a,1b  [ ]
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;1C,1D
             DB 000H,041H,000H,000H,000H,053H,0C0H,006H    ;1E,1F  A S
             DB 000H,044H,020H,001H,000H,046H,0E0H,001H    ;20,21  D F
             DB 000H,047H,040H,002H,000H,048H,0A0H,002H    ;22,23  G H
             DB 000H,04AH,030H,003H,000H,04BH,0C0H,003H    ;24,25  J K
             DB 000H,04CH,020H,004H,0FFH,03BH,080H,004H    ;26,27  L ;
             DB 0FFH,027H,0E0H,004H,0FFH,060H,040H,005H    ;28,29  ' `
             DB 000H,000H,000H,000H,0FFH,05CH,0E0H,007H    ;2A,2B
             DB 000H,05AH,060H,009H,000H,058H,0A0H,008H    ;2C,2D  Z X
             DB 000H,043H,0C0H,000H,000H,056H,0E0H,007H    ;2E,2F  C V
             DB 000H,042H,060H,000H,000H,04EH,0E0H,004H    ;30,31  B N
             DB 000H,04DH,080H,004H,0FFH,02CH,060H,006H    ;32,33  M ,
             DB 0FFH,02EH,0C0H,006H,0FFH,02FH,020H,007H    ;34,35  . /
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;36,37
             DB 000H,000H,000H,000H,0FFH,020H,000H,000H    ;38,39
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;3A,3B
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;3C,3D
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;3E,3F
             ;∏a∑q∑Å âw∂Å π∑¨˜∑i ¡°¨˜∑aù° §aéâòÅ∑Å àb CODEói
jongchotable DB 000H,002H,088H,000H,000H,005H,090H,000H   ;àA êA
             DB 000H,008H,094H,000H,000H,009H,09CH,000H   ;îA úA
             DB 000H,011H,0A0H,000H,000H,013H,0A4H,000H   ;†A §A
             DB 000H,015H,0ACH,000H,000H,017H,0B4H,000H   ;¨A ¥A
             DB 000H,018H,0B8H,000H,000H,019H,0C0H,000H   ;∏A ¿A
             DB 000H,01AH,0C4H,000H,000H,01BH,0C8H,000H   ;ƒA »A
             DB 000H,01CH,0CCH,000H,000H,01DH,0d0H,000H   ;ÃA –A
             DB 000H,000H,000H,000H,000H,000H,000H,000H
              ;∫ó¨˜ ∏˜•° …A∑°ßi,àb ∫ó¨˜µA òaüe ¡°¨˜,∫ó¨˜∑Å —w»Å ∏˜∑Å
              ;àb 4 BYTEù° äÅ¨˜,¥|∑Å ñÅBYTEìe ∫ó¨˜  CODE
              ;≠A§ÂºÅ BYTE B4,B3:π∑¨˜ TYPE
              ;≠A§ÂºÅ BYTE B0~B2:§h√± ∑∂∑i âw∂Å ¡°¨˜ TYPE
              ;ëA§ÂºÅ BYTE B0~B2:§h√± ¥Ù∑i âw∂Å ¡°¨˜ TYPE
junginfotble db 000h,000h,000h,000h,000h,000h,000h,000h   ;
              DB 000H,060H,001H,000H,000H,0A0H,001H,000H   ; Äa Ä°
              db 000h,0e0h,009h,000h,001h,060h,009h,000h   ;  Ä· Åa
              db 000h,080h,009h,000h,000h,0c0h,009h,000h   ;  ÄÅ Ä¡
              db 001h,040h,009h,000h,001h,080h,009h,000h   ;  ÅA ÅÅ
              db 003h,0a0h,009h,000h,001h,0a0h,01bh,002h   ;  É° Å°
              db 002h,060h,01bh,002h,003h,060h,01bh,002h   ;  Ça Éa
              db 002h,080h,01bh,003h,003h,040h,01bh,003h   ;  ÇÅ ÉA
              db 003h,080h,00ch,006h,001h,0c0h,004h,006h   ;  ÉÅ Å¡
              db 001h,0e0h,00ch,006h,002h,040h,00ch,006h   ;  Å· ÇA
              db 002h,0e0h,00ch,005h,002h,0a0h,00ch,005h   ;  Ç· Ç°
              db 002h,0c0h,00ch,005h,003h,0c0h,01bh,002h   ;  Ç¡ ¥aúÅ¥a
              db 000h,000h,000h,000h,000h,000h,000h,000h
                ;Æï∏a,«°—°«° ówµA SHIFTàa î·–Åª© âw∂Å∑Å CODE
                ;¨w∂· BYTE:¢Å SHIFT ≈°óa
                ;–a∂· BYTE:SHIFT CODE
iicolshifttable  DB 030H,029H,031H,021H,032H,040H,033H,023H  ;0 1 2 3
           DB 034H,024H,035H,025H,036H,05EH,037H,026H  ;4 5 6 7
           DB 038H,02AH,039H,028H,03DH,02BH,03BH,03AH  ;8 9 = ;
           DB 027H,022H,060H,07EH,05BH,07BH,05DH,07DH  ;' ` [ ]
           DB 02CH,03CH,02EH,03EH,02FH,03FH,02DH,05FH  ;, . / -
           DB 05CH,07CH                                ;\

          ;àa¨w ØaƒÖ
vscantable  DB 002H,001H,003H,002H,004H,003H,005H,004H
           DB 006H,005H,007H,006H,008H,007H,009H,008H
           DB 00AH,009H,00BH,00AH,01EH,00BH,030H,00CH
           DB 02EH,00DH,020H,00EH,012H,00FH,021H,010H
           DB 022H,011H,023H,012H,017H,013H,024H,014H
            ;•¢°°∑q TABLE
            ;BYTE1:  ¨Åù° ói¥·µ• SCAN CODE
            ;BYTE2,3:∂•úÅ ∑∂îÂ °°∑q CODE
            ;BYTE4,5:¨Åù° –s¨˜ñE •¢°°∑q CODE
bokmotable DB 025H,000H,060H,000H,0A0H,000H,000H,000H  ; Ä°
           DB 024H,000H,0E0H,001H,060H,000H,000H,000H  ; Åa
           DB 023H,001H,0A0H,002H,060H,000H,000H,000H  ; Ça
           DB 032H,002H,080H,003H,040H,000H,000H,000H  ; ÉA
           DB 025H,001H,0A0H,001H,0C0H,000H,000H,000H  ; Å¡
           DB 026H,001H,0A0H,002H,040H,000H,000H,000H  ; ÇA
           DB 018H,001H,0A0H,001H,0E0H,000H,000H,000H  ; Å·
           DB 026H,002H,080H,002H,0E0H,000H,000H,000H  ; Ç·
           DB 024H,002H,080H,002H,0A0H,000H,000H,000H  ; Ç°
           DB 019H,002H,080H,002H,0C0H,000H,000H,000H  ; Ç¡
           DB 026H,003H,060H,003H,080H,000H,000H,000H  ; ÉÅ
           DB 019H,001H,040H,001H,080H,000H,000H,000H  ; ÅÅ
           DB 018H,000H,080H,000H,0C0H,000H,000H,000H  ; Ä¡
           DB 025H,003H,060H,003H,0C0H,000H,000H,000H  ; ¥aúÅ¥a(Éa+Äa)
           DB 000H,000H
           ;•¢∏a∑q TABLE
           ;BYTE1:  ¨Åù° ói¥·µ• SCAN CODE
           ;BYTE2,3:∂•úÅ ∑∂îÂ ∏a∑q ≈°óa
           ;BYTE4,5:¨Åù° –s¨˜ñE •¢∏a∑q ≈°óa
           ;BYTE6,7:¶Ö–ÅØ° ¡°¨˜≈°óa
bokjatable DB 013H,000H,002H,000H,003H,088H,000H,000H ; ¥„
           DB 014H,000H,002H,000H,004H,0ACH,000H,000H ; ¥‰
           DB 011H,000H,005H,000H,006H,0B8H,000H,000H ; ¥Ê
           DB 022H,000H,005H,000H,007H,0D0H,000H,000H ; ¥Á
           DB 013H,000H,009H,000H,00AH,088H,000H,000H ; ¥Í
           DB 01EH,000H,009H,000H,00BH,0A0H,000H,000H ; ¥Î
           DB 010H,000H,009H,000H,00CH,0A4H,000H,000H ; ¥Ï
           DB 014H,000H,009H,000H,00DH,0ACH,000H,000H ; ¥Ì
           DB 02DH,000H,009H,000H,00EH,0C8H,000H,000H ; ¥Ó
           DB 02FH,000H,009H,000H,00FH,0CCH,000H,000H ; ¥Ô
           DB 022H,000H,009H,000H,010H,0D0H,000H,000H ; ¥
           DB 014H,000H,013H,000H,014H,0ACH,000H,000H ; ¥Ù
           DB 014H,000H,015H,000H,016H,0ACH,000H,000H ; ¥ˆ
           DB 000H,000H,000H,000H,000H,000H,000H,000H ;



 ; ∫ó¨˜ ∏˜•° …A∑°ßi .–e ≈°óaîw 2 §a∑°Àaù° äÅ¨˜
 ; ¿ı§ÂºÅ §a∑°Àa∑Å b3:π∑¨˜ »a∑≥  b0..b2:§h√±¥Ù∑iØ° ¡°¨˜ »a∑≥
 ; ñÅ§ÂºÅ §a∑°Àa∑Å b0..b2:§h√± ∑∂∑iØ° ¡°¨˜ »a∑≥
JUNGINFOTABLE DB 000H,000H,000H,000H,000H,000H,000H,001H   ; 0,1,2,3
              DB 000H,001H,000H,001H,000h,001H,000H,001H   ; 4,5,6,7
              DB 000H,000H,000H,000H,000H,001H,000H,001H   ; 8,9,A,B
              DB 000H,001H,00AH,003H,006H,004H,006H,004H   ; C,D,E,F
              DB 000H,000H,000H,000H,006H,004H,00AH,003H   ; 10,11,12,13
              DB 00BH,003H,005H,004H,005H,004H,005H,004H   ; 14,15,16,17
              DB 000H,000H,000H,000H,00BH,003H,00AH,003H   ; 18,19,1A,1B
              DB 006H,004H,000H,001H,000H,000H,000H,000H   ; 1C,1D,1E,1F
;–eãi ∏a°°∑Å DATA Ø°∏b §Âª°
;–e ≈°óaîw ¡°,∫ó,π∑¨˜ 2 BYTE≥¢ 6BYTE äÅ¨˜ñA¥· ∑∂îa.
SIJACTABLE DW 00000H,00000H,00000H,00000H,00000H,00000H   ; 0,1
           DW 00000H,00000H,01180H,01840H,00c40H,011c0H   ; 2,3
           DW 000e0H,00ec0H,01200H,001c0H,00d40H,01240H   ; 4,5
           DW 01920H,01100H,01280H,002a0H,00c80H,012C0H   ; 6,7
           DW 00380H,00000H,01300H,00460H,00000H,01340H   ; 8,9
           DW 01A00H,00f00H,01380H,00540H,00d80H,013c0H   ; A,B
           DW 01AE0H,01140H,01400H,00620H,00cc0H,01440H   ; C,D
           DW 00700H,00f80H,01480H,01BC0H,00fc0H,014c0H   ; E,F
           DW 007e0H,00000H,01500H,008c0H,00000H,01540H   ; 10,11
           DW 009a0H,01000H,00000H,00a80H,00dc0H,01580H   ; 12,13
           DW 00b60H,00d00H,015C0H,00000H,01080H,01600H   ; 14,15
           DW 00000H,010c0H,01640H,00000H,01040H,01680H   ; 16,17
           DW 00000H,00000H,016c0H,00000H,00000H,01700H   ; 18,19
           DW 00000H,00e00H,01740H,00000H,00e40H,01780H   ; 1A,1B
           DW 00000H,00f40H,017c0H,00000H,00e80H,01800H   ; 1C,1D
           DW 00000H,00000H,00000H,00000H,00000H,00000H   ; 1E,1F
engfont      DB   000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
             DB   000h,000h,000h,000h,000h,000h,000h,000h,000H,000H
             DB   018h,03Ch
             DB   03Ch,03Ch,018h,018h,000h,018h,018h,000h,000h,000h
             DB   000h,000h,000h,066h,066h,066h,044h,000h,000h,000h
             DB   000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
             DB   06Ch,06Ch,0FEh,06Ch,06Ch,06Ch,0FEh,06Ch,06Ch,000h
             DB   000h,000h,000h,000h,018h,018h,07Ch,0C6h,0C2h,0C0h
             DB   07Ch,006h,086h,0C6h,07Ch,018h,018h,000h,000h,000h
             DB   000h,000h,000h,000h,0C2h,0C6h,00Ch,018h,030h,066h
             DB   0C6h,000h,000h,000h,000h,000h,000h,000h,038h,06Ch
             DB   06Ch,038h,076h,0DCh,0CCh,0CCh,076h,000h,000h,000h
             DB   000h,000h,000h,030h,030h,030h,060h,000h,000h,000h
             DB   000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
             DB   00Ch,018h,030h,030h,030h,030h,030h,018h,00Ch,000h
             DB   000h,000h,000h,000h,000h,000h,030h,018h,00Ch,00Ch
             DB   00Ch,00Ch,00Ch,018h,030h,000h,000h,000h,000h,000h
             DB   000h,000h,000h,000h,066h,03Ch,0FFh,03Ch,066h,000h
             DB   000h,000h,000h,000h,000h,000h,000h,000h,000h,018h
             DB   018h,018h,0FFh,018h,018h,018h,000h,000h,000h,000h
             DB   000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
             DB   000h,000h,018h,018h,018h,030h,000h,000h,000h,000h
             DB   000h,000h,000h,000h,07Eh,000h,000h,000h,000h,000h
             DB   000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
             DB   000h,000h,000h,000h,000h,030h,030h,000h,000h,000h
             DB   000h,000h,002h,006h,00Ch,018h,030h,060h,0C0h,080h
             DB   000h,000h,000h,000h,000h,000h,000h,000h,07Ch,0C6h
             DB   0CEh,0DEh,0F6h,0E6h,0C6h,0C6h,07Ch,000h,000h,000h
             DB   000h,000h,000h,000h,018h,038h,078h,018h,018h,018h
             DB   018h,018h,07Eh,000h,000h,000h,000h,000h,000h,000h
             DB   07Ch,0C6h,006h,00Ch,018h,030h,060h,0C6h,0FEh,000h
             DB   000h,000h,000h,000h,000h,000h,07Ch,0C6h,006h,006h
             DB   03Ch,006h,006h,0C6h,07Ch,000h,000h,000h,000h,000h
             DB   000h,000h,00Ch,01Ch,03Ch,06Ch,0CCh,0FEh,00Ch,00Ch
             DB   01Eh,000h,000h,000h,000h,000h,000h,000h,0FEh,0C0h
             DB   0C0h,0C0h,0FCh,006h,006h,0C6h,07Ch,000h,000h,000h
             DB   000h,000h,000h,000h,038h,060h,0C0h,0C0h,0FCh,0C6h
             DB   0C6h,0C6h,07Ch,000h,000h,000h,000h,000h,000h,000h
             DB   0FEh,0C6h,006h,00Ch,018h,030h,030h,030h,030h,000h
             DB   000h,000h,000h,000h,000h,000h,07Ch,0C6h,0C6h,0C6h
             DB   07Ch,0C6h,0C6h,0C6h,07Ch,000h,000h,000h,000h,000h
             DB   000h,000h,07Ch,0C6h,0C6h,0C6h,07Eh,006h,006h,00Ch
             DB   078h,000h,000h,000h,000h,000h,000h,000h,000h,018h
             DB   018h,000h,000h,000h,018h,018h,000h,000h,000h,000h
             DB   000h,000h,000h,000h,000h,018h,018h,000h,000h,000h
             DB   018h,018h,030h,000h,000h,000h,000h,000h,000h,000h
             DB   006h,00Ch,018h,030h,060h,030h,018h,00Ch,006h,000h
             DB   000h,000h,000h,000h,000h,000h,000h,000h,000h,07Eh
             DB   000h,000h,07Eh,000h,000h,000h,000h,000h,000h,000h
             DB   000h,000h,060h,030h,018h,00Ch,006h,00Ch,018h,030h
             DB   060h,000h,000h,000h,000h,000h,000h,000h,07Ch,0C6h
             DB   0C6h,00Ch,018h,018h,000h,018h,018h,000h,000h,000h
             DB   000h,000h,000h,000h,07Ch,0C6h,0C6h,0DEh,0DEh,0DEh
             DB   0DCh,0C0h,07Ch,000h,000h,000h,000h,000h,000h,000h
             DB   010h,038h,06Ch,0C6h,0C6h,0FEh,0C6h,0C6h,0C6h,000h
             DB   000h,000h,000h,000h,000h,000h,0FCh,066h,066h,066h
             DB   07Ch,066h,066h,066h,0FCh,000h,000h,000h,000h,000h
             DB   000h,000h,03Ch,066h,0C2h,0C0h,0C0h,0C0h,0C2h,066h
             DB   03Ch,000h,000h,000h,000h,000h,000h,000h,0F8h,06Ch
             DB   066h,066h,066h,066h,066h,06Ch,0F8h,000h,000h,000h
             DB   000h,000h,000h,000h,0FEh,066h,062h,068h,078h,068h
             DB   062h,066h,0FEh,000h,000h,000h,000h,000h,000h,000h
             DB   0FEh,066h,062h,068h,078h,068h,060h,060h,0F0h,000h
             DB   000h,000h,000h,000h,000h,000h,03Ch,066h,0C2h,0C0h
             DB   0C0h,0DEh,0C6h,066h,03Ah,000h,000h,000h,000h,000h
             DB   000h,000h,0C6h,0C6h,0C6h,0C6h,0FEh,0C6h,0C6h,0C6h
             DB   0C6h,000h,000h,000h,000h,000h,000h,000h,03Ch,018h
             DB   018h,018h,018h,018h,018h,018h,03Ch,000h,000h,000h
             DB   000h,000h,000h,000h,01Eh,00Ch,00Ch,00Ch,00Ch,00Ch
             DB   0CCh,0CCh,078h,000h,000h,000h,000h,000h,000h,000h
             DB   0E6h,066h,06Ch,06Ch,078h,06Ch,06Ch,066h,0E6h,000h
             DB   000h,000h,000h,000h,000h,000h,0F0h,060h,060h,060h
             DB   060h,060h,062h,066h,0FEh,000h,000h,000h,000h,000h
             DB   000h,000h,0C6h,0EEh,0FEh,0D6h,0C6h,0C6h,0C6h,0C6h
             DB   0C6h,000h,000h,000h,000h,000h,000h,000h,0C6h,0E6h
             DB   0F6h,0FEh,0DEh,0CEh,0C6h,0C6h,0C6h,000h,000h,000h
             DB   000h,000h,000h,000h,038h,06Ch,0C6h,0C6h,0C6h,0C6h
             DB   0C6h,06Ch,038h,000h,000h,000h,000h,000h,000h,000h
             DB   0FCh,066h,066h,066h,07Ch,060h,060h,060h,0F0h,000h
             DB   000h,000h,000h,000h,000h,000h,07Ch,0C6h,0C6h,0C6h
             DB   0C6h,0D6h,0DEh,07Ch,00Ch,00Eh,000h,000h,000h,000h
             DB   000h,000h,0FCh,066h,066h,066h,07Ch,06Ch,066h,066h
             DB   0E7h,000h,000h,000h,000h,000h,000h,000h,07Ch,0C6h
             DB   0C6h,060h,038h,00Ch,0C6h,0C6h,07Ch,000h,000h,000h
             DB   000h,000h,000h,000h,0FFh,099h,018h,018h,018h,018h
             DB   018h,018h,03Ch,000h,000h,000h,000h,000h,000h,000h
             DB   0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,0C6h,07Ch,000h
             DB   000h,000h,000h,000h,000h,000h,0C6h,0C6h,0C6h,0C6h
             DB   0C6h,0C6h,06Ch,038h,010h,000h,000h,000h,000h,000h
             DB   000h,000h,0C6h,0C6h,0C6h,0C6h,0C6h,0D6h,0FEh,0EEh
             DB   0C6h,000h,000h,000h,000h,000h,000h,000h,0C6h,0C6h
             DB   0C6h,06Ch,038h,06Ch,0C6h,0C6h,0C6h,000h,000h,000h
             DB   000h,000h,000h,000h,0C3h,0C3h,066h,03Ch,018h,018h
             DB   018h,018h,03Ch,000h,000h,000h,000h,000h,000h,000h
             DB   0FEh,0C6h,08Ch,018h,030h,060h,0C2h,0C6h,0FEh,000h
             DB   000h,000h,000h,000h,000h,000h,03Ch,030h,030h,030h
             DB   030h,030h,030h,030h,03Ch,000h,000h,000h,000h,000h
             DB   000h,000h,080h,0C0h,0E0h,070h,038h,01Ch,00Eh,006h
             DB   002h,000h,000h,000h,000h,000h,000h,000h,03Ch,00Ch
             DB   00Ch,00Ch,00Ch,00Ch,00Ch,00Ch,03Ch,000h,000h,000h
             DB   000h,000h,010h,038h,06Ch,0C6h,000h,000h,000h,000h
             DB   000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
             DB   000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
             DB   0FFh,000h,000h,000h,018h,018h,00Ch,000h,000h,000h
             DB   000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
             DB   000h,000h,000h,000h,000h,078h,00Ch,07Ch,0CCh,0CCh
             DB   076h,000h,000h,000h,000h,000h,000h,000h,0E0h,060h
             DB   060h,078h,06Ch,066h,066h,066h,0DCh,000h,000h,000h
             DB   000h,000h,000h,000h,000h,000h,000h,07Ch,0C6h,0C0h
             DB   0C0h,0C6h,07Ch,000h,000h,000h,000h,000h,000h,000h
             DB   01Ch,00Ch,00Ch,03Ch,06Ch,0CCh,0CCh,0CCh,076h,000h
             DB   000h,000h,000h,000h,000h,000h,000h,000h,000h,07Ch
             DB   0C6h,0FEh,0C0h,0C6h,07Ch,000h,000h,000h,000h,000h
             DB   000h,000h,01Ch,036h,032h,030h,07Ch,030h,030h,030h
             DB   078h,000h,000h,000h,000h,000h,000h,000h,000h,000h
             DB   000h,076h,0CCh,0CCh,0CCh,07Ch,00Ch,0CCh,078h,000h
             DB   000h,000h,000h,000h,0E0h,060h,060h,06Ch,076h,066h
             DB   066h,066h,0E6h,000h,000h,000h,000h,000h,000h,000h
             DB   018h,018h,000h,038h,018h,018h,018h,018h,03Ch,000h
             DB   000h,000h,000h,000h,000h,000h,006h,006h,000h,00Eh
             DB   006h,006h,006h,006h,066h,066h,03Ch,000h,000h,000h
             DB   000h,000h,0E0h,060h,060h,066h,06Ch,078h,06Ch,066h
             DB   0E6h,000h,000h,000h,000h,000h,000h,000h,038h,018h
             DB   018h,018h,018h,018h,018h,018h,03Ch,000h,000h,000h
             DB   000h,000h,000h,000h,000h,000h,000h,0ECh,0FEh,0D6h
             DB   0D6h,0D6h,0D6h,000h,000h,000h,000h,000h,000h,000h
             DB   000h,000h,000h,0DCh,066h,066h,066h,066h,066h,000h
             DB   000h,000h,000h,000h,000h,000h,000h,000h,000h,07Ch
             DB   0C6h,0C6h,0C6h,0C6h,07Ch,000h,000h,000h,000h,000h
             DB   000h,000h,000h,000h,000h,0DCh,066h,066h,066h,07Ch
             DB   060h,060h,0F0h,000h,000h,000h,000h,000h,000h,000h
             DB   000h,076h,0CCh,0CCh,0CCh,07Ch,00Ch,00Ch,01Eh,000h
             DB   000h,000h,000h,000h,000h,000h,000h,0DCh,076h,066h
             DB   060h,060h,0F0h,000h,000h,000h,000h,000h,000h,000h
             DB   000h,000h,000h,07Ch,0C6h,070h,01Ch,0C6h,07Ch,000h
             DB   000h,000h,000h,000h,000h,000h,010h,030h,030h,0FCh
             DB   030h,030h,030h,036h,01Ch,000h,000h,000h,000h,000h
             DB   000h,000h,000h,000h,000h,0CCh,0CCh,0CCh,0CCh,0CCh
             DB   076h,000h,000h,000h,000h,000h,000h,000h,000h,000h
             DB   000h,0C6h,0C6h,0C6h,06Ch,038h,010h,000h,000h,000h
             DB   000h,000h,000h,000h,000h,000h,000h,0C6h,0C6h,0C6h
             DB   0D6h,0FEh,06Ch,000h,000h,000h,000h,000h,000h,000h
             DB   000h,000h,000h,0C6h,06Ch,038h,038h,06Ch,0C6h,000h
             DB   000h,000h,000h,000h,000h,000h,000h,000h,000h,0C6h
             DB   0C6h,0C6h,0C6h,07Eh,006h,00Ch,078h,000h,000h,000h
             DB   000h,000h,000h,000h,000h,0FEh,0CCh,018h,030h,066h
             DB   0FEh,000h,000h,000h,000h,000h,000h,000h,00Eh,018h
             DB   018h,018h,070h,018h,018h,018h,00Eh,000h,000h,000h
             DB   000h,000h,000h,000h,018h,018h,018h,018h,000h,018h
             DB   018h,018h,018h,000h,000h,000h,000h,000h,000h,000h
             DB   070h,018h,018h,018h,00Eh,018h,018h,018h,070h,000h
             DB   000h,000h,000h,000h,000h,000h,076h,0DCh,000h,000h
             DB   000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
HANFONT      DB 000H,000H,000H,000H,000H,000H,000H,000H    ;  àa
             DB 03FH,080H,011H,080H,003H,000H,006H,000H
             DB 00CH,000H,018H,000H,030H,000H,040H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,03FH,000H   ;   àb
             DB 003H,000H,006H,000H,00CH,000H,018H,000H
             DB 030H,000H,040H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,01FH,0F0H   ;   â°
             DB 00CH,030H,000H,030H,000H,030H,000H,030H
             DB 000H,020H,000H,040H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,01FH,0F0H,008H,030H   ;   äÅ
             DB 000H,030H,000H,020H,000H,040H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,03FH,000H,013H,000H   ;   â◊
             DB 003H,000H,002H,000H,004H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,03FH,000H   ;   ä·
             DB 013H,000H,003H,000H,003H,000H,002H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H   ;   â¡
             DB 03FH,080H,011H,080H,001H,080H,001H,080H
             DB 001H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;  êa
             DB 060H,000H,030H,000H,030H,000H,030H,000H
             DB 030H,000H,030H,080H,03fH,000H,01cH,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,060H,000H   ;   êe
             DB 030H,000H,030H,000H,031H,000H,03eH,000H
             DB 018H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,018H,000H   ;   ë°
             DB 00CH,000H,00CH,000H,00CH,010H,007H,0F8H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,018H,000H,00CH,000H   ;   íÅ
             DB 00CH,020H,007H,0F0H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,060H,000H,030H,000H   ;   ë≈
             DB 032H,000H,01FH,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,060H,000H   ;   í·
             DB 030H,000H,032H,000H,01FH,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H   ;   ë¡
             DB 060H,000H,030H,000H,032H,000H,01FH,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,002H,000H    ;  îa
             DB 07fH,000H,030H,000H,030H,000H,030H,000H
             DB 030H,000H,030H,080H,03fH,000H,01cH,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,002H,000H,07fH,000H   ;   îh
             DB 030H,000H,030H,000H,030H,000H,031H,080H
             DB 01eH,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,020H,03FH,0F0H   ;   ï°
             DB 018H,000H,018H,000H,018H,020H,00FH,0F0H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,020H,01FH,0F0H,00CH,000H   ;   ñÅ
             DB 00CH,000H,007H,0F0H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,07FH,000H,030H,000H   ;   ï◊
             DB 030H,000H,01FH,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,07FH,000H   ;   ñ·
             DB 030H,000H,030H,000H,01FH,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H   ;   ï¡
             DB 07FH,000H,030H,000H,030H,000H,01FH,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H   ;   úa
             DB 07fH,000H,023H,000H,002H,000H,07eH,000H
             DB 030H,000H,030H,080H,03fH,000H,01cH,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,03fH,000H   ;   úi
             DB 003H,000H,07eH,000H,030H,000H,031H,080H
             DB 01eH,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,00FH,0F0H   ;   ù°
             DB 000H,030H,01FH,0E0H,00CH,000H,00CH,020H
             DB 007H,0F0H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,00FH,0F0H,000H,030H,01FH,0E0H   ;   ûÅ
             DB 00CH,000H,007H,0F0H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,00FH,000H,003H,000H,01EH,000H   ;   ù◊
             DB 018H,080H,00FH,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,01FH,000H,003H,000H   ;   û·
             DB 03EH,000H,030H,000H,01FH,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,01FH,000H   ;   ù¡
             DB 003H,000H,03EH,000H,030H,080H,01FH,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;  †a
             DB 07fH,000H,033H,000H,033H,000H,033H,000H
             DB 033H,000H,032H,000H,03fH,000H,020H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,07fH,000H   ;   †q
             DB 033H,000H,033H,000H,032H,000H,03fH,000H
             DB 020H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,01FH,0F0H   ;   °°
             DB 00CH,070H,00CH,060H,00CH,040H,00FH,0E0H
             DB 008H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,01FH,0F0H,00CH,070H   ;   ¢Å
             DB 00CH,060H,007H,0E0H,004H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,03FH,000H,01BH,000H   ;   °—
             DB 01BH,000H,01FH,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,07FH,000H   ;   ¢·
             DB 033H,000H,033H,000H,03FH,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,03FH,000H   ;   °¡
             DB 01BH,000H,01BH,000H,01BH,000H,01FH,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,006H,000H   ;   §a
             DB 063H,000H,033H,000H,033H,000H,03fH,000H
             DB 033H,000H,033H,000H,03fH,000H,022H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,006H,000H,063H,000H   ;   §s
             DB 033H,000H,03fH,000H,033H,000H,03fH,000H
             DB 020H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,060H,018H,030H   ;   •°
             DB 00CH,030H,00FH,0F0H,00CH,030H,00FH,0F0H
             DB 008H,020H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,0C0H,018H,060H,00CH,060H   ;   ¶Å
             DB 00FH,0E0H,00CH,060H,00FH,0E0H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,033H,000H,01FH,000H   ;   •◊
             DB 01BH,000H,01FH,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,036H,000H,01BH,000H   ;   ¶·
             DB 01FH,000H,01BH,000H,01FH,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,036H,000H   ;   •¡
             DB 01BH,000H,01FH,000H,01BH,000H,01FH,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,004H,000H   ;   ¨a
             DB 006H,000H,006H,000H,006H,000H,00eH,000H
             DB 00fH,000H,019H,080H,031H,080H,040H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,004H,000H,006H,000H   ;   ¨u
             DB 006H,000H,00cH,000H,01eH,000H,033H,000H
             DB 041H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,001H,000H,001H,080H   ;   ≠°
             DB 001H,080H,003H,000H,007H,0F0H,00CH,038H
             DB 030H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,001H,000H,001H,080H,001H,080H   ;   ÆÅ
             DB 003H,0F0H,006H,038H,018H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,004H,000H,006H,000H,006H,000H   ;   ≠◊
             DB 00FH,080H,019H,080H,060H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,004H,000H,006H,000H   ;   Æ·
             DB 006H,000H,00FH,080H,019H,080H,060H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,004H,000H,006H,000H   ;   ≠¡
             DB 006H,000H,00FH,080H,019H,080H,060H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H   ;   ¥a
             DB 01cH,000H,036H,000H,063H,000H,063H,000H
             DB 063H,000H,036H,000H,01cH,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,01cH,000H   ;   ¥w
             DB 036H,000H,036H,000H,036H,000H,01cH,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,007H,0C0H   ;   µ°
             DB 00CH,060H,00CH,060H,00CH,060H,007H,0C0H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,003H,0C0H,006H,060H   ;   ∂Å
             DB 006H,060H,006H,060H,003H,0C0H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,01EH,000H,033H,000H   ;   µ◊
             DB 033H,000H,01EH,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,01EH,000H,033H,000H   ;   ∂·
             DB 033H,000H,033H,000H,01EH,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,01EH,000H   ;   µ¡
             DB 033H,000H,033H,000H,033H,000H,01EH,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;  ∏a
             DB 03FH,000H,013H,000H,006H,000H,00CH,000H
             DB 01BH,000H,031H,080H,041H,080H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,03FH,000H   ;   ∏b
             DB 006H,000H,00CH,000H,01BH,000H,031H,080H
             DB 041H,080H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,00FH,0E0H   ;   π°
             DB 001H,0C0H,003H,080H,006H,0F0H,00CH,038H
             DB 010H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,00FH,0E0H,003H,0C0H   ;   ∫Å
             DB 006H,070H,00CH,038H,030H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,03FH,000H,00CH,000H   ;   π◊
             DB 01EH,000H,033H,000H,040H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,03FH,000H   ;   ∫·
             DB 00CH,000H,01EH,000H,033H,000H,040H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,03FH,000H   ;   π¡
             DB 00CH,000H,01EH,000H,033H,000H,040H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,00CH,000H,006H,000H   ;   ¿a
             DB 000H,000H,03FH,000H,013H,000H,006H,000H
             DB 00EH,000H,01BH,000H,031H,080H,041H,080H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,00CH,000H,006H,000H   ;   ¿y
             DB 03FH,000H,006H,000H,00CH,000H,01BH,000H
             DB 031H,080H,041H,080H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,003H,000H,001H,080H   ;   ¡°
             DB 01FH,0F0H,001H,0E0H,003H,080H,006H,0F0H
             DB 018H,038H,020H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,003H,000H,001H,080H,01FH,0F0H   ;   ¬Å
             DB 003H,0C0H,00EH,078H,038H,018H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,018H,000H,00CH,000H,03FH,000H   ;   ¡◊
             DB 00EH,000H,01BH,000H,061H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,018H,000H,00CH,000H   ;   ¬·
             DB 03FH,000H,00EH,000H,01BH,000H,061H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,018H,000H,00CH,000H   ;   ¡¡
             DB 03FH,000H,00EH,000H,01BH,000H,061H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H   ;   ƒa
             DB 03FH,080H,001H,080H,07FH,080H,023H,000H
             DB 006H,000H,00CH,000H,018H,000H,030H,000H
             DB 040H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,03FH,000H   ;   ƒb
             DB 003H,000H,03FH,000H,006H,000H,00CH,000H
             DB 018H,000H,030H,000H,040H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,00FH,0F8H   ;   ≈°
             DB 004H,018H,000H,038H,00FH,0D8H,004H,018H
             DB 000H,018H,000H,010H,000H,020H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,00FH,0F0H,004H,030H   ;   ∆Å
             DB 000H,0F0H,00FH,030H,004H,030H,000H,020H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,03FH,000H,003H,000H   ;   ≈◊
             DB 03FH,000H,003H,000H,002H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,03FH,000H   ;   ∆·
             DB 003H,000H,03FH,000H,003H,000H,002H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H   ;   ≈¡
             DB 03FH,080H,001H,080H,01FH,080H,001H,080H
             DB 001H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,002H,000H   ;   »a
             DB 07FH,000H,030H,000H,030H,000H,03FH,000H
             DB 030H,000H,030H,080H,03FH,000H,018H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,002H,000H,07FH,000H   ;   »{
             DB 030H,000H,03EH,000H,030H,000H,030H,080H
             DB 01FH,000H,00CH,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,020H,01FH,0F0H   ;   …°
             DB 00CH,000H,00FH,0E0H,00CH,000H,00CH,020H
             DB 007H,0F0H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,040H,00FH,0E0H,006H,000H   ;    Å
             DB 007H,0E0H,006H,000H,003H,0E0H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,03FH,000H,018H,000H   ;   …◊
             DB 01FH,000H,018H,000H,00FH,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,03FH,000H,018H,000H   ;    ·
             DB 01FH,000H,018H,000H,00FH,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,03FH,000H   ;   …¡
             DB 018H,000H,01FH,000H,018H,000H,00FH,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,002H,000H   ;   Ãa
             DB 07FH,000H,004H,000H,024H,000H,034H,000H
             DB 014H,080H,00BH,000H,07EH,000H,020H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,002H,000H,07FH,000H   ;   Ãe
             DB 024H,000H,036H,000H,015H,080H,07EH,000H
             DB 020H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,020H,01FH,0F8H   ;   Õ°
             DB 00CH,060H,006H,060H,006H,0C0H,01FH,0F8H
             DB 008H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,010H,01FH,0F8H,006H,060H   ;   ŒÅ
             DB 006H,060H,01FH,0F8H,008H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,03FH,080H,01BH,000H   ;   Õ◊
             DB 00AH,080H,03FH,000H,018H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,03FH,080H   ;   Œ·
             DB 01BH,000H,00AH,080H,03FH,000H,018H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H   ;   Õ¡
             DB 03FH,080H,01BH,000H,00AH,080H,03FH,000H
             DB 018H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,018H,000H   ;   –a
             DB 00CH,000H,005H,000H,07FH,080H,00CH,000H
             DB 01EH,000H,033H,000H,033H,000H,01EH,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,018H,000H,00CH,000H   ;   –w
             DB 07FH,000H,01CH,000H,036H,000H,036H,000H
             DB 01CH,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,003H,000H,001H,090H   ;   —°
             DB 01FH,0F8H,00BH,0C0H,006H,060H,006H,060H
             DB 003H,0C0H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,003H,000H,001H,0A0H,00FH,0F0H   ;   “Å
             DB 003H,0C0H,006H,060H,003H,0C0H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,00CH,000H,006H,000H,03FH,080H   ;   —◊
             DB 00EH,000H,01BH,000H,00EH,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,00CH,000H,006H,000H   ;   “·
             DB 03FH,080H,00EH,000H,01BH,000H,00EH,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,00CH,000H,006H,000H   ;   —¡
             DB 03FH,080H,00EH,000H,01BH,000H,00EH,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,030H,000H,018H,000H,018H    ;  ¥a
             DB 000H,018H,000H,01AH,000H,01FH,000H,018H
             DB 000H,018H,000H,018H,000H,018H,000H,018H
             DB 000H,018H,000H,010H,000H,010H,000H,000H
             DB 000H,000H,000H,030H,000H,018H,000H,018H
             DB 000H,018H,000H,01FH,000H,018H,000H,018H
             DB 000H,010H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,018H,000H,00cH,000H,00cH    ;  ¥·
             DB 000H,00cH,000H,00cH,000H,00cH,000H,0fcH
             DB 000H,00cH,000H,00cH,000H,00cH,000H,00cH
             DB 000H,00cH,000H,00cH,000H,008H,000H,000H
             DB 000H,000H,000H,018H,000H,00cH,000H,00cH
             DB 000H,00cH,000H,0fcH,000H,00cH,000H,00cH
             DB 000H,008H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;   µ°
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 001H,000H,001H,080H,001H,080H,001H,08cH
             DB 07fH,0feH,020H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,001H,000H,001H,080H,001H,08cH
             DB 07fH,0feH,020H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;   ∂Å
             DB 000H,000H,000H,000H,000H,000H,000H,00cH
             DB 07fH,0feH,021H,080H,001H,080H,001H,080H
             DB 001H,080H,001H,080H,001H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,00cH,07fH,0feH
             DB 021H,080H,001H,080H,001H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,030H,000H,018H,000H,018H    ;   ¥°
             DB 000H,018H,000H,01AH,000H,01FH,000H,018H
             DB 000H,01AH,000H,01FH,000H,018H,000H,018H
             DB 000H,018H,000H,018H,000H,010H,000H,000H
             DB 000H,000H,000H,030H,000H,018H,000H,018H
             DB 000H,01FH,000H,018H,000H,01FH,000H,018H
             DB 000H,010H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,018H,000H,00CH,000H,00CH    ;   µa
             DB 000H,00CH,000H,07CH,000H,00CH,000H,00CH
             DB 000H,07CH,000H,00CH,000H,00CH,000H,00CH
             DB 000H,00CH,000H,00CH,000H,008H,000H,000H
             DB 000H,000H,000H,018H,000H,00CH,000H,00CH
             DB 000H,0FCH,000H,00CH,000H,0FCH,000H,00CH
             DB 000H,008H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;   ∂a
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 004H,080H,006H,0C0H,006H,0C0H,006H,0CCH
             DB 07FH,0FEH,020H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,004H,040H,006H,060H,006H,064H
             DB 07FH,0FEH,020H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;    ∑A
             DB 000H,000H,000H,000H,000H,000H,000H,00CH
             DB 07FH,0FEH,026H,060H,006H,060H,006H,060H
             DB 006H,060H,006H,060H,004H,040H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,004H,07FH,0FEH
             DB 026H,060H,006H,060H,004H,040H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;    ∑a
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,00CH
             DB 07FH,0FEH,020H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,002H
             DB 07FH,0FEH,020H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,00cH,000H,006H,000H,006H    ;    ∑°
             DB 000H,006H,000H,006H,000H,006H,000H,006H
             DB 000H,006H,000H,006H,000H,006H,000H,006H
             DB 000H,006H,000H,006H,000H,004H,000H,000H
             DB 000H,000H,000H,00cH,000H,006H,000H,006H
             DB 000H,006H,000H,006H,000H,006H,000H,006H
             DB 000H,004H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,00CH,000H,066H,000H,036H    ;     ¥Å
             DB 000H,036H,000H,036H,000H,036H,000H,03EH
             DB 000H,036H,000H,036H,000H,036H,000H,036H
             DB 000H,036H,000H,026H,000H,004H,000H,000H
             DB 000H,000H,000H,00CH,000H,066H,000H,036H
             DB 000H,03EH,000H,036H,000H,036H,000H,036H
             DB 000H,024H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,00CH,000H,066H,000H,036H    ;    µA
             DB 000H,036H,000H,036H,000H,036H,001H,0F6H
             DB 000H,036H,000H,036H,000H,036H,000H,036H
             DB 000H,036H,000H,026H,000H,006H,000H,000H
             DB 000H,000H,000H,00CH,000H,066H,000H,036H
             DB 000H,036H,001H,0F6H,000H,036H,000H,036H
             DB 000H,024H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,018H,000H,00CH,000H,00CH    ;    ∑Å
             DB 000H,00CH,000H,00CH,000H,00CH,000H,00CH
             DB 000H,00CH,000H,02CH,000H,0CCH,03FH,00CH
             DB 018H,00CH,000H,00CH,000H,008H,000H,000H
             DB 000H,000H,000H,018H,000H,00CH,000H,00CH
             DB 000H,00CH,000H,00CH,000H,02CH,000H,0CCH
             DB 07FH,00CH,030H,008H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,030H,000H,018H,000H,018H    ;    µ¡
             DB 000H,018H,000H,018H,000H,018H,000H,01AH
             DB 00CH,01FH,00CH,038H,00CH,0D8H,07FH,018H
             DB 030H,018H,000H,018H,000H,010H,000H,000H
             DB 000H,000H,000H,030H,000H,018H,000H,018H
             DB 000H,01AH,00CH,01FH,00CH,018H,00CH,078H
             DB 07FH,098H,030H,010H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,00CH,000H,066H,000H,036H    ;    µ·
             DB 000H,036H,000H,036H,000H,036H,000H,036H
             DB 00CH,036H,00CH,076H,00DH,0B6H,07EH,036H
             DB 030H,036H,000H,026H,000H,004H,000H,000H
             DB 000H,000H,000H,00CH,000H,066H,000H,036H
             DB 000H,036H,00CH,03EH,00CH,036H,00CH,0F6H
             DB 07FH,026H,030H,004H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,018H,000H,00CH,000H,00CH    ;   ∂A
             DB 000H,00CH,000H,00CH,000H,00CH,000H,00CH
             DB 00CH,00CH,00CH,02CH,00CH,0CCH,07FH,00CH
             DB 030H,00CH,000H,00CH,000H,008H,000H,000H
             DB 000H,000H,000H,018H,000H,00CH,000H,00CH
             DB 000H,00CH,00CH,00CH,00CH,00CH,00CH,06CH
             DB 07FH,08CH,030H,008H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,018H,000H,00CH,000H,00CH    ;   ∂·
             DB 000H,00CH,000H,00CH,000H,00CH,000H,01CH
             DB 000H,06CH,07FH,08CH,036H,00CH,006H,00CH
             DB 006H,00CH,004H,00CH,000H,008H,000H,000H
             DB 000H,000H,000H,018H,000H,00CH,000H,00CH
             DB 000H,00CH,000H,00CH,000H,06CH,07FH,08CH
             DB 036H,00CH,006H,03CH,004H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,018H,000H,00CH,000H,00CH    ;   ∂°
             DB 000H,00CH,000H,00CH,000H,00CH,000H,01CH
             DB 000H,06CH,07FH,08CH,036H,0FCH,006H,04CH
             DB 006H,00CH,004H,00CH,000H,008H,000H,000H
             DB 000H,000H,000H,018H,000H,00CH,000H,00CH
             DB 000H,00CH,000H,00CH,000H,06CH,07FH,08CH
             DB 036H,00CH,006H,03CH,004H,008H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,00CH,000H,066H,000H,036H    ;   ∂¡
             DB 000H,036H,000H,036H,000H,036H,000H,076H
             DB 001H,0B6H,07EH,036H,02DH,0F6H,00CH,0B6H
             DB 00CH,036H,008H,026H,000H,004H,000H,000H
             DB 000H,000H,000H,00CH,000H,066H,000H,036H
             DB 000H,036H,000H,036H,000H,0B6H,07FH,036H
             DB 036H,0F6H,006H,026H,006H,006H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,00CH,000H,066H,000H,036H    ;   ¥¡
             DB 000H,036H,000H,03EH,000H,036H,000H,036H
             DB 000H,03EH,000H,036H,000H,036H,000H,036H
             DB 000H,036H,000H,026H,000H,004H,000H,000H
             DB 000H,000H,000H,00CH,000H,066H,000H,036H
             DB 000H,03EH,000H,036H,000H,03EH,000H,036H
             DB 000H,024H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,00CH,000H,066H,000H,036H    ;    µÅ
             DB 000H,036H,000H,036H,001H,0F6H,000H,036H
             DB 001H,0F6H,000H,036H,000H,036H,000H,036H
             DB 000H,036H,000H,026H,000H,004H,000H,000H
             DB 000H,000H,000H,00CH,000H,066H,000H,036H
             DB 000H,0F6H,000H,036H,000H,0F6H,000H,036H
             DB 000H,024H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;    ¥b
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,00fH,0f8H,000H,018H
             DB 000H,018H,000H,018H,000H,010H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,00fH,0f0H,000H,030H
             DB 000H,030H,000H,030H,000H,020H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;    ¥c
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,00FH,0F8H,001H,098H
             DB 001H,098H,001H,098H,001H,010H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,01FH,0F0H,003H,030H
             DB 003H,030H,003H,030H,002H,020H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;    ¥d
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,010H,00FH,098H,001H,098H
             DB 001H,098H,001H,0BAH,001H,064H,000H,080H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,020H,01FH,030H,003H,030H
             DB 003H,030H,003H,068H,002H,0C8H,001H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;    ¥e
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,006H,000H,003H,000H
             DB 003H,000H,003H,008H,001H,0fcH,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,00cH,000H,006H,000H
             DB 006H,000H,006H,010H,003H,0f8H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;   ¥f
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,018H,07EH,00CH,018H
             DB 00CH,018H,00CH,0BCH,007H,066H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,030H,0FCH,018H,030H
             DB 018H,030H,019H,078H,00EH,0CCH,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;   ¥g
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,030H,00CH,018H,006H,07CH
             DB 006H,038H,006H,06CH,003H,0B8H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,060H,018H,030H,00CH,0F8H
             DB 00CH,070H,00CH,0D8H,007H,070H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;   ¥h
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,00fH,0f0H,006H,000H
             DB 006H,000H,006H,010H,003H,0f8H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,00fH,0f0H,006H,000H
             DB 006H,000H,006H,010H,003H,0f8H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;   ¥i
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,007H,0f8H,000H,010H
             DB 007H,0f0H,006H,000H,003H,0f8H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,00fH,0f0H,000H,030H
             DB 00fH,0e0H,00cH,010H,007H,0f8H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;   ¥j
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,00FH,0F8H,003H,018H
             DB 00FH,018H,00CH,018H,007H,090H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,01EH,0F0H,006H,030H
             DB 01EH,030H,018H,030H,00FH,020H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;   ¥k
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,01FH,0F8H,003H,0D8H
             DB 01FH,0D8H,018H,0D8H,00FH,0F8H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,01FH,0F8H,003H,0D8H
             DB 01FH,0D8H,018H,0D8H,00FH,0F8H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;   ¥l
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,00FH,0D8H,0D3H,0D8H
             DB 01FH,0F8H,018H,0D8H,00FH,0F8H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,00FH,0D8H,0D3H,0D8H
             DB 01FH,0F8H,018H,0D8H,00FH,0F8H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;   ¥m
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,01FH,020H,003H,030H
             DB 01FH,030H,018H,06CH,00FH,0C6H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,01FH,020H,003H,030H
             DB 01FH,030H,018H,06CH,00FH,0C6H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;   ¥n
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,01FH,0F8H,003H,050H
             DB 01FH,050H,018H,050H,00FH,0F8H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,01FH,0F8H,003H,050H
             DB 01FH,050H,018H,050H,00FH,0F8H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;    ¥o
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,01FH,0F8H,003H,050H
             DB 01FH,050H,018H,050H,00FH,0F8H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,01FH,0F8H,003H,050H
             DB 01FH,050H,018H,050H,00FH,0F8H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;   ¥p
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,060H,01FH,030H,003H,0FCH
             DB 01FH,038H,018H,06CH,00FH,038H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,060H,01FH,030H,003H,0FCH
             DB 01FH,038H,018H,06CH,00FH,038H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;   ¥q
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,007H,0F8H,003H,018H
             DB 003H,018H,003H,010H,003H,0F8H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,01FH,0F0H,00CH,030H
             DB 00CH,030H,00CH,020H,00FH,0F0H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;   ¥s
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,006H,018H,003H,018H
             DB 003H,0F8H,003H,018H,003H,0F8H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,00CH,060H,006H,030H
             DB 007H,0F0H,006H,030H,007H,0F0H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;   ¥t
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,009H,018H,00DH,018H
             DB 00FH,098H,00DH,0BEH,00FH,0E6H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,009H,018H,00DH,018H
             DB 00FH,098H,00DH,0BEH,00FH,0E6H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;    ¥u
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,0C0H,000H,0C0H
             DB 001H,0E0H,003H,038H,006H,018H,008H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,001H,000H,001H,080H,001H,080H
             DB 003H,0C0H,006H,070H,00CH,030H,010H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;    ¥v
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,002H,020H,003H,030H,003H,030H
             DB 007H,0F0H,00CH,0DCH,019H,08CH,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,004H,040H,006H,060H,006H,060H
             DB 00FH,0E0H,019H,0B8H,033H,018H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;   ¥w
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,001H,0F0H,003H,018H
             DB 003H,018H,003H,018H,001H,0F0H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,007H,0E0H,00CH,030H
             DB 00CH,030H,00CH,030H,007H,0E0H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;    ¥x
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,003H,0F8H,000H,060H
             DB 000H,0F0H,001H,09CH,003H,00CH,004H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,00FH,0E0H,001H,080H
             DB 003H,0C0H,006H,070H,00CH,030H,010H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;    ¥y
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,0C0H,000H,060H,003H,0F8H
             DB 000H,0E0H,001H,0BCH,003H,00CH,004H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,003H,000H,001H,080H,007H,0E0H
             DB 003H,080H,006H,0F0H,00CH,030H,010H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;    ¥z
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,007H,0F8H,000H,018H
             DB 007H,0F8H,002H,018H,000H,010H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,00FH,0F0H,000H,030H
             DB 00FH,0F0H,004H,030H,000H,020H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;    ¥{
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,00FH,0F8H,006H,000H
             DB 007H,0F0H,006H,000H,003H,0F8H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,01FH,0F8H,00CH,000H
             DB 00FH,0E0H,00CH,000H,007H,0F8H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;    ¥|
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,007H,0F8H,003H,030H
             DB 001H,0B0H,001H,020H,007H,0F8H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,00FH,0F0H,006H,060H
             DB 003H,060H,002H,060H,00FH,0F0H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;    ¥}
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,001H,080H,000H,0C0H,007H,0F8H
             DB 001H,0E0H,003H,030H,001H,0E0H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,003H,000H,001H,080H,00FH,0F0H
             DB 003H,0C0H,006H,060H,003H,0C0H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;   åa
             DB 03FH,080H,00DH,080H,00DH,080H,019H,080H
             DB 01BH,000H,033H,000H,046H,000H,00CH,000H
             DB 010H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,03FH,080H    ;  åb
             DB 00DH,080H,01BH,000H,036H,000H,04CH,000H
             DB 018H,000H,020H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,03EH,0F8H    ;  ç°
             DB 006H,018H,006H,018H,006H,018H,004H,010H
             DB 008H,020H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,03EH,0F8H,006H,018H    ;  éÅ
             DB 004H,010H,008H,020H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,07FH,080H,019H,080H    ;  ç◊
             DB 019H,080H,011H,000H,022H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,07FH,080H    ;  é·
             DB 019H,080H,019H,080H,011H,000H,022H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;  ç¡
             DB 07FH,080H,019H,080H,019H,080H,011H,000H
             DB 022H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;  òa
             DB 07FH,080H,036H,000H,036H,000H,036H,000H
             DB 036H,000H,036H,040H,03FH,080H,01BH,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,07FH,080H    ;  òb
             DB 036H,000H,036H,000H,036H,000H,03FH,080H
             DB 01BH,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,03FH,0F8H    ;  ô°
             DB 018H,0C0H,018H,0C0H,019H,0D0H,00EH,078H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,00FH,0F0H,006H,0C0H    ;  öÅ
             DB 006H,0C0H,003H,070H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,0000,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,07FH,080H,036H,000H    ;  ô◊
             DB 036H,000H,01BH,080H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,07FH,080H    ;  ö·
             DB 036H,000H,036H,000H,01BH,080H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;  ô¡
             DB 07FH,080H,036H,000H,036H,000H,01BH,080H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;  ®a
             DB 0DBH,000H,06DH,080H,06DH,080H,07FH,080H
             DB 06DH,080H,06DH,080H,07FH,080H,049H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,0DBH,000H    ;  ®b
             DB 06DH,080H,07FH,080H,06DH,080H,07FH,080H
             DB 049H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,033H,030H    ;  ©°
             DB 019H,098H,01FH,0F8H,019H,098H,01FH,0F8H
             DB 011H,010H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,01BH,060H,00DH,0B0H,00FH,0F0H    ;  ™Å
             DB 00DH,0B0H,01FH,0E0H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,0DBH,000H,06DH,080H    ;  ©◊
             DB 07FH,080H,06DH,080H,07FH,080H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,0DBH,000H,06DH,080H    ;  ™·
             DB 07FH,080H,06DH,080H,07FH,080H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,0DBH,000H    ;  ©¡
             DB 06DH,080H,07FH,080H,06DH,080H,07FH,080H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;  ∞a
             DB 012H,000H,01BH,000H,01BH,000H,01BH,000H
             DB 03FH,000H,06DH,080H,08DH,080H,010H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,002H,000H,013H,000H    ;  ∞b
             DB 01BH,000H,01BH,000H,03FH,000H,04DH,080H
             DB 010H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,002H,040H,003H,060H    ;  ±°
             DB 003H,060H,006H,0C0H,00FH,0F0H,01BH,038H
             DB 024H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,004H,040H,006H,060H,006H,060H    ;  ≤Å
             DB 00CH,0C0H,01FH,0F8H,033H,018H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,002H,000H,013H,000H   ;  ±◊
             DB 01BH,000H,01FH,080H,02DH,080H,050H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,002H,000H,013H,000H    ;  ≤·
             DB 01BH,000H,01BH,080H,02DH,080H,050H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,002H,000H    ;  ±¡
             DB 013H,000H,01BH,000H,01FH,080H,02DH,080H
             DB 050H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H    ;  ºa
             DB 07FH,080H,02DH,080H,00DH,080H,01BH,000H
             DB 03EH,000H,06EH,000H,0DBH,000H,033H,000H
             DB 020H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,03FH,080H    ;  ºb
             DB 009H,080H,01BH,000H,03FH,000H,04BH,080H
             DB 010H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,01FH,0F0H    ;  Ω°
             DB 00BH,060H,006H,0C0H,00FH,0E0H,01BH,038H
             DB 024H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,01FH,0F0H,006H,060H    ;  æÅ
             DB 00CH,0C0H,01FH,0F0H,026H,030H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,03FH,080H,01BH,000H    ;  Ω◊
             DB 01FH,080H,02DH,080H,050H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,03FH,080H    ;  æ·
             DB 01BH,000H,01FH,080H,02DH,080H,050H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,03FH,080H,01BH,000H    ;  Ω¡
             DB 01FH,080H,02DH,080H,050H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H
             DB 000H,000H,000H,000H,000H,000H,000H,000H

HERCIN: mov ax,cs
        mov ds,ax
        mov dx,03bfh 
        mov al,03h
        out dx,al
        mov dx,03b8h
        mov al,2h
        out dx,al
        mov ah,00                       ;;
        mov dx,03b4h                     ;
        mov cx,000ch                     ; 6845 register ∏˜∑Å
        mov si,offset data640400         ;
ginlp01:mov al,ah                        ;  AH:register§Â—°
        out dx,al                        ;  SI:data offset
        inc dx                           ;  DX:port §Â—°
        mov al,cs:[si]                   ;
        out dx,al                        ;
        dec dx                           ;
        inc ah                           ;
        inc si                           ;
        loop ginlp01                    ;;
        mov cx,4000h                 ;;
        mov ax,0B000H                 ;  B000∑°–a 8000 Byteüi 0∑aù°
        cld                           ;   ¿Å∂ë
        mov es,ax                     ;  Graphic —¡°e clear
        xor di,di                     ;
        mov ax,0                      ;
        repe stosw                   ;;
        mov dx,03b8h                     ;
        mov al,0ah                       ;    screen on
        out dx,al                        ;
        RET
HERCOUT:
THEEND01:
        MOV DX,03B8H   ;
        MOV AL,02      ;
        OUT DX,AL      ;
        MOV AH,0                ;;
        MOV DX,03B4H             ;
        MOV CX,0CH               ;
        MOV SI,OFFSET DATATEXT   ; 6845 register ∏˜∑Å
THEENDLP01:     MOV AL,AH        ;       
        OUT DX,AL                ;
        INC DX                   ;
        MOV AL,CS:[SI]           ;
        OUT DX,AL                ;
        DEC DX                   ;
        INC AH                   ;
        INC SI                   ;
        LOOP THEENDLP01         ;;
        MOV CX,0800H                ;;
        CLD                          ; B000∑°–a 0720(space ¢Å≠¢¨˜)
        XOR DI,DI                    ;      ∑aù° ¿Å∂ë
        MOV AX,0720H                 ;
        REPZ STOSW                  ;;   
        MOV DX,03B8H           ;
        MOV AL,08              ; screen on
        OUT DX,AL              ;
        RET
data640400 db 31h,28h,29h,08h,68h,02h,64h,65h,02h,03h,02h,01h
DATA720348 DB 35H,2DH,2EH,07H,5BH,02H,57H,57H,02H,03H,00H,00H
datatext   db 61h,50h,52h,0fh,19h,06h,19h,19h,02h,0dh,0bh,0ch   
_TEXT ENDS
END start
