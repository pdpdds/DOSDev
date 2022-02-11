; ASMZOOM.ASM

; Thomas H.

VOXLINJER	= 100
VOXBREDDE	= 192

AC_INDEX        = 03c0h
MISC_OUTPUT     = 03c2h
SC_INDEX        = 03c4h
GC_INDEX	= 03ceh
CRTC_INDEX      = 03d4h
INPUT_STATUS_0  = 03dah
MAP_MASK	= 02h
READ_MAP        = 04h
BIT_MASK	= 08h

public _asmzoom, _farfrac1, _farfrac2, _fartwist, _fartune
public _shx,_shy,_sdx,_sdy, _topx, _topy, _lines, _kdyhalf
public _khx,_khy,_kdx,_kdy, _voxeltwist, _farbp, _drawvoxel, _kdxhalf
public _farfrom, _farto, _farmove, _farmove320, _movef32

.model large

.code

skr	db 14736 dup (?)

EVEN

_fartwist label dword
	dw offset skr, seg skr
_fartune dw ?
_farfrac1 dd ?
_farfrac2 dd ?
_farfrom	dd ?
_farto	dd ?

_shx	dw 0
_shy	dw 0
_sdx	dw 0
_sdy	dw 256
_khx	dw 0
_khy	dw 0
_kdx	dw 0
_kdy	dw 128
_kdxhalf dw ?
_kdyhalf dw ?

_topx	dw ?
_topy	dw ?
_topx2	dw ?
_topy2	dw ?
tempax	dw ?
tempbx	dw ?
tempcx	dw ?
tempdx	dw ?
tempdi	dw ?
topdi	dw ?
tempsi	dw ?
_farbp	dw ?
_lines	db ?


.386

; ***** CENTER *****

center	proc near

	xor di,di
	add si,offset skr
	add si,_fartune
	add si,_fartune
	mov _lines,80
xxnext_line:
	mov ax,_topx
	mov _topx2,ax
	mov ax,_topy
	mov _topy2,ax
	mov topdi,di
	mov tempsi,si
	mov al,cs:[si]
	cmp al,120
	je xxnull_lengde
	mov bl,al
	or al,al
	jz xxikkegange
	xor ah,ah
	mov tempax,ax
	mov cx,_sdy
	mul cx
	add _topy2,ax
	mov ax,tempax
	mov cx,_sdx
	mul cx
	add _topx2,ax
	mov ax,tempax
	shl ax,4
	add di,ax
	add ax,ax
	add ax,ax
	add di,ax
xxikkegange:
	mov ah,cs:[si+1]
	sub ah,bl
	jz xxnull_lengde

	mov dx,_topx2
	mov cx,_topy2

	mov si,_sdy
	mov bp,_sdx

xxnext_down:
	mov bh,ch
	mov bl,dh
	mov al,[bx]
	mov [es:di],al
	add cx,si
	add dx,bp
	add di,80
	dec ah
	jne xxnext_down

xxnull_lengde:
	mov si,_sdy
	mov bp,_sdx

	add _topx,si
	add _topx,si
	sub _topy,bp
	sub _topy,bp

	mov si,tempsi
	add si,184

	mov di,topdi
	inc di

	dec _lines
	jnz xxnext_line

	ret
center	endp

; ***** TOP *****

top	proc near

	xor di,di
	add si,offset skr
	add si,_fartune
	add si,_fartune
	mov _lines,80
yynext_linje:
	mov ax,_topx
	mov _topx2,ax
	mov ax,_topy
	mov _topy2,ax
	mov topdi,di
	mov tempsi,si
	mov ah,cs:[si]
	or ah,ah
	jz yynull_lengde

	mov dx,_topx2
	mov cx,_topy2

	mov si,_kdy
	mov bp,_kdx

yynext_down:
	mov bh,ch
	mov bl,dh
	mov al,[bx]
	mov [es:di],al
	add cx,si
	add dx,bp
	add di,80
	dec ah
	jne yynext_down

yynull_lengde:
	mov si,_kdy
	mov bp,_kdx

	add _topx,si
	add _topx,si
	sub _topy,bp
	sub _topy,bp

	mov si,tempsi
	add si,184

	mov di,topdi
	inc di

	dec _lines
	jnz yynext_linje

	ret
top	endp

; ***** BUNN *****

bunn	proc near

	xor di,di
	add si,offset skr
	add si,_fartune
	add si,_fartune
	mov _lines,80
zznext_linje:
	mov ax,_topx
	mov _topx2,ax
	mov ax,_topy
	mov _topy2,ax
	mov topdi,di
	mov tempsi,si
	mov al,cs:[si+1]

	cmp al,120
	je zznull_lengde
	mov bl,al
	xor ah,ah
	mov tempax,ax
	mov cx,_kdy
	mul cx
	add _topy2,ax
	mov ax,tempax
	mov cx,_kdx
	mul cx
	add _topx2,ax
	mov ax,tempax
	shl ax,4
	add di,ax
	add ax,ax
	add ax,ax
	add di,ax

	mov ah,120
	sub ah,bl
	jz zznull_lengde

	mov dx,_topx2
	mov cx,_topy2

	mov si,_kdy
	mov bp,_kdx

zznext_down:
	mov bh,ch
	mov bl,dh
	mov al,[bx]
	mov [es:di],al
	add cx,si
	add dx,bp
	add di,80
	dec ah
	jne zznext_down

zznull_lengde:
	mov si,_kdy
	mov bp,_kdx

	add _topx,si
	add _topx,si
	sub _topy,bp
	sub _topy,bp

	mov si,tempsi
	add si,184

	mov di,topdi
	inc di

	dec _lines
	jnz zznext_linje

	ret
bunn	endp

; ***** WHOLE *****

whole	proc near

	xor di,di
	mov _lines,80
qqnext_linje:
	mov ax,_topx
	mov _topx2,ax
	mov ax,_topy
	mov _topy2,ax
	mov topdi,di
	mov ax,120*256

	mov dx,_topx2
	mov cx,_topy2

	mov si,_kdyhalf
	mov bp,_kdx

qqnext_down:
	mov bh,ch
	mov bl,dh
	mov al,[bx]
	mov [es:di],al
	add cx,si
	add dx,bp
	add di,80
	dec ah
	jne qqnext_down

qqnull_lengde:
	mov si,_kdy
	mov bp,_kdxhalf

	add _topx,si
	add _topx,si
	sub _topy,bp
	sub _topy,bp

	mov di,topdi
	inc di

	dec _lines
	jnz qqnext_linje

	ret
whole	endp


; ***** ASMZOOM *****

_asmzoom proc far

	mov ax,_shx
	mov _topx,ax
	mov ax,_shy
	mov _topy,ax

	lds si,_farfrac1
	xor si,si
	mov ax,0300h+MAP_MASK
	mov dx,SC_INDEX
	out dx,ax

	call center

	mov ax,_shx
	add ax,_sdy
	mov _topx,ax
	mov ax,_shy
	sub ax,_sdx
	mov _topy,ax

	lds si,_farfrac1
	mov si,92
	mov ax,0c00h+MAP_MASK
	mov dx,SC_INDEX
	out dx,ax

	call center

	mov ax,_khx
	mov _topx,ax
	mov ax,_khy
	mov _topy,ax

	lds si,_farfrac2
	xor si,si
	mov ax,0300h+MAP_MASK
	mov dx,SC_INDEX
	out dx,ax

	call top

	mov ax,_khx
	add ax,_kdy
	mov _topx,ax
	mov ax,_khy
	sub ax,_kdx
	mov _topy,ax

	lds si,_farfrac2
	mov si,92
	mov ax,0c00h+MAP_MASK
	mov dx,SC_INDEX
	out dx,ax

	call top

	mov ax,_khx
	mov _topx,ax
	mov ax,_khy
	mov _topy,ax

	lds si,_farfrac2
	xor si,si
	mov ax,0300h+MAP_MASK
	mov dx,SC_INDEX
	out dx,ax

	call bunn

	mov ax,_khx
	add ax,_kdy
	mov _topx,ax
	mov ax,_khy
	sub ax,_kdx
	mov _topy,ax

	lds si,_farfrac2
	mov si,92
	mov ax,0c00h+MAP_MASK
	mov dx,SC_INDEX
	out dx,ax

	call bunn

	retf
_asmzoom endp

; ***** VOXELTWIST *****

_voxeltwist proc far

	mov ax,_khx
	mov _topx,ax
	mov ax,_khy
	mov _topy,ax

	lds si,_farfrac2
	xor si,si
	mov ax,0300h+MAP_MASK
	mov dx,SC_INDEX
	out dx,ax

	call whole

	mov ax,_khx
	add ax,_kdy
	mov _topx,ax
	mov ax,_khy
	sub ax,_kdx
	mov _topy,ax

	lds si,_farfrac2
	mov si,92
	mov ax,0c00h+MAP_MASK
	mov dx,SC_INDEX
	out dx,ax

	call whole

	retf

_voxeltwist endp

; ***** DRAWVOXEL *****

_drawvoxel proc far

	les si,_farfrac1
	lds si,_farfrac2

	mov _lines,VOXBREDDE
	xor di,di

next_vertical:
	xor ah,ah
	mov tempdi,di
	mov bp,_farbp
	mov dx,VOXLINJER+255*256

next_right:
	mov cx,[bp]
	inc bp
	inc bp
	add cx,[bp]
	mov [bp],cx
	inc bp
	inc bp

	mov bx,[bp]
	inc bp
	inc bp
	add bx,[bp]
	mov [bp],bx
	inc bp
	inc bp

	mov bl,ch
	mov bh,[bx]
	mov al,bh
	mov si,dx
	or al,[es:si]
	mov bl,dl
	add bh,VOXBREDDE
	mov cl,[es:bx]
	sub cl,ah
	jle skip_denne
	add ah,cl
	xor ch,ch
	rep stosb

skip_denne:
	dec ah
	dec dl
	jnz next_right

	mov ax,di
	mov al,192
	mov si,ax
	mov [es:si],di

	mov di,tempdi
	add di,256
	dec _lines
	jnz next_vertical

	retf

_drawvoxel endp

; ***** FARMOVE

_farmove proc far

	pusha
	push ds
	push es
	cld

	lds si,_farfrom
	les di,_farto

	mov cx,16000
	rep movsd

	pop es
	pop ds
	popa

	retf

_farmove endp

; ***** FARMOVE320

_farmove320 proc far

	pusha
	push ds
	push es
	cld

	lds si,_farfrom
	les di,_farto

	mov cx,80
	rep movsd


	pop es
	pop ds
	popa

	retf

_farmove320 endp

; ***** MOVEF32 *****

_movef32 proc far
	mov ax,ds
	mov es,ax

	mov di,si
	add si,62

	mov cx,(80*31)/4
	cld
	rep movsd

	retf
_movef32 endp

end

