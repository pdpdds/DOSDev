int mouse_initialize(void);     							/* ax = 0 */
void mouse_show_cursor(void);                               /* ax = 1 */
void mouse_hide_cursor(void);   							/* ax = 2 */
void mouse_information(int *left,int *right,int *x,int *y); 	/* ax = 3 */
int mouse_times_released(int button,int *num,int *x,int *y);/* ax = 6 */
void mouse_range(int left,int top,int right,int bottom);    /* ax = 7,8 */
int mouse_check_click(int left,int top,int right,int bottom);
int mouse_get_cursor(void);