
_TEXT		SEGMENT		BYTE PUBLIC 'CODE'
_TEXT 	ENDS
CONST 	SEGMENT		WORD PUBLIC 'CONST'
CONST		ENDS
_BSS		SEGMENT		WORD PUBLIC 'BSS'
_BSS		ENDS
_DATA		SEGMENT		WORD PUBLIC 'DATA'
_DATA		ENDS
DGROUP		GROUP		CONST, _BSS, _DATA
	ASSUME CS: _TEXT, DS: DGROUP
		TOTAL		equ  40
		TOP     equ  39
		BOTTOM  equ  0

_DATA		SEGMENT
PUBLIC  _mport
				_mport  dw 0
				buffer	db TOTAL dup('@')
				front		dw BOTTOM
				back 		dw TOTAL
				count		dw BOTTOM
_DATA   ENDS

_TEXT		SEGMENT

PUBLIC _serisr
_serisr PROC	NEAR
				push	ax
				push	dx
				push	si
				push	ds
				mov		ax, DGROUP
				mov 	ds, ax

				mov		dx, _mport
				in		al, dx

				mov 	si, front
				mov		buffer[si], al

				inc 	front
				inc		count
				cmp		front, TOP
				jbe		front_below
				mov 	front, BOTTOM
front_below:
				cmp		count, TOTAL
				jbe		count_below
				mov		count, TOTAL
count_below:
				mov		al, 20h
				out		20h, al
				pop		ds
				pop		si
				pop		dx
				pop		ax
				ret
_serisr ENDP

PUBLIC	_readbuf
_readbuf 	PROC	NEAR
				cli
				push	bp
				mov		bp, sp
				push	si
				push	ds
				mov		ax, DGROUP
				mov		ds, ax
				xor		ax, ax

				cmp		count, BOTTOM
				jz    back_below

				mov		si, back
				mov		al, buffer[si]
				dec 	count
				inc		back
				cmp		back, TOP
				jbe		back_below
				mov		back, BOTTOM
back_below:
				pop		ds
				pop		si
				mov		sp, bp
				pop		bp
				sti
				ret
_readbuf 	ENDP

_TEXT		ENDS
END
