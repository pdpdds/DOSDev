; ASMIFS.ASM

; Thomas H.

.model large

public _ifsx ,_ifsy,_ifsdx,_ifsdy,_ifslen,_ifslev,_asmifs
public _drawpage32, _movebak32, _skheight, _writepage32, _blanker
public _onestrip

.code

EVEN

_ifsx	dw ?
_ifsy	dw ?
_ifsdx	dw ?
_ifsdy	dw ?
_ifslen	dw ?
_skheight dw 7*257
_writepage32 dd ?
calls	dw offset line1,offset line1, offset line1, offset line1
	dw offset line1,offset line1, offset line1, offset line0
_ifslev	db ?

.386

; ***** MOVEBAK32 *****

_movebak32 proc far

	mov dl,200
ned32:	mov cx,64
	rep movsd
	add si,64
	dec dl
	jnz ned32
	retf

_movebak32 endp

; ***** TEGNSKJERM32 *****

_drawpage32 proc far

	mov dl,200
ned322:	mov cx,64
	rep movsd
	add di,64
	dec dl
	jnz ned322
	retf

_drawpage32 endp

; ***** ASMIFS *****

_asmifs	proc far
	mov ax,_ifslen
	or ah,ah
	jz glemdet
	mov cx,_ifsx
	mov dx,_ifsy
	mov bl,_ifslev
	mov al,bl
	add al,128
	xor bh,bh
	add bx,bx
	add bx,offset calls
	jmp [cs:bx]
glemdet:
	retf
_asmifs	endp

; ***** LINE0 *****

line0	proc near
nextline0:
	mov bl,ch
	mov bh,dh
	mov byte ptr[bx],al
	add bx,[cs:_skheight]
	or byte ptr[bx],64
	add cx,_ifsdx
	add dx,_ifsdy
	dec ah
	jne nextline0
	mov _ifsx,cx
	mov _ifsy,dx
	retf
line0	endp

; ***** LINE1 *****

line1	proc near
	mov bl,ah
	mov ah,al
	mov bp,ax
	mov ah,bl
nextline1:
	mov bl,ch
	mov bh,dh
	mov [bx],bp
	mov [bx+256],bp
	add bx,[cs:_skheight]
	or word ptr[bx],64+256*64
	or word ptr[bx+256],64+256*64
	add cx,_ifsdx
	add dx,_ifsdy
	dec ah
	jne nextline1
	mov _ifsx,cx
	mov _ifsy,dx
	retf
line1	endp

; ***** BLANKER *****

_blanker proc far
	les di,_writepage32
	xor eax,eax
	mov cx,dx
	shl cx,6
	rep stosd
	retf

_blanker endp

; ***** ONESTRIP *****

_onestrip proc far

	mov cx,64
	rep movsd
	retf
_onestrip endp

end
