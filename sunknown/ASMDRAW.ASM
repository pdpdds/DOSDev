; ASMDRAW.ASM

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

.model large

public _draw80, _sizeb, _size14
public _idy4, _leftdi, _inx, _iny, _idx, _idy, _idx4
public _spos,_sw_ptr, _hswap, _vswap, sw_whole
public _FA, _FB, _FAC, _FBC, _cxmin, _cxmax, _cymin, _cymax, _Vx, _Vy
public _Nx, _Ny, _CAMx, _CAMy

.code
.386

swapt 	dw (256+8) dup (?)
label _sw_ptr	dword
sw_corr	dw offset swapt, seg swapt

_FA	dd ?
_FB	dd ?
_FAC	dd ?
_FBC	dd ?
_cxmin	dd ?
_cxmax	dd ?
_cymin	dd ?
_cymax	dd ?
_Vx	dd ?
_Vy	dd ?
_Nx	dd ?
_Ny	dd ?
_CAMx	dw ?
_CAMy	dw ?


swapoff	dw ?
sw_left	dw ?
sw_whole	db ?
keepcx	dw ?
keepdx	dw ?
keepdi	dw ?
tempbp	dw ?
tempsi	dw ?
_idx4	dw ?
_idy4  	dw ?
_spos	dw ?
_leftdi	dw ?
_idx	dw ?
_idy	dw ?
_inx	dw ?
_iny	dw ?
_size14 dw ?
asmx	db ?
asmy	db ?
_sizeb	db ?
keepcb	db ?

; ***************************************************************************
;
; TEGN80
;
; ***************************************************************************

_draw80 proc far

	mov tempbp,bx
	mov keepcx,cx
	mov keepdx,dx

rept	39
	mov bl,ch
	mov bh,dh
	mov ah,[bx]
	mov es:[di],ah
	dec di
	sub cx,si
	sub dx,tempbp
endm
	mov bl,ch
	mov bh,dh
	mov ah,[bx]
	mov es:[di],ah
	add di,40
	mov cx,keepcx
	mov dx,keepdx
	add cx,si
	add dx,tempbp

rept	39
	mov bl,ch
	mov bh,dh
	mov ah,[bx]
	;and ah,al
	;or ah,al
	mov es:[di],ah
	inc di
	add cx,si
	add dx,tempbp
endm
	mov bl,ch
	mov bh,dh
	mov ah,[bx]
	;and ah,07h
	;or ah,al
	mov es:[di],ah
	add di,40

	retf

_draw80 endp

; ***************************************************************************
;
; HSWAP
;
; ***************************************************************************

_hswap	proc far

	pusha

	mov ah,cl
	and ax,0700h
	mov swapoff,ax
	and cx,not 7
	mov ax,cx
	shl ax,5
	mov cx,dx
	shr dx,3
	add ax,dx
	mov si,ax
	mov tempsi,ax
	mov al,[si]
	xor ah,ah
	add ax,ax
	mov si,ax
	add si,sw_corr
	mov si,[cs:si]
	and cx,7
	add si,cx
	add si,swapoff
	mov sw_whole,32
	mov keepcx,cx
	jz none_1
	dec sw_whole
	neg cx
	add cx,8
next_1:
	mov al,[si]
	inc si
	mov [es:bx],al
	inc bl
	dec cl
	jnz next_1

next_whole:
	inc tempsi
none_1:
	mov si,tempsi
	mov al,[si]
	xor ah,ah
	add ax,ax
	add ax,sw_corr
	mov si,ax
	mov si,[cs:si]
	add si,swapoff
rept 8
	mov al,[si]
	inc si
	mov [es:bx],al
	inc bl
endm
	dec sw_whole
	jnz next_whole

	inc tempsi
	mov si,tempsi
	mov al,[si]
	xor ah,ah
	add ax,ax
	mov si,ax
	add si,sw_corr
	mov si,[cs:si]
	add si,swapoff
	mov cx,keepcx
	or cl,cl
	jz none_2
next2:
	mov al,[si]
	inc si
	mov [es:bx],al
	inc bl
	dec cl
	jnz next2
none_2:

	popa
	retf
_hswap	endp

; ***************************************************************************
;
; VSWAP
;
; ***************************************************************************

_vswap	proc far

	pusha

	mov al,dl
	and ax,0007h
	mov swapoff,ax

	mov ax,cx
	and ax,not 7
	shl ax,5
	shr dx,3
	add ax,dx
	mov si,ax
	mov tempsi,ax
	mov al,[si]
	xor ah,ah
	add ax,ax
	mov si,ax
	add si,sw_corr
	mov si,[cs:si]
	and cl,7
	mov ch,cl
	xor cl,cl
	add si,cx

	add si,swapoff
	mov sw_whole,32
	and cx,0700h
	mov keepcx,cx
	jz none_1b
	dec sw_whole
	neg ch
	add ch,8
next_1b:
	mov al,[si]
	cmp al,15
	jbe ok
	nop
ok:
	add si,256
	mov [es:bx],al
	inc bh
	dec ch
	jnz next_1b

next_wholeb:
	add tempsi,256
none_1b:
	mov si,tempsi
	mov al,[si]
	xor ah,ah
	add ax,ax
	add ax,sw_corr
	mov si,ax
	mov si,[cs:si]
	add si,swapoff
rept 8
	mov al,[si]
	add si,256
	mov [es:bx],al
	inc bh
endm
	dec sw_whole
	jnz next_wholeb

	mov si,tempsi
	add si,256
	mov al,[si]
	xor ah,ah
	add ax,ax
	add ax,sw_corr
	mov si,ax
	mov si,[cs:si]
	add si,swapoff
	mov cx,keepcx
	or ch,ch
	jz none_2b
next2b:
	mov al,[si]
	add si,256
	mov [es:bx],al
	inc bh
	dec ch
	jnz next2b
none_2b:
	popa
	retf
_vswap  endp

end

