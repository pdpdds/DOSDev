/****************************************************************************/
/*                                Ðq® ¶¥Ñw                                 */
/****************************************************************************/

/* <Ðe>· œa·¡§aœáŸ¡ Ðq®—i·i µÅ¸åÐe ‹aœÏ¢ ¬wÈµA¬á ·¡¶wÐi ® ·¶•¡¢ ®¸÷  */
/* Ðe Ðq®—i. ¡¡– ·³ÁA ¤b¯aŸi ¬a¶wÐa¡a, getimage(), putimage()¡ ÑÁ¡e·i ¸á */
/* ¸wÐa‰¡ ¥¢ŠÐe”a.                                                         */
int  my_popup_han(int x, int y, char **string, int number, int *current);
int  my_pulldown_han(pulldown_menu_struct *menu_data);
int  my_choose_hanja(char *source, char *dest);
int  my_get_yesno(int box_y, char *string);
int  my_adv_gets_han(int x, int y, char *string, int limit, int attr,
                     char *user_exit_key);

/* ·³ÁA ¤b¯aµA ‰Åe–E Ðq®—i */
void draw_3dborder(int left, int top, int right, int bottom, int is_relief);
void draw_3dbox(int left, int top, int right, int bottom, int color);

/* ‹¡Èa */
void put_msg(char *string, int wait_time);
int  note_pad(int box_x, int box_y, char **text, int height, int width);
int  error_handler(int errval, int ax, int bp, int si);
void show_help(void);
