#include "hanio.h"

void set_internal_font(void)
{
   han1_font = hangul_font;
   han2_font = (char *) han1_font + NO_BUL1*(NO_HAN1+1)*HAN_FONT_SIZE;
   han3_font = (char *) han2_font + NO_BUL2*(NO_HAN2+1)*HAN_FONT_SIZE;
   eng_font  = english_font;
   spc_font  = special_font;
}
