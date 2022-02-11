; ASMVOXEL

; Thomas H.

VOXX		= 128
VOXY		= 96

SMOOTHCUT 	= 10

AC_INDEX        = 03c0h
MISC_OUTPUT     = 03c2h
SC_INDEX        = 03c4h
GC_INDEX	= 03ceh
CRTC_INDEX      = 03d4h
INPUT_STATUS_0  = 03dah
MAP_MASK	= 02h
READ_MAP        = 04h
BIT_MASK	= 08h

public	_lseg, _sseg, _hseg, _fseg, _alti, _asmvox, _interpolate
public _voxpos, _voypos, _ofraction, _copper_32, copper, _leapright, _asmsky
public _interlimit

.model large

.code

org 0
EVEN

copper	db (3*96*128+32) dup (?)

_sseg	dw ?
_hseg	dw ?
_fseg	dw ?
_lseg	dw ?
_voxpos dw ?
_voypos	dw ?
tempw	dw ?

_copper_32 label dword
	dw offset copper, seg copper
_leapright label dword
	dw offset leap, seg leap
tempax	dw ?

leap	db 128 dup (20)

_alti	db 0
_ofraction db ?
_interlimit db 64
fraction	db ?

x	db ?
y     	db ?
origx	db ?
origy	db ?
top	db ?
forrigecol db ?

EVEN

.386

; ***** INTERPOLATE *****

_interpolate proc far

	pusha
	push ds

	mov ax,_sseg
	mov ds,ax
	mov es,ax

	xor si,si
	mov di,256

	mov ch,64
	mov edx,0fefefefeh
next_right:
	mov cl,128
	mov eax,[si]
next_down:
	add si,512
	mov ebx,[si]
	add eax,ebx
	and eax,edx
	shr eax,1
	mov [di],eax
	add di,512
	mov eax,ebx
	dec cl
	jnz next_down
	sub si,512*127-4
	sub di,512*127-4
	dec ch
	jnz next_right

	pop ds
	popa
	retf

_interpolate endp

; ***** ASMVOX *****

_asmvox	proc far

	pusha
	push ds

	mov ds,_sseg
	mov es,_hseg
	mov fs,_fseg
	mov gs,_lseg
	lds bp,_copper_32
	mov ds,_sseg

	mov ax,_voxpos
	mov _ofraction,al
	mov origx,ah

	mov ax,_voypos
	mov origy,al

	mov x,128
	xor di,di
next_x:
	mov bx,128
	sub bl,x
	mov bl,leap[bx]
	mov top,bl
	add di,bx
	mov forrigecol,0
	mov y,95

next_yxx:
	mov ebx,cs:[bp]
	add bp,3

	ror ebx,8
	mov fraction,bh
	rol ebx,8
	add bh,origy
	mov al,_ofraction
	sub fraction,al
	adc bl,origx
	mov si,bx
	mov bl,es:[si]
	sub bl,_alti
	mov bh,y
	mov dl,gs:[bx]
	mov cl,fraction
	mov al,cl
	mul dl
	mov dx,ax
	mov bx,si
	inc bl
	mov bl,es:[bx]
	sub bl,_alti
	mov bh,y
	mov ch,gs:[bx]
	mov al,cl
	not al
	mul ch
	add ax,dx
	cmp ah,top
	jbe not_high_enoughxx

	mov dh,ah
	sub dh,top
	mov top,ah
	mov bx,si
	mov al,fs:[bx]
	mul cl
	mov dl,ah
	inc bl
	mov al,fs:[bx]
	dec bl
	not cl
	mul cl
	add ah,dl
	cmp dh,2
	ja over2xx
	je akkurat2xx
	mov [di],ah
	inc di
	mov forrigecol,ah
	dec y
	cmp y,64
	jne next_yxx
	jmp next_yyy
akkurat2xx:
	mov al,forrigecol
	or al,al
	jz toingengammelxx
	add al,ah
	shr al,1
	mov [di],ax
	add di,2
	mov forrigecol,ah
	dec y
	cmp y,64
	jne next_yxx
	jmp next_yyy
toingengammelxx:
	mov al,ah
	mov [di],ax
	add di,2
	mov forrigecol,ah
	dec y
	cmp y,64
	jne next_yxx
	jmp next_yyy

over2xx:
	mov bl,dh
	mov dh,forrigecol
	mov forrigecol,ah
	or dh,dh
	jz notfrombeforexx
	dec bl
	xor al,al
	mov cx,dx
	sub ah,dh
	xor bh,bh
	cwd

	idiv bx

	mov [di],ch
	inc di

nextbortxx:
	add cx,ax
	mov [di],ch
	inc di
	dec bl
	jnz nextbortxx

	dec y
	cmp y,64
	jne next_yxx
	jmp short next_yyy
notfrombeforexx:
nextnotfrombeforexx:
	mov [di],ah
	inc di
	dec bl
	jnz nextnotfrombeforexx
	dec y
	cmp y,64
	jne next_yxx
	jmp short next_yyy
not_high_enoughxx:
	mov forrigecol,0
drawetokxx:
	dec y
	cmp y,64
	jne next_yxx

; ******************************************************************
next_yyy:
	mov bx,cs:[bp]
	add bp,3

	add bh,origy
	add bl,origx
	mov si,bx
	mov bl,es:[si]
	sub bl,_alti
	mov bh,y
	mov ah,gs:[bx]
	cmp ah,top
	jbe not_high_enoughyy

	mov dh,ah
	sub dh,top
	mov top,ah
	mov bx,si
	mov ah,fs:[bx]
	cmp dh,2
	ja over2yy
	je akkurat2yy
	mov [di],ah
	inc di
	mov forrigecol,ah
	dec y
	jns next_yyy
	jmp thatsyy
akkurat2yy:
	mov al,forrigecol
	or al,al
	jz toingengammelyy
	add al,ah
	shr al,1
	mov [di],ax
	add di,2
	mov forrigecol,ah
	dec y
	jns next_yyy
	jmp thatsyy
toingengammelyy:
	mov al,ah
	mov [di],ax
	add di,2
	mov forrigecol,ah
	dec y
	jns next_yyy
	jmp thatsyy

over2yy:
	mov bl,dh
	mov dh,forrigecol
	mov forrigecol,ah
	or dh,dh
	jz notfrombeforeyy
	dec bl
	xor al,al
	mov cx,dx
	sub ah,dh
	xor bh,bh
	cwd

	idiv bx

	mov [di],ch
	inc di

nextbortyy:
	add cx,ax
	mov [di],ch
	inc di
	dec bl
	jnz nextbortyy

	dec y
	jns next_yyy
	jmp short thatsyy
notfrombeforeyy:
nextnotfrombeforeyy:
	mov [di],ah
	inc di
	dec bl
	jnz nextnotfrombeforeyy
	dec y
	jns next_yyy
	jmp short thatsyy
not_high_enoughyy:
	mov forrigecol,0
drawetokyy:
	dec y
	jns next_yyy
thatsyy:
	xor eax,eax
	mov [di],eax
	or di,255
	mov al,top
	sub al,2	; fixme!
	mov [di],al

	add di,257

	dec x
	jnz next_x

	pop ds
	popa

	retf
_asmvox endp

; ***** ASMSKY *****

_asmsky proc far

	xor di,di
	mov dl,128
	mov si,-257
	mov dh,es:[si]
next_double:
	mov al,es:[di+255]
	mov ah,al
	cmp al,dh
	jb allavest
	mov al,dh
allavest:
	mov dh,ah
	mov cx,256
	xor ah,ah
	sub cx,ax
	shr cx,1
	jnc notodd

	add di,ax
	add bx,ax
	mov ah,[bx]
	mov es:[di],ah
	inc bh
	mov ah,[bx]
	mov es:[di+256],ah
	dec bh

	xor ah,ah
	sub di,ax
        sub bx,ax
	inc ax

notodd:
	shr cx,1
	jnc quadrupel
	mov tempax,ax
	add di,ax
	add bx,ax

	mov ax,[bx]
	mov es:[di],ax
	inc bh
	mov ax,[bx]
	mov es:[di+256],ax
	dec bh
	mov ax,tempax
	sub di,ax
	sub bx,ax
	add ax,2
quadrupel:
	mov si,bx
	add di,ax
	add si,ax
	mov bp,cx
	rep movsd
	inc bh
	mov si,bx
	add di,ax
	add si,ax
	mov cx,bp
	rep movsd
	inc bh
	dec dl
	jnz next_double

	retf
_asmsky endp

end
