	ifndef	??version
?debug	macro
	endm
	endif
	?debug	S "lodimage.c"
LODIMAGE_TEXT	segment	byte public 'CODE'
DGROUP	group	_DATA,_BSS
	assume	cs:LODIMAGE_TEXT,ds:DGROUP
LODIMAGE_TEXT	ends
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
	?debug	C E965A1271A0A6C6F64696D6167652E63
	?debug	C E900101D111A433A5C4C414E475C54435C494E434C5544455C7374+
	?debug	C 64696F2E68
	?debug	C E900101D111B433A5C4C414E475C54435C494E434C5544455C7374+
	?debug	C 646172672E68
	?debug	C E94A7EDD180768616E696F2E68
_BSS	ends
LODIMAGE_TEXT	segment	byte public 'CODE'
;	?debug	L 16
_load_image_han	proc	far
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
	push	di
;	?debug	L 22
	push	ds
	mov	ax,offset DGROUP:s@
	push	ax
	push	word ptr [bp+12]
	push	word ptr [bp+10]
	call	far ptr _fopen
	add	sp,8
	mov	word ptr [bp-6],dx
	mov	word ptr [bp-8],ax
	or	dx,ax
	jne	@2
	jmp	@1
@2:
;	?debug	L 24
		cmp		_in_vga_system, 1
;	?debug	L 25
		je		@3
;	?debug	L 27
		mov		es, _video_seg   	
;	?debug	L 28
		mov		di, [bp+6]
;	?debug	L 29
		mov		ax, _abs_maxx_han
;	?debug	L 30
		inc		ax
;	?debug	L 31
		shl		ax, 1
;	?debug	L 32
		shl		ax, 1
;	?debug	L 33
		mov		cx, [bp+8]
;	?debug	L 34
  	mul		cx
;	?debug	L 35
		add		di, ax
;	?debug	L 37
	les	bx,dword ptr [bp-8]
	dec	word ptr es:[bx]
	jl	@19
	les	bx,dword ptr [bp-8]
	inc	word ptr es:[bx+12]
	les	bx,dword ptr es:[bx+12]
	dec	bx
	mov	al,byte ptr es:[bx]
	mov	ah,0
	jmp	short @18
@19:
	push	word ptr [bp-6]
	push	word ptr [bp-8]
	call	far ptr __fgetc
	pop	cx
	pop	cx
@18:
;	?debug	L 38
		mov		[bp-4], ax
;	?debug	L 39
	les	bx,dword ptr [bp-8]
	dec	word ptr es:[bx]
	jl	@21
	les	bx,dword ptr [bp-8]
	inc	word ptr es:[bx+12]
	les	bx,dword ptr es:[bx+12]
	dec	bx
	mov	al,byte ptr es:[bx]
	mov	ah,0
	jmp	short @20
@21:
	push	word ptr [bp-6]
	push	word ptr [bp-8]
	call	far ptr __fgetc
	pop	cx
	pop	cx
@20:
;	?debug	L 40
		mov		[bp-2], ax              
;	?debug	L 42
		mov		ax, _abs_maxx_han        
;	?debug	L 43
		inc		ax
;	?debug	L 44
		mov		cx, [bp-4]		
;	?debug	L 45
		mov		dx, [bp-2]      	
;	?debug	L 46
		mov		bx, 02000H		
;	?debug	L 47
		sub		bx, cx
@4:
;	?debug	L 51
		push		di
;	?debug	L 53
		push		cx
@5:
;	?debug	L 57
		cmp		cx, 0
;	?debug	L 58
		jbe		@6
;	?debug	L 59
		dec		cx
;	?debug	L 60
	les	bx,dword ptr [bp-8]
	dec	word ptr es:[bx]
	jl	@23
	les	bx,dword ptr [bp-8]
	inc	word ptr es:[bx+12]
	les	bx,dword ptr es:[bx+12]
	dec	bx
	mov	al,byte ptr es:[bx]
	mov	ah,0
	jmp	short @22
@23:
	push	word ptr [bp-6]
	push	word ptr [bp-8]
	call	far ptr __fgetc
	pop	cx
	pop	cx
@22:
;	?debug	L 61
  	movsb	
;	?debug	L 62
		jmp		@5
@6:
;	?debug	L 66
		pop		cx
;	?debug	L 67
		add		di, bx
;	?debug	L 69
		push		cx
@7:
;	?debug	L 73
		cmp		cx, 0
;	?debug	L 74
		jbe		@8
;	?debug	L 75
		dec		cx
;	?debug	L 76
	les	bx,dword ptr [bp-8]
	dec	word ptr es:[bx]
	jl	@25
	les	bx,dword ptr [bp-8]
	inc	word ptr es:[bx+12]
	les	bx,dword ptr es:[bx+12]
	dec	bx
	mov	al,byte ptr es:[bx]
	mov	ah,0
	jmp	short @24
@25:
	push	word ptr [bp-6]
	push	word ptr [bp-8]
	call	far ptr __fgetc
	pop	cx
	pop	cx
@24:
;	?debug	L 77
  	movsb	
;	?debug	L 78
		jmp		@7
@8:
;	?debug	L 82
		pop		cx
;	?debug	L 83
		add		di, bx
;	?debug	L 85
		push		cx
@9:
;	?debug	L 89
		cmp		cx, 0
;	?debug	L 90
		jbe		@10
;	?debug	L 91
		dec		cx
;	?debug	L 92
	les	bx,dword ptr [bp-8]
	dec	word ptr es:[bx]
	jl	@27
	les	bx,dword ptr [bp-8]
	inc	word ptr es:[bx+12]
	les	bx,dword ptr es:[bx+12]
	dec	bx
	mov	al,byte ptr es:[bx]
	mov	ah,0
	jmp	short @26
@27:
	push	word ptr [bp-6]
	push	word ptr [bp-8]
	call	far ptr __fgetc
	pop	cx
	pop	cx
@26:
;	?debug	L 93
  	movsb	
;	?debug	L 94
		jmp		@9
@10:
;	?debug	L 98
		pop		cx
;	?debug	L 99
		add		di, bx
;	?debug	L 101
		push		cx
@11:
;	?debug	L 105
		cmp		cx, 0
;	?debug	L 106
		jbe		@12
;	?debug	L 107
		dec		cx
;	?debug	L 108
	les	bx,dword ptr [bp-8]
	dec	word ptr es:[bx]
	jl	@29
	les	bx,dword ptr [bp-8]
	inc	word ptr es:[bx+12]
	les	bx,dword ptr es:[bx+12]
	dec	bx
	mov	al,byte ptr es:[bx]
	mov	ah,0
	jmp	short @28
@29:
	push	word ptr [bp-6]
	push	word ptr [bp-8]
	call	far ptr __fgetc
	pop	cx
	pop	cx
@28:
;	?debug	L 109
  	movsb	
;	?debug	L 110
		jmp		@11
@12:
;	?debug	L 114
		pop		cx
;	?debug	L 116
		pop		di
;	?debug	L 117
		add		di, ax
;	?debug	L 118
		dec		dx
;	?debug	L 119
		jg		@4
;	?debug	L 120
		jmp		@13
@3:
;	?debug	L 124
 		mov	 	es, _video_seg		
;	?debug	L 125
		mov		di, [bp+6]
;	?debug	L 126
  	mov	     ax, 80 
;	?debug	L 127
  	mov	     bx, [bp+8]
;	?debug	L 128
  	mul	     bx
;	?debug	L 129
		shl		ax, 1
;	?debug	L 130
		shl		ax, 1
;	?debug	L 131
		shl		ax, 1
;	?debug	L 132
		shl		ax, 1
;	?debug	L 133
  	add	     di, ax
;	?debug	L 135
	les	bx,dword ptr [bp-8]
	dec	word ptr es:[bx]
	jl	@31
	les	bx,dword ptr [bp-8]
	inc	word ptr es:[bx+12]
	les	bx,dword ptr es:[bx+12]
	dec	bx
	mov	al,byte ptr es:[bx]
	mov	ah,0
	jmp	short @30
@31:
	push	word ptr [bp-6]
	push	word ptr [bp-8]
	call	far ptr __fgetc
	pop	cx
	pop	cx
@30:
;	?debug	L 136
		mov		[bp-4], ax
;	?debug	L 137
	les	bx,dword ptr [bp-8]
	dec	word ptr es:[bx]
	jl	@33
	les	bx,dword ptr [bp-8]
	inc	word ptr es:[bx+12]
	les	bx,dword ptr es:[bx+12]
	dec	bx
	mov	al,byte ptr es:[bx]
	mov	ah,0
	jmp	short @32
@33:
	push	word ptr [bp-6]
	push	word ptr [bp-8]
	call	far ptr __fgetc
	pop	cx
	pop	cx
@32:
;	?debug	L 138
		mov		[bp-2], ax              
;	?debug	L 140
  	mov	  	bx, [bp-2]
;	?debug	L 141
 		mov	 	dx, 003c4H		
;	?debug	L 142
		mov		al, 002H		
;	?debug	L 143
		mov		cx, [bp-4]
@14:
;	?debug	L 147
		mov		ah, 008H		
@15:
;	?debug	L 151
		out		dx, ax
;	?debug	L 152
		push		ax
;	?debug	L 153
		push		cx
@16:
;	?debug	L 157
		cmp		cx, 0
;	?debug	L 158
		jbe		@17
;	?debug	L 159
		dec		cx
;	?debug	L 160
	les	bx,dword ptr [bp-8]
	dec	word ptr es:[bx]
	jl	@35
	les	bx,dword ptr [bp-8]
	inc	word ptr es:[bx+12]
	les	bx,dword ptr es:[bx+12]
	dec	bx
	mov	al,byte ptr es:[bx]
	mov	ah,0
	jmp	short @34
@35:
	push	word ptr [bp-6]
	push	word ptr [bp-8]
	call	far ptr __fgetc
	pop	cx
	pop	cx
@34:
;	?debug	L 161
  	movsb	
;	?debug	L 162
		jmp		@16
@17:
;	?debug	L 166
		pop		cx
;	?debug	L 167
		pop		ax
;	?debug	L 168
		sub		di, [bp-4]
;	?debug	L 169
		shr		ah, 1
;	?debug	L 170
		jnc		@15
;	?debug	L 172
		add		di, 80 
;	?debug	L 173
		dec		bx
;	?debug	L 174
		jg		@14
;	?debug	L 176
		mov		ax, 00f02H
;	?debug	L 177
		out		dx, ax
@13:
;	?debug	L 181
	push	word ptr [bp-6]
	push	word ptr [bp-8]
	call	far ptr _fclose
	pop	cx
	pop	cx
@1:
;	?debug	L 182
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_load_image_han	endp
LODIMAGE_TEXT	ends
	?debug	C E9
_DATA	segment word public 'DATA'
s@	label	byte
	db	114
	db	116
	db	0
_DATA	ends
	extrn	_in_vga_system:byte
	extrn	_abs_maxx_han:word
	extrn	_video_seg:word
	extrn	_fopen:far
	extrn	_fclose:far
	extrn	__fgetc:far
LODIMAGE_TEXT	segment	byte public 'CODE'
LODIMAGE_TEXT	ends
	public	_load_image_han
	end
