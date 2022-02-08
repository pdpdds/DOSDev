#ifndef __INPUT_H
#define __INPUT_H

#define KEY_LETTER_Q                16
#define KEY_LETTER_P                25
#define KEY_LETTER_A                30
#define KEY_LETTER_D                32
#define KEY_LETTER_Z                44
#define KEY_LETTER_X                45
#define KEY_LETTER_C                46
#define KEY_LETTER_V                47
#define KEY_LETTER_S                31
#define KEY_CRSR_UP                 72
#define KEY_CRSR_DOWN               80
#define KEY_CRSR_LEFT               75
#define KEY_CRSR_RIGHT              77

#define UIFN_QUIT                   0
#define UIFN_PAUSE                  1
#define UIFN_UP                     2
#define UIFN_DOWN                   3
#define UIFN_LEFT                   4
#define UIFN_RIGHT                  5
#define UIFN_FIRE_CANNON            6
#define UIFN_FIRE_LASER             7
#define UIFN_FIRE_SIDE_LASER        8
#define UIFN_FIRE_MEGABLAST         9
#define UIFN_SHIELD                 10
#define UIFN_DEBUG                  11
#define UIFN_COUNT                  12

void input_init(void);
void input_close(void);
char input_pop_event(void);

extern unsigned char uifn_hold[UIFN_COUNT];

#endif
