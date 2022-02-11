; ASMDUNE.ASM

; Thomas H.

AC_INDEX        = 03c0h
MISC_OUTPUT     = 03c2h
SC_INDEX        = 03c4h
GC_INDEX	= 03ceh
CRTC_INDEX      = 03d4h
INPUT_STATUS_0  = 03dah
MAP_MASK	= 02h
READ_MAP        = 04h
BIT_MASK	= 08h

public _makeplasma, _drawrelieff, _dune_map, _dune_buffer, _dune_light
public _dunepl1, _dunepl2, _dunepl3, _duneplus


.model large

.code

EVEN

_dune_map dd ?
_dune_light dd ?
_dune_buffer dd ?
_dunepl1 dw ?
_dunepl2 dw ?
_dunepl3 dw ?
_duneplus dw ?

count	db ?

EVEN

.386

; ***** MAKEPLASMA *****

_makeplasma proc far

	pusha
	push ds

	lds si, _dune_map
	mov ax,ds
	add ax,64*32
	mov ds,ax
	les di,_dune_buffer

	mov si,_dunepl1
	mov bx,_dunepl2
	sub bx,si
	mov bp,_dunepl3
	sub bp,si

	mov ch,202
l1:	mov cl,2*81
l2:	lodsw
	mov dx,[si+bx]
	cmp al,dl
	ja alok1
	mov al,dl
alok1:
	cmp ah,dh
	ja ahok1
	mov ah,dh
ahok1:
	mov dx,ds:[si+bp]
	cmp al,dl
	ja alok2
	mov al,dl
alok2:
	cmp ah,dh
	ja ahok2
	mov ah,dh
ahok2:
	stosw
	dec cl
	jnz l2
	mov dx,ds
	add dx,512/16
	mov ds,dx
	sub si,324

	dec ch
	jnz l1

	pop ds
	popa
	retf

_makeplasma endp

; ***** DRAWRELIEFF *****

_drawrelieff proc far

	pusha
	push ds

	lds si,_dune_buffer
	lfs di,_dune_light

	mov ax,0a000h
	mov es,ax
	xor di,di
	mov dx,2
	mov count,200
	mov bp,2
lo1:
	mov cl,160
	mov dx,[si]
	add si,2
lo2:
	mov ax,[si]
	mov bx,dx
	mov dx,ax
	mov ch,bh
	mov bh,al
	mov al,fs:[bx]
	mov bl,ch
	mov bh,ah
	mov ah,fs:[bx]
	add ax,_duneplus
	mov es:[di],ax

	add si,2
	add di,2
	dec cl
	jnz lo2
	add si,2
	dec count
	jnz lo1

	pop ds
	popa
	retf

_drawrelieff endp

end
