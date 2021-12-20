	ifndef	??version
?debug	macro
	endm
publicdll macro	name
	public	name
	endm
$comm	macro	name,dist,size,count
	comm	dist name:BYTE:count*size
	endm
	else
$comm	macro	name,dist,size,count
	comm	dist name[size]:BYTE:count
	endm
	endif
	?debug	V 300h
	?debug	S "pgrker0.c"
	?debug	C E9B3998217097067726B6572302E63
	?debug	C E94019CA1817633A2F626333312F696E636C7564655C616C6C6F63+
	?debug	C 2E68
	?debug	C E94019CA1817633A2F626333312F696E636C7564655C5F64656673+
	?debug	C 2E68
	?debug	C E94019CA1817633A2F626333312F696E636C7564655C5F6E756C6C+
	?debug	C 2E68
	?debug	C E94019CA1817633A2F626333312F696E636C7564655C737464696F+
	?debug	C 2E68
	?debug	C E94019CA1818633A2F626333312F696E636C7564655C5F6E66696C+
	?debug	C 652E68
	?debug	C E94019CA1818633A2F626333312F696E636C7564655C7374646C69+
	?debug	C 622E68
	?debug	C E94019CA1815633A2F626333312F696E636C7564655C646F732E68
PGRKER0_TEXT	segment byte public 'CODE'
PGRKER0_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:PGRKER0_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
PGRKER0_TEXT	segment byte public 'CODE'
	assume	cs:PGRKER0_TEXT
_vga320Mode	proc	far
	push	bp
	mov	bp,sp
	push	si
	mov	 ax,013h
	int	 10h
	xor	si,si
	jmp	@1@170
@1@114:
	mov	ax,si
	mov	dx,320
	imul	dx
	xor	dx,dx
	add	ax,0
	adc	dx,-24576
	mov	bx,si
	mov	cl,2
	shl	bx,cl
	mov	word ptr DGROUP:__ADD[bx+2],dx
	mov	word ptr DGROUP:__ADD[bx],ax
	mov	bx,si
	shl	bx,1
	mov	ax,seg __SEG
	push	bx
	push	ax
	mov	ax,si
	mov	dx,20
	imul	dx
	add	ax,-24576
	pop	es
	pop	bx
	mov	word ptr es:__SEG[bx],ax
	mov	bx,si
	shl	bx,1
	mov	ax,seg __PSE
	push	bx
	push	ax
	mov	ax,si
	mov	dx,5
	imul	dx
	add	ax,-24576
	pop	es
	pop	bx
	mov	word ptr es:__PSE[bx],ax
	mov	bx,si
	shl	bx,1
	mov	ax,seg __PSE
	push	bx
	push	ax
	mov	ax,si
	mov	dx,5
	imul	dx
	add	ax,-23552
	pop	es
	pop	bx
	mov	word ptr es:__PSE[bx+400],ax
	mov	bx,si
	shl	bx,1
	mov	ax,seg __PSE
	push	bx
	push	ax
	mov	ax,si
	mov	dx,5
	imul	dx
	add	ax,-22528
	pop	es
	pop	bx
	mov	word ptr es:__PSE[bx+800],ax
	mov	bx,si
	shl	bx,1
	mov	ax,seg __PSE
	push	bx
	push	ax
	mov	ax,si
	mov	dx,5
	imul	dx
	add	ax,-21504
	pop	es
	pop	bx
	mov	word ptr es:__PSE[bx+1200],ax
	inc	si
@1@170:
	cmp	si,200
	jae	@@14
	jmp	@1@114
@@14:
	xor	ax,ax
	mov	word ptr DGROUP:__YL,ax
	mov	word ptr DGROUP:__XL,ax
	mov	word ptr DGROUP:__XR,319
	mov	word ptr DGROUP:__YR,199
	mov	word ptr DGROUP:__PSEG+2,seg __PSE
	mov	word ptr DGROUP:__PSEG,offset __PSE
	mov	ax,13000
	push	ax
	call	far ptr _malloc
	pop	cx
	mov	word ptr DGROUP:__EBF+2,dx
	mov	word ptr DGROUP:__EBF,ax
	xor	ax,ax
	jmp	short @1@226
@1@226:
	pop	si
	pop	bp
	ret	
_vga320Mode	endp
	assume	cs:PGRKER0_TEXT
_vgaTextMode	proc	far
	push	bp
	mov	bp,sp
	mov	 ax,03h
	int	 10h
	xor	ax,ax
	jmp	short @2@114
@2@114:
	pop	bp
	ret	
_vgaTextMode	endp
	assume	cs:PGRKER0_TEXT
_setStAdd	proc	far
	push	bp
	mov	bp,sp
 	mov	 dx,03dah
@3@58:
	in	  al,dx
	test	 al,08h
	je	short @3@58
	mov	 bx,st
	mov	 dx,03d4h
	mov	 al,0dh
	mov	 ah,bl
	out	 dx,ax
	mov	 dx,03d4h
	mov	 al,0ch
	mov	 ah,bh
	out	 dx,ax
	pop	bp
	ret	
_setStAdd	endp
	assume	cs:PGRKER0_TEXT
_actPage	proc	far
	push	bp
	mov	bp,sp
	mov	ax,word ptr [bp+6]
	mov	dx,400
	imul	dx
	add	ax,offset __PSE
	mov	word ptr DGROUP:__PSEG+2,seg __PSE
	mov	word ptr DGROUP:__PSEG,ax
	pop	bp
	ret	
_actPage	endp
	assume	cs:PGRKER0_TEXT
_planeMode	proc	far
	push	bp
	mov	bp,sp
	mov	dx,964
	mov	ax,1540
	out	dx,ax
	mov	dx,980
	mov	ax,20
	out	dx,ax
	mov	dx,980
	mov	ax,-7401
	out	dx,ax
	pop	bp
	ret	
_planeMode	endp
	assume	cs:PGRKER0_TEXT
_writeMode	proc	far
	push	bp
	mov	bp,sp
	sub	sp,2
	mov	dx,974
	mov	al,5
	out	dx,al
	mov	dx,975
	in	al,dx
	mov	byte ptr [bp-1],al
	mov	al,byte ptr [bp-1]
	and	al,252
	or	al,byte ptr [bp+6]
	mov	dx,975
	out	dx,al
	mov	sp,bp
	pop	bp
	ret	
_writeMode	endp
	assume	cs:PGRKER0_TEXT
_pBox2	proc	far
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	si,word ptr [bp+8]
	mov	di,word ptr [bp+14]
	push	di
	push	word ptr [bp+10]
	push	word ptr [bp+6]
	mov	ax,si
	shl	ax,1
	les	bx,dword ptr DGROUP:__PSEG
	add	bx,ax
	push	word ptr es:[bx]
	call	far ptr _pImgHFill
	add	sp,8
	push	di
	push	word ptr [bp+10]
	push	word ptr [bp+6]
	mov	ax,word ptr [bp+12]
	shl	ax,1
	les	bx,dword ptr DGROUP:__PSEG
	add	bx,ax
	push	word ptr es:[bx]
	call	far ptr _pImgHFill
	add	sp,8
	push	di
	mov	ax,word ptr [bp+12]
	sub	ax,si
	inc	ax
	push	ax
	mov	ax,80
	push	ax
	push	word ptr [bp+6]
	mov	ax,si
	shl	ax,1
	les	bx,dword ptr DGROUP:__PSEG
	add	bx,ax
	push	word ptr es:[bx]
	call	far ptr _pImgVFill
	add	sp,10
	push	di
	mov	ax,word ptr [bp+12]
	sub	ax,si
	inc	ax
	push	ax
	mov	ax,80
	push	ax
	push	word ptr [bp+10]
	mov	ax,si
	shl	ax,1
	les	bx,dword ptr DGROUP:__PSEG
	add	bx,ax
	push	word ptr es:[bx]
	call	far ptr _pImgVFill
	add	sp,10
	pop	di
	pop	si
	pop	bp
	ret	
_pBox2	endp
	assume	cs:PGRKER0_TEXT
_pBoxL	proc	far
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	si,word ptr [bp+6]
	mov	di,word ptr [bp+8]
	push	word ptr [bp+14]
	mov	ax,si
	add	ax,word ptr [bp+10]
	dec	ax
	push	ax
	push	si
	mov	ax,di
	shl	ax,1
	les	bx,dword ptr DGROUP:__PSEG
	add	bx,ax
	push	word ptr es:[bx]
	call	far ptr _pImgHFill
	add	sp,8
	push	word ptr [bp+14]
	mov	ax,si
	add	ax,word ptr [bp+10]
	dec	ax
	push	ax
	push	si
	mov	ax,word ptr [bp+12]
	add	ax,di
	dec	ax
	shl	ax,1
	les	bx,dword ptr DGROUP:__PSEG
	add	bx,ax
	push	word ptr es:[bx]
	call	far ptr _pImgHFill
	add	sp,8
	push	word ptr [bp+14]
	push	word ptr [bp+12]
	mov	ax,80
	push	ax
	push	si
	mov	ax,di
	shl	ax,1
	les	bx,dword ptr DGROUP:__PSEG
	add	bx,ax
	push	word ptr es:[bx]
	call	far ptr _pImgVFill
	add	sp,10
	push	word ptr [bp+14]
	push	word ptr [bp+12]
	mov	ax,80
	push	ax
	mov	ax,word ptr [bp+10]
	add	ax,si
	dec	ax
	push	ax
	mov	ax,di
	shl	ax,1
	les	bx,dword ptr DGROUP:__PSEG
	add	bx,ax
	push	word ptr es:[bx]
	call	far ptr _pImgVFill
	add	sp,10
	pop	di
	pop	si
	pop	bp
	ret	
_pBoxL	endp
	assume	cs:PGRKER0_TEXT
_pPutImgMode	proc	far
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	si,word ptr [bp+6]
	mov	di,word ptr [bp+8]
	mov	ax,word ptr [bp+18]
	cmp	ax,1
	je	short @9@170
	cmp	ax,2
	je	short @9@226
	cmp	ax,3
	je	short @9@254
	jmp	short @9@282
@9@170:
	push	word ptr [bp+16]
	push	word ptr [bp+14]
	push	word ptr [bp+12]
	push	word ptr [bp+10]
	push	di
	push	si
	call	far ptr _pCTrpRImg1
@9@198:
	add	sp,12
	jmp	short @9@310
@9@226:
	push	word ptr [bp+16]
	push	word ptr [bp+14]
	push	word ptr [bp+12]
	push	word ptr [bp+10]
	push	di
	push	si
	call	far ptr _pCTrpRImg2
	jmp	short @9@198
@9@254:
	push	word ptr [bp+16]
	push	word ptr [bp+14]
	push	word ptr [bp+12]
	push	word ptr [bp+10]
	push	di
	push	si
	call	far ptr _pCTrpRImg3
	jmp	short @9@198
@9@282:
	push	word ptr [bp+16]
	push	word ptr [bp+14]
	push	word ptr [bp+12]
	push	word ptr [bp+10]
	push	di
	push	si
	call	far ptr _pCTrpRImg0
	jmp	short @9@198
@9@310:
	pop	di
	pop	si
	pop	bp
	ret	
_pPutImgMode	endp
	assume	cs:PGRKER0_TEXT
_pCTrpRImg0	proc	far
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
	push	di
	mov	si,word ptr [bp+6]
	mov	di,word ptr [bp+8]
	cmp	si,word ptr DGROUP:__XL
	jle	short @10@86
	mov	ax,si
	jmp	short @10@114
@10@86:
	mov	ax,word ptr DGROUP:__XL
@10@114:
	mov	word ptr [bp-2],ax
	mov	ax,si
	add	ax,word ptr [bp+14]
	dec	ax
	cmp	ax,word ptr DGROUP:__XR
	jge	short @10@170
	mov	ax,si
	add	ax,word ptr [bp+14]
	dec	ax
	jmp	short @10@198
@10@170:
	mov	ax,word ptr DGROUP:__XR
@10@198:
	sub	ax,word ptr [bp-2]
	inc	ax
	mov	word ptr [bp-6],ax
	cmp	di,word ptr DGROUP:__YL
	jle	short @10@254
	mov	ax,di
	jmp	short @10@282
@10@254:
	mov	ax,word ptr DGROUP:__YL
@10@282:
	mov	word ptr [bp-4],ax
	mov	ax,di
	add	ax,word ptr [bp+16]
	dec	ax
	cmp	ax,word ptr DGROUP:__YR
	jge	short @10@338
	mov	ax,di
	add	ax,word ptr [bp+16]
	dec	ax
	jmp	short @10@366
@10@338:
	mov	ax,word ptr DGROUP:__YR
@10@366:
	sub	ax,word ptr [bp-4]
	inc	ax
	mov	word ptr [bp-8],ax
	cmp	word ptr [bp-6],0
	jle	short @10@450
	cmp	word ptr [bp-8],0
	jle	short @10@450
	mov	ax,80
	push	ax
	push	word ptr [bp-2]
	mov	ax,word ptr [bp-4]
	shl	ax,1
	les	bx,dword ptr DGROUP:__PSEG
	add	bx,ax
	push	word ptr es:[bx]
	push	word ptr [bp-8]
	push	word ptr [bp-6]
	push	word ptr [bp+14]
	mov	ax,word ptr [bp-4]
	sub	ax,di
	imul	word ptr [bp+14]
	mov	dx,word ptr [bp+10]
	add	dx,ax
	add	dx,word ptr [bp-2]
	sub	dx,si
	push	dx
	push	word ptr [bp+12]
	call	far ptr _pSprCp
	add	sp,16
@10@450:
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_pCTrpRImg0	endp
	assume	cs:PGRKER0_TEXT
_pCTrpRImg1	proc	far
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
	push	di
	mov	si,word ptr [bp+6]
	mov	di,word ptr [bp+8]
	cmp	si,word ptr DGROUP:__XL
	jle	short @11@86
	mov	ax,si
	jmp	short @11@114
@11@86:
	mov	ax,word ptr DGROUP:__XL
@11@114:
	mov	word ptr [bp-2],ax
	mov	ax,si
	add	ax,word ptr [bp+14]
	dec	ax
	cmp	ax,word ptr DGROUP:__XR
	jge	short @11@170
	mov	ax,si
	add	ax,word ptr [bp+14]
	dec	ax
	jmp	short @11@198
@11@170:
	mov	ax,word ptr DGROUP:__XR
@11@198:
	mov	word ptr [bp-6],ax
	cmp	di,word ptr DGROUP:__YL
	jle	short @11@254
	mov	ax,di
	jmp	short @11@282
@11@254:
	mov	ax,word ptr DGROUP:__YL
@11@282:
	mov	word ptr [bp-4],ax
	mov	ax,di
	add	ax,word ptr [bp+16]
	dec	ax
	cmp	ax,word ptr DGROUP:__YR
	jge	short @11@338
	mov	ax,di
	add	ax,word ptr [bp+16]
	dec	ax
	jmp	short @11@366
@11@338:
	mov	ax,word ptr DGROUP:__YR
@11@366:
	sub	ax,word ptr [bp-4]
	inc	ax
	mov	word ptr [bp-8],ax
	mov	ax,word ptr [bp-6]
	cmp	ax,word ptr [bp-2]
	jl	short @11@450
	cmp	word ptr [bp-8],0
	jle	short @11@450
	mov	ax,80
	push	ax
	push	word ptr [bp-2]
	mov	ax,word ptr [bp-4]
	shl	ax,1
	les	bx,dword ptr DGROUP:__PSEG
	add	bx,ax
	push	word ptr es:[bx]
	push	word ptr [bp-8]
	mov	ax,word ptr [bp-6]
	sub	ax,word ptr [bp-2]
	inc	ax
	push	ax
	push	word ptr [bp+14]
	mov	ax,word ptr [bp-4]
	sub	ax,di
	imul	word ptr [bp+14]
	mov	dx,word ptr [bp+10]
	add	dx,ax
	add	dx,word ptr [bp+14]
	add	dx,si
	dec	dx
	sub	dx,word ptr [bp-6]
	push	dx
	push	word ptr [bp+12]
	call	far ptr _pSprCpMir
	add	sp,16
@11@450:
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_pCTrpRImg1	endp
	assume	cs:PGRKER0_TEXT
_pCTrpRImg2	proc	far
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
	push	di
	mov	si,word ptr [bp+6]
	mov	di,word ptr [bp+8]
	cmp	si,word ptr DGROUP:__XL
	jle	short @12@86
	mov	ax,si
	jmp	short @12@114
@12@86:
	mov	ax,word ptr DGROUP:__XL
@12@114:
	mov	word ptr [bp-2],ax
	mov	ax,si
	add	ax,word ptr [bp+14]
	dec	ax
	cmp	ax,word ptr DGROUP:__XR
	jge	short @12@170
	mov	ax,si
	add	ax,word ptr [bp+14]
	dec	ax
	jmp	short @12@198
@12@170:
	mov	ax,word ptr DGROUP:__XR
@12@198:
	sub	ax,word ptr [bp-2]
	inc	ax
	mov	word ptr [bp-6],ax
	cmp	di,word ptr DGROUP:__YL
	jle	short @12@254
	mov	ax,di
	jmp	short @12@282
@12@254:
	mov	ax,word ptr DGROUP:__YL
@12@282:
	mov	word ptr [bp-4],ax
	mov	ax,di
	add	ax,word ptr [bp+16]
	dec	ax
	cmp	ax,word ptr DGROUP:__YR
	jge	short @12@338
	mov	ax,di
	add	ax,word ptr [bp+16]
	dec	ax
	jmp	short @12@366
@12@338:
	mov	ax,word ptr DGROUP:__YR
@12@366:
	mov	word ptr [bp-8],ax
	cmp	word ptr [bp-6],0
	jle	short @12@450
	mov	ax,word ptr [bp-8]
	cmp	ax,word ptr [bp-4]
	jl	short @12@450
	mov	ax,80
	push	ax
	push	word ptr [bp-2]
	mov	ax,word ptr [bp-4]
	shl	ax,1
	les	bx,dword ptr DGROUP:__PSEG
	add	bx,ax
	push	word ptr es:[bx]
	mov	ax,word ptr [bp-8]
	sub	ax,word ptr [bp-4]
	inc	ax
	push	ax
	push	word ptr [bp-6]
	push	word ptr [bp+14]
	mov	ax,word ptr [bp+16]
	add	ax,di
	sub	ax,word ptr [bp-8]
	dec	ax
	imul	word ptr [bp+14]
	mov	dx,word ptr [bp+10]
	add	dx,ax
	add	dx,word ptr [bp-2]
	sub	dx,si
	push	dx
	push	word ptr [bp+12]
	call	far ptr _pSprCpUD
	add	sp,16
@12@450:
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_pCTrpRImg2	endp
	assume	cs:PGRKER0_TEXT
_pCTrpRImg3	proc	far
	push	bp
	mov	bp,sp
	sub	sp,8
	push	si
	push	di
	mov	si,word ptr [bp+6]
	mov	di,word ptr [bp+8]
	cmp	si,word ptr DGROUP:__XL
	jle	short @13@86
	mov	ax,si
	jmp	short @13@114
@13@86:
	mov	ax,word ptr DGROUP:__XL
@13@114:
	mov	word ptr [bp-2],ax
	mov	ax,si
	add	ax,word ptr [bp+14]
	dec	ax
	cmp	ax,word ptr DGROUP:__XR
	jge	short @13@170
	mov	ax,si
	add	ax,word ptr [bp+14]
	dec	ax
	jmp	short @13@198
@13@170:
	mov	ax,word ptr DGROUP:__XR
@13@198:
	mov	word ptr [bp-6],ax
	cmp	di,word ptr DGROUP:__YL
	jle	short @13@254
	mov	ax,di
	jmp	short @13@282
@13@254:
	mov	ax,word ptr DGROUP:__YL
@13@282:
	mov	word ptr [bp-4],ax
	mov	ax,di
	add	ax,word ptr [bp+16]
	dec	ax
	cmp	ax,word ptr DGROUP:__YR
	jge	short @13@338
	mov	ax,di
	add	ax,word ptr [bp+16]
	dec	ax
	jmp	short @13@366
@13@338:
	mov	ax,word ptr DGROUP:__YR
@13@366:
	mov	word ptr [bp-8],ax
	mov	ax,word ptr [bp-6]
	cmp	ax,word ptr [bp-2]
	jl	short @13@450
	mov	ax,word ptr [bp-8]
	cmp	ax,word ptr [bp-4]
	jl	short @13@450
	mov	ax,80
	push	ax
	push	word ptr [bp-2]
	mov	ax,word ptr [bp-4]
	shl	ax,1
	les	bx,dword ptr DGROUP:__PSEG
	add	bx,ax
	push	word ptr es:[bx]
	mov	ax,word ptr [bp-8]
	sub	ax,word ptr [bp-4]
	inc	ax
	push	ax
	mov	ax,word ptr [bp-6]
	sub	ax,word ptr [bp-2]
	inc	ax
	push	ax
	push	word ptr [bp+14]
	mov	ax,word ptr [bp+16]
	add	ax,di
	dec	ax
	sub	ax,word ptr [bp-8]
	imul	word ptr [bp+14]
	mov	dx,word ptr [bp+10]
	add	dx,ax
	add	dx,word ptr [bp+14]
	add	dx,si
	dec	dx
	sub	dx,word ptr [bp-6]
	push	dx
	push	word ptr [bp+12]
	call	far ptr _pSprCpMirUD
	add	sp,16
@13@450:
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_pCTrpRImg3	endp
	assume	cs:PGRKER0_TEXT
_pImgCpLU	proc	far
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	 ds
	push	 si
	mov	 ax,[bp+6]
	mov	 ds,ax
	mov	 ax,[bp+16]
	mov	 es,ax
	cld	
	mov	 si,[bp+8]
	mov	 di,[bp+18]
	mov	 bx,[bp+10]
	xor	 ax,ax
	mov	 dx,[bp+12]
@14@366:
	mov	 cx,dx
	rep movsb	
	inc	 ax
	sub	 si,dx
	sub	 di,dx
	add	 si,bx
	add	 di,[bp+20]
	cmp	 ax,[bp+14]
	jne	short @14@366
	pop	 si
	pop	 ds
	pop	di
	pop	si
	pop	bp
	ret	
_pImgCpLU	endp
	assume	cs:PGRKER0_TEXT
_pSprCp	proc	far
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
	push	di
	mov	ax,word ptr [bp+14]
	imul	word ptr [bp+10]
	mov	word ptr [bp-2],ax
	mov	ax,word ptr [bp+14]
	imul	word ptr [bp+20]
	mov	word ptr [bp-4],ax
	push	 ds
	push	 si
	mov	 ax,[bp+6]
	mov	 ds,ax
	mov	 ax,[bp+16]
	mov	 es,ax
	mov	 si,[bp+8]
	mov	 dx,3c4h
	mov	 al,2
	out	 dx,al
	mov	 al,11h
	mov	 cx,[bp+18]
	mov	 di,cx
	shr	 di,1
	shr	 di,1
	and	 cx,3
	rol	 al,cl
	mov	 dx,3c5h
	out	 dx,al
	mov	 bx,[bp+12]
@15@618:
 	mov	 cx,[bp+14]
@15@646:
	mov	 ah,ds:[si]
	cmp	 ah,0
	je	short @15@758
	mov	 es:[di],ah
@15@758:
	add	 si,[bp+10]
	add	 di,[bp+20]
	loop	short @15@646
	rol	 al,1
	out	 dx,al
	adc	 di,0
	sub	 si,[bp-2]
	sub	 di,[bp-4]
	inc	 si
	dec	 bx
	jg	short @15@618
	pop	 si
	pop	 ds
 	mov	 dx,03c4h
 	mov	 ax,0f02h
 	out	 dx,ax
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_pSprCp	endp
	assume	cs:PGRKER0_TEXT
_pSprCpUD	proc	far
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
	push	di
	mov	ax,word ptr [bp+14]
	imul	word ptr [bp+10]
	mov	word ptr [bp-2],ax
	mov	ax,word ptr [bp+14]
	imul	word ptr [bp+20]
	mov	word ptr [bp-4],ax
	mov	ax,word ptr [bp+10]
	imul	word ptr [bp+14]
	sub	ax,word ptr [bp+10]
	add	word ptr [bp+8],ax
	push	 ds
	push	 si
	mov	 ax,[bp+6]
	mov	 ds,ax
	mov	 ax,[bp+16]
	mov	 es,ax
	mov	 si,[bp+8]
	mov	 dx,3c4h
	mov	 al,2
	out	 dx,al
	mov	 al,11h
	mov	 cx,[bp+18]
	mov	 di,cx
	shr	 di,1
	shr	 di,1
	and	 cx,3
	rol	 al,cl
	mov	 dx,3c5h
	out	 dx,al
	mov	 bx,[bp+12]
@16@618:
 	mov	 cx,[bp+14]
@16@646:
	mov	 ah,ds:[si]
	cmp	 ah,0
	je	short @16@758
	mov	 es:[di],ah
@16@758:
	sub	 si,[bp+10]
	add	 di,[bp+20]
	loop	short @16@646
	rol	 al,1
	out	 dx,al
	adc	 di,0
	add	 si,[bp-2]
	sub	 di,[bp-4]
	inc	 si
	dec	 bx
	jg	short @16@618
	pop	 si
	pop	 ds
 	mov	 dx,03c4h
 	mov	 ax,0f02h
 	out	 dx,ax
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_pSprCpUD	endp
	assume	cs:PGRKER0_TEXT
_pSprCpMir	proc	far
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
	push	di
	mov	ax,word ptr [bp+14]
	imul	word ptr [bp+10]
	mov	word ptr [bp-2],ax
	mov	ax,word ptr [bp+14]
	imul	word ptr [bp+20]
	mov	word ptr [bp-4],ax
	mov	ax,word ptr [bp+12]
	dec	ax
	add	word ptr [bp+8],ax
	push	 ds
	push	 si
	mov	 ax,[bp+6]
	mov	 ds,ax
	mov	 ax,[bp+16]
	mov	 es,ax
	mov	 si,[bp+8]
	mov	 dx,3c4h
	mov	 al,2
	out	 dx,al
	mov	 al,11h
	mov	 cx,[bp+18]
	mov	 di,cx
	shr	 di,1
	shr	 di,1
	and	 cx,3
	rol	 al,cl
	mov	 dx,3c5h
	out	 dx,al
	mov	 bx,[bp+12]
@17@618:
 	mov	 cx,[bp+14]
@17@646:
	mov	 ah,ds:[si]
	cmp	 ah,0
	je	short @17@758
	mov	 es:[di],ah
@17@758:
	add	 si,[bp+10]
	add	 di,[bp+20]
	loop	short @17@646
	rol	 al,1
	out	 dx,al
	adc	 di,0
	sub	 si,[bp-2]
	sub	 di,[bp-4]
	dec	 si
	dec	 bx
	jg	short @17@618
	pop	 si
	pop	 ds
 	mov	 dx,03c4h
 	mov	 ax,0f02h
 	out	 dx,ax
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_pSprCpMir	endp
	assume	cs:PGRKER0_TEXT
_pSprCpMirUD	proc	far
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
	push	di
	mov	ax,word ptr [bp+14]
	imul	word ptr [bp+10]
	mov	word ptr [bp-2],ax
	mov	ax,word ptr [bp+14]
	imul	word ptr [bp+20]
	mov	word ptr [bp-4],ax
	mov	ax,word ptr [bp+10]
	imul	word ptr [bp+14]
	mov	dx,word ptr [bp+12]
	add	dx,ax
	sub	dx,word ptr [bp+10]
	dec	dx
	add	word ptr [bp+8],dx
	push	 ds
	push	 si
	mov	 ax,[bp+6]
	mov	 ds,ax
	mov	 ax,[bp+16]
	mov	 es,ax
	mov	 si,[bp+8]
	mov	 dx,3c4h
	mov	 al,2
	out	 dx,al
	mov	 al,11h
	mov	 cx,[bp+18]
	mov	 di,cx
	shr	 di,1
	shr	 di,1
	and	 cx,3
	rol	 al,cl
	mov	 dx,3c5h
	out	 dx,al
	mov	 bx,[bp+12]
@18@618:
 	mov	 cx,[bp+14]
@18@646:
	mov	 ah,ds:[si]
	cmp	 ah,0
	je	short @18@758
	mov	 es:[di],ah
@18@758:
	sub	 si,[bp+10]
	add	 di,[bp+20]
	loop	short @18@646
	rol	 al,1
	out	 dx,al
	adc	 di,0
	add	 si,[bp-2]
	sub	 di,[bp-4]
	dec	 si
	dec	 bx
	jg	short @18@618
	pop	 si
	pop	 ds
 	mov	 dx,03c4h
 	mov	 ax,0f02h
 	out	 dx,ax
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_pSprCpMirUD	endp
	assume	cs:PGRKER0_TEXT
_imgHFill	proc	far
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	 ds
	push	 si
	mov	 ax,seg
	mov	 es,ax
	mov	 di,[bp+8]
	cld	
	mov	 al,[bp+12]
	mov	 ah,al
	mov	 cx,[bp+10]
	shr	 cx,1
	jae	short @19@366
	stosb	
@19@366:
	rep stosw	
	pop	 si
	pop	 ds
	pop	di
	pop	si
	pop	bp
	ret	
_imgHFill	endp
	assume	cs:PGRKER0_TEXT
_pImgVFill	proc	far
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	 ds
	push	 si
	mov	 dx,3c4h
	mov	 al,2
	out	 dx,al
	mov	 ax,seg
	mov	 es,ax
	mov	 cx,[bp+8]
	mov	 di,cx
	shr	 di,1
	shr	 di,1
	and	 cx,3
	mov	 al,1
	shl	 al,cl
	mov	 dx,3c5h
	out	 dx,al
	mov	 dx,[bp+10]
	mov	 ah,[bp+14]
	mov	 cx,[bp+12]
@20@562:
	mov	 es:[di],ah
	add	 di,dx
	loop	short @20@562
	pop	 si
	pop	 ds
 	mov	 dx,03c4h
 	mov	 ax,0f02h
 	out	 dx,ax
	pop	di
	pop	si
	pop	bp
	ret	
_pImgVFill	endp
	assume	cs:PGRKER0_TEXT
_pBytePut	proc	far
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	 ds
	push	 si
	mov	 dx,3c4h
	mov	 al,2
	out	 dx,al
	mov	 ax,seg
	mov	 es,ax
	mov	 cx,[bp+8]
	mov	 di,cx
	shr	 di,1
	shr	 di,1
	and	 cx,3
	mov	 al,1
	shl	 al,cl
	mov	 dx,3c5h
	out	 dx,al
	mov	 ah,[bp+10]
	mov	 es:[di],ah
	pop	 si
	pop	 ds
 	mov	 dx,03c4h
 	mov	 ax,0f02h
 	out	 dx,ax
	pop	di
	pop	si
	pop	bp
	ret	
_pBytePut	endp
	assume	cs:PGRKER0_TEXT
_pImgHFill	proc	far
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	push	di
	push	 ds
	push	 si
	mov	 ax,seg
	mov	 es,ax
	mov	 dx,3c4h
	mov	 al,2
	out	 dx,al
	mov	 cx,[bp+8]
	mov	 bx,cx
	and	 cx,3
	mov	 al,0fh
	shl	 al,cl
	mov	 cx,[bp+10]
	mov	 [bp-2],cx
	and	 cx,3
	mov	 ah,0e1h
	rol	 ah,cl
	mov	 cl,2
	shr	 bx,cl
	shr	 word ptr [bp-2],cl
	mov	 dx,03c5h
	cmp	 bx,[bp-2]
	je	short @22@1150
	mov	 di,bx
	out	 dx,al
	mov	 al,ah
	mov	 ah,[bp+12]
	mov	 es:[di],ah
	mov	 di,[bp-2]
	out	 dx,al
	mov	 es:[di],ah
	mov	 al,0fh
	out	 dx,al
	mov	 di,bx
	mov	 cx,[bp-2]
@22@1010:
	inc	 di
	cmp	 di,cx
	je	short @22@1290
	mov	 es:[di],ah
	jmp	short @22@1010
@22@1150:
	and	 al,ah
	out	 dx,al
	mov	 di,bx
	mov	 ah,[bp+12]
	mov	 es:[di],ah
@22@1290:
 	pop	 si
 	pop	 ds
 	mov	 dx,03c4h
 	mov	 ax,0f02h
 	out	 dx,ax
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_pImgHFill	endp
	assume	cs:PGRKER0_TEXT
_pImgFill	proc	far
	push	bp
	mov	bp,sp
	sub	sp,2
	push	si
	push	di
	push	 ds
	push	 si
	mov	 ax,seg
	mov	 es,ax
	mov	 dx,3c4h
	mov	 al,2
	out	 dx,al
	mov	 cx,[bp+8]
	mov	 bx,cx
	and	 cx,3
	mov	 al,0fh
	shl	 al,cl
	mov	 cx,[bp+12]
	mov	 [bp-2],cx
	and	 cx,3
	mov	 ah,0e1h
	rol	 ah,cl
	mov	 cl,2
	shr	 bx,cl
	shr	 word ptr [bp-2],cl
	mov	 dx,03c5h
	cmp	 bx,[bp-2]
	je	short @23@1570
	mov	 di,bx
	out	 dx,al
	mov	 al,ah
	mov	 ah,[bp+16]
	mov	 cx,[bp+14]
@23@814:
	mov	 es:[di],ah
	add	 di,[bp+10]
	loop	short @23@814
	out	 dx,al
	mov	 di,[bp-2]
	mov	 cx,[bp+14]
@23@982:
	mov	 es:[di],ah
	add	 di,[bp+10]
	loop	short @23@982
	mov	 al,0fh
	out	 dx,al
	mov	 dx,[bp-2]
	inc	 bx
	sub	 dx,bx
	mov	 cx,[bp+14]
	mov	 di,bx
@23@1262:
 	mov	 bx,0
@23@1290:
 	cmp	 bx,dx
	je	short @23@1458
	mov	 es:[di],ah
	inc	 di
	inc	 bx
	jmp	short @23@1290
@23@1458:
	sub	 di,dx
	add	 di,[bp+10]
	loop	short @23@1262
	jmp	short @23@1794
@23@1570:
	and	 al,ah
	out	 dx,al
	mov	 di,bx
	mov	 cx,[bp+14]
	mov	 ah,[bp+16]
@23@1710:
	mov	 es:[di],ah
	add	 di,[bp+10]
	loop	short @23@1710
@23@1794:
 	pop	 si
 	pop	 ds
 	mov	 dx,03c4h
 	mov	 ax,0f02h
 	out	 dx,ax
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_pImgFill	endp
	assume	cs:PGRKER0_TEXT
_pBitImgCp	proc	far
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
	push	di
	mov	ax,word ptr [bp+12]
	imul	word ptr [bp+10]
	mov	word ptr [bp-2],ax
	mov	ax,word ptr [bp+12]
	imul	word ptr [bp+18]
	mov	word ptr [bp-4],ax
	push	 ds
	push	 si
	mov	 ax,[bp+6]
	mov	 ds,ax
	mov	 ax,[bp+14]
	mov	 es,ax
	mov	 si,[bp+8]
	mov	 dx,3c4h
	mov	 al,2
	out	 dx,al
	mov	 al,11h
	mov	 cx,[bp+16]
	mov	 di,cx
	shr	 di,1
	shr	 di,1
	and	 cx,3
	rol	 al,cl
	mov	 dx,3c5h
	out	 dx,al
	mov	 bl,[bp+20]
	mov	 bh,[bp+10]
@24@646:
 	mov	 cl,1
@24@674:
 	mov	 ch,[bp+12]
@24@702:
	mov	 ah,ds:[si]
	shl	 ah,cl
	jae	short @24@814
	mov	 es:[di],bl
@24@814:
	add	 si,[bp+10]
	add	 di,[bp+18]
	dec	 ch
	jg	short @24@702
	rol	 al,1
	out	 dx,al
	adc	 di,0
	sub	 si,[bp-2]
	sub	 di,[bp-4]
	inc	 cl
	cmp	 cl,9
	jne	short @24@674
	inc	 si
	dec	 bh
	jg	short @24@646
	pop	 si
	pop	 ds
 	mov	 dx,03c4h
 	mov	 ax,0f02h
 	out	 dx,ax
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_pBitImgCp	endp
	assume	cs:PGRKER0_TEXT
_fPBitImgCp	proc	far
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	 ds
	push	 si
	mov	 ax,[bp+6]
	mov	 ds,ax
	mov	 ax,[bp+14]
	mov	 es,ax
	mov	 si,[bp+8]
	mov	 di,[bp+16]
	mov	 dx,3c4h
	mov	 al,2
	out	 dx,al
	mov	 dx,3c5h
	mov	 cl,04
	mov	 ch,[bp+20]
 	mov	 bh,[bp+12]
@25@450:
 	mov	 bl,[bp+10]
@25@478:
	mov	 ah,ds:[si]
	mov	 al,ah
	and	 al,0f0h
	shr	 al,cl
	out	 dx,al
	mov	 es:[di],ch
	inc	 di
	mov	 al,ah
	and	 al,0fh
	out	 dx,al
	mov	 es:[di],ch
	inc	 di
	inc	 si
	dec	 bl
	jg	short @25@478
	add	 di,[bp+18]
	sub	 di,[bp+10]
	sub	 di,[bp+10]
	dec	 bh
	jg	short @25@450
	pop	 si
	pop	 ds
 	mov	 dx,03c4h
 	mov	 ax,0f02h
 	out	 dx,ax
	pop	di
	pop	si
	pop	bp
	ret	
_fPBitImgCp	endp
	assume	cs:PGRKER0_TEXT
_get1DACR	proc	far
	push	bp
	mov	bp,sp
	mov	dx,967
	mov	al,byte ptr [bp+6]
	out	dx,al
	mov	dx,969
	in	al,dx
	les	bx,dword ptr [bp+8]
	mov	byte ptr es:[bx],al
	mov	dx,969
	in	al,dx
	les	bx,dword ptr [bp+12]
	mov	byte ptr es:[bx],al
	mov	dx,969
	in	al,dx
	les	bx,dword ptr [bp+16]
	mov	byte ptr es:[bx],al
	pop	bp
	ret	
_get1DACR	endp
	assume	cs:PGRKER0_TEXT
_getNDACR	proc	far
	push	bp
	mov	bp,sp
	mov	dx,967
	mov	al,byte ptr [bp+6]
	out	dx,al
	xor	cx,cx
	jmp	short @27@114
@27@58:
	mov	dx,969
	in	al,dx
	les	bx,dword ptr [bp+10]
	add	bx,cx
	mov	byte ptr es:[bx],al
	mov	dx,969
	in	al,dx
	les	bx,dword ptr [bp+14]
	add	bx,cx
	mov	byte ptr es:[bx],al
	mov	dx,969
	in	al,dx
	les	bx,dword ptr [bp+18]
	add	bx,cx
	mov	byte ptr es:[bx],al
	inc	cx
@27@114:
	cmp	cx,word ptr [bp+8]
	jl	short @27@58
	pop	bp
	ret	
_getNDACR	endp
	assume	cs:PGRKER0_TEXT
_set1DACR	proc	far
	push	bp
	mov	bp,sp
	mov	dx,968
	mov	al,byte ptr [bp+6]
	out	dx,al
	mov	dx,969
	mov	al,byte ptr [bp+8]
	out	dx,al
	mov	dx,969
	mov	al,byte ptr [bp+10]
	out	dx,al
	mov	dx,969
	mov	al,byte ptr [bp+12]
	out	dx,al
	pop	bp
	ret	
_set1DACR	endp
	assume	cs:PGRKER0_TEXT
_setNDACR	proc	far
	push	bp
	mov	bp,sp
	mov	dx,968
	mov	al,byte ptr [bp+6]
	out	dx,al
	xor	cx,cx
	jmp	short @29@114
@29@58:
	les	bx,dword ptr [bp+10]
	add	bx,cx
	mov	al,byte ptr es:[bx]
	mov	dx,969
	out	dx,al
	les	bx,dword ptr [bp+14]
	add	bx,cx
	mov	al,byte ptr es:[bx]
	mov	dx,969
	out	dx,al
	les	bx,dword ptr [bp+18]
	add	bx,cx
	mov	al,byte ptr es:[bx]
	mov	dx,969
	out	dx,al
	inc	cx
@29@114:
	cmp	cx,word ptr [bp+8]
	jl	short @29@58
	pop	bp
	ret	
_setNDACR	endp
	assume	cs:PGRKER0_TEXT
_brightCtrl	proc	far
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
	mov	si,word ptr [bp+22]
	mov	dx,968
	mov	al,byte ptr [bp+6]
	out	dx,al
	xor	cx,cx
	jmp	short @30@114
@30@58:
	les	bx,dword ptr [bp+10]
	add	bx,cx
	mov	al,byte ptr es:[bx]
	mov	ah,0
	imul	si
	mov	bx,100
	cwd	
	idiv	bx
	mov	byte ptr [bp-1],al
	mov	dx,969
	mov	al,byte ptr [bp-1]
	out	dx,al
	les	bx,dword ptr [bp+14]
	add	bx,cx
	mov	al,byte ptr es:[bx]
	mov	ah,0
	imul	si
	mov	bx,100
	cwd	
	idiv	bx
	mov	byte ptr [bp-2],al
	mov	dx,969
	mov	al,byte ptr [bp-2]
	out	dx,al
	les	bx,dword ptr [bp+18]
	add	bx,cx
	mov	al,byte ptr es:[bx]
	mov	ah,0
	imul	si
	mov	bx,100
	cwd	
	idiv	bx
	mov	byte ptr [bp-3],al
	mov	dx,969
	mov	al,byte ptr [bp-3]
	out	dx,al
	inc	cx
@30@114:
	cmp	cx,word ptr [bp+8]
	jl	short @30@58
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_brightCtrl	endp
	assume	cs:PGRKER0_TEXT
_greyCtrl	proc	far
	push	bp
	mov	bp,sp
	sub	sp,2
	mov	dx,968
	mov	al,byte ptr [bp+6]
	out	dx,al
	xor	cx,cx
	jmp	short @31@114
@31@58:
	les	bx,dword ptr [bp+10]
	add	bx,cx
	mov	al,byte ptr es:[bx]
	mov	ah,0
	mov	dx,3
	imul	dx
	les	bx,dword ptr [bp+14]
	add	bx,cx
	mov	dl,byte ptr es:[bx]
	mov	dh,0
	mov	bx,5
	push	ax
	mov	ax,dx
	imul	bx
	mov	dx,ax
	pop	ax
	add	ax,dx
	les	bx,dword ptr [bp+18]
	add	bx,cx
	mov	dl,byte ptr es:[bx]
	mov	dh,0
	shl	dx,1
	add	ax,dx
	mov	bx,10
	cwd	
	idiv	bx
	imul	word ptr [bp+22]
	mov	bx,100
	cwd	
	idiv	bx
	mov	byte ptr [bp-1],al
	mov	dx,969
	mov	al,byte ptr [bp-1]
	out	dx,al
	mov	dx,969
	mov	al,byte ptr [bp-1]
	out	dx,al
	mov	dx,969
	mov	al,byte ptr [bp-1]
	out	dx,al
	inc	cx
@31@114:
	cmp	cx,word ptr [bp+8]
	jl	short @31@58
	mov	sp,bp
	pop	bp
	ret	
_greyCtrl	endp
	assume	cs:PGRKER0_TEXT
_setRGBPalette	proc	far
	push	bp
	mov	bp,sp
	sub	sp,4
	push	si
	push	di
	xor	si,si
	mov	word ptr [bp-2],0
	jmp	short @32@282
@32@58:
	xor	di,di
	jmp	short @32@226
@32@86:
	mov	word ptr [bp-4],0
	jmp	short @32@170
@32@114:
	mov	ax,word ptr [bp-4]
	mov	cl,4
	shl	ax,cl
	add	ax,8
	push	ax
	mov	ax,di
	mov	cl,3
	shl	ax,cl
	add	ax,4
	push	ax
	mov	ax,si
	mov	cl,3
	shl	ax,cl
	add	ax,4
	push	ax
	push	word ptr [bp-2]
	push	cs
	call	near ptr _set1DACR
	add	sp,8
	inc	word ptr [bp-4]
	inc	word ptr [bp-2]
@32@170:
	cmp	word ptr [bp-4],4
	jl	short @32@114
	inc	di
@32@226:
	cmp	di,8
	jl	short @32@86
	inc	si
@32@282:
	cmp	si,8
	jl	short @32@58
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret	
_setRGBPalette	endp
	assume	cs:PGRKER0_TEXT
_asmDecode	proc	far
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	 ds
	push	 si
	lds	 si,[bp+6]
	les	 di,[bp+10]
	mov	 bx,[bp+14]
	mov	 dx,di
	cld	
@33@226:
	xor	 ch,ch
	mov	 cl,ds:[si]
	inc	 si
	dec	 bx
	jl	short @33@842
	xor	 ax,ax
	shr	 cx,1
	jae	short @33@478
	stosb	
@33@478:
 	rep stosw	
	mov	 cx,ds:[si]
	inc	 si
	inc	 si
	dec	 bx
	dec	 bx
	jl	short @33@842
	sub	 bx,cx
	shr	 cx,1
	jae	short @33@786
	movsb	
@33@786:
 	rep movsw	
	jmp	short @33@226
@33@842:
	mov	 cx,100
	xor	 ax,ax
	rep stosw	
	mov	 ax,di
	sub	 ax,dx
	pop	 si
	pop	 ds
	pop	di
	pop	si
	pop	bp
	ret	
_asmDecode	endp
PGRKER0_TEXT	ends
_BSS	segment word public 'BSS'
__YR	label	word
	db	2 dup (?)
__XR	label	word
	db	2 dup (?)
__YL	label	word
	db	2 dup (?)
__XL	label	word
	db	2 dup (?)
__EBF	label	dword
	db	4 dup (?)
__PSEG	label	dword
	db	4 dup (?)
_BSS	ends
PGRKER05_DATA	segment para public 'FAR_DATA'
__PSE	label	word
	db	1600 dup (?)
__SEG	label	word
	db	400 dup (?)
PGRKER05_DATA	ends
_BSS	segment word public 'BSS'
__ADD	label	dword
	db	800 dup (?)
	?debug	C E9
	?debug	C FA00000000
_BSS	ends
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
PGRKER0_TEXT	segment byte public 'CODE'
PGRKER0_TEXT	ends
	public	_vgaTextMode
	public	_vga320Mode
	public	__YR
	public	__XR
	public	__YL
	public	__XL
	public	__EBF
	public	__PSEG
	public	__PSE
	public	__SEG
	public	__ADD
	public	_asmDecode
	public	_setRGBPalette
	public	_greyCtrl
	public	_brightCtrl
	public	_setNDACR
	public	_set1DACR
	public	_getNDACR
	public	_get1DACR
	public	_fPBitImgCp
	public	_pBitImgCp
	public	_pImgFill
	public	_pImgHFill
	public	_pBytePut
	public	_pImgVFill
	public	_pSprCpMirUD
	public	_pSprCpMir
	public	_pSprCpUD
	public	_pSprCp
	public	_imgHFill
	public	_pImgCpLU
	public	_pCTrpRImg3
	public	_pCTrpRImg2
	public	_pCTrpRImg1
	public	_pCTrpRImg0
	public	_pPutImgMode
	public	_pBoxL
	public	_pBox2
	public	_writeMode
	public	_planeMode
	public	_actPage
	public	_setStAdd
	extrn	_malloc:far
_s@	equ	s@
	end
