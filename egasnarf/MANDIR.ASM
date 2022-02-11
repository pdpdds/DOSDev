page 60,132
title TFMISC --- MISCELLANEOUS ASSEMBLY ROUTINES FOR TIGER FOX
;
debug	equ	0
;
_text	segment	byte public 'code'
_text	ends
_data	segment	word public 'data'
_data	ends
const	segment	word public 'const'
const	ends
_bss	segment	word public 'bss'
_bss	ends
;
		extrn	_flabel:near
;
		public	_set_bug_loc
		public	_bug_dir
		public	_man_dirs
		public	_man_dist
;
dgroup	group	const, _bss, _data
	assume	cs:_text, ds:dgroup, ss:dgroup, es:dgroup
;
_data	segment	word public 'data'
;
wmaze		db	820 dup (?)
stack1		db	400 dup (0)
stack2		db	400 dup (0)
mandist		dw	?
mandir		db	?
stk2ptr		dw	?
stk1ptr		dw	?
buglocs		dw	20 dup (-2)
bugdirs		dw	20 dup (?)
;
_data	ends
;
;
;
_text	segment
;
;
; man_dirs(&field, man-field-index);
;
_man_dirs:
	push	bp
	mov	bp,sp
	push	si
	push	di
	call	mansub2
	pop	di
	pop	si
	pop	bp
	ret
;
; man_dist(&field, loc x, loc y, man x, man y);
;
_man_dist:
	push	bp
	mov	bp,sp
	push	si
	push	di
	call	mansub1
	mov	ax,mandist
	pop	di
	pop	si
	pop	bp
	ret
;
; bug_dir(n);
;
_bug_dir:
	push	bp
	mov	bp,sp
	mov	bx,[bp+4]
	shl	bx,1
	mov	ax,[bx+bugdirs]
	pop	bp
	ret
;
; set_bug_loc(n, bug-field-index);
;
_set_bug_loc:
	push	bp
	mov	bp,sp
	lea	ax,wmaze
	mov	ax,[bp+6]
	mov	bx,[bp+4]
	shl	bx,1
	mov	[bx+buglocs],ax
	pop	bp
	ret
;
;
mansub1:
	cld
	mov	mandist,1
	mov	mandir,-1
	mov	si,[bp+4]		; get maze address
	lea	di,wmaze
	mov	cx,410
	rep	movsw

	mov	ax,[bp+12]		; get man y
	mov	bl,41
	mul	bl
	add	ax,[bp+10]
	lea	si,wmaze
	add	si,ax
	mov	byte ptr [si],30h	; place man flag

	mov	ax,[bp+8]		; get ptr to bug location
	mov	bl,41
	mul	bl
	add	ax,[bp+6]
	lea	si,wmaze
	add	si,ax			; si = search start point

	mov	byte ptr [si],1		; put brick at bug's start point

	lea	ax,stack1		; init stacks
	mov	stk1ptr,ax
	lea	ax,stack2
	mov	stk2ptr,ax

	mov	dh,0			; seed stack1 with first 4 dirs
	mov	di,si
	inc	di
	call	chkmovi

	mov	dh,1
	mov	di,si
	sub	di,41
	call	chkmovi

	mov	dh,2
	mov	di,si
	dec	di
	call	chkmovi

	mov	dh,3
	mov	di,si
	add	di,41
	call	chkmovi
whilp10:
	lea	si,stack1		; ripple search out from there
	cmp	si,stk1ptr
	mov	si,stk1ptr
	jz	whilp20
	dec	si
	mov	dh,[si]
	dec	si
	dec	si
	mov	stk1ptr,si
	mov	di,[si]

	push	di
	inc	di
	call	chkmov
	pop	di

	push	di
	sub	di,41
	call	chkmov
	pop	di

	push	di
	dec	di
	call	chkmov
	pop	di

	add	di,41
	call	chkmov
	cmp	mandir,-1
	jz	whilp10
exitmandir:
	ret
whilp20:
	inc	mandist
	lea	si,stack2
	lea	di,stack1
	mov	cx,stk2ptr
	sub	cx,si
	jz	exitmandir
	mov	stk2ptr,si
	rep	movsb
	mov	stk1ptr,di
	jmp	whilp10
;
; di=maze ptr    dh=original direction
;
chkmov:
	cmp	byte ptr [di],0		; empty?
	jz	cmiyes			; jif yes
	cmp	byte ptr [di],30h	; man?
	jnz	cmino			; jif no
	mov	mandir,dh
cmiyes:
	mov	byte ptr [di],1		; mark as been there
	mov	ax,di
	mov	di,stk2ptr
	stosw
	mov	al,dh
	stosb
	mov	stk2ptr,di
cmino:
	ret
;
;
;
chkmovi:
	call	chkmov
	lea	di,stack2
	cmp	di,stk2ptr
	jz	cmino
	mov	stk2ptr,di
	mov	ax,[di]
	mov	bx,stk1ptr
	mov	[bx],ax
	inc	bx
	inc	bx
	mov	al,[di+2]
	mov	[bx],al
	inc	bx
	mov	stk1ptr,bx
	ret
;
;
;
;
;
mansub2:
	cld
	mov	si,[bp+4]		; get maze address
	lea	di,wmaze
	mov	cx,410
	rep	movsw

	lea	di,bugdirs		; clear bugdirs table
	mov	cx,20
	mov	ax,-1
	rep	stosw

	lea	si,buglocs
	lea	bx,wmaze
	mov	al,50h			; bug flag
ms21:
	mov	di,[si]			; get ptr to bug location
	inc	si
	inc	si
	cmp	di,-2			; end of list?
	jz	ms22			; jif yes
	or	di,di			; bug exist?
	jz	ms26
	add	di,bx
	mov	[di],al			; set bug location in maze
ms26:
	inc	al			; bump flag to next bug number
	jmp	ms21
ms22:
	lea	si,wmaze
	add	si,[bp+6]			; si = search start point
	mov	byte ptr [si],1		; put brick at man's start point

	lea	ax,stack1		; init stacks
	mov	stk1ptr,ax
	lea	ax,stack2
	mov	stk2ptr,ax

	mov	dx,2			; seed stack1 with first 4 dirs
	mov	di,si
	inc	di
	call	chkmovi2

	mov	dx,3
	mov	di,si
	sub	di,41
	call	chkmovi2

	mov	dx,0
	mov	di,si
	dec	di
	call	chkmovi2

	mov	dx,1
	mov	di,si
	add	di,41
	call	chkmovi2
ms23:
	lea	si,stack1		; ripple search out from there
	cmp	si,stk1ptr
	mov	si,stk1ptr
	jnz	ms24

	lea	si,stack2
	lea	di,stack1
	mov	cx,stk2ptr
	sub	cx,si
	jz	ms25
	mov	stk2ptr,si
	rep	movsb
	mov	stk1ptr,di
	jmp	ms23
ms24:
	dec	si
	dec	si
	mov	stk1ptr,si
	mov	di,[si]

	push	di
	inc	di
	mov	dx,2
	call	chkmov2
	pop	di

	push	di
	sub	di,41
	mov	dx,3
	call	chkmov2
	pop	di

	push	di
	dec	di
	mov	dx,0
	call	chkmov2
	pop	di

	add	di,41
	mov	dx,1
	call	chkmov2
	jmp	ms23
ms25:
	ret
;
; di=maze ptr    dh=original direction
;
chkmov2:
	cmp	byte ptr [di],0		; empty?
	jz	cmiyes2			; jif yes
	mov	al,[di]			; get byte
	and	al,0f0h
	cmp	al,50h			; bug?
	jnz	cmino2			; jif no
	mov	bl,[di]
	and	bl,0fh
	xor	bh,bh
	shl	bx,1
	mov	[bx+bugdirs],dx
cmiyes2:
	mov	byte ptr [di],1		; mark as been there
	mov	ax,di
	mov	di,stk2ptr
	stosw
	mov	stk2ptr,di
cmino2:
	ret
;
;
;
chkmovi2:
	call	chkmov2
	lea	di,stack2
	cmp	di,stk2ptr
	jz	cmino2
	mov	stk2ptr,di
	mov	ax,[di]
	mov	di,stk1ptr
	stosw
	mov	stk1ptr,di
	ret
;
if debug
;
;
showhere:
	push	bp
	push	si
	push	di
	mov	bp,sp
	lea	di,wmaze
	cld
	add	al,'0'
	stosb
	xor	ax,ax
	stosb
	push	ax
	push	ax
	mov	ax,15
	push	ax
	lea	di,wmaze
	push	di
	mov	ax,8
	push	ax
	mov	ax,30
	push	ax
	call	_flabel
	mov	sp,bp
	pop	di
	pop	si
	pop	bp
	ret
;
;
show_hex:
	push	ax
	push	bp
	push	si
	push	di
	mov	bp,sp
	lea	di,wmaze
	cld
	push	ax
	mov	al,ah
	call	show_al
	pop	ax
	call	show_al
	xor	ax,ax
	stosb
	push	ax
	push	ax
	mov	ax,15
	push	ax
	lea	di,wmaze
	push	di
	mov	ax,8
	push	ax
	mov	ax,50
	push	ax
	call	_flabel
	mov	sp,bp
	pop	di
	pop	si
	pop	bp
	pop	ax
	ret

show_al:
	shl	ax,1
	shl	ax,1
	shl	ax,1
	shl	ax,1
	shr	al,1
	shr	al,1
	shr	al,1
	shr	al,1
	and	ax,0f0fh
	add	ax,3030h
	cmp	al,'9'
	jbe	alok
	add	al,7
alok:
	cmp	ah,'9'
	jbe	ahok
	add	ah,7
ahok:
	xchg	al,ah
	stosw
	ret
;
endif
;
_text	ends
;
	end
