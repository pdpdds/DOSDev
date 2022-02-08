#include <dos.h>
#include <stdlib.h>
#include <conio.h>

#include "hanio.h"
#include "extendio.h"
#include "misc.h"
#include "keydef.h"
#include "mylib.h"

#define default_color() set_vbcolor_han(BLACK, LIGHTGRAY)

static void interrupt (*old_isr09)(void);
static void interrupt new_isr09(void);
void setintr_new(void);
void setintr_old(void);

void init(void);
void status_line(void);
void logo_display(void);
void logo2_display(void);
void draw_game_screen(void);
void f_display(void);
void disp_game_quiz(int num);
void result_dis_page0(void);
void result_dis_page1(void);
void result_dis_page2(void);
void result_dis_page3(void);
void delete(int del_top, int del_left, int del_right);

int item_num[10];
int sex=0;
char name[10];
char pi[3];


char ss1[8][65+1] = {{"¸a·A³‰A µe¬wÐ º¯³¯¡µ¡."},
		     {"Çe ‰·¶¥·¡ ·¶¯s“¡”a."},
		     {"‰·¶¥·a¡ —i´áˆa“e ¢…·e 3ˆa»¡ˆa ·¶‰¡ ´á—¡¡—e —i´áˆi ® ·¶·s“¡”a."},
		     {"‰·¶¥´e·e £¡¡Àáœñ –A´á ·¶·a¡a ¬e, ˆw, Ñ¡®, ®œ—i, •·Š‰, ‰¡¬÷,"},
		     {"ÓA¯¢Àá ‹aŸ¡‰¡ µaœáˆa»¡ •·¢‰—i•¡  g·¡ ·¶·s“¡”a."},
		     {"¢‰¥ ‹a¶A ”aŸe ¸£¡·¶“e ˆõ·¡a ¬wÑ×·i ¬w¬wÐ•¡ ¹½¯s“¡”a."},
		     {"”a·q »©¢…µA Ðaa³¢ ”sÐ ˆa¡e¬á ‰·¶¥·a¡ —i´áˆa¡e –S“¡”a."},
		     {"¸a, ‹aœñ »iˆá¶…  a·q·a¡ ¡¡Ðñ· aœa¡ ˆa ¥³¯¡”a."}};

char ss_quest[10][3][45] ={{{"1. ‰·¶¥·a¡ —i´áˆa“e ·³Š“e ­A‰µ·¡ ·¶·s“¡”a."},
			  {"   ”w¯¥·e ´á“a ·³Š¡ —i´áˆa‰¡ ¯¼¯s“¡Œa?"}},
			 {{"2. ·³ŠµA¬á ·³¸wžaŸi ´¡ Ðs“¡”a."},
			  {"   ´é a·©Œa¶a?"}},
			 {{"3. ·³ŠµA¬á ‹©·¡ ­A ‰µ·a¡ a ·¶·s“¡”a."},
			  {"   ´á“a ‹©·i È‚Ða‰V¯s“¡Œa?"}},
			 {{"4. ¹·¡ Ï¡´á ·¶·s“¡”a."},
			  {"   ´á˜å ¬‚ ¹·©Œa¶a?"}},
			 {{"5. ®œ·¡ ·¶·s“¡”a."},
			  {"   ´á˜å ®œ·©Œa¶a?"}},
			 {{"6. ®œ­¢µA •·¢‰·¡ ·¶·s“¡”a."},
			  {"   ´á˜å •·¢‰ ·©Œa¶a?"}},
                         {{"7. ´|µA §¥Èáˆa aµ© ˆõ ˆ{·s“¡”a."},
                          {"   ´á˜å ‰µ·©Œa¶a?"}},
                         {{"8.  x·e ÍeµA¬á ¬aœq·¡ ˆé´áµ¡‰¡ ·¶·s“¡”a."},
                          {"   ´á˜å ¬aœq·©Œa¶a?"}},
                         {{"9. ”w¯¥·e ‹a ¬aœq‰Á ´á˜ý‰A Ða¯¡‰V·s“¡Œa?"}},
                         {{"10.  éŸ¡ ´|µA ˆå¢‰·¡ ¥¡·¡‹¡ ¯¡¸bÐs“¡”a."},
                          {"   ´á˜å »³·©Œa¶a?"}}};

char ss_item[10][3][40]={{{" •·½¢ ·³Š"},
                          {" q½¢ ·³Š"},
                          {" ¬á½¢ ·³Š"}},
						 {{" 300¶¥"},
                          {" 1,000¶¥"},
                          {" 2,000¶¥"}},
                         {{" ¶E½¢· Š¦‰Š¦‰Ðe ¹³·e ‹©"},
                          {" º—´w· ™¢¤a¡ ¨è·e Í¡¸w–E ‹©"},
                          {" µ¡Ÿe½¢·a¡ Ð·Ða“e ¸å w·¡ ¹½·e‹©"}},
                         {{" Ìaœe¬‚"},
                          {" ¨iˆe¬‚"},
			  {" ‘¡œe¬‚"}},
                         {{" Ã±µs®¡ –E ´á–¶… ®œ"},
                          {" µi”· ¸÷‹i"},
                          {" ¤j·e ¸s¡¢Ÿ±"}},
                         {{" ¬eÉ¡¡"},
			  {" “b”"},
                          {" Ì…”á‰±"}},
                         {{" ‰×¸w"},
                          {" Ä‘ÏaÁ¥"},
			  {" ´a¢•¡ ´ô“e ¹¡¶wÐe ‰µ"}},
                         {{" µa¸a"},
                          {" q¸a"},
                          {" ®œ­¢· ¶a¸÷"}},
                         {{" ‹a ¬aœq‰Á Ðq‰A ¡¡Ðñ·i ‰­¢Ðe”a."},
                          {" ˆa¥s‰A ·¥¬aÐa‰¡ ¸a¯¥· ‹©·i ˆe”a."},
                          {" ¡¡ŸeÁA Ða‰¡ »¡aˆe”a."}},
                         {{" Ñ×ÍÐe ‰¡¬÷"},
                          {" ¹¡¶wÐe ¸é"},
			  {" ÑÁaÐe §©—·"}}};

char ss_dis0[][50] = {{"·¡¹A ”w¯¥·¡ È‚Ðe ˆå¢‰ ­¢µA¬á ¸qŒe ®á¯³¯¡µ¡."},
		      {"‰·¶¥´e·e ´aº ‹¼·a“¡Œa µa‹¡¬á Ñ¡‹¡¯±·¡ ˆwÐe ¬aœq"},
		      {"—i·i ¶áÐaµa µa‹¡ˆa»¡ »©¢…µA ”sÐe ¶w·i É¡”¡ ”w"},
		      {"¯¥· ¬÷‰b·i ”sÐ —aŸ¡‰V·s“¡”a."}};

char ss_dis1[][3][3][75] ={{{{"  ”w¯¥·e »¡¸â·¥ Èa·³·a¡ Ñ¥¸a¬á ¬a¬‚µA ¸q‹¡ˆáa ¡·¬w·i »i‹¡“e ·©·¡  g»¡"},
							 {"  ´g¯s“¡Œa?"}},
							{{"  ”w¯¥·e ¤j‰¡ ¬aŠa¸â·¥ ¬÷‰b·a¡ ÑÁaÐe “a±·i Î—‹¡»¡ e ¯©·e ¶A¡¶¡ Ða“e"},
							 {"  ·©·¡  g·¡ ·¶·s“¡”a."}},
							{{"  ”w¯¥·e  a·q·¡ ˜a›uÐa‰¡ ˆa¸÷¸â·¥ ¬aœq·³“¡”a. ‹aœáa ’ŠµA‰Aa á¢ ˜a"},
							 {"  ›uÐa‰A ”Ð º”a ¥¡¡e ­¥Ð¥¡“e ·©•¡  g»¡ ´g¯s“¡Œa?"}}},
						   {{{"  á¢ ·¥¬‚ÐaŠ…¶a. Ð·¸â·¡¡a ¶®µA Àe “a±·i º‹¡ ®á¶… ”w¯¥·e ­¥¸º"},
							 {"  ˆa µaœá¡¡¡ šá´áe ¸“w·i ¥¡·³“¡”a."}},
							{{"  Íw¤ñÐe ¬—ˆb·i ˆa»¥ ¬aœq·¡Š…¶a. Ó±—e ­A¬w·i ¬i´aˆa“e •A ”w¯¥· ˆå¸åÐe"},
							 {"  Ìe”eb·¡  ¶ º—¶aÐi ˆõ·³“¡”a. ¸i ÑÉ¶wÐ º¯³¯¡µ¡. "}},
							{{"  ¬÷‰b ÉA¯aËaŸi ¶áÐ ‹aœý‰A  g·e •¥·i “e ”w¯¥·¡œa¡e ¸ñ®‰·i á¢ £¨»¡"},
							 {"  ´g•¡¢ Ð´¡ Ði ˆõ·³“¡”a."}}},
						   {{{"  ¡ …Ë¢Ða‰¡ ¬w¬wb·¡ Î—¦Ð Ž‘·i ¸i Ž‰VŠ…¶a. ‹aœáa Ñe¯©·e —¸÷Ðs“¡"},
							 {"  ”a. §¡¢ Ó±—i ”áœa•¡ ”á ¸a¯¥· ¬w¬wb·i Â—¦…Ó¡ ÑÉ¶wÐi ® ·¶“e ·©·i Àx"},
							 {"  ´a º¯³¯¡µ¡."}},
							{{"  µa‹¡“e ¯aÏ¡—aŸi ‰ ® ·¶“e ‹©·¡ ´a“³“¡”a. á¢ ´|·i Ð·Ð •©»¥Ða“e ˆõ"},
							 {"  ·e ¶áÐñ Ðs“¡”a. ¸â‹b¸â·¥ ”w¯¥· ¬÷‰b·e ¹½»¡ e –I ® ·¶“eÐe ¯¥º—Ó¡ Ð—•·"},
							 {"  Ð º¯³¯¡µ¡."}},
							{{"  Ñe¯©·i »¢¯¡Ði º‰ ´a“e ¬aœq·³“¡”a. ¬÷¯©Ða‰¡, ¢´õ·i ¦ÈbÐ•¡ ´e¯±Ði ® "},
							 {"  ·¶“e ¬aœq·³“¡”a. ‹aœáa ÐqA ‘¡“e •A“e ¸£¡´ô“e ¬aœq·³“¡”a. ˆaq·e"},
							 {"   a·qŒõ ¡¡Ðñ•¡ Ð ¥¡¯³¯¡µ¡."}}},
						   {{{"  ¶´aÐa‰¡ Ê¡wÐe ´aŸq”a¶‘·i ¹½´aÐa“e ¬aœq, ´á“a½¢·¡¡ Ða¡e, Ñ¥¸a·¶‹¡Ÿi"},
							 {"  ¹½´aÐs“¡”a. ËiŸ³“¡Œa?"}},
							{{"  ¸÷i¸â·¡‰¡ ·¡¬÷¥¡”a ˆq¸÷·i º—¯¡Ða“e ¬aœq,  g·e Ã¥Š—i‰Á »iˆó‰A ´á¶‰Ÿ¡"},
							 {"  “e ˆõ·i ¹½´aÐs“¡”a. ‹aœýºa?"}},
							{{"  ¤å›b·¡“e »¢ˆqb·i ˆa»¥ ¬aœq. ‹aœáa ¹½‰¡ ¯°·e ¬aœq· Èa·³·¡ á¢ š"},
							 {"  uÐa‰A Š¦… –A´á ·¶“e ˆõ ˆ{Š…¶a."}}},
						   {{{"  ”w¯¥·e  a·q­¢µA µaœáˆa»¡ ˆa“w¬÷·i ˆqÂ‰¡ ·¶·s“¡”a. ¸a‹¡ ¸a¯¥µA ”Ð"},
							 {"  ”á ¬—ˆbÐ ¥¡¯³¯¡µ¡. ¬¡¶… ¸a¯¥·i ¤i‰eÐi ˆõ·³“¡”a."}},
							{{"  ˆ¤w¸â·¡‰¡ ¸â‹b¸â·¥ ”w¯¥·e ¯a¯a¡ £¡œŸi ˆÀâÐa“e Èa·³·³“¡”a. Ó±—e "},
							 {"  ·©·i ˆó»¡  i‰¡ ÀâÀâ ´|·a¡ aˆa ¥¡¯³¯¡µ¡. ”e ¸÷‹iµA“e  —®a •¢¬a"},
							 {"  •¡ ·¶·a“¡Œa º·Ð ¥¡¯³¯¡µ¡."}},
							{{"  ”w¯¥·e ¡¡—e·©·¡ ¥¢¸sÐa‰A ¬—ˆbÐa»¡ ´g·a¡a ¸a¯¥·  a·q·i ®‘‹¡a Ða»¡ ´g"},
							 {"  “e ­©»¢Ðe ¬aœq·³“¡”a. ¸s¡¢Ÿ±·e ‰a¶‰·¡ –A¡e ·¼·¡ ˜é´á¹a Ð•¥{·¡ ¸i §¡"},
							 {"  Ã¡“e •A ¤j‰¡ bÀå¸â·¥ ”w¯¥·e Ñe¯©µA•¡ ¸i ¸â·wÐ aˆs“¡”a."}}},
						   {{{"  ¹± ˆó·¡  g‰¡ º· ‹¼·e Íe·¡Š…¶a. Àá·q  ea“e ¬aœqµA‰A“e ‹a”a»¡ ¹½·e “a"},
							 {"  ±·i º»¡ ¡µÐa»¡ e, ¬aŠá´á ¥©®¢ Ã¥¸éÐa‰¡ ®ó‰A Óe—iŸ¡»¡ ´g“e ¬aœq·±"},
							 {"  ·i ´i ® ·¶·s“¡”a."}},
							{{"  ¸÷·ˆq·¡ ˆwÐa‰¡ ¬w¬wb·¡ Î—¦Ða¡a ¡¡EË¢Ðe Íe·³“¡”a. Ã±Àb¬÷·¡ ´ô‰¡ ¯±"},
							 {"  ®‰ Š˜‰A ¥¡·¡“e ¡e•¡ ·¶»¡ e  a·q·e ¬w”wÐs“¡”a."}},
							{{"  ·¡œå ‰µµA¬á Ì…”á‰±·¡ ·¶·iŸ¡ ´ô‰V»¡¶a. ¬w¬wb·¡ §¡´¢–A´á Ñe¯©¬÷·¡ ´ôŠ…"},
							 {"  ¶a ‹aœáa 10­A ·¡Ða ´áŸ¥´· ””s·¡œa¡e ­©»¢Ðe ””s·¡œa Ði ® ·¶·i ˆõ"},
							 {"  ·³“¡”a."}}}};

char ss_dis2[][4][75] ={{{"  ”w¯¥·e ‰¨ ¸i ¶A¡¶¡Ða¡a ´áŸ¡‰×•¡ ¦Ÿ¡»¡ e ’Š…ˆa Ò‰ŸWÐe ¬aœq·i º—¯±·a"},
						 {"  ¡ ‹až“»¡´á ÑÉ•·Ða‹¡Ÿi ¹½´aÐs“¡”a. ·Ÿ¡ g‰¡ ·¥¸÷·¡  g·a¡a »¡•¡¸aa "},
						 {"  ¬å¤Ÿi ¸i ˜aŸa“e ¡e•¡ ·¶¯s“¡”a. ‹aœáa ¸â·¡  g‰¡ ¹½´aÐa“e Èa·³‰Á ¯°´á"},
						 {"  Ða“e Èa·³·¡ ÑÂ¯©Ða‰A Š¦…–A´á ·¶·s“¡”a."}},
						{{"  ·¡¬w¸â·¡¡a ‹až“· ‹AÃ¢·i ¸i »¡Ç¡“e µ· ¤aŸe ¬aœq·³“¡”a. Ñ¥¸a¬á ¢¥ˆa"},
						 {"  ·©·i Ða‹¡¥¡”a ‹až“µA¬á £¥º¸â·a¡ Ð—•·Ða‹¡Ÿi ¹½´aÐs“¡”a. ¬w”¤w·"},
						 {"  ·³¸w·i ¹¥º—Ða‰¡ Àõ ”¡eÐa“e ¬aœq‰Á  ¶ »iˆó‰A ¬aŠé® ·¶‰VŠ…¶a."}},
						{{"  ”w¯¥·¡ Ñ¥¸a¬á ‰©™©Ó¡ ¬—ˆbµA ¸q‹¡“e ÀéÐb¸a ·³“¡”a. ¡w¬wµA ‹¼·¡ ¸q‰a ·¶"},
						 {"  “e ‹¡¦…·¡a Ž‘ˆ{·e ÑÅ¬wµA ¸q‹¡‹¡Ÿi ¹½´aÐs“¡”a. ¸a¯¥· ¬—ˆb·¡a “a±·i"},
						 {"  qµA‰A  iÐa“e ·©•¡ ·¶»¡ e  a·qµA ˆqÂ´á –“e ·©·¡ ”á  g¯s“¡”a."}}};

char ss_dis_f[][4][75] = {{{"  µa¬÷·¥ ”w¯¥·¡ ”a·q‰Á ˆ{·e µa¬÷·i  ev·i ˜µA“e ËiŸ±´ô·¡  i¬w”ˆa Ï©¶a"},
			   {"  Ða‰V»¡¶a. ‹a ¬aœq‰Á ·¡´¡‹¡ˆa ¸i É·Ða¡e ‹a ¬aœq·i É·Ð ·¡¬÷·i  ea‰A"},
			   {"  –I»¡•¡ ¡¡Ÿs“¡”a. ˆó»¡  i‰¡ ¹±”á ¸â‹b¸â·a¡ ¸ó‹eÐ ¥¡¡e ´á˜éŒa¶a!"}},
			  {{"  µa¬÷·¥ ”w¯¥·¡ q¬÷·i ¶µeÓ¡  ev·i ˜µA“e º·Ð º¯³¯¡µ¡. ¸q¥¢Ða‰¡"},
			   {"  ‹¡”aŸ¡‰¡ ·¶”å “b”·¥»¡•¡ ¡¡Ÿs“¡”a. ·¡ ­A¬wµA“e q¸aˆa  g·¡ ·¶·s“¡”a."},
			   {"  ·¡ ¬aœq Ðe ¬aœq e q¸a“e ´a“³“¡”a. ´á¼–—e á¢ ‹¡”Ð–”aˆa aº—µA ¯© w"},
			   {"  Ða»¡ ´g•¡¢ º·Ða¯³¯¡µ¡."}},
			  {{"  q¬÷·¥ ”w¯¥·¡ µa¬÷·i  ev·i ˜“e ”á¶‚ ”á ¹½·i ˆõ·³“¡”a. ‹aœáa ®œ­¢µA"},
			   {"  ¬á  ee µa¬÷µA‰A ˆs¸a‹¡ Ã¥Ð»¡‹¡Ÿi ¤aœ¬á“e ´e–S“¡”a. ‹a‘a“e ”w¯¥·i"},
			   {"  Ìa¡i¡ ·¡a“e  a‘a·©»¡•¡ ¡¡Ÿs“¡”a. ¬á–Ÿa»¡  i‰¡ ¬w”¤w·i ‰ÅÀiÐ ¥¡¯³"},
                           {"  ¯¡µ¡."}},
                          {{"  q¬÷·¥ ”w¯¥·¡ q¬÷·i  ev·i ‰w¶µA“e ·©·i Ða“e•A ¬á¡ •³“e ¬a·¡ˆa –I "},
                           {"  ®•¡ ·¶·s“¡”a. ¹½·e ¬aœq·e  ea‹¡ ´ás¯s“¡”a ´áa¶‰ ˜ ·»¡ˆa –I ®"},
                           {"  ·¶“e ¬aœq·¡¡e ”á¶‚ ¹½‰V»¡¶a."}},
                          {{"  ¶a¸÷·¡œa ¬—ˆbÐa“e ”w¯¥·e Àq §¡Ñe¯©¸â·¥ ¬aœq·© ˆõ ˆ{¯s“¡”a. ‹aœáa ”w"},
                           {"  ¯¥·e µw¶¥Ó¡ ´áŸ¥´  a·q·i ˆx“e ´aŸq”a¶… ¬aœq·¡œa‰¡ ¬—ˆbÐs“¡”a. ‰iÅ¡"},
			   {"  ·A´a¸â·¥ ¬aœq·¡ ´a“¥,  e´¢ ”w¯¥·¡ ­EÃ¡Ðe bÀåº·¸aˆa ´a“¡œa¡e ¯¡·¥·¡"},
                           {"  –I ®•¡ ·¶·i ˆõ·³“¡”a."}}};

char ss_dis4[][3][5][75] = {{{{"  ¸â‹b¸â·¡Š…¶a. ¹½¯s“¡”a. ‹aœáa ¹± ”éœ÷”“e ˆõ ˆ{Š…¶a ´|·a¡ ¡¡ÐñµA“e "},
			      {"  ¹±”á ¯¥º—Ða‰A, –¬aœq·¡ ·¶´á¬á•¡ ¥¡”a Ã±ÀbÐa‰A ¬—ˆbÐa‰¡ a¬á Ð—•·Ða“e "},
                              {"  ˆõ·¡ ¹½·i ˆõ·³“¡”a."}},
                             {{"  µ· ¤aŸe ¬aœq·¡Š…¶a. ¯¥º—Ða‰¡ º·‹¼‰A Ð—•·Ða“e ˆõ·e a«a»¡ ´g»¡ e ˆa"},
                              {"  q ÑÉ•·¸â·¥ ¬aœq·¡ –A´á ¥¡“e ˆõ•¡ ¹½·i ˆõ·³“¡”a. á¢ ­¡‹b¸â·¥ Ð—•··i "},
                              {"  ¥¡·¡¡e ¶…¡w· Ìa•¡µA Ðe ¡¤ ¡»¡•¡ ¡µÐa‰¡ aº—µA ÒÒAÐi ®•¡ ·¶·s“¡”a."}},
                             {{"  —¸÷Ðe ¬aœq·¡Š…¶a ‹aœáa ‹a ‹¡¦…·e ·¡Ð Ði ® ·¶·s“¡”a. ”w¯¥·e ¹± Œa"},
                              {"  ”a³‰¡ Âá£¡Ÿi ŠáÀg´a Ða“e ¬aœq·³“¡”a. ‹aœ¬á ”á¶‚ ”á ¶A¡¶¡ Ða‹¡•¡ Ða"},
                              {"  »¡ e ¬a¯©·e  i·i ˆé‰¡ ¯¼´á•¡ µå»¡ ‹¡ÒAŸi ‘½Ã¡“e Èa·³·¡ ´a“³“¡Œa? ‰A”a"},
                              {"  ˆa ¡¡Àáœñ Ñ¥¸a¬á ¡¡Ðñ·i ¯¡•¡Ð µÖ“e•A ¯¥‰w»©¸â·¥ ”w¯¥·¡ ¸i´i»¡•¡ ¡µ"},
			      {"  Ða“e ¬aœq‰Á ÐqA »i‹© ® ·¶·i »¡ ¡¡Ÿa‰VŠ…¶a."}}},
                            {{{"  Ñ¡‹¡¯±·¡  gŠ…¶a. ¶wˆqÐa‰¡ ¸â‹b¸â·¡¡a Ž‘·¡ ·¶“e ”w¯¥ˆ{·e Èa·³·e ¡¡Ðñ·"},
                              {"  aœaµA“e ´e¬÷  aÂ‘·¡»¡ e Ñ¡‹¡¯±·¡ á¢ ˆwÐs“¡”a. á¢ ¸a¯¥  e eÐ Ða»¡ "},
                              {"   i•¡¢, ‹aˆõ·¡ ·¥¬—µA¬á ¬÷‰·Ða“e ¶¥Àå·³“¡”a."}},
                             {{"  ¡w¬w¸â·¥ ¬aœq·¡Š…¶a. ”áŠa ¡¡EË¢Ðe ¬aœq·³“¡”a. ¶A˜e ‰µµA Ñ©¡ ¬á ·¶"},
                              {"  “e ¸é ‹a ­¢µA ’ˆa ¬i‰¡ ·¶·i Œa¶a. ”w¯¥· ¶…–…¸â·¥ ‹¡¦…·e ´i»¡ e ¸qŒe "},
                              {"  ®á‰¡ ¡¡Ðñ·i ‰­¢Ð º¯³¯¡µ¡."}},
                             {{"  ·¡œå ®œµA¬á ÑÁaÐe §©—··i ¤i‰eÐa”a“¡, á¢ §¡´¢¸â·¡Š…¶a. ‹aœý»¡ ´g·a"},
                              {"  ¡e ´aŒa  ee ‹a‘aµÁ Ðq‰A ¥¡‰ ¬—ˆb·³“¡Œa? ‹aˆõ·e ›u”¡ –A“e ·©·¡ ´a"},
			      {"  “³“¡”a.  i·¡ ¸i É·Ðe”a“e ˆõ·¡ µ¡Ó¡a ®¬wÐs“¡”a. ­¢»¡ ´g•¡¢ º·Ð"},
			      {"  º¯³¯¡µ¡."}}}};
int main(void)
{
   int i, j;

   if(init_han() != OK_HAN)	   exit(2);
   if(get_abs_maxx_han() != 79)
	   if(!getch())	getch();
   harderr(error_handler);

   setintr_new();

   logo_display();   if(!getch()) getch();
   logo2_display();   delete(5,20,60);
   init();            delete(10,30,52);
   status_line();
   f_display();
   if(!getch_han())
	getch_han(); delete(3,6,72);
   for(i=0; i<10; i++)
   {
	  set_vbcolor_han(BLUE,LIGHTGRAY);
	  draw_game_screen();
	  disp_game_quiz(i);
	  delete(7,17,66);
   }
   result_dis_page0();   if(!getch_han()) getch_han();   delete(10,12,67);
   result_dis_page1();   if(!getch_han()) getch_han();   delete(6,1,79);
   result_dis_page2();   if(!getch_han()) getch_han();   delete(4,1,79);
   result_dis_page3();   if(!getch_han()) getch_han();   delete(6,1,79);

   close_han();
   setintr_old();
//   exit(1);
   return(0);
}

static void interrupt new_isr09(void)
{
   char far *ptr1;
   char far *ptr2;
   __emit__(0xFA);
   ptr1 = MK_FP(0x40,0x1C);
   ptr2 = MK_FP(0x40,0x1A);
   (*ptr2) = (*ptr1);
   (*old_isr09)();
   __emit__(0xFB);
}

void setintr_new(void)
{
   old_isr09 = getvect(0x9);
   setvect(0x9,new_isr09);
}

void setintr_old(void)
{
   setvect(0x9,old_isr09);
}


void draw_game_screen(void)
{
   int i, j=1;

   draw_3dbox(17, 7, 66, 21, LIGHTGRAY);
   draw_horiz_line(17*8+5, 13*16+8, 49*8-3, BLUE);
   draw_vert_line( 20*8+5, 7*16+9, 14*16-3, BLUE);
   vputs_han(18,  9, "»©", BLUE);
   vputs_han(18, 11, "¢…", BLUE);
   for(i = 0; i < 3; i++, ++j)
	  vprintf_han(18, 14+i+j, BLUE,"%c%c", SPC_ID, i+'1');
}

void f_display(void)
{
   int i=0, j=2;

   draw_3dbox(6, 3, 72, 23, LIGHTBLUE);
   set_vbcolor_han(LIGHTCYAN,LIGHTBLUE);
   aputs_han(12, 5, ss1[0], FONT_HORIZ_DBL|FONT_VERT_DBL);
   for(i=1; i < 8; ++i, j+=2)
	  vputs_han(8, 6+j, ss1[i], LIGHTCYAN);
}

void disp_game_quiz(int num)
{
   int i, j=1,key, guess;

   for(i = 0; i < 3;i++,++j) {
	  if (i <= 1)
		  nputs_han(21,  8+i+j, ss_quest[num][i], 45);
	  nputs_han(23, 14+i+j, ss_item[num][i], 40);
   }
   while (1)
   {
	  if (kbhit())
	  {
		 if ((key = getch_han()) == 0) getch_han();

		 item_num[num] = (key - '1') + 1;
		 if(item_num[num] == 1) break;
		 else if(item_num[num] == 2) break;
		 else if(item_num[num] == 3) break;
	  }
   }
}

void result_dis_page1(void)
{
   int i=0,j=0,temp=0, line=0;

   set_vbcolor_han(BLUE,LIGHTGRAY);
   draw_3dbox(1,6,79,22,LIGHTGRAY);
   for(i=0; i<4; i++,temp++){
	  put_han_font(3,7+line+temp,212,239);
	  for(j=0; j<3; j++, line++)
		 nputs_han(5, 7+line+temp,ss_dis1[i][item_num[i]-1][j],73);
   }
}

void result_dis_page2(void)
{
   int i=0, j=0, temp=0, line=0, tt;

   set_vbcolor_han(BLUE, LIGHTGRAY);
   draw_3dbox(1,4,79, 24, LIGHTGRAY);
   for(i=4; i<6; i++, temp++){
	  put_han_font(3,5+line+temp, 212,239);
	  for(j=0; j<3; j++, line++)
		  nputs_han(5,5+line+temp,ss_dis1[i][item_num[i]-1][j],73);
   }
   temp++;
   put_han_font(3,5+line+temp, 212,239);
   for(j=0; j<4; j++, line++)
	  nputs_han(5,5+line+temp,ss_dis2[item_num[6]-1][j],73);

   if (item_num[7]==3) tt = 4;
   else
   if(sex==0) {
	if(item_num[7] == 1) tt= 0;
	else		     tt= 1;
   } else {
	if(item_num[7] == 1) tt= 2;
	else  		     tt= 3;
   }
   put_han_font(3,6+line+temp, 212, 239);         line++;
   nputs_han(5,5+line+temp,ss_dis_f[tt][0],73);   line++;
   nputs_han(5,5+line+temp,ss_dis_f[tt][1],73);   line++;
   nputs_han(5,5+line+temp,ss_dis_f[tt][2],73);   line++;
   nputs_han(5,5+line+temp,ss_dis_f[tt][3],73);
}

void result_dis_page3(void)
{
   int i,j,temp=0, line=0;

   set_vbcolor_han(BLUE,LIGHTGRAY);
   draw_3dbox(1,6,79,22,LIGHTGRAY);
   for(i=8;i<10; i++, temp++){
	  put_han_font(3,7+line+temp,212,239);
	  for(j=0; j<5; ++j, line++)
		  nputs_han(5,7+line+temp,ss_dis4[temp][item_num[i]-1][j],73);
   }
}


void logo_display(void)
{
   vclrscr_han(BLACK);
   draw_3dbox(20,5,60,18,LIGHTBLUE);   set_vbcolor_han(BLUE,LIGHTBLUE);

   aputs_han(24,7,"¬÷‰bÉA¯aËa C.S.C.",FONT_HORIZ_DBL|FONT_VERT_DBL);
   vputs_han(31,10,"Àå´e ‰·´ó ¸å¢… ”Ðb",BLUE);
   vputs_han(31,12,"¸å¸a‰¬e‰Á 94Ðb¤å",BLUE);
   vputs_han(31,14," e —e ·¡ : ˆw £¥ ‹A",BLUE);
   vputs_han(31,16,"´á“a ­¡‘aµA‰A...",YELLOW);

   draw_3dbox(20,18,60,22,LIGHTGRAY);   set_vbcolor_han(BLUE,LIGHTGRAY);
   draw_horiz_line(46*8,19*16+12,10*8-1,WHITE);
   draw_horiz_line(46*8+1,21*16+2,10*8,DARKGRAY);
   draw_vert_line(46*8,19*16+12,1*16+6,WHITE);
   draw_vert_line(56*8,19*16+12,1*16+6,DARKGRAY);

   put_han_font(47,20,212,78);
   put_han_font(49,20,212,69);
   put_han_font(51,20,212,88);
   put_han_font(53,20,212,84);
}

void delete(int del_top, int del_left, int del_right)
{
   set_vbcolor_han(BLACK,BLACK);
   delay(500);
   del_top--;
   while((++del_top) <= 24){
	  vclear_text_han(del_left, 25, del_right, 25, BLACK);
	  xscrolldn_han(del_left,del_top,del_right,24);
	  delay(10);
   }
   delay(300);
}

void init(void)
{
   char key[2];

   draw_3dbox(30,10,52,16,LIGHTGRAY);
   set_vbcolor_han(BLACK,LIGHTGRAY);
   draw_horiz_line(30*8+5,12*16+8,22*8-3,BLUE);
   draw_horiz_line(30*8+5,14*16+8,22*8-3,BLUE);
   vputs_han(32,11,"·¡  Ÿq : ",BLUE);

   set_to_han_mode();
   set_max_strlen_han(9);
   disable_han_change();
   gotoxy_han(41,11);
   gets_han(name);
   vputs_han(32,13,"Ñi´‚Ñw : ",BLUE);

   set_keyflag_han(NUM_LOCK|CAPS_LOCK);
   set_max_strlen_han(2);
   set_to_eng_mode();
   gotoxy_han(41,13);
   gets_han(pi);
   vputs_han(32,15,"¬÷  ¥i : ",BLUE);
   draw_3dbox(33,17,50,19,LIGHTGRAY);
   vputs_han(34,18,"1. µa¸a  2. q¸a",BLUE);
   set_max_strlen_han(1);
   while(1){
	   gotoxy_han(41,15);	   gets_han(key);
	   sex = (int)key[0] - '1';
	   if(sex == 0) break;
	   else
	   if(sex == 1) break;
  }
}

void status_line(void)
{
   vclear_text_han(0,29,79,29,LIGHTGRAY);
   set_vbcolor_han(BLUE,LIGHTGRAY);
   vputs_han(1,29, "·¡  Ÿq : ",BLUE);   vputs_han(10,29,name,BLUE);
   vputs_han(20,29,"Ñi´‚Ñw :", BLUE);   vputs_han(29,29,pi,  BLUE);
   vputs_han(31,29,"Ñw",BLUE);
   vputs_han(40,29,"¸å¸a ‰¬e‰Á ¸å‰·°áÇi   C.  S.  C.",BLUE);
}

void result_dis_page0(void)
{
   draw_3dbox(12,10,67,20,LIGHTGRAY);
   vputs_han(17,12,ss_dis0[0],YELLOW);
   set_vbcolor_han(BLUE,LIGHTGRAY);
   vputs_han(15,14,ss_dis0[1],BLUE);
   vputs_han(15,16,ss_dis0[2],BLUE);
   vputs_han(15,18,ss_dis0[3],BLUE);
}

void logo2_display(void)
{
   draw_horiz_line(46*8, 19*16+12,10*8-1,DARKGRAY);
   draw_horiz_line(46*8+1,21*16+2,10*8,  WHITE);
    draw_vert_line(46*8, 19*16+12,1*16+6,DARKGRAY);
    draw_vert_line(56*8, 19*16+12,1*16+6,WHITE);
}
