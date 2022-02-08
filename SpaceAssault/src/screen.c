#include <dos.h>
#include "screen.h"

char far *g_screen = (char far *)0xb8000000;

static char             screen_buff[160];
static char             *screen_buff_ptr;
static unsigned char    screen_buff_len;
static char             screen_attr;

void screen_init(void) {

    _AX = 0x1112;
    _BX = 0;
    geninterrupt(0x10);
}

void screen_close(void) {

    _AX = 0x0003;
    _BX = 0;
    geninterrupt(0x10);
}

void screen_clear(char ascii, char color) {

    asm {
    
        push ES

        mov AL, ascii
        mov AH, color
        mov BX, 0B800h
        mov ES, BX
        xor DI, DI
        mov CX, 80*50
        cld
        rep stosw
        
        pop ES
    }
}

void screen_blit(const char *data, unsigned int start, char w, char h) {

    asm {

        push ES

        // step to be added to DI after every line (DX := 160 - 2 * w)
        mov BL, w
        xor BH, BH
        shl BX, 1
        mov DX, 160
        sub DX, BX

        // base destination address at the start of the screen
        mov BX, 0B800h
        mov ES, BX
        mov SI, data
        mov DI, start

        // prepare counters
        xor CH, CH
        mov BL, h
        cld
    }
    _nextline: asm {

        mov CL, w
        rep movsw
        add DI, DX
        dec BL
        jnz _nextline
        
        pop ES
    }
}

void screen_fill_region(char ascii, char color, unsigned int start, char w, char h) {

    asm {
    
        push ES

        // store char, color and screen offset to be used in STOSW later
        mov AL, ascii
        mov AH, color
        mov DI, start
        
        // step to be added to DI after every line (DX := 160 - 2 * w)
        mov BL, w
        xor BH, BH
        shl BX, 1
        mov DX, 160
        sub DX, BX

        // base destination address at the start of the screen
        mov BX, 0B800h
        mov ES, BX
        
        // prepare counters
        xor CH, CH
        mov BL, h
        cld
    }
    _nextline: asm {

        mov CL, w
        rep stosw
        add DI, DX
        dec BL
        jnz _nextline

        pop ES
    }
}

void screen_wait_retrace() {

    while((inp(0x03da) & 0x08))
        ;
    while(!(inp(0x03da) & 0x08))
        ;
}

static void screen_buff_text(const char *text) {

    unsigned char i;

    screen_buff_ptr = screen_buff;
    screen_buff_len = 0;
    while (*text) {
    
        screen_buff[screen_buff_len++] = *text++;
        screen_buff[screen_buff_len++] = screen_attr;
    }
}

static void screen_buff_hex16(unsigned int value) {

    static char digit[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

    screen_buff_ptr = screen_buff;
    screen_buff_len = 8;

    screen_buff[7] = screen_attr;
    screen_buff[5] = screen_attr;
    screen_buff[3] = screen_attr;
    screen_buff[1] = screen_attr;
    
    screen_buff[6] = digit[value & 0x0f];
    value >>= 4;
    screen_buff[4] = digit[value & 0x0f];
    value >>= 4;
    screen_buff[2] = digit[value & 0x0f];
    value >>= 4;
    screen_buff[0] = digit[value & 0x0f];
}

static void screen_print_buff(unsigned int start) {

    asm {

        push ES
    
        // prepare source data and destination indexes (DS being already set)
        mov SI, screen_buff_ptr
        mov DI, start

        // base destination address at the start of the screen
        mov BX, 0B800h
        mov ES, BX

        // string length
        xor CH, CH
        mov CL, screen_buff_len

        // copy to screen
        rep movsb
        
        pop ES
    }
}

void screen_color(char color) {

    screen_attr = color;
}

void screen_print_text(char x, char y, const char *text) {

    screen_buff_text(text);
    screen_print_buff(screen_offset_xy(x, y));
}

void screen_print_hex16(char x, char y, unsigned int value) {

    screen_buff_hex16(value);
    screen_print_buff(screen_offset_xy(x, y));
}

void screen_print_uint(char x, char y, unsigned int value) {

    // TODO
    value = value;
//    screen_buff_uint(value);
    screen_print_buff(screen_offset_xy(x, y));
}

void screen_print_bar(char x, char y, char total, char filled) {

    unsigned int start = screen_offset_xy(x, y);
    char rest = total - filled;

    asm {

        push ES
    
        mov DI, start
        mov BX, 0B800h
        mov ES, BX

        xor CH, CH
        mov AH, screen_attr

        mov CL, filled
        mov AL, 177
        rep stosw
        
        mov CL, rest
        mov AL, ' '
        rep stosw
        
        pop ES
    }
    
}
