;****************************************************************************
;*   TITLE              Test Program                                        *
;*   SUB-TITLE          How to make driver function?                        *
;*   FILENAME           driver.asm                                          *
;*   DATE & TIME        12/06/92(SUN) 12:21                                 *
;*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           *
;****************************************************************************

driver_text     segment para public 'code'
                assume  cs:driver_text

                org     0h

                ;offset of each procedure from head of this file
                dw      _bell
                dw      _put_char
                dw      _add

                ;***** type 1 : void _bell(void) *****

_bell           proc    far
                push    bp
                mov     bp, sp
                mov     ah, 2
                mov     dl, 7
                int     21h
                mov     sp, bp
                pop     bp
                retf
_bell           endp

                ;***** type 2 : void put_char(char ascii) *****

_put_char       proc    far
                push    bp
                mov     bp, sp
                mov     ah, 2
                mov     dl, byte ptr[bp+6]
                int     21h
                mov     sp, bp
                pop     bp
                retf
_put_char       endp

                ;***** type 3 : int add(int x, int y) *****

_add            proc    far
                ;not necessary to reserve sp(only example)
                push    bp
                mov     bp, sp
                mov     ax, word ptr[bp+6]
                add     ax, word ptr[bp+8]
                mov     sp, bp
                pop     bp
                retf
_add            endp

driver_text     ends
                end

