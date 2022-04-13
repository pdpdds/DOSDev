	ifndef	??version
?debug	macro
	endm
publicdll macro	name
	public	name
	endm
	endif
	?debug	V 300h
	?debug	S "TESTSH.C"
	?debug	C E9710CAD1C085445535453482E43
	?debug	C E94019CA1815433A5C42435C494E434C5544455C535444494F2E48
	?debug	C E94019CA1815433A5C42435C494E434C5544455C5F444546532E48
	?debug	C E94019CA1816433A5C42435C494E434C5544455C5F4E46494C452E+
	?debug	C 48
	?debug	C E94019CA1815433A5C42435C494E434C5544455C5F4E554C4C2E48
	?debug	C E94019CA1818433A5C42435C494E434C5544455C47524150484943+
	?debug	C 532E48
	?debug	C E91D07AD1C07534848414E2E48
TESTSH_TEXT	segment byte public 'CODE'
TESTSH_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:TESTSH_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
TESTSH_TEXT	segment byte public 'CODE'
	?debug	C E801085445535453482E43710CAD1C
	?debug	L 4
	assume	cs:TESTSH_TEXT
_main	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	sub	sp,154
	push	si
	?debug	B
	?debug	L 6
	mov	word ptr [bp-2],0
	?debug	L 8
	push	ds
	mov	ax,offset DGROUP:s@
	push	ax
	push	ss
	lea	ax,word ptr [bp-4]
	push	ax
	push	ss
	lea	ax,word ptr [bp-2]
	push	ax
	call	far ptr _initgraph
	add	sp,12
	?debug	L 12
	cmp	word ptr [bp-2],9
	je	short @1@86
	mov	word ptr [bp-4],0
@1@86:
	?debug	L 13
	push	word ptr [bp-4]
	push	word ptr [bp-2]
	call	far ptr _shset
	pop	cx
	pop	cx
	?debug	L 14
	mov	ax,1
	push	ax
	mov	ax,1
	push	ax
	call	far ptr _setfillstyle
	pop	cx
	pop	cx
	?debug	L 15
	mov	ax,479
	push	ax
	mov	ax,639
	push	ax
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	call	far ptr _bar
	add	sp,8
	?debug	L 16
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1
	push	ax
	mov	ax,10
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 17
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+22
	push	ax
	mov	ax,50
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 18
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+71
	push	ax
	mov	ax,70
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 19
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+95
	push	ax
	mov	ax,90
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 20
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+115
	push	ax
	mov	ax,110
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 21
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+153
	push	ax
	mov	ax,130
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 22
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+180
	push	ax
	mov	ax,150
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 23
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+210
	push	ax
	mov	ax,170
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 24
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+252
	push	ax
	mov	ax,230
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 25
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+318
	push	ax
	mov	ax,250
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 26
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+383
	push	ax
	mov	ax,270
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 27
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+447
	push	ax
	mov	ax,290
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 28
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+496
	push	ax
	mov	ax,350
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 29
	call	far ptr _getch
	?debug	L 30
	call	far ptr _cleardevice
	?debug	L 31
	mov	ax,479
	push	ax
	mov	ax,639
	push	ax
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	call	far ptr _bar
	add	sp,8
	?debug	L 33
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+521
	push	ax
	mov	ax,10
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 34
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+583
	push	ax
	mov	ax,30
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 35
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+591
	push	ax
	mov	ax,50
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 36
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+631
	push	ax
	mov	ax,70
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 37
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+649
	push	ax
	mov	ax,90
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 38
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+682
	push	ax
	mov	ax,110
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 39
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+732
	push	ax
	mov	ax,130
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 40
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+796
	push	ax
	mov	ax,150
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 41
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+845
	push	ax
	mov	ax,170
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 42
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+909
	push	ax
	mov	ax,190
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 43
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+965
	push	ax
	mov	ax,210
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 44
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1010
	push	ax
	mov	ax,250
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 45
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1018
	push	ax
	mov	ax,270
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 46
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1036
	push	ax
	mov	ax,290
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 47
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1059
	push	ax
	mov	ax,310
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 48
	call	far ptr _getch
	?debug	L 49
	call	far ptr _cleardevice
	?debug	L 50
	mov	ax,479
	push	ax
	mov	ax,639
	push	ax
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	call	far ptr _bar
	add	sp,8
	?debug	L 52
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1073
	push	ax
	mov	ax,10
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 53
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1089
	push	ax
	mov	ax,70
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 54
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1150
	push	ax
	mov	ax,90
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 55
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1186
	push	ax
	mov	ax,110
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 56
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1226
	push	ax
	mov	ax,130
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 57
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1244
	push	ax
	mov	ax,150
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 58
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1272
	push	ax
	mov	ax,170
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 59
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1287
	push	ax
	mov	ax,190
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 60
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1327
	push	ax
	mov	ax,250
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 61
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1335
	push	ax
	mov	ax,270
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 62
	call	far ptr _getch
	?debug	L 63
	call	far ptr _cleardevice
	?debug	L 64
	mov	ax,479
	push	ax
	mov	ax,639
	push	ax
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	call	far ptr _bar
	add	sp,8
	?debug	L 66
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1372
	push	ax
	mov	ax,10
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 67
	mov	si,20
	jmp	short @1@170
@1@114:
	?debug	L 68
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1395
	push	ax
	mov	ax,si
	add	ax,20
	push	ax
	mov	ax,si
	add	ax,-20
	mov	cl,4
	shl	ax,cl
	add	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 67
	inc	si
@1@170:
	cmp	si,55
	jl	short @1@114
	?debug	L 69
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1398
	push	ax
	mov	ax,100
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 70
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,4
	push	ax
	mov	ax,14
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1438
	push	ax
	mov	ax,120
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 71
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,14
	push	ax
	mov	ax,4
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1471
	push	ax
	mov	ax,140
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 72
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,4
	push	ax
	mov	ax,1
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1504
	push	ax
	mov	ax,160
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 73
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,8
	push	ax
	mov	ax,7
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1537
	push	ax
	mov	ax,180
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 74
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1570
	push	ax
	mov	ax,200
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 75
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,13
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1603
	push	ax
	mov	ax,220
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 76
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,5
	push	ax
	mov	ax,12
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1636
	push	ax
	mov	ax,240
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 77
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,8
	push	ax
	mov	ax,6
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1669
	push	ax
	mov	ax,260
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 78
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,3
	push	ax
	mov	ax,2
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1702
	push	ax
	mov	ax,280
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 79
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,4
	push	ax
	mov	ax,11
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1735
	push	ax
	mov	ax,300
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 80
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1768
	push	ax
	mov	ax,320
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 81
	xor	si,si
	jmp	short @1@282
@1@226:
	?debug	L 82
	push	si
	mov	ax,200
	push	ax
	mov	ax,320
	push	ax
	call	far ptr _circle
	add	sp,6
	?debug	L 81
	add	si,10
@1@282:
	cmp	si,200
	jl	short @1@226
	?debug	L 83
	call	far ptr _getch
	?debug	L 84
	call	far ptr _cleardevice
	?debug	L 85
	mov	ax,479
	push	ax
	mov	ax,639
	push	ax
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	call	far ptr _bar
	add	sp,8
	?debug	L 87
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1784
	push	ax
	mov	ax,10
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 88
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1800
	push	ax
	mov	ax,70
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 89
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1861
	push	ax
	mov	ax,90
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 90
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1899
	push	ax
	mov	ax,110
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 91
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1939
	push	ax
	mov	ax,130
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 92
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1957
	push	ax
	mov	ax,150
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 93
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+1985
	push	ax
	mov	ax,170
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 94
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+2008
	push	ax
	mov	ax,190
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 95
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+2040
	push	ax
	mov	ax,210
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 96
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+2074
	push	ax
	mov	ax,250
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 97
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+2082
	push	ax
	mov	ax,270
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 98
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+2096
	push	ax
	mov	ax,290
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 99
	xor	ax,ax
	push	ax
	mov	ax,20
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ss
	lea	ax,word ptr [bp-154]
	push	ax
	mov	ax,310
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shget
	add	sp,16
	?debug	L 100
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ss
	lea	ax,word ptr [bp-154]
	push	ax
	mov	ax,330
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 101
	call	far ptr _getch
	?debug	L 102
	call	far ptr _cleardevice
	?debug	L 103
	mov	ax,479
	push	ax
	mov	ax,639
	push	ax
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	call	far ptr _bar
	add	sp,8
	?debug	L 105
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+2125
	push	ax
	mov	ax,10
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 106
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+2152
	push	ax
	mov	ax,70
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 107
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+2181
	push	ax
	mov	ax,110
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 108
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+2204
	push	ax
	mov	ax,130
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 109
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+2227
	push	ax
	mov	ax,150
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 110
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+2280
	push	ax
	mov	ax,170
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 111
	xor	ax,ax
	push	ax
	call	far ptr _shmode
	pop	cx
	?debug	L 112
	xor	ax,ax
	push	ax
	mov	ax,20
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ss
	lea	ax,word ptr [bp-154]
	push	ax
	mov	ax,190
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shget
	add	sp,16
	?debug	L 113
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+2304
	push	ax
	mov	ax,210
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 114
	mov	ax,1
	push	ax
	call	far ptr _shmode
	pop	cx
	?debug	L 115
	xor	ax,ax
	push	ax
	mov	ax,20
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ss
	lea	ax,word ptr [bp-154]
	push	ax
	mov	ax,230
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shget
	add	sp,16
	?debug	L 116
	call	far ptr _getch
	?debug	L 117
	call	far ptr _cleardevice
	?debug	L 118
	mov	ax,479
	push	ax
	mov	ax,639
	push	ax
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	call	far ptr _bar
	add	sp,8
	?debug	L 120
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+2328
	push	ax
	mov	ax,10
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 121
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+2371
	push	ax
	mov	ax,50
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 122
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+2418
	push	ax
	mov	ax,90
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 123
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+2471
	push	ax
	mov	ax,130
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 124
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+2499
	push	ax
	mov	ax,170
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 125
	mov	ax,4
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+2561
	push	ax
	mov	ax,210
	push	ax
	mov	ax,10
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 126
	xor	ax,ax
	push	ax
	xor	ax,ax
	push	ax
	mov	ax,1
	push	ax
	mov	ax,15
	push	ax
	push	ds
	mov	ax,offset DGROUP:s@+2622
	push	ax
	mov	ax,250
	push	ax
	mov	ax,50
	push	ax
	call	far ptr _shhan
	add	sp,16
	?debug	L 127
	call	far ptr _getch
	?debug	L 128
	call	far ptr _closegraph
	?debug	L 129
	pop	si
	mov	sp,bp
	pop	bp
	ret	
	?debug	C E3180096001A02
	?debug	C E603737472180266FF0001690404060002676D04+
	?debug	C 02FCFF000267640402FEFF00
	?debug	E
	?debug	E
_main	endp
	?debug	C E9
	?debug	C FA00000000
TESTSH_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
	db	0
	db	208
	db	'e'
	db	139
	db	'i '
	db	156
	db	'a'
	db	183
	db	161
	db	167
	db	'a'
	db	156
	db	225
	db	159
	db	161
	db	' test'
	db	0
	db	185
	db	'A'
	db	184
	db	'b'
	db	184
	db	'a:'
	db	137
	db	'w'
	db	211
	db	129
	db	148
	db	129
	db	208
	db	'b'
	db	138
	db	'a '
	db	196
	db	241
	db	207
	db	'A'
	db	200
	db	225
	db	' '
	db	181
	db	'e'
	db	138
	db	129
	db	210
	db	'A COM.COM '
	db	208
	db	'b'
	db	174
	db	137
	db	166
	db	129
	db	184
	db	'w'
	db	0
	db	185
	db	'A'
	db	184
	db	'b'
	db	180
	db	229
	db	180
	db	225
	db	':full Assembler'
	db	0
	db	185
	db	'A'
	db	184
	db	'b'
	db	183
	db	169
	db	175
	db	161
	db	':1993'
	db	145
	db	'e 9'
	db	182
	db	169
	db	0
	db	187
	db	161
	db	182
	db	165
	db	' '
	db	139
	db	'a'
	db	156
	db	129
	db	207
	db	162
	db	' '
	db	151
	db	'a'
	db	156
	db	'a'
	db	183
	db	161
	db	164
	db	225
	db	':'
	db	208
	db	225
	db	199
	db	'I'
	db	159
	db	161
	db	175
	db	'a,VGA,EGA'
	db	0
	db	187
	db	161
	db	182
	db	165
	db	180
	db	229
	db	180
	db	225
	db	':Turbo C 2.0 , C++'
	db	0
	db	187
	db	161
	db	182
	db	165
	db	' memory model:large model'
	db	0
	db	156
	db	'a'
	db	183
	db	161
	db	167
	db	'a'
	db	156
	db	225
	db	159
	db	161
	db	' '
	db	199
	db	'a'
	db	139
	db	161
	db	':'
	db	205
	db	165
	db	203
	db	'a '
	db	184
	db	'a'
	db	193
	db	'A '
	db	205
	db	161
	db	208
	db	'q 10000 byte'
	db	0
	db	183
	db	161
	db	' '
	db	156
	db	'a'
	db	183
	db	161
	db	167
	db	'a'
	db	156
	db	225
	db	159
	db	161
	db	147
	db	'e '
	db	181
	db	197
	db	184
	db	229
	db	208
	db	'e '
	db	137
	db	183
	db	136
	db	129
	db	' '
	db	173
	db	161
	db	207
	db	'a'
	db	203
	db	'a '
	db	182
	db	193
	db	180
	db	225
	db	183
	db	161
	db	163
	db	'a'
	db	157
	db	161
	db	' '
	db	180
	db	'a'
	db	162
	db	129
	db	144
	db	'a '
	db	165
	db	162
	db	172
	db	'a'
	db	208
	db	'i '
	db	174
	db	129
	db	' '
	db	183
	db	182
	db	0
	db	183
	db	'a'
	db	161
	db	'a '
	db	172
	db	'w'
	db	180
	db	243
	db	182
	db	'w '
	db	173
	db	161
	db	207
	db	'a'
	db	203
	db	'a'
	db	182
	db	193
	db	180
	db	225
	db	' '
	db	185
	db	'A'
	db	184
	db	'b'
	db	181
	db	'A'
	db	149
	db	161
	db	' '
	db	184
	db	225
	db	184
	db	'b'
	db	184
	db	'a'
	db	183
	db	129
	db	' '
	db	208
	db	225
	db	136
	db	'a'
	db	180
	db	244
	db	183
	db	161
	db	' '
	db	172
	db	'a'
	db	182
	db	'w'
	db	208
	db	'i '
	db	174
	db	129
	db	' '
	db	183
	db	182
	db	183
	db	's'
	db	0
	db	147
	db	161
	db	148
	db	'a.'
	db	175
	db	177
	db	187
	db	161
	db	180
	db	225
	db	' '
	db	173
	db	161
	db	175
	db	'a'
	db	140
	db	'a'
	db	187
	db	161
	db	' '
	db	181
	db	197
	db	165
	db	'b'
	db	208
	db	'a'
	db	137
	db	'A '
	db	137
	db	183
	db	136
	db	129
	db	208
	db	'a'
	db	181
	db	'a '
	db	166
	db	133
	db	172
	db	226
	db	','
	db	136
	db	129
	db	184
	db	'b'
	db	208
	db	'i '
	db	174
	db	129
	db	' '
	db	183
	db	182
	db	183
	db	's'
	db	147
	db	161
	db	148
	db	'a.'
	db	0
	db	148
	db	'e '
	db	183
	db	161
	db	' '
	db	156
	db	'a'
	db	183
	db	161
	db	167
	db	'a'
	db	156
	db	225
	db	159
	db	161
	db	' '
	db	184
	db	'a'
	db	193
	db	'A'
	db	159
	db	'i '
	db	172
	db	'w'
	db	206
	db	145
	db	209
	db	193
	db	' '
	db	208
	db	'i '
	db	174
	db	129
	db	147
	db	'e '
	db	180
	db	244
	db	183
	db	's'
	db	147
	db	161
	db	148
	db	'a.'
	db	0
	db	207
	db	'a'
	db	157
	db	'A'
	db	175
	db	'a '
	db	181
	db	'A'
	db	147
	db	161
	db	199
	db	161
	db	' '
	db	202
	db	129
	db	' '
	db	196
	db	229
	db	203
	db	161
	db	147
	db	'A '
	db	0
	db	'shhan library'
	db	159
	db	'i '
	db	172
	db	'a'
	db	182
	db	'w'
	db	208
	db	'a'
	db	139
	db	161
	db	' '
	db	182
	db	225
	db	208
	db	129
	db	172
	db	225
	db	147
	db	'e shhan.h'
	db	159
	db	'i '
	db	164
	db	'e'
	db	151
	db	'a'
	db	175
	db	161
	db	' '
	db	205
	db	161
	db	208
	db	'q'
	db	175
	db	161
	db	197
	db	'a'
	db	180
	db	161
	db	0
	db	208
	db	's'
	db	147
	db	161
	db	148
	db	'a.'
	db	0
	db	'shhan.h'
	db	181
	db	'A'
	db	147
	db	'e '
	db	208
	db	'q'
	db	174
	db	129
	db	183
	db	129
	db	' '
	db	182
	db	165
	db	209
	db	'w'
	db	183
	db	161
	db	' '
	db	151
	db	'i'
	db	180
	db	225
	db	183
	db	182
	db	183
	db	's'
	db	147
	db	161
	db	148
	db	'a.'
	db	0
	db	193
	db	161
	db	139
	db	161
	db	209
	db	193
	db	' '
	db	208
	db	'q'
	db	174
	db	129
	db	':shset'
	db	0
	db	182
	db	165
	db	209
	db	'w: void shset(int gd,int gm);'
	db	0
	db	'initgraph'
	db	208
	db	'q'
	db	174
	db	129
	db	' '
	db	164
	db	'a'
	db	157
	db	161
	db	' '
	db	150
	db	225
	db	181
	db	'A '
	db	183
	db	161
	db	' '
	db	208
	db	'q'
	db	174
	db	129
	db	159
	db	'i '
	db	209
	db	161
	db	194
	db	137
	db	208
	db	129
	db	' '
	db	186
	db	129
	db	175
	db	179
	db	175
	db	161
	db	182
	db	'a'
	db	0
	db	'gd:'
	db	139
	db	'a'
	db	156
	db	129
	db	207
	db	162
	db	' '
	db	151
	db	'a'
	db	156
	db	'a'
	db	183
	db	161
	db	164
	db	225
	db	159
	db	'i '
	db	187
	db	161
	db	184
	db	247
	db	208
	db	's'
	db	147
	db	161
	db	148
	db	'a.7'
	db	183
	db	161
	db	161
	db	'e HGC 9'
	db	161
	db	'e VGA'
	db	157
	db	161
	db	' '
	db	183
	db	165
	db	175
	db	162
	db	208
	db	's'
	db	147
	db	161
	db	148
	db	'a.'
	db	0
	db	'gm:'
	db	139
	db	'a'
	db	156
	db	129
	db	207
	db	162
	db	' '
	db	161
	db	161
	db	151
	db	'a'
	db	159
	db	'i '
	db	187
	db	161
	db	184
	db	247
	db	208
	db	's'
	db	147
	db	161
	db	148
	db	'a.0:720*348  1:640*400'
	db	0
	db	183
	db	165
	db	174
	db	129
	db	183
	db	129
	db	' '
	db	136
	db	't'
	db	183
	db	'i '
	db	183
	db	169
	db	166
	db	129
	db	156
	db	225
	db	' initgraph'
	db	183
	db	129
	db	' '
	db	183
	db	165
	db	174
	db	129
	db	183
	db	129
	db	' '
	db	136
	db	't'
	db	137
	db	193
	db	' '
	db	183
	db	169
	db	195
	db	161
	db	175
	db	161
	db	197
	db	'a '
	db	150
	db	129
	db	180
	db	246
	db	183
	db	'a'
	db	163
	db	'a'
	db	157
	db	161
	db	' '
	db	0
	db	'initgraph'
	db	181
	db	'A'
	db	172
	db	225
	db	' '
	db	185
	db	161
	db	172
	db	'a'
	db	208
	db	'e gd,gm'
	db	183
	db	'i '
	db	139
	db	'a'
	db	148
	db	129
	db	157
	db	161
	db	' '
	db	172
	db	'a'
	db	182
	db	'w'
	db	208
	db	'i '
	db	174
	db	129
	db	' '
	db	183
	db	182
	db	183
	db	's'
	db	147
	db	161
	db	148
	db	'a.'
	db	0
	db	148
	db	'e HGC'
	db	183
	db	129
	db	' '
	db	137
	db	'w'
	db	182
	db	129
	db	' gm'
	db	136
	db	't'
	db	183
	db	'i '
	db	184
	db	226
	db	148
	db	'w'
	db	211
	db	161
	db	' '
	db	172
	db	233
	db	184
	db	247
	db	208
	db	129
	db	' '
	db	186
	db	129
	db	180
	db	225
	db	180
	db	161
	db	' '
	db	208
	db	'q'
	db	0
	db	172
	db	'a'
	db	182
	db	'w'
	db	181
	db	129
	db	')'
	db	0
	db	'int gd=DETECT,gm;'
	db	0
	db	'initgraph(&gd,&gm,"");'
	db	0
	db	'shset(gd,gm);'
	db	0
	db	194
	db	137
	db	157
	db	'b '
	db	208
	db	'q'
	db	174
	db	129
	db	':shhan'
	db	0
	db	182
	db	165
	db	209
	db	'w: void shhan(int x,int y,char *str,int fcolor,int bcolor'
	db	0
	db	'                  ,int attr,int put'
	db	0
	db	'x,y:'
	db	162
	db	133
	db	184
	db	'a'
	db	181
	db	'i '
	db	194
	db	137
	db	157
	db	'b '
	db	182
	db	225
	db	195
	db	161
	db	' '
	db	187
	db	161
	db	184
	db	247
	db	','
	db	139
	db	'a'
	db	156
	db	129
	db	207
	db	162
	db	' '
	db	185
	db	193
	db	206
	db	'a'
	db	183
	db	177
	db	0
	db	'str:'
	db	194
	db	137
	db	157
	db	'b'
	db	208
	db	'i '
	db	162
	db	133
	db	184
	db	'a'
	db	181
	db	'i'
	db	0
	db	'fcolor,bcolor:'
	db	184
	db	229
	db	137
	db	'w'
	db	172
	db	130
	db	','
	db	164
	db	129
	db	137
	db	'w'
	db	172
	db	130
	db	0
	db	'attr:'
	db	173
	db	162
	db	172
	db	247
	db	' '
	db	187
	db	161
	db	184
	db	247
	db	0
	db	'put:'
	db	139
	db	'i'
	db	184
	db	'a'
	db	159
	db	'i '
	db	209
	db	193
	db	161
	db	'e'
	db	181
	db	'A '
	db	145
	db	189
	db	183
	db	'i'
	db	152
	db	129
	db	183
	db	129
	db	' bit'
	db	181
	db	'e'
	db	172
	db	'e '
	db	187
	db	161
	db	184
	db	247
	db	0
	db	172
	db	'a'
	db	182
	db	'w'
	db	181
	db	129
	db	')'
	db	0
	db	'shhan(10,30,"'
	db	139
	db	177
	db	139
	db	161
	db	162
	db	133
	db	' '
	db	164
	db	'a'
	db	165
	db	161
	db	'",15,1,0,0);'
	db	0
	db	172
	db	241
	db	173
	db	'A'
	db	208
	db	'e '
	db	185
	db	193
	db	206
	db	'a '
	db	185
	db	'A'
	db	180
	db	225
	db	' '
	db	136
	db	'a'
	db	147
	db	'w '
	db	0
	db	172
	db	'w'
	db	0
	db	184
	db	229
	db	137
	db	'w'
	db	172
	db	130
	db	' '
	db	164
	db	129
	db	137
	db	'w'
	db	172
	db	130
	db	' '
	db	187
	db	161
	db	184
	db	247
	db	'(HGC'
	db	147
	db	'e '
	db	172
	db	130
	db	172
	db	'w'
	db	187
	db	161
	db	182
	db	165
	db	' '
	db	166
	db	137
	db	136
	db	'a)'
	db	0
	db	137
	db	'w'
	db	211
	db	129
	db	148
	db	129
	db	208
	db	'b'
	db	138
	db	'a '
	db	196
	db	241
	db	207
	db	'A'
	db	200
	db	225
	db	' '
	db	181
	db	'e'
	db	138
	db	129
	db	210
	db	'A COM.COM'
	db	0
	db	137
	db	'w'
	db	211
	db	129
	db	148
	db	129
	db	208
	db	'b'
	db	138
	db	'a '
	db	196
	db	241
	db	207
	db	'A'
	db	200
	db	225
	db	' '
	db	181
	db	'e'
	db	138
	db	129
	db	210
	db	'A COM.COM'
	db	0
	db	137
	db	'w'
	db	211
	db	129
	db	148
	db	129
	db	208
	db	'b'
	db	138
	db	'a '
	db	196
	db	241
	db	207
	db	'A'
	db	200
	db	225
	db	' '
	db	181
	db	'e'
	db	138
	db	129
	db	210
	db	'A COM.COM'
	db	0
	db	137
	db	'w'
	db	211
	db	129
	db	148
	db	129
	db	208
	db	'b'
	db	138
	db	'a '
	db	196
	db	241
	db	207
	db	'A'
	db	200
	db	225
	db	' '
	db	181
	db	'e'
	db	138
	db	129
	db	210
	db	'A COM.COM'
	db	0
	db	137
	db	'w'
	db	211
	db	129
	db	148
	db	129
	db	208
	db	'b'
	db	138
	db	'a '
	db	196
	db	241
	db	207
	db	'A'
	db	200
	db	225
	db	' '
	db	181
	db	'e'
	db	138
	db	129
	db	210
	db	'A COM.COM'
	db	0
	db	137
	db	'w'
	db	211
	db	129
	db	148
	db	129
	db	208
	db	'b'
	db	138
	db	'a '
	db	196
	db	241
	db	207
	db	'A'
	db	200
	db	225
	db	' '
	db	181
	db	'e'
	db	138
	db	129
	db	210
	db	'A COM.COM'
	db	0
	db	137
	db	'w'
	db	211
	db	129
	db	148
	db	129
	db	208
	db	'b'
	db	138
	db	'a '
	db	196
	db	241
	db	207
	db	'A'
	db	200
	db	225
	db	' '
	db	181
	db	'e'
	db	138
	db	129
	db	210
	db	'A COM.COM'
	db	0
	db	137
	db	'w'
	db	211
	db	129
	db	148
	db	129
	db	208
	db	'b'
	db	138
	db	'a '
	db	196
	db	241
	db	207
	db	'A'
	db	200
	db	225
	db	' '
	db	181
	db	'e'
	db	138
	db	129
	db	210
	db	'A COM.COM'
	db	0
	db	137
	db	'w'
	db	211
	db	129
	db	148
	db	129
	db	208
	db	'b'
	db	138
	db	'a '
	db	196
	db	241
	db	207
	db	'A'
	db	200
	db	225
	db	' '
	db	181
	db	'e'
	db	138
	db	129
	db	210
	db	'A COM.COM'
	db	0
	db	137
	db	'w'
	db	211
	db	129
	db	148
	db	129
	db	208
	db	'b'
	db	138
	db	'a '
	db	196
	db	241
	db	207
	db	'A'
	db	200
	db	225
	db	' '
	db	181
	db	'e'
	db	138
	db	129
	db	210
	db	'A COM.COM'
	db	0
	db	139
	db	'a'
	db	156
	db	129
	db	207
	db	162
	db	137
	db	193
	db	183
	db	129
	db	' '
	db	209
	db	165
	db	182
	db	'w'
	db	0
	db	183
	db	179
	db	157
	db	'b '
	db	208
	db	'q'
	db	174
	db	129
	db	':shget'
	db	0
	db	182
	db	165
	db	209
	db	'w: void shget(int x,int y,char *str,int fcolor,int bcolor'
	db	0
	db	'                  ,int limit,int keep'
	db	0
	db	'x,y:'
	db	162
	db	133
	db	184
	db	'a'
	db	181
	db	'i '
	db	194
	db	137
	db	157
	db	'b '
	db	182
	db	225
	db	195
	db	161
	db	' '
	db	187
	db	161
	db	184
	db	247
	db	','
	db	139
	db	'a'
	db	156
	db	129
	db	207
	db	162
	db	' '
	db	185
	db	193
	db	206
	db	'a'
	db	183
	db	177
	db	0
	db	'str:'
	db	194
	db	137
	db	157
	db	'b'
	db	208
	db	'i '
	db	162
	db	133
	db	184
	db	'a'
	db	181
	db	'i'
	db	0
	db	'fcolor,bcolor:'
	db	184
	db	229
	db	137
	db	'w'
	db	172
	db	130
	db	','
	db	164
	db	129
	db	137
	db	'w'
	db	172
	db	130
	db	0
	db	'limit:'
	db	194
	db	'A'
	db	148
	db	129
	db	' '
	db	183
	db	179
	db	157
	db	'b '
	db	162
	db	133
	db	184
	db	'a'
	db	174
	db	129
	db	0
	db	'keep  0:'
	db	164
	db	225
	db	204
	db	225
	db	159
	db	'i '
	db	187
	db	161
	db	182
	db	129
	db	137
	db	161
	db	' '
	db	183
	db	179
	db	157
	db	'b '
	db	164
	db	'h'
	db	183
	db	'q'
	db	0
	db	'      1:'
	db	164
	db	225
	db	204
	db	225
	db	159
	db	'i '
	db	183
	db	'A'
	db	187
	db	161
	db	208
	db	'a'
	db	137
	db	161
	db	' '
	db	183
	db	179
	db	157
	db	'b '
	db	164
	db	'h'
	db	183
	db	'q'
	db	0
	db	172
	db	'a'
	db	182
	db	'w'
	db	181
	db	129
	db	')'
	db	0
	db	'char str[20];'
	db	0
	db	'shget(10,310,str,15,1,20,0);'
	db	0
	db	183
	db	179
	db	157
	db	'b '
	db	161
	db	161
	db	151
	db	'a '
	db	185
	db	'A'
	db	180
	db	225
	db	' '
	db	208
	db	'q'
	db	174
	db	129
	db	':shmode'
	db	0
	db	182
	db	165
	db	209
	db	'w: void shmode(int mode);'
	db	0
	db	'mode  0:'
	db	208
	db	'e'
	db	139
	db	'i '
	db	183
	db	179
	db	157
	db	'b '
	db	161
	db	161
	db	151
	db	'a'
	db	0
	db	'      1:'
	db	181
	db	'w'
	db	162
	db	133
	db	' '
	db	183
	db	179
	db	157
	db	'b '
	db	161
	db	161
	db	151
	db	'a'
	db	0
	db	183
	db	179
	db	157
	db	'b'
	db	186
	db	151
	db	181
	db	'A mode'
	db	159
	db	'i '
	db	164
	db	'a'
	db	142
	db	129
	db	157
	db	'a'
	db	161
	db	'e shift+space'
	db	159
	db	'i '
	db	146
	db	137
	db	156
	db	225
	db	' '
	db	186
	db	129
	db	175
	db	179
	db	175
	db	161
	db	182
	db	'a'
	db	0
	db	208
	db	'e'
	db	139
	db	'i '
	db	161
	db	161
	db	151
	db	'a'
	db	157
	db	161
	db	' '
	db	183
	db	179
	db	157
	db	'b'
	db	150
	db	'S'
	db	147
	db	161
	db	148
	db	'a.'
	db	0
	db	181
	db	'w'
	db	162
	db	133
	db	' '
	db	161
	db	161
	db	151
	db	'a'
	db	157
	db	161
	db	' '
	db	183
	db	179
	db	157
	db	'b'
	db	150
	db	'S'
	db	147
	db	161
	db	148
	db	'a.'
	db	0
	db	184
	db	'i '
	db	160
	db	'e'
	db	151
	db	'i'
	db	180
	db	225
	db	' '
	db	165
	db	169
	db	157
	db	'a'
	db	137
	db	161
	db	' '
	db	208
	db	150
	db	147
	db	'e'
	db	149
	db	'A '
	db	165
	db	'i'
	db	157
	db	161
	db	' '
	db	165
	db	169
	db	206
	db	145
	db	183
	db	161
	db	' '
	db	180
	db	244
	db	145
	db	'A'
	db	182
	db	'a'
	db	0
	db	184
	db	229
	db	175
	db	161
	db	210
	db	'A'
	db	136
	db	'a '
	db	139
	db	's'
	db	164
	db	'b'
	db	208
	db	129
	db	172
	db	225
	db	' '
	db	183
	db	161
	db	191
	db	'q'
	db	208
	db	129
	db	172
	db	225
	db	' 1.0'
	db	183
	db	'i '
	db	160
	db	'a'
	db	162
	db	129
	db	159
	db	161
	db	208
	db	's'
	db	147
	db	161
	db	148
	db	'a.'
	db	0
	db	172
	db	151
	db	136
	db	'b'
	db	165
	db	161
	db	148
	db	'a '
	db	185
	db	169
	db	184
	db	'b'
	db	183
	db	161
	db	' '
	db	144
	db	'a'
	db	181
	db	165
	db	136
	db	245
	db	' '
	db	136
	db	'{'
	db	183
	db	'e'
	db	149
	db	'A '
	db	184
	db	'w'
	db	184
	db	241
	db	183
	db	161
	db	156
	db	'a'
	db	161
	db	'e '
	db	199
	db	'a'
	db	139
	db	161
	db	136
	db	'a '
	db	184
	db	'b'
	db	137
	db	161
	db	0
	db	172
	db	'a'
	db	182
	db	'w'
	db	183
	db	161
	db	' '
	db	136
	db	'e'
	db	205
	db	'e'
	db	208
	db	'a'
	db	148
	db	'a'
	db	147
	db	'e '
	db	136
	db	245
	db	183
	db	179
	db	147
	db	161
	db	148
	db	'a.'
	db	0
	db	175
	db	161
	db	136
	db	'e'
	db	183
	db	161
	db	' '
	db	208
	db	225
	db	156
	db	'b'
	db	208
	db	'a'
	db	147
	db	'e'
	db	149
	db	'A'
	db	157
	db	161
	db	' '
	db	185
	db	177
	db	148
	db	225
	db	' '
	db	185
	db	189
	db	183
	db	'e '
	db	156
	db	'a'
	db	183
	db	161
	db	167
	db	'a'
	db	156
	db	225
	db	159
	db	161
	db	159
	db	'i '
	db	185
	db	'A'
	db	184
	db	'b'
	db	208
	db	129
	db	' '
	db	165
	db	161
	db	137
	db	'V'
	db	183
	db	's'
	db	147
	db	161
	db	148
	db	'a.'
	db	0
	db	173
	db	161
	db	175
	db	'a'
	db	159
	db	'i '
	db	137
	db	183
	db	136
	db	129
	db	208
	db	129
	db	' '
	db	145
	db	189
	db	180
	db	'v'
	db	183
	db	'a'
	db	147
	db	161
	db	' '
	db	137
	db	197
	db	175
	db	177
	db	183
	db	182
	db	147
	db	'e '
	db	166
	db	133
	db	183
	db	'e '
	db	181
	db	'e'
	db	138
	db	129
	db	208
	db	129
	db	' '
	db	165
	db	161
	db	175
	db	161
	db	139
	db	161
	db	' '
	db	164
	db	'a'
	db	156
	db	's'
	db	147
	db	161
	db	148
	db	'a.'
	db	0
	db	136
	db	'q'
	db	172
	db	'a'
	db	208
	db	's'
	db	147
	db	161
	db	148
	db	'a.'
	db	0
_DATA	ends
TESTSH_TEXT	segment byte public 'CODE'
TESTSH_TEXT	ends
	extrn	_getch:far
	public	_main
	extrn	_shget:far
	extrn	_shmode:far
	extrn	_shset:far
	extrn	_shhan:far
	extrn	_setfillstyle:far
	extrn	_initgraph:far
	extrn	_closegraph:far
	extrn	_cleardevice:far
	extrn	_circle:far
	extrn	_bar:far
_s@	equ	s@
	?debug	C EA010C
	?debug	C E31900000023040400
	?debug	C EB065F67657463681900
	?debug	C E31A00000023040400
	?debug	C EC055F6D61696E1A1800
	?debug	C E31B00000023010400
	?debug	C EB065F73686765741B00
	?debug	C E31C00000023010400
	?debug	C EB075F73686D6F64651C00
	?debug	C E31D00000023010400
	?debug	C EB065F73687365741D00
	?debug	C E31E00000023010400
	?debug	C EB065F736868616E1E00
	?debug	C E31F00000023010400
	?debug	C EB0D5F73657466696C6C7374796C651F00
	?debug	C E32000000023010400
	?debug	C EB0A5F696E697467726170682000
	?debug	C E32100000023010400
	?debug	C EB0B5F636C6F736567726170682100
	?debug	C E32200000023010400
	?debug	C EB0C5F636C6561726465766963652200
	?debug	C E32300000023010400
	?debug	C EB075F636972636C652300
	?debug	C E32400000023010400
	?debug	C EB045F6261722400
	?debug	C E60666706F735F740606000673697A655F740A06+
	?debug	C 00
	end
