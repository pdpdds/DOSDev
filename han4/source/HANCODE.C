/****************************************************************************/
/*   TITLE              Hangul I/O Libarary <HAN> 4.0                       */
/*   SUB-TITLE          hangul code conversion functions                    */
/*   FILENAME           hancode.c                                           */
/*   DATE & TIME        11/10/91(SUN) 20:21                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

/****************************************************************************/
/*                             Macro Constants                              */
/****************************************************************************/

#define JOHAP           	0           /* 2 byte Jo-Hap code */
#define KS5601          	1           /* KS5601 code */
#define BIT7            	2           /* 7 bit Wan-Sung code*/
#define TABLE_NO     		1326

/****************************************************************************/
/*                               Data Types                                 */
/****************************************************************************/

typedef unsigned char mini;

/****************************************************************************/
/*               Tables(to be referenced by convert_han_code()              */  
/****************************************************************************/

const char han_code_table[][TABLE_NO*2] = 
{ 
   "ˆaˆbˆeˆhˆiˆjˆqˆsˆtˆuˆvˆwˆxˆ{ˆ|ˆˆ‚ˆ…ˆ‰ˆ‘ˆ“ˆ—ˆ¡ˆ©ˆáˆâˆåˆèˆéˆñ"
   "ˆóˆõˆ÷ˆû‰A‰E‰I‰U‰V‰a‰b‰c‰e‰i‰q‰s‰v‰w‰{‰‰¡‰¢‰¥‰¨‰©‰«‰±‰³‰µ‰·"
   "‰Á‰Â‰Å‰É‰×‰á‰å‰÷ŠAŠEŠWŠaŠŠ‚Š…ŠˆŠ‰ŠŠŠ‹Š‘Š“Š•Š—Š¥Š©ŠÁŠáŠõ‹A‹E"
   "‹I‹a‹b‹e‹i‹j‹q‹s‹u‹w‹¡‹¥‹©‹±‹µ‹¼ŒaŒbŒcŒeŒiŒqŒsŒwŒ{ŒŒ‰ŒáŒâŒã"
   "ŒñŒóŒ÷Aau¡¢¥©±³·¸¹ÁÂ÷ŽAŽaŽŽ‚Ž…Ž‰ŽŽ‘Ž¡ŽÁŽáa"
   "egiqu{¡¥©±abceijqsuvwxy{}…‰‘“"
   "•–—¡·áâäåéìñ÷ý‘A‘B‘E‘I‘U‘a‘b‘e‘q‘v‘w‘z‘¡‘¢‘¥‘©"
   "‘±‘³‘µ‘·‘¼‘½‘Ö’A’a’w’’…’‰’¡’á“A“Q“a“b“e“i“j“q“w“x“|““¡“¢“¥"
   "“©“±“³“·”a”b”c”e”h”i”j”k”p”q”s”u”w”y”}””‚”…”‰”‘”•”—”á”â”å”é"
   "”ñ”ó”õ”÷”ù”ü•A•E•I•U•W•¡•¢•¥•¨•©•±•³•·•á•ö–A–E–I–Q–S––‚–…–‰"
   "–‘–“–—–¡–¶–á–õ—A—a—b—e—h—i—q—s—u—w—¡—¢—¥—¨—©—±—³—µ—·—¸˜a˜b˜e"
   "˜i˜q˜w˜˜…˜‘˜—˜á˜â˜å˜é˜ì˜ö˜ý™A™I™¡™¢™·šAšš‚šš—šášåšñ›a›b›e"
   "›h›i›q›u››¡›¥œaœbœeœiœqœsœuœvœwœ}œœ‚œ…œ‰œ‘œ“œ•œ–œ—œ¡œ¢œ·œá"
   "œâœåœéœñœóœõœöœ÷œýABEIQUabeiqsuvw¡¢¥©±"
   "³µ·žAžažwžž‚ž…ž‰ž‘ž“ž•ž¡ŸAŸBŸEŸIŸQŸWŸaŸbŸeŸiŸqŸsŸuŸwŸ|Ÿ¡"
   "Ÿ¢Ÿ¥Ÿ©Ÿ±Ÿ³ŸµŸ· a b e g h i j q s u w x { }  ‚ … ‰ ‘ “ — ˜ á"
   " â å é ñ õ ÷¡A¡B¡E¡I¡Q¡U¡a¡e¡i¡w¡y¡¡¡¢¡¤¡¥¡©¡±¡³¡µ¡·¢a¢¢‚¢ƒ"
   "¢…¢ˆ¢‰¢‘¢•¢—¢¢¡¢¥¢©¢µ£A£Q£a£i£q£¡£¢£¥£¨£©£±£³£·£¹£»¤a¤b¤c¤e"
   "¤h¤i¤j¤l¤q¤s¤u¤w¤{¤¤‚•¡¤‰¤‘¤•¤—¤›¤á¤â¤å¤é¤ñ¤ó¤õ¤÷¤ø¥A¥E¥I¥W"
   "¥a¥b¥e¥i¥s¥u¥w¥{¥¡¥¢¥¥¥©¥±¥³¥µ¥·¥Á¥Ö¦A¦I¦¦‚¦…¦‰¦Š¦‘¦•¦—¦›¦á"
   "§A§a§e§i§¡§¢§¥§©§±§µ§·§¸§¹¨a¨b¨e¨i¨w¨‰¨‚¨…¨‰¨‘¨•¨¡¨±¨á¨å¨è¨é"
   "¨÷©A©a©¡©³©·ªª‚ª…ª‰ª‘«a«e«i«q«¡¬a¬b¬e¬i¬k¬q¬s¬u¬v¬w¬{¬¬‚¬…"
   "¬‰¬‘¬—¬¡¬©¬µ¬·¬á¬â¬ã¬å¬é¬ñ¬ó¬õ¬ö¬÷­A­B­E­I­Q­S­U­a­e­v­¡­¢­¥"
   "­©­±­µ­·­»­á®A®a®e®®‚®…®ˆ®‰®‘®•®—®œ®¡®Á®É®á®å®é®ñ®ó¯A¯Q¯U¯a"
   "¯b¯e¯i¯j¯q¯s¯u¯w¯¡¯¢¯¥¯¨¯©¯°¯±¯³¯µ¯·¯¼°a°b°e°i°w°}°°…°á°â°å"
   "°é°ö°÷±A±¡±¨±©±Á²²‚³a³b³e³i³q³s³³¡³¢³¥³©³±³µ³·´a´b´e´f´g´i"
   "´p´q´s´u´v´w´|´´‚´…´‰´‘´—´¡´¢´¥´©´¬´±´·´»´½´Á´á´â´å´æ´è´é´ê"
   "´ñ´ó´ô´õ´ö´÷´ú´üµAµBµEµIµQµUµaµbµcµeµiµiµqµsµuµwµ}´½´Áµ¡µ¢µ¥"
   "µ©µªµ«µ°µ±µ³µµµ·µÁµÂµÅµÉµÖµ×µá¶A¶E¶a¶b¶e¶w¶¶‚¶…¶‰¶‘¶“¶•¶—¶¡"
   "¶¢¶¥¶©¶¶¶Á¶Å¶É¶á¶å¶é¶õ¶÷·A·B·E·I·W·a·b·e·i·q·s·w··¡·¢·¥·©·ª"
   "·°·±·³·µ·¶···¸·¼¸a¸b¸e¸g¸i¸q¸s¸u¸w¸x¸¸‰¸•¸—¸¡¸á¸â¸å¸é¸ë¸ñ¸ó"
   "¸õ¸÷¸ø¹A¹B¹E¹I¹U¹a¹e¹v¹¡¹¢¹¥¹©¹±¹³¹·¹¹¹½¹ÁºAºaºeºº‚º…º‰º‘º“"
   "º•º—º¡ºá»A»a»b»e»i»q»s»w»¡»¢»¥»©»«»±»³»µ»·»¸»»»¼¼a¼b¼e¼i¼l¼w"
   "¼¼–¼—¼á¼â¼å¼é¼÷½¡½¢½¹¾¾‚¾‰¿a¿q¿¡¿¢¿¥¿©¿¸¿½ÀaÀbÀeÀgÀiÀqÀsÀu"
   "ÀvÀwÀxÀÀ‚À…À‘À—À¡ÀáÀâÀåÀéÀñÀóÀõÀ÷ÁAÁBÁIÁaÁvÁ¡Á¢Á¥Á©ÁµÁ·Á·ÁÉ"
   "ÂAÂÂ‚Â…Â‰Â‘Â“Â—Â¡ÂÁÂáÃAÃaÃbÃqÃwÃ¡Ã¢Ã¥Ã©Ã±Ã³ÃµÃ·ÄaÄbÄeÄiÄqÄw"
   "ÄÄ…Ä‰Ä‘Ä“ÄáÄâÄåÄéÄñÄóÄõÄöÅAÅEÅIÅUÅaÅeÅiÅvÅ¡Å¢Å¥Å©Å±ÅµÅ·ÅáÆ"
   "Æ‚Æ‰Æ—ÆáÆåÇAÇaÇeÇiÇqÇ¡Ç¢Ç¥Ç©Ç±ÇµÇ·ÈaÈbÈeÈiÈqÈsÈuÈwÈÈ‚È…È‰È‘"
   "È—ÈáÈâÈåÈéÈñÈóÈ÷ÉAÉBÉEÉIÉQÉ¡É¢É¥É©É±É³É·ÊAÊÊ‚Ê‰Ê—ÊáËAËaËbËe"
   "ËiËqËuËË¡Ë¢Ë¥Ë©Ë±Ë³Ë·ÌaÌbÌcÌeÌiÌqÌsÌuÌvÌwÌ{ÌÌ‚Ì…Ì—ÌáÌâÌåÌé"
   "ÌñÌöÍAÍEÍIÍaÍeÍiÍwÍÍ¡Í¢Í¥Í©Í±Í·ÎaÎÎ‚Î…Î‰Î‘Î•Î—ÏAÏaÏeÏiÏqÏ¡"
   "Ï¢Ï¥Ï©Ï±Ï³ÏµÏ·ÐaÐbÐeÐiÐqÐsÐuÐwÐÐ‚Ð…Ð‘Ð•Ð–Ð—Ð·Ð·ÐáÐåÐéÐñÐõÐ÷"
   "ÑAÑBÑEÑIÑaÑbÑeÑiÑqÑsÑvÑwÑÑ¡Ñ¢Ñ¥Ñ©Ñ±Ñ³Ð÷ÑBÑÁÑÂÑÅÑÉÑÕÑ×ÑáÑâÑõ"
   "ÒAÒBÒWÒaÒÒ‚Ò…Ò‰Ò‘Ò•Ó¡Ò¥Ò©ÒÁÒáÒéÒñÓAÓWÓaÓbÓeÓiÓjÓqÓsÓwÓ{ÓÓ…"
   "Ó¡Ó¥Ó©Ó±ÓµÓµ",

   "°¡°¢°£°¤°¥°¦°¨°©°ª°«°¬°­°®°°°±°³°´°µ°¶°·°¸°»°¼°¿°Å°Æ°Ç°È°É°Ë"
   "°Ì°Í°Ï°Ñ°Ô°Õ°Ö°Ù°Ú°Ü°Ý°Þ°ß°á°â°ã°å°æ°ç°è°í°î°ï°ð°ñ°ò°õ°ö°÷°ø"
   "°ú°û°ü°ý±¤±¥±¦±ª±«±­±²±³±¸±¹±º±»±¼±½±¾±À±Á±Â±Ã±Ç±È±Ë±Í±Ó±Ô±Õ"
   "±Ö±×±Ø±Ù±Û±Ü±Ý±Þ±ß±à±â±ä±æ±è±ê±í±î±ï±ð±ñ±ò±ô±õ±ø±ù±ú±ý²¨²©²ª"
   "²­²®²±²²²¸²»²¿²À²Á²Ã²Ä²Å²Ç²È²É²Ê²Ë²Ñ²Ò²Ø²Ù²Ú²Û²Ü²Ý²Þ²ã²ç²î²ô"
   "²ö²÷²ø²û²ý³¡³¢³¤³¥³¦³ª³«³¬³­³¯³°³²³³³´³µ³¶³·³¸³¹³º³»³½³¾³¿³À"
   "³Á³Â³Ã³Ä³É³Ê³Ë³Ì³Í³Î³Ð³Ñ³Õ³Ö³×³Ø³Ù³Ú³Ý³à³á³â³ä³æ³ç³è³ë³ì³í³î"
   "³ð³ñ³ò³ó³ô³õ³ù³ú´¢´¨´©´«´­´²´µ´º´½´À´Á´Â´Ã´Ä´Æ´É´Ê´Ë´Ì´Ï´Ð´Ñ"
   "´Ò´Ô´Õ´×´Ù´Ú´Û´Ü´Ý´Þ´ß´à´â´ã´ä´å´ç´é´ê´ë´ì´í´î´ï´ñ´ó´õ´ö´ø´ú"
   "´ý´þµ¡µ¢µ£µ¤µ¥µ§µ¨µ«µ­µµµ¶µ·µ¸µ¹µ¼µ½µ¿µÅµÆµÇµÈµÉµÊµËµÎµÏµÐµÑ"
   "µÒµÓµÕµÖµ×µÚµÞµàµåµæµçµèµéµëµìµíµîµðµñµòµóµôµõµöµ÷µùµúµûµüµý"
   "µþ¶¡¶¥¶§¶©¶«¶¯¶°¶±¶²¶³¶µ¶¹¶»¶¼¶¿¶Ç¶È¶Ë¶Ï¶Ñ¶Ò¶Õ¶×¶Ù¶Ú¶Ü¶ß¶à¶á"
   "¶â¶ã¶ä¶æ¶ç¶ì¶í¶ó¶ô¶õ¶ö¶÷¶ø¶ù¶ú¶û¶þ·¡·¢·£·¤·¥·¦·§·¨·©·ª·«·®·¯"
   "·°·±·²·³·´·µ·¶···¸·¹·º·»·¼·½·¿·Á·Â·Ã·Ä·Å·Æ·Ç·È·É·Ê·Î·Ï·Ð·Ñ·Ò"
   "·Ó·Ô·Õ·Ú·á·æ·ç·è·é·ê·ë·ì·í·ï·ù·ú·û·ü·ý¸¢¸£¸¤¸¥¸¦¸§¸¨¸©¸ª¸­¸®"
   "¸¯¸°¸±¸²¸³¸´¸µ¸¶¸·¸¸¸¹¸º¸»¸¼¸¾¸¿¸À¸Á¸Â¸Ã¸Ä¸Å¸Æ¸Ç¸È¸É¸Ê¸Í¸Î¸Ó"
   "¸Ô¸Õ¸Ö¸Ø¸Ú¸Û¸Þ¸ß¸à¸á¸â¸ä¸ç¸é¸ê¸í¸î¸ð¸ñ¸ò¸ó¸ô¸ö¸÷¸ø¸ù¹¦¹«¹¬¹­"
   "¹®¹¯¹°¹³¹µ¹¶¹¸¹¹¹º¹»¹½¹Â¹Å¹Ç¹É¹Ê¹Ì¹Í¹Î¹Ï¹Ð¹Ò¹Ó¹Ö¹×¹Ø¹Ù¹Ú¹Û¹Ý"
   "¹Þ¹ß¹à¹â¹ã¹ä¹å¹æ¹ç¹è¹éµµ¹ë¹ì¹î¹ð¹ñ¹ö¹÷¹ø¹ú¹ü¹ý¹þº¡º¢º£º¥º§º¬"
   "º­º®º¯º°º±º²º´ºµº¸º¹º»º¼º½º¾º¿ºÀºÁºÃºÆºÉºÎºÏºÐºÒºÓºÕº×ºØºÙºß"
   "ºäºêºìºíºñºòºóºôºöºøºùºúºûºüºýºþ»¡»§»¬»ª»«»¬»­»¯»²»´»µ»·»¸»¹"
   "»½»¾»À»Ç»Ì»Í»Ñ»Ò»Ó»Ô»Õ»Ú»Û»Ü»Ý»ß»ç»è»ê»ì»î»ï»ð»ñ»ò»ó»ô»õ»ö»÷"
   "»ø»ù»ý»þ¼£¼¦¼§¼­¼®¼¯¼±¼³¼¶¼·¼¸¼¹¼º¼¼¼½¼¾¼¿¼À¼Á¼Â¼Å¼Ç¼Ì¼Ò¼Ó¼Õ"
   "¼Ö¼Ø¼Ú¼Û¼Ü¼â¼è¼î¼ð¼ö¼÷¼ø¼ù¼ú¼û¼ý¼þ½£½¤½¦½©½¬½®½¯½°½±½´½·½¸½º"
   "½»½¼½½½¾½¿½À½Á½Â½Ã½Ä½Å½Æ½Ç½È½É½Ê½Ë½Ì½Í½Î½Ï½Ñ½Ò½Ö½×½Ø½Ú½á½â½ã"
   "½ä½è½é½ê½î½ñ½ò½÷¾¥¾¦¾²¾³¾´¾µ¾¸¾¹¾º¾¾¾¿¾À¾Á¾Ã¾Ä¾Å¾Æ¾Ç¾È¾É¾Ê¾Ë"
   "¾Î¾Ï¾Ð¾Ñ¾Ò¾Ó¾Õ¾Ö¾×¾Ø¾Ù¾Ú¾Þ¾ß¾à¾á¾â¾ã¾ä¾ç¾è¾é¾ê¾î¾ï¾ð¾ñ¾ò¾ó¾ô"
   "¾ö¾÷¾ø¾ù¾ú¾û¾ý¾þ¿¡¿¢¿£¿¤¿¥¿§¿©¿ª¿«¿¬¿­¿­¿°¿±¿³¿µ¿¸¾é¾ê¿À¿Á¿Â"
   "¿Ã¿Ä¿Å¿Ç¿È¿É¿Ê¿Ë¿Í¿Î¿Ï¿Ð¿Ô¿Õ¿Ö¿Ü¿Þ¿ä¿å¿æ¿ë¿ì¿í¿î¿ï¿ò¿ó¿ô¿õ¿ö"
   "¿÷¿ø¿ù¿ü¿þÀ¢À£À§À©ÀªÀ­À®À¯À°À±À²À¶À¸À¹ÀºÀ»À½À¾ÀÀÀÇÀÌÀÍÀÎÀÏÀÐ"
   "ÀÒÀÓÀÔÀÕÀÖÀ×ÀØÀÙÀÚÀÛÀÜÀÝÀßÀáÀâÀãÀåÀæÀçÀêÀíÀïÀðÀúÀûÀüÀýÀþÁ¡Á¢"
   "Á£Á¤Á¥Á¦Á§Á¨Á©Á¬Á®Á¯Á³Á¶Á·Á¸Á¹Á»Á¼Á¾ÁÀÁÁÁÂÁËÁÒÁÔÁÖÁ×ÁØÁÙÁÜÁÝ"
   "ÁÞÁßÁàÁãÁêÁîÁïÁðÁñÁòÁóÁõÁöÁ÷ÁøÁúÁûÁüÁýÁþÂ¡Â¢Â£Â¤Â¥Â¦Â§Â©ÂªÂ¯"
   "Â°Â·Â¸Â¼Â½Â¾Â¿ÂÄÂÉÂÊÂÑÂÞÂßÂáÂêÂëÂîÂïÂðÂñÂõÂöÂ÷ÂøÂùÂúÂûÂüÂýÂþ"
   "Ã¡Ã¢Ã£Ã¤Ã¥Ã¦Ã¨Ã¬Ã­Ã³Ã´ÃµÃ¶Ã·Ã¸Ã¹Ã»Ã¼Ã½Ã¿ÃÄÃÆÃÊÃËÃÌÃÍÃÐÃÑÃÑÃÔ"
   "ÃÖÃßÃàÃáÃâÃãÃäÃæÃçÃéÃëÃòÃ÷ÃøÃûÃþÄ¡Ä¢Ä£Ä¥Ä§Ä¨Ä©ÄªÄ«Ä¬Ä­Ä®Ä¯Ä²"
   "Ä³ÄµÄ¶Ä·Ä¸Ä¿ÄÀÄÁÄÃÄÄÄÅÄÆÄÇÄÉÄËÄÌÄÏÄÑÄÒÄÓÄ×ÄÚÄÛÄÜÄÝÄÞÄàÄáÄèÄí"
   "ÄîÄðÄôÄûÄýÅ¥Å©Å«Å¬Å­Å°Å±Å²Å³Å´Å¶Å·Å¸Å¹ÅºÅ»Å½Å¾Å¿ÅÁÅÂÅÃÅÄÅÅÅÆ"
   "ÅÊÅÍÅÎÅÏÅÐÅÒÅÓÅÖÅ×ÅØÅÙÅÚÅÛÅäÅåÅæÅçÅèÅéÅëÅðÅõÅöÅøÅüÆ¢Æ©Æ®Æ¯Æ°"
   "Æ²Æ´Æ¶Æ·Æ¼Æ½Æ¾Æ¿ÆÀÆÁÆÃÆÄÆÅÆÆÆÇÆÈÆÊÆËÆÌÆÍÆÎÆÏÆÐÆÑÆÒÆØÆÛÆÜÆÝÆÞ"
   "ÆßÆâÆäÆæÆçÆìÆíÆîÆòÆóÆ÷ÆøÆùÆúÆûÆþÇ¥ÇªÇ«Ç¬Ç®Ç°Ç²Ç³Ç»ÇÁÇÂÇÃÇÄÇÇ"
   "ÇÈÇÉÇÊÇËÇÌÇÍÇÎÇÏÇÐÇÑÇÒÇÔÇÕÇÖÇ×ÇØÇÙÇÚÇÜÇÞÇßÇàÇâÇâÇãÇåÇæÇèÇêÇë"
   "ÇìÇíÇîÇïÇôÇõÇöÇ÷ÇøÇùÇûÇüÇýÈ£È¤È¥È¦È¨È©ÇëÇíÈ­È®È¯È°È±È²È³È´È¶"
   "È¸È¹È¾È¿ÈÄÈÅÈÆÈÇÈÉÈÊÈ÷ÈÍÈÎÈÑÈÖÈÙÈÚÈÞÈäÈåÈæÈçÈêÈëÈìÈíÈïÈðÈñÈò"
   "È÷ÈùÈúÈûÈýÈý",

   "_U_V_W_X_Y_Y_Z_[_\\_]_^___`_a_b_c_d_e_f_g_h_i_j_j_k_l_m_n_o_p"
   "_q_r_s_t_u_v_w_x_y_z_{_|_}_~`!`#`$`%`&`'`(`)`*`+`,`-`.`/`0`1"
   "`2`3`4`5`6`7`8`9`:`;`<`=`>`?`@`A`B`C`D`E`F`G`H`I`J`K`L`M`N`O"
   "`P`Q`R`S`T`U`V`W`X`Y`Z`[`\\`]`^`_```a`b`c`d`e`f`g`h`i`j`k`l`m"
   "`n`o`p`q`s`s`t`u`v`w`x`y`z`{`|`}`~aAaBaCaDaEaFaGaHaIaJaKaLaM"
   "aNaOaPaQaRaSaTaUaVaWaXaYaZa[a\\a]a^a_b@bAbBbCbDbEbFbGbHbIbJbK"
   "bLbMbNbObPbQbRbSbTbUbVbWbXbYbZb[b\\b]b^b_c@cAcBcCcDcEcFcGcHcI"
   "cJcKcLcMcNcOcPcQcRcScTcUcVcWcXcYcZc[c\\c]c^c_d@dAdBdCdDdEdFdG"
   "dHdIdJdKdLdMdNdOdPdQdRdSdTdUdVdWdXdYdZd[d\\d]d^d_e@eAeBeCeDeE"
   "eFeGeHeIeJeKeLeMeNeOePeQeReSeTeUeVeWeXeYeZe[e\\e]e^e_f@fAfBfC"
   "fDfEfFfGfHfIfJfKfLfMfNfOfPfQfRfSfTfUfVfWfXfYfZf[f\\f]f^f_g@gA"
   "gBgCgDgEgFgGgHgIgJgKgLgMgNgOgPgQgRgSgTgUgVgWgXgYgZg[g\\g]g^g_"
   "h@hAhBhChDhEhFhGhHhIhJhKhLhMhNhOhPhQhRhShThUhVhWhXhYhZh[h\\h]"
   "h^h_i@iAiBiCiDiEiFiGiHiIiJiKiLiMiNiOiPiQiRiSiTiUiViWiXiYiZi["
   "i\\i]i^i_j@jAjBjCjDjEjFjGjHjIjJjKjLjMjNjOjPjQjRjSjTjUjVjWjXjY"
   "jZj[j\\j]j^j_k@kAkBkCkDkEkFkGkHkIkJkKkLkMkNkOkPkQkRkSkTkUkVkW"
   "kXkYkZk[k\\k]k^k_l@lAlBlClDlElFlGlHlIlJlKlLlMlNlOlPlQlRlSlTlU"
   "lVlWlXlYlZl[l\\l]l^l_m@mAmBmCmDmEmFmGmHmImJmKmLmMmNmOmPmQmRmS"
   "mTmUmVmWmXmYmZm[m\\m]m^eQn@nAnBnCnDnEnFnGnHnInJnKnLnMnNnOnPnQ"
   "nRnSnTnUnVnWnXnYnZn[n\\n]n^n_o@oAoBoCoDoEoFoGoHoIoJoKoLoMoNoO"
   "oPoQoRoSoToUoVoWoXoYoZo[o\\o]o^o_p@pApEpCpDpEpFpGpHpHpIpJpKpL"
   "pMpNpOpPpQpRpSpTpUpVpWpXpYpZp[p\\p]p^p_q@qAqBqCqDqEqFqGqHqIqJ"
   "qKqLqMqNqOqPqQqRqSqTqUqVqWqXqYqZq[q\\q]q^q_r@rArBrCrDrErFrGrH"
   "rIrJrKrLrMrNrOrPrQrRrSrTrUrVrWrXrYrZr[r\\r]r^r_s@s@sAsBsCsDsE"
   "sFsGsHsIsJsKsLsMsNsOsPsQsRsSsTsUsVsWsXsYsZs[s\\s]s^s_s_t@tAtB"
   "tCtDtEtFtGtHtItJtKtLtMtNtOtPtQtRtStTtUtVtWtXtYtZt[t\\t]t^t_u@"
   "uAuBuCuDuEuFuGuHuIuJuKuLuMuNuOuPuQuRuSuTuUuVuWuXuYuZu[u\\u]u^"
   "u_v@vAvBvCvDvEvFvGvHvIvJvKvLvNvOvPvQvRvRvSvTvUvWvYuVuWv\\v]v^"
   "v_w@wAwBwCwDwEwFwGwHwIwJwKwLwMwNwOwPwQwRwSwTwUwVwWwXwYwZw[w\\"
   "w]w^w_x@xAxBxCxDxExFxGxHxIxJxKxLxMxNxOxPxQxRxSxTxUxVxWxXxYxZ"
   "x[x\\x]x^x_y@yAyByCyDyEyFyGyHyIyJyKyLyMyNyOyPyQyRySyTyUyVyWyX"
   "yYyZy[y\\y]y^y_z@zAzBzCzDzEzFzGzHzIzJzKzLzMzNzOzPzQzRzSzTzUzV"
   "zWzXzYzZz[z\\z]z^z_{!{#{${%{&{'{({){*{+{,{-{.{/{0{1{2{3{4{5{6"
   "{7{8{9{:{;{<{={>{?{@{A{B{C{D{E{F{G{H{I{J{K{L{M{N{O{P{Q{R{S{T"
   "{U{V{W{X{Y{Z{[{\\{]{^{_{`{a{b{c{d{e{f{g{h{i{j{k{l{m{n{o{p{p{q"
   "{r{s{t{u{v{w{x{y{z{{{|{}{~|!|#|$|%|&|'|(|)|*|+|,|-|.|/|0|1|2"
   "|3|4|5|6|7|8|9|:|;|<|=|>|?|@|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P"
   "|Q|R|S|T|U|V|W|X|Y|Z|[|\\|]|^|_|`|a|b|c|d|e|f|g|h|i|j|k|l|m|n"
   "|o|p|q|r|s|t|u|v|w|x|y|z|{|||}|~}!}#}$}%}&}'}(})}*}+},}-}.}/"
   "}0}1}2}3}4}5}6}7}8}9}:};}<}=}>}?}@}A}B}C}D}E}F}G}H}I}J}K}L}M"
   "}N}O}P}Q}R}S}T}U}V}W}X}Y}Z}[}\\}]}^}_}`}a}b}c}d}e}f}g}h}i}j}k"
   "}l}m}n}o}p}q}r}s}t}u}v}w}x}y}z}{}|}}}~~!~#~$~%~%~&~'~(~)~*~+"
   "~,~-~.~/~0~1~2~3~4~5~6~7~8~9~:~;~<~=~>~+~-~A~B~C~D~E~F~F~F~F"
   "~G~H~I~J~K~L~M~N~O~P~d~Q~R~S~T~U~V~W~X~Y~Z~[~\\~]~^~_~`~a~b~c"
   "~d~e~f~g~h~h"
};

/****************************************************************************/
/*                   Prototypes of the External Functions                   */
/****************************************************************************/
#include <string.h>
int convert_han_code(char *string, int source_code, int dest_code);

/****************************************************************************/
/*                    Implementations of the C Functions                    */
/****************************************************************************/

int convert_han_code(char *string, int source_code, int dest_code)
{
   mini byte0,byte1;
   int index,ret_code,i;

   if(source_code < JOHAP || source_code > BIT7 ||
      dest_code   < JOHAP || dest_code   > BIT7) return(-1);

   ret_code = index = 0;
   while(index < strlen(string))
   {
      byte0 = string[index++];

      /* if not HANGUL, continue */      
      if(source_code == BIT7)
      {
         byte1 = string[index];
         switch(byte0)
         {
            case '_' : if(byte1 >= 0x21 && byte1 <= 0x7e) 
                          break;
                       else 
                          continue;
            case '`' : if((byte1 >= 0x00 && byte1 <= 0x9f) || (byte1 >= 0xc0 && byte1 <= 0xdf))
                          break;
                       else
                          continue;
            case '{' : 
            case '!' : 
            case '}' :
            case '|' :
            case '~' : if(byte1 >= 0x21 && byte1 <= 0x7e)
                          break; 
                       else 
                          continue;
            default  :
               if( (byte0 >= 'a' && byte0 <= 'z') &&
		   ((byte1 >= 0x00 && byte1 <= 0x1f) ||
                    (byte1 >= 0x40 && byte1 <= 0x5f) ||
                    (byte1 >= 0x80 && byte1 <= 0x9f) ||
                    (byte1 >= 0xc0 && byte1 <= 0xdf)) )
                  break;
               else
                  continue;
         }
      }
      else if(!(byte0 & 0x80)) 
         continue;

      byte1 = string[index++];

      /* get offset by string <str> */
      for(i = 0; i < TABLE_NO; i++)
         if(byte0 == han_code_table[source_code][i*2] && byte1 == han_code_table[source_code][i*2+1]) break;

      /* if founded replace it or, mask on error_code */
      if(i < TABLE_NO) 
      {
         string[index-2] = han_code_table[dest_code][i*2];
         string[index-1] = han_code_table[dest_code][i*2+1];
      }
      else
         ret_code = -2;
   }
   return(ret_code);
}
