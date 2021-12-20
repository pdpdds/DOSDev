#include <dos.h>
#include "input.h"

unsigned char uifn_hold[UIFN_COUNT];
static char ui_event;
static void interrupt (*input_old_kbd_irq)();

static int input_handle_key() {

    int key;

    asm {
    
        sti   
        in      AL, 60h
        xor     AH, AH   
        mov     key, AX   
        in      AL, 61h   
        or      AL, 82h
        out     61h, AL   
        and     AL, 7fh   
        out     61h, AL   
        mov     AL, 20h   
        out     20h, AL   
     }

     return key;
}

static void interrupt input_kbd_irq() {

    int key = input_handle_key();
    int uifn_index;
    
    switch (key & ~0x80) {
    
        case KEY_LETTER_Q:      uifn_index = UIFN_QUIT; break;
        case KEY_LETTER_P:      uifn_index = UIFN_PAUSE; break;
        case KEY_LETTER_A:      uifn_index = UIFN_SHIELD; break;
        case KEY_LETTER_D:      uifn_index = UIFN_DEBUG; break;
        case KEY_LETTER_Z:      uifn_index = UIFN_FIRE_CANNON; break;
        case KEY_LETTER_X:      uifn_index = UIFN_FIRE_LASER; break;
        case KEY_LETTER_C:      uifn_index = UIFN_FIRE_SIDE_LASER; break;
        case KEY_LETTER_V:      uifn_index = UIFN_FIRE_MEGABLAST; break;
        case KEY_CRSR_UP:       uifn_index = UIFN_UP; break;
        case KEY_CRSR_DOWN:     uifn_index = UIFN_DOWN; break;
        case KEY_CRSR_LEFT:     uifn_index = UIFN_LEFT; break;
        case KEY_CRSR_RIGHT:    uifn_index = UIFN_RIGHT; break;
        default: return;
    }

    if (uifn_index == UIFN_PAUSE || uifn_index == UIFN_DEBUG)
        if (key & 0x80)
            ui_event = uifn_index;
    
    uifn_hold[uifn_index] = !(key & 0x80);
}

void input_init(void) {

    ui_event = 0xff;
    input_old_kbd_irq = getvect(0x09);
    setvect(0x09, input_kbd_irq);
}

void input_close(void) {

    setvect(0x09, input_old_kbd_irq);
}

char input_pop_event(void) {

    char result = ui_event;
    ui_event = 0xff;
    return result;
}
