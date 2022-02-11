; This is originally the file TOTSR.C, copyright (C) 1993 Forte.
;
; Due to a bug in Borland C++ (only 16-bit registers are by default pushed
; onto the stack in interrupt-handlers, whereas 32-bit registers are used
; throughout the code) we've chosen this easy approach ("push ax" ->
; "push eax") to get around without modifying any code.
;
; File created by Thomas H.

	.386p
	ifndef	??version
?debug	macro
	endm
publicdll macro	name
	public	name
	endm
	endif
	?debug	V 300h
	?debug	S "TOTSR.C"
	?debug	C E9946A851C07544F5453522E43
	?debug	C E94019CA181B433A5C424F524C414E44435C494E434C5544455C53+
	?debug	C 5444494F2E48
	?debug	C E94019CA181B433A5C424F524C414E44435C494E434C5544455C5F+
	?debug	C 444546532E48
	?debug	C E94019CA181C433A5C424F524C414E44435C494E434C5544455C5F+
	?debug	C 4E46494C452E48
	?debug	C E94019CA181B433A5C424F524C414E44435C494E434C5544455C5F+
	?debug	C 4E554C4C2E48
	?debug	C E94019CA181C433A5C424F524C414E44435C494E434C5544455C53+
	?debug	C 54444C49422E48
	?debug	C E94019CA181C433A5C424F524C414E44435C494E434C5544455C53+
	?debug	C 5452494E472E48
	?debug	C E94019CA181B433A5C424F524C414E44435C494E434C5544455C43+
	?debug	C 4F4E494F2E48
	?debug	C E94019CA1819433A5C424F524C414E44435C494E434C5544455C44+
	?debug	C 4F532E48
	?debug	C E9E4A97A1C064D4944492E48
	?debug	C E9E4A97A1C07544F5453522E48
	?debug	C E9E4A97A1C0A554C5452414D49442E48
TOTSR_TEXT	segment byte public use16 'CODE'
TOTSR_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:TOTSR_TEXT,ds:DGROUP
_DATA	segment word public use16 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public use16 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
_DATA	segment word public use16 'DATA'
orig_clock	label	dword
	db	0
	db	0
	db	0
	db	0
clock_interval	label	word
	db	142
	db	32
	db	0
	db	0
_um_hook	label	dword
	db	0
	db	0
	db	0
	db	0
_DATA	ends
TOTSR_TEXT	segment byte public use16 'CODE'
	?debug	C E80107544F5453522E43946A851C
	?debug	L 32
	assume	cs:TOTSR_TEXT
_um_data_out	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	?debug	C E60462797465040A060000
	?debug	B
	?debug	L 34
	cmp	dword ptr DGROUP:_um_hook,large 0
	je	short @1@86
	?debug	L 35
	mov	ax,16
	?debug	L 36
	mov	cx,word ptr [bp+6]
	?debug	L 37
	call	dword ptr DGROUP:_um_hook
@1@86:
	?debug	L 39
	xor	ax,ax
	?debug	L 40
	pop	bp
	ret	
	?debug	C E60462797465040A060000
	?debug	E
	?debug	E
_um_data_out	endp
	?debug	L 42
	assume	cs:TOTSR_TEXT
_um_pitch_bend	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	?debug	C E6036C7362040A0A0000036D7362040A08000007+
	?debug	C 6368616E6E656C040A060000
	?debug	B
	?debug	L 44
	mov	ax,word ptr [bp+6]
	add	ax,224
	push	ax
	push	cs
	call	near ptr _um_data_out
	pop	cx
	?debug	L 45
	push	word ptr [bp+8]
	push	cs
	call	near ptr _um_data_out
	pop	cx
	?debug	L 46
	push	word ptr [bp+10]
	push	cs
	call	near ptr _um_data_out
	pop	cx
	?debug	L 47
	pop	bp
	ret	
	?debug	C E6076368616E6E656C040A060000036D7362040A+
	?debug	C 080000036C7362040A0A0000
	?debug	E
	?debug	E
_um_pitch_bend	endp
	?debug	L 49
	assume	cs:TOTSR_TEXT
_um_program	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	?debug	C E60470726F67040A080000076368616E6E656C04+
	?debug	C 0A060000
	?debug	B
	?debug	L 51
	mov	ax,word ptr [bp+6]
	add	ax,192
	push	ax
	push	cs
	call	near ptr _um_data_out
	pop	cx
	?debug	L 52
	push	word ptr [bp+8]
	push	cs
	call	near ptr _um_data_out
	pop	cx
	?debug	L 53
	pop	bp
	ret	
	?debug	C E6076368616E6E656C040A0600000470726F6704+
	?debug	C 0A080000
	?debug	E
	?debug	E
_um_program	endp
	?debug	L 55
	assume	cs:TOTSR_TEXT
_um_chanpressure	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	?debug	C E6087072657373757265040A0A00000570697463+
	?debug	C 68040A080000076368616E6E656C040A060000
	?debug	B
	?debug	L 57
	mov	ax,word ptr [bp+6]
	add	ax,208
	push	ax
	push	cs
	call	near ptr _um_data_out
	pop	cx
	?debug	L 58
	push	word ptr [bp+8]
	push	cs
	call	near ptr _um_data_out
	pop	cx
	?debug	L 59
	push	word ptr [bp+10]
	push	cs
	call	near ptr _um_data_out
	pop	cx
	?debug	L 60
	pop	bp
	ret	
	?debug	C E6076368616E6E656C040A060000057069746368+
	?debug	C 040A080000087072657373757265040A0A0000
	?debug	E
	?debug	E
_um_chanpressure	endp
	?debug	L 62
	assume	cs:TOTSR_TEXT
_um_pressure	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	?debug	C E6087072657373757265040A0A00000570697463+
	?debug	C 68040A080000076368616E6E656C040A060000
	?debug	B
	?debug	L 64
	mov	ax,word ptr [bp+6]
	add	ax,160
	push	ax
	push	cs
	call	near ptr _um_data_out
	pop	cx
	?debug	L 65
	push	word ptr [bp+8]
	push	cs
	call	near ptr _um_data_out
	pop	cx
	?debug	L 66
	push	word ptr [bp+10]
	push	cs
	call	near ptr _um_data_out
	pop	cx
	?debug	L 67
	pop	bp
	ret	
	?debug	C E6076368616E6E656C040A060000057069746368+
	?debug	C 040A080000087072657373757265040A0A0000
	?debug	E
	?debug	E
_um_pressure	endp
	?debug	L 69
	assume	cs:TOTSR_TEXT
_um_parameter	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	?debug	C E60576616C7565040A0A000007636F6E74726F6C+
	?debug	C 040A080000076368616E6E656C040A060000
	?debug	B
	?debug	L 71
	mov	ax,word ptr [bp+6]
	add	ax,176
	push	ax
	push	cs
	call	near ptr _um_data_out
	pop	cx
	?debug	L 72
	push	word ptr [bp+8]
	push	cs
	call	near ptr _um_data_out
	pop	cx
	?debug	L 73
	push	word ptr [bp+10]
	push	cs
	call	near ptr _um_data_out
	pop	cx
	?debug	L 74
	pop	bp
	ret	
	?debug	C E6076368616E6E656C040A06000007636F6E7472+
	?debug	C 6F6C040A0800000576616C7565040A0A0000
	?debug	E
	?debug	E
_um_parameter	endp
	?debug	L 76
	assume	cs:TOTSR_TEXT
_um_note_on	proc	far
	?debug	B
	enter	2,0
	?debug	C E60876656C6F63697479040A0A0000046E6F7465+
	?debug	C 040A080000076368616E6E656C040A060000
	?debug	B
	?debug	L 80
	mov	ax,word ptr [bp+6]
	add	ax,144
	push	ax
	push	cs
	call	near ptr _um_data_out
	pop	cx
	mov	word ptr [bp-2],ax
	?debug	L 81
	push	word ptr [bp+8]
	push	cs
	call	near ptr _um_data_out
	pop	cx
	add	word ptr [bp-2],ax
	?debug	L 82
	push	word ptr [bp+10]
	push	cs
	call	near ptr _um_data_out
	pop	cx
	add	word ptr [bp-2],ax
	?debug	L 83
	mov	ax,word ptr [bp-2]
	?debug	L 84
	leave	
	ret	
	?debug	C E6056572726F720402FEFF00076368616E6E656C+
	?debug	C 040A060000046E6F7465040A0800000876656C6F+
	?debug	C 63697479040A0A0000
	?debug	E
	?debug	E
_um_note_on	endp
	?debug	L 86
	assume	cs:TOTSR_TEXT
_um_note_off	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	?debug	C E60876656C6F63697479040A0A0000046E6F7465+
	?debug	C 040A080000076368616E6E656C040A060000
	?debug	B
	?debug	L 88
	mov	ax,word ptr [bp+6]
	add	ax,128
	push	ax
	push	cs
	call	near ptr _um_data_out
	pop	cx
	?debug	L 89
	push	word ptr [bp+8]
	push	cs
	call	near ptr _um_data_out
	pop	cx
	?debug	L 90
	push	word ptr [bp+10]
	push	cs
	call	near ptr _um_data_out
	pop	cx
	?debug	L 91
	pop	bp
	ret	
	?debug	C E6076368616E6E656C040A060000046E6F746504+
	?debug	C 0A0800000876656C6F63697479040A0A0000
	?debug	E
	?debug	E
_um_note_off	endp
	?debug	L 93
	assume	cs:TOTSR_TEXT
_reset_um	proc	far
	?debug	B
	enter	2,0
	?debug	B
	?debug	L 97
	mov	ax,18
	?debug	L 98
	cmp	dword ptr DGROUP:_um_hook,large 0
	je	short @9@86
	call	dword ptr DGROUP:_um_hook
@9@86:
	?debug	L 99
	mov	word ptr [bp-2],0
@9@114:
	?debug	L 100
	push	large 120
	push	word ptr [bp-2]
	push	cs
	call	near ptr _um_parameter
	add	sp,6
	?debug	L 101
	push	large 121
	push	word ptr [bp-2]
	push	cs
	call	near ptr _um_parameter
	add	sp,6
	?debug	L 102
	push	large 100
	push	word ptr [bp-2]
	push	cs
	call	near ptr _um_parameter
	add	sp,6
	?debug	L 103
	push	large 101
	push	word ptr [bp-2]
	push	cs
	call	near ptr _um_parameter
	add	sp,6
	?debug	L 104
	push	large 000020006h
	push	word ptr [bp-2]
	push	cs
	call	near ptr _um_parameter
	add	sp,6
	?debug	L 105
	push	large 38
	push	word ptr [bp-2]
	push	cs
	call	near ptr _um_parameter
	add	sp,6
	?debug	L 99
	inc	word ptr [bp-2]
	cmp	word ptr [bp-2],16
	jl	short @9@114
	?debug	L 107
	mov	ax,1
	?debug	L 108
	leave	
	ret	
	?debug	C E601690402FEFF00
	?debug	E
	?debug	E
_reset_um	endp
TOTSR_TEXT	ends
_DATA	segment word public use16 'DATA'
us_timer	label	word
	db	0
	db	0
	db	0
	db	0
timer	label	word
	db	0
	db	0
	db	0
	db	0
tempo	label	word
	db	192
	db	18
	db	0
	db	0
_DATA	ends
TOTSR_TEXT	segment byte public use16 'CODE'
	?debug	L 114
	assume	cs:TOTSR_TEXT
clock	proc	far
	?debug	B
	push	eax ; Oops... this was originally "push ax"
	push	ebx
	push	ecx
	push	edx
	push	es
	push	ds
	push	esi
	push	edi
	push	bp
	mov	bp,DGROUP
	mov	ds,bp
	mov	bp,sp
	sub	sp,6
	?debug	B
	?debug	L 118
	inc	dword ptr DGROUP:_ticks
	?debug	L 119
	mov	eax,dword ptr DGROUP:clock_interval
	add	dword ptr DGROUP:us_timer,eax
	?debug	L 120
	mov	eax,dword ptr DGROUP:us_timer
	cmp	eax,dword ptr DGROUP:tempo
	jbe	short @10@86
	?debug	L 121
	xor	edx,edx
	div	dword ptr DGROUP:tempo
	mov	word ptr [bp-2],ax
	?debug	L 122
	movsx	eax,word ptr [bp-2]
	mov	dword ptr [bp-6],eax
	imul	eax,dword ptr DGROUP:tempo
	sub	dword ptr DGROUP:us_timer,eax
	?debug	L 123
	mov	eax,dword ptr [bp-6]
	add	dword ptr DGROUP:timer,eax
@10@86:
	?debug	L 125
	cmp	byte ptr DGROUP:_callmidi,0
	je	short @10@142
	?debug	L 126
	call	far ptr _smidigo
@10@142:
	?debug	L 127
	cmp	dword ptr DGROUP:orig_clock,large 0
	je	short @10@198
	?debug	L 128
	pushf	
	call	dword ptr DGROUP:orig_clock
@10@198:
	?debug	L 129
	leave	
	pop	edi
	pop	esi
	pop	ds
	pop	es
	pop	edx
	pop	ecx
	pop	ebx
	pop	eax
	iret
	?debug	C E601740402FEFF00
	?debug	E
	?debug	E
clock	endp
	?debug	L 131
	assume	cs:TOTSR_TEXT
_um_set_tempo	proc	far
	?debug	B
	enter	4,0
	?debug	C E60764697669736F72040A0A00000275730C0A06+
	?debug	C 0000
	?debug	B
	?debug	L 141
	cmp	word ptr [bp+10],0
	jle	short @11@86
	?debug	L 142
	movsx	ebx,word ptr [bp+10]
	mov	eax,dword ptr [bp+6]
	xor	edx,edx
	div	ebx
	jmp	short @11@114
@11@86:
	?debug	L 144
	mov	ax,word ptr [bp+10]
	neg	ax
	sar	ax,8
	and	ax,127
	mov	word ptr [bp-2],ax
	?debug	L 145
	mov	ax,word ptr [bp+10]
	and	ax,127
	mov	word ptr [bp-4],ax
	?debug	L 146
	mov	ax,word ptr [bp-2]
	imul	word ptr [bp-4]
	movsx	ebx,ax
	mov	eax,large 0000F4240h
	cdq	
	idiv	ebx
@11@114:
	mov	dword ptr DGROUP:tempo,eax
	?debug	L 148
	cmp	dword ptr DGROUP:tempo,large 0
	ja	short @11@198
	mov	dword ptr DGROUP:tempo,large 1
@11@198:
	?debug	L 149
	leave	
	ret	
	?debug	C E610736D7074655F7265736F6C7574696F6E0402+
	?debug	C FCFF000C736D7074655F666F726D61740402FEFF+
	?debug	C 000275730C0A0600000764697669736F72040A0A+
	?debug	C 0000
	?debug	E
	?debug	E
_um_set_tempo	endp
	?debug	L 151
	assume	cs:TOTSR_TEXT
_um_reset_tick_counter	proc	far
	?debug	B
	?debug	B
	?debug	L 153
	mov	dword ptr DGROUP:timer,large 0
	?debug	L 154
	mov	dword ptr DGROUP:us_timer,large 0
	?debug	L 155
	ret	
	?debug	E
	?debug	E
_um_reset_tick_counter	endp
	?debug	L 157
	assume	cs:TOTSR_TEXT
_um_wait_for	proc	far
	?debug	B
	push	bp
	mov	bp,sp
	?debug	C E609736F6D655F74696D650C0A060000
	?debug	B
	?debug	L 160
	mov	eax,dword ptr [bp+6]
	cmp	eax,dword ptr DGROUP:timer
	jbe	short @13@86
	mov	ax,1
	jmp	short @13@114
@13@86:
	xor	ax,ax
@13@114:
	?debug	L 164
	pop	bp
	ret	
	?debug	C E609736F6D655F74696D650C0A060000
	?debug	E
	?debug	E
_um_wait_for	endp
	?debug	L 167
	assume	cs:TOTSR_TEXT
_um_init_hardware	proc	far
	?debug	B
	enter	14,0
	push	di
	?debug	C E319074D4944494C49425A001E01
	?debug	C E31B00000023040400
	?debug	C E31A000400161B00
	?debug	C E31D00000023010400
	?debug	C E31C000400161D00
	?debug	C E31F00000023010400
	?debug	C E31E000400161F00
	?debug	C E32100000023010400
	?debug	C E320000400162100
	?debug	C E32300000023010400
	?debug	C E322000400162300
	?debug	C E32500000023040400
	?debug	C E324000400162500
	?debug	C E32700000023010400
	?debug	C E326000400162700
	?debug	C E32900000023010400
	?debug	C E328000400162900
	?debug	C E32B00000023040400
	?debug	C E32A000400162B00
	?debug	C E32D00000023040400
	?debug	C E32C000400162D00
	?debug	C E32F00000023010400
	?debug	C E32E000400162F00
	?debug	C E33100000023010400
	?debug	C E330000400163100
	?debug	C E33300000023010400
	?debug	C E332000400163300
	?debug	C E33500000023010400
	?debug	C E334000400163500
	?debug	C E33700000023010400
	?debug	C E336000400163700
	?debug	C E33900000023010400
	?debug	C E338000400163900
	?debug	C E33B00000023040400
	?debug	C E33A000400163B00
	?debug	C E33D00000023010400
	?debug	C E33C000400163D00
	?debug	C E33F05747261636B1C001E1A
	?debug	C E340000400160801
	?debug	C E3410A706C61795F7374617465020022000080FF+
	?debug	C 7F24
	?debug	C E33E000400163F00
	?debug	C E318000400161900
	?debug	C E6026D6C180A060000
	?debug	B
	?debug	L 174
	mov	word ptr [bp-2],120
	jmp	short @14@114
@14@58:
	?debug	L 175
	push	word ptr [bp-2]
	call	far ptr _getvect
	pop	cx
	mov	word ptr DGROUP:_um_hook+2,dx
	mov	word ptr DGROUP:_um_hook,ax
	?debug	L 176
	mov	ax,word ptr DGROUP:_um_hook+2
	mov	word ptr [bp-4],ax
	mov	word ptr [bp-6],259
	?debug	L 177
	push	8
	push	ds
	push	offset DGROUP:s@
	push	ax
	push	word ptr [bp-6]
	call	far ptr _strncmp
	add	sp,10
	or	ax,ax
	je	short @14@142
	?debug	L 174
	inc	word ptr [bp-2]
@14@114:
	cmp	word ptr [bp-2],127
	jle	short @14@58
@14@142:
	?debug	L 179
	cmp	word ptr [bp-2],127
	jle short	@@13
	jmp	@14@674
@@13:
	?debug	L 180
	push	ds
	push	offset DGROUP:s@+9
	call	far ptr _midi_message
	add	sp,4
	?debug	L 181
	cmp	dword ptr DGROUP:_um_hook,large 0
	je	short @14@226
	?debug	L 182
	mov	ax,26
	?debug	L 183
	call	dword ptr DGROUP:_um_hook
@14@226:
	?debug	L 185
	push	cs
	call	near ptr _reset_um
	?debug	L 186
	cmp	dword ptr DGROUP:_um_hook,large 0
	jne short	@@14
	jmp	@14@702
@@14:
	?debug	L 187
	les	bx,dword ptr [bp+6]
	mov	eax,dword ptr es:[bx+72]
	jmp	short @14@590
@14@282:
	?debug	L 188
	les	bx,dword ptr [bp-10]
	mov	es,word ptr es:[bx+2]
	?debug	L 189
 	push	 es
	?debug	L 190
	les	bx,dword ptr [bp-10]
	mov	di,word ptr es:[bx]
	?debug	L 191
 	push	 di
	?debug	L 192
	les	bx,dword ptr [bp-10]
	mov	eax,dword ptr es:[bx+12]
	mov	dword ptr [bp-14],eax
	?debug	L 193
 	mov	 bx, word ptr [bp-14]+2
	?debug	L 194
 	mov	 dx, word ptr [bp-14]
	?debug	L 195
 	pop	 di
	?debug	L 196
 	pop	 es
	?debug	L 197
	mov	ax,10
	?debug	L 198
	call	dword ptr DGROUP:_um_hook
	?debug	L 187
	les	bx,dword ptr [bp-10]
	mov	eax,dword ptr es:[bx+24]
@14@590:
	mov	dword ptr [bp-10],eax
	cmp	dword ptr [bp-10],large 0
	jne	short @14@282
	?debug	L 200
	mov	ax,14
	?debug	L 201
	call	dword ptr DGROUP:_um_hook
	jmp	short @14@702
@14@674:
	?debug	L 204
	mov	dword ptr DGROUP:_um_hook,large 0
	?debug	L 205
	push	ds
	push	offset DGROUP:s@+52
	call	far ptr _midi_error
	add	sp,4
	?debug	L 206
	mov	ax,1
	jmp	short @14@730
@14@702:
	?debug	L 208
	xor	ax,ax
@14@730:
	?debug	L 209
	pop	di
	leave	
	ret	
	?debug	C E343000400160200
	?debug	C E6066C656E6774680C02F2FF0005747261636B3E+
	?debug	C 02F6FF00057374616D704302FAFF000676656374+
	?debug	C 6F720402FEFF00026D6C180A060000
	?debug	E
	?debug	E
_um_init_hardware	endp
	?debug	L 212
	assume	cs:TOTSR_TEXT
_um_cleanup	proc	far
	?debug	B
	?debug	B
	?debug	L 214
	push	cs
	call	near ptr _reset_um
	?debug	L 215
	cmp	dword ptr DGROUP:_um_hook,large 0
	je	short @15@58
	?debug	L 216
	mov	ax,27
	?debug	L 217
	call	dword ptr DGROUP:_um_hook
@15@58:
	?debug	L 219
	ret	
	?debug	E
	?debug	E
_um_cleanup	endp
	?debug	L 221
	assume	cs:TOTSR_TEXT
_um_init_timers	proc	far
	?debug	B
	?debug	B
	?debug	L 223
	push	8
	call	far ptr _getvect
	pop	cx
	mov	word ptr DGROUP:orig_clock+2,dx
	mov	word ptr DGROUP:orig_clock,ax
	?debug	L 224
	push	seg clock
	push	offset clock
	push	8
	call	far ptr _setvect
	add	sp,6
	?debug	L 225
	mov	dx,67
	mov	al,54
	out	dx,al
	?debug	L 226
	mov	dx,64
	mov	al,216
	out	dx,al
	?debug	L 227
	mov	al,38
	out	dx,al
	?debug	L 228
	ret	
	?debug	E
	?debug	E
_um_init_timers	endp
	?debug	L 230
	assume	cs:TOTSR_TEXT
_um_cleanup_timers	proc	far
	?debug	B
	?debug	B
	?debug	L 232
	mov	dx,67
	mov	al,54
	out	dx,al
	?debug	L 233
	mov	dx,64
	mov	al,0
	out	dx,al
	?debug	L 234
	out	dx,al
	?debug	L 235
	cmp	dword ptr DGROUP:orig_clock,large 0
	je	short @17@58
	push	dword ptr DGROUP:orig_clock
	push	8
	call	far ptr _setvect
	add	sp,6
@17@58:
	?debug	L 236
	ret	
	?debug	E
	?debug	E
_um_cleanup_timers	endp
	?debug	C E9
	?debug	C FA01000000
TOTSR_TEXT	ends
_DATA	segment word public use16 'DATA'
s@	label	byte
	db	'ULTRAMID'
	db	0
	db	'UltraMID is now loading instrument patches'
	db	0
	db	'Couldn'
	db	39
	db	't find UltraMID'
	db	0
_DATA	ends
TOTSR_TEXT	segment byte public use16 'CODE'
TOTSR_TEXT	ends
_tempo	equ	tempo
_timer	equ	timer
_us_timer	equ	us_timer
	public	_um_data_out
	public	_um_hook
_clock_interval	equ	clock_interval
_clock	equ	clock
_orig_clock	equ	orig_clock
	extrn	_ticks:word
	extrn	_smidigo:far
	extrn	_callmidi:byte
	public	_um_cleanup
	public	_reset_um
	public	_um_wait_for
	public	_um_reset_tick_counter
	public	_um_cleanup_timers
	public	_um_init_timers
	public	_um_init_hardware
	public	_um_set_tempo
	public	_um_note_off
	public	_um_note_on
	public	_um_parameter
	public	_um_pressure
	public	_um_chanpressure
	public	_um_program
	public	_um_pitch_bend
	extrn	_midi_message:far
	extrn	_midi_error:far
	extrn	_setvect:far
	extrn	_getvect:far
	extrn	_strncmp:far
_s@	equ	s@
	?debug	C EA010C
	?debug	C E34400000023040400
	?debug	C EC0C5F756D5F646174615F6F7574441800
	?debug	C E34600000023010400
	?debug	C E345000400164600
	?debug	C EC085F756D5F686F6F6B450000
	?debug	C E34700000023010700
	?debug	C E34900000023010700
	?debug	C E348000400164900
	?debug	C EB065F7469636B730600
	?debug	C E34A00000023010400
	?debug	C EB085F736D696469676F4A00
	?debug	C EB095F63616C6C6D6964690800
	?debug	C E34B00000023010400
	?debug	C EC0B5F756D5F636C65616E75704B1800
	?debug	C E34C00000023040400
	?debug	C EC095F72657365745F756D4C1800
	?debug	C E34D00000023040400
	?debug	C EC0C5F756D5F776169745F666F724D1800
	?debug	C E34E00000023010400
	?debug	C EC165F756D5F72657365745F7469636B5F636F75+
	?debug	C 6E7465724E1800
	?debug	C E34F00000023010400
	?debug	C EC125F756D5F636C65616E75705F74696D657273+
	?debug	C 4F1800
	?debug	C E35000000023010400
	?debug	C EC0F5F756D5F696E69745F74696D657273501800
	?debug	C E35100000023040400
	?debug	C EC115F756D5F696E69745F686172647761726551+
	?debug	C 1800
	?debug	C E35200000023010400
	?debug	C EC0D5F756D5F7365745F74656D706F521800
	?debug	C E35300000023010400
	?debug	C EC0C5F756D5F6E6F74655F6F6666531800
	?debug	C E35400000023040400
	?debug	C EC0B5F756D5F6E6F74655F6F6E541800
	?debug	C E35500000023010400
	?debug	C EC0D5F756D5F706172616D65746572551800
	?debug	C E35600000023010400
	?debug	C EC0C5F756D5F7072657373757265561800
	?debug	C E35700000023010400
	?debug	C EC105F756D5F6368616E70726573737572655718+
	?debug	C 00
	?debug	C E35800000023010400
	?debug	C EC0B5F756D5F70726F6772616D581800
	?debug	C E35900000023010400
	?debug	C EC0E5F756D5F70697463685F62656E64591800
	?debug	C E35A00000023010401
	?debug	C EB0D5F6D6964695F6D6573736167655A00
	?debug	C E35B00000023010401
	?debug	C EB0B5F6D6964695F6572726F725B00
	?debug	C E35C00000023010400
	?debug	C EB085F736574766563745C00
	?debug	C E35F00000023010700
	?debug	C E35E000400165F00
	?debug	C E35D000000235E0400
	?debug	C EB085F676574766563745D00
	?debug	C E36000000023040400
	?debug	C EB085F7374726E636D706000
	?debug	C E60574656D706F0C0000021400000574696D6572+
	?debug	C 0C0000021000000875735F74696D65720C000002+
	?debug	C 0C00000E636C6F636B5F696E74657276616C0C00+
	?debug	C 00020400000A6F7269675F636C6F636B48000002+
	?debug	C 0000000777636861725F740206000666706F735F+
	?debug	C 740606000673697A655F740A0600074D4944494C+
	?debug	C 494219070005747261636B3F07000A706C61795F+
	?debug	C 7374617465410700
	?debug	F clock 71 152 0
	?debug	C E200074D665F676574631A00084D665F6572726F+
	?debug	C 721C00094D665F6865616465721E00154D705F72+
	?debug	C 657365745F7469636B5F636F756E74657220000C+
	?debug	C 4D705F7365745F74656D706F22000B4D705F7761+
	?debug	C 69745F666F7224000E4D705F696E69745F74696D+
	?debug	C 6572732600114D705F636C65616E75705F74696D+
	?debug	C 65727328000D4D705F72657365745F6D6964692A+
	?debug	C 000A4D705F6E6F74655F6F6E2C000B4D705F6E6F+
	?debug	C 74655F6F66662E000B4D705F7072657373757265+
	?debug	C 30000C4D705F706172616D6574657232000D4D70+
	?debug	C 5F70697463685F62656E6434000A4D705F70726F+
	?debug	C 6772616D36000F4D705F6368616E707265737375+
	?debug	C 72653800104D705F696E69745F68617264776172+
	?debug	C 653A000A4D705F636C65616E75703C00094D705F+
	?debug	C 747261636B733E0008747261636B6E756D040010+
	?debug	C 4D705F6D61737465725F766F6C756D6504000A4D+
	?debug	C 665F6E6F6D6572676504000B4D665F746F626572+
	?debug	C 6561640600124D705F6E6578745F6576656E745F+
	?debug	C 74696D650CC05A000000
	?debug	C E2000464617461400005706C617970400008706F+
	?debug	C 736974696F6E0C00066C656E6774680C0009706C+
	?debug	C 61795F74696D650C000673746174757308401600+
	?debug	C 0000000A706C61795F737461746541000A6E6578+
	?debug	C 745F747261636B3EC01C000000
	?debug	C E400094D50535F424547494E0000000B4D50535F+
	?debug	C 504C4159494E47010080084D50535F444F4E4502+
	?debug	C 00
	end
