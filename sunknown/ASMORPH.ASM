; ASMORPH.ASM

; Thomas H.

.model large

public _mobuffer, _mo192, _drawmorph, _gomorph, _pixies

BIGPIX	= 0

.code

_mobuffer dd ?
_mo192	dd ?
_pixies	dw ?
.386

; ***** TEGNMORPH *****

_drawmorph proc far

	pusha
	push ds

	mov ax,0a000h
	mov es,ax
	mov di,32

	lds si,_mobuffer
	mov dl,200
	mov ax,64
next_copy:
	mov cx,ax
	rep movsd
	add di,ax
	dec dl
	jnz next_copy

	pop ds
	popa

	retf
_drawmorph endp

; ***** GOMORPH *****

_gomorph proc far

	pusha
	push ds

	les di,_mobuffer
	xor eax,eax
	mov cx,200*64
	rep stosd

	lds si,_mo192
	mov ax,ds
	add ax,4096
	mov fs,ax
	add ax,4096
	mov gs,ax

	xor si,si
	mov cx,_pixies
next_pixie:
	mov eax,[si]
	mov dx,ax
	shr eax,16
	add dx,ax
	mov [si],dx
	mov bl,dh

	mov eax,fs:[si]
	mov dx,ax
	shr eax,16
	add dx,ax
	mov fs:[si],dx
	mov bh,dh

	mov eax,gs:[si]
	mov dx,ax
	shr eax,16
	add dx,ax
	mov gs:[si],dx

if BIGPIX
	mov dl,dh
	mov [es:bx],dx
	inc bh
	mov [es:bx],dx
	dec bh
else
	mov [es:bx],dh
endif
	add si,4

	dec cx
	jnz next_pixie

	pop ds
	popa

	retf
_gomorph endp

end