/****************************************************************************/
/*                                �q�� ���w                                 */
/****************************************************************************/

/* <�e>�� �a���a�១ �q���i�i �Ÿ��e �a��Ϣ �wȁ�A�� ���w�i �� ������ ����  */
/* �e �q���i. ���� ���A �b�a�i �a�w�a�a, getimage(), putimage()�� ���e�i �� */
/* �w�a�� �����e�a.                                                         */
int  my_popup_han(int x, int y, char **string, int number, int *current);
int  my_pulldown_han(pulldown_menu_struct *menu_data);
int  my_choose_hanja(char *source, char *dest);
int  my_get_yesno(int box_y, char *string);
int  my_adv_gets_han(int x, int y, char *string, int limit, int attr,
                     char *user_exit_key);

/* ���A �b�a�A �ŝe�E �q���i */
void draw_3dborder(int left, int top, int right, int bottom, int is_relief);
void draw_3dbox(int left, int top, int right, int bottom, int color);

/* ���a */
void put_msg(char *string, int wait_time);
int  note_pad(int box_x, int box_y, char **text, int height, int width);
int  error_handler(int errval, int ax, int bp, int si);
void show_help(void);
