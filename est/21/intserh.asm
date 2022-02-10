
SER_TEXT		SEGMENT		BYTE PUBLIC 'CODE'
SER_TEXT 	ENDS
SER_DATA		SEGMENT		WORD PUBLIC 'FAR_DATA'
SER_DATA		ENDS
ASSUME CS: SER_TEXT, DS: SER_DATA
		TOTAL		equ  64
		TOP     equ  63
		BOTTOM  equ  0

SER_DATA		SEGMENT
PUBLIC			_mport
				_mport  dw 0
				buffer	db TOTAL dup('@')
				front		dw BOTTOM
				back 		dw TOTAL
				count		dw BOTTOM
SER_DATA   ENDS

SER_TEXT		SEGMENT

PUBLIC _serisr
_serisr PROC	FAR
				push	ax
				push	dx
				push	si
				push	ds
				mov		ax, SER_DATA
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
_readbuf 	PROC	FAR
				push	bp
				mov		bp, sp
				push	si
				push	ds
				mov		ax, SER_DATA
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
				ret
_readbuf 	ENDP

SER_TEXT		ENDS
END
