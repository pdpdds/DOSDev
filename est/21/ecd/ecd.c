#include <stdio.h>
#include <dir.h>
#include <dos.h>
#include <conio.h>
#include <string.h>
#include <mem.h>
#include <alloc.h>
#include <est.h>
#include "..\21lib\init_66.h"
//#include "..\21lib\21_all.h"

#define _GETTREE (tree *)calloc(1, sizeof(tree))

byte Lines[5][16]={
 {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
	0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
 {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
	0xFF,0x18,0x18,0x18,0x18,0x18,0x18,0x18,},
 {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
	0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18},
 {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x1F,
	0x1F,0x18,0x18,0x18,0x18,0x18,0x18,0x18,},
 {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x1F,
	0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
};

byte Conti[2][16]={
 {0xFE,0x7C,0x7C,0x38,0x38,0x10,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
 {0x00,0x00,0x00,0x00,0x00,0x80,0xE0,0xF8,
	0xFC,0xF8,0xE0,0x80,0x00,0x00,0x00,0x00},
};

struct branch {
	byte name[13];
	byte deep;
	byte sflag;
	byte end;
	int x;
	int y;
	struct branch *next;
	struct branch *mother;
};

typedef struct branch tree;

tree *begin=0, *cur_tree=0, *def_tree=0;
int work_drv;
byte cur_dir_name[15];
byte cur_dir[80];
byte path[80];
byte header[]={"PNCI"};
word tot_dir;
word check_sum;
byte tree_mody=OFF;

byte far *ecd_buf;//[20000];
byte far *emenu_buf;//[10000];

int E_x=0, E_y=0, E_lx, E_ly;
int E_scrx=0, E_scry=0;
int E_ox=0, E_oy=0;
int ecd_h, ecd_w;
int Hx, Hex, Hy, Wx, Wy, Wey;

extern MaxX, MaxY;

void ecd_end();
void off_ecd_win();
void put_ecd_win();
int get_ecd_menu();
void rev_ecd_menu();

void put_str(int x, int y, byte far *str, byte mode)
{
	byte t;
	if(E_scrx) {
		x-=E_scrx;
		if(x<0) return;
	}
	x=x*16+E_x+24;
	y+=E_y+1;
	if(E_scry) {
		y-=E_scry*15;
		if(y<E_y+1) return;
	}
/*
	if(mode==1) {
		t=0;
	}
	else if(mode==0) t=0x04;
	if(strcmp(str, cur_dir_name)==0) t|=0x40;
*/
	if(y>E_y+E_ly-5) return;
/*
	switch(mode) {
		case 0  : t=0x04; break;
		case 1  : t=0;    break;
		case 11 : t=0x04 | 0x40; break;
		case 10 : t=0x40; break;
	}
*/
	put_string(x, y, str, mode);
}

void put_tree_name(int x, int y, tree *node, byte mode)
{
	if(mode==1) mode=0;
	else mode=0x04;
	if(node==def_tree) mode |=0x40;
	put_str(x, y, node->name, mode);
}

void put_char(int x, int y, byte c, byte mode)
{
	byte num[5]={196,194,179,195,192};
	byte t_str[2];
	byte i, m;

	for(i=0; i<5; i++) if(c==num[i]) break;
	if(E_scrx) {
		x-=E_scrx;
		if(x<0) return;
	}
	x=x*16+E_x+24;
	y+=E_y+1;
	if(E_scry) {
		y-=E_scry*15;
		if(y<E_y+1) return;
	}
	if(mode==7) {
		m=0;
	}
	else m=1;
	if(y>E_y+E_ly-5) return;
	if(i<5) {
		if(c==179 || c==194 || c==195) {
			if(x==13*16+E_x+24) {
				put_free_image(x+17, y, 16, 8, Conti[0], 0);
			}
		}
		if(c==196) {
			if(y==(E_y+E_ly-5)) {
				put_free_image(x, y+1, 16, 8, Conti[1], 0);
			}
		}
		put_free_image(x, y, 16, 8, Lines[i], m);
	}
	else {
		t_str[0]=c;
		t_str[1]=0;
		if(m==1) m=0x04;
		put_string(x, y, t_str, m);
	}
}

void put_str2(int x, int y, byte far *str, byte mode)
{
	int i, len;

	len=strlen(str);
	for(i=0; i<len; i++) {
		put_char(x, y+i, str[i], mode);
	}
}

void get_ecd_max()
{
	int h=0, w=0, i;
	tree *temp;

	temp=begin;
	for(i=0; temp;) {
		if(temp->deep==0) {
			temp->x=0;
			temp->y=1;
		}
		else {
			temp->x=i;
			temp->y=6+(temp->deep-1)*15;
		}
		if(!temp->next->sflag) {
			i++;
		}
		if(temp->x > h) h=temp->x;
		if(temp->deep > w) w=temp->deep;
		temp=temp->next;
	}
	ecd_w=w+1;
	ecd_h=h+1;
}

void clear_space(int x)
{
	if(E_scrx) {
		x-=E_scrx;
		if(x<0) return;
	}
	x=x*16+E_x+24;
	barb(x, E_y+1, x+16, E_y+E_ly-4, 0);
}

void clear_tree(void)
{
	tree *temp;
	tree *del;

	if(!begin) return;
	temp=begin;
	while(temp) {
		del=temp;
		temp=temp->next;
		free(del);
	}
	cur_tree=begin=0;
}

void init_tree(void)
{
	clear_tree();
	begin=_GETTREE;
	strcpy(begin->name, "\\");
	begin->mother=0;
	begin->deep=0;
	begin->sflag=1;
	begin->end=1;
	begin->next=0;
	cur_tree=begin;
}

byte insert_tree(byte deep, byte sflag, byte *name, tree *up, tree *mo)
{
	tree *temp;

	temp=_GETTREE;
	if(!temp) return 0;
	strcpy(temp->name, name);
	temp->deep=deep;
	temp->sflag=sflag;
	temp->end=0;
	temp->mother=mo;
	temp->next=up->next;
	up->next=temp;
	return 1;
}

int scan_path(byte deep, byte *mopath, tree *up)
{
	tree *temp;
	int x, y;
	int done;
	word total=1;
	byte sflag=1;
	struct ffblk file;

	x=E_x+(E_lx-10)*4;
	y=E_y+(E_ly-30)/2;

	temp=up;
	strcpy(path, mopath);
	strcat(path, "*.*");
	done=findfirst(path, &file,FA_DIREC);
	while(!done) {
		if(file.ff_attrib==FA_DIREC) {
			if(file.ff_name[0]!='.') {
				if(!insert_tree(deep, sflag, file.ff_name, temp, up)) {
           error("°A°°ü°àa ¶Åπ¢–sì°îa!!");
					 getch();
					 if(total>1) temp->end=1;
					 return total;
				}
				temp=temp->next;
				sflag=0;
				mc_off();
				gprintf(x+40, y+13, 0, "%3d", tot_dir+total);
				mc_on();
				total++;
			}
		}
		done=findnext(&file);
	}
	if(total>1)	temp->end=1;
	return total;
}

void ins_path(byte *dest, byte *src)
{
	byte len;

	len=strlen(src);
	if(src[0]!='\\') {
		movmem(dest, dest+len+1, 80-len-1);
		memcpy(dest, src, len+1);
		dest[len]='\\';
	}
	else {
		movmem(dest, dest+1, 79);
		dest[0]='\\';
	}
}

/*
tree *find_mother(tree *cur)
{
	tree *temp;
	byte check;

	check=cur->deep;
	temp=cur;
	while(temp->deep<check) temp->
*/

void get_full_path(byte *dest, tree *temp)
{
	if(!temp->mother) return;
	temp=temp->mother;
	do {
		ins_path(dest, temp->name);
		temp=temp->mother;
	} while(temp);
}

tree *prev_tree(tree *temp)
{
	tree *cur;

	if(temp==begin) return begin;
	cur=begin;
	while(cur->next) {
		if(cur->next==temp) return cur;
		cur=cur->next;
	}
	return temp;
}

tree *find_cur(void)
{
	tree *cur;

	cur=begin;
	while(cur) {
		strcpy(path, cur->name);
		if(path[0]!='\\') get_full_path(path, cur);
		if(strcmp(cur_dir, path)==0) return cur;
		cur=cur->next;
	}
	return 0;
}

tree *find_deep(int x, byte deep)
{
	tree *temp;

	temp=begin;
	while(temp) {
		while(temp->x==x) {
			if(temp->deep==deep) return temp;
			if(temp->next->x==x) temp=temp->next;
			else {
				x++;
				break;
			}
		}
		temp=temp->next;
	}
	return 0;
}

tree *find_deep_back(int x, byte deep)
{
	tree *temp;

	while(x>=0) {
		temp=begin;
		while(temp) {
			if(temp->x>x) break;
			if(temp->deep==deep && temp->x==x) return temp;
			temp=temp->next;
		}
		x--;
	}
	return 0;
}

void del_tree(tree *up)
{
	tree *del;
	tree *temp;

	del=up->next;
	up->next=del->next;
	if(del->sflag && del->mother==del->next->mother) up->next->sflag=1;
	if(del->end) {
		temp=find_deep_back(del->x-1, del->deep);
		if(temp && del->mother==temp->mother) temp->end=1;
	}
	free(del);
}

void make_check_sum(void)
{
	tree *temp;
	int i, len;

	check_sum=0;
	temp=begin;
	while(temp) {
		len=strlen(temp->name);
		for(i=0;i<len;i++) check_sum+=temp->name[i];
		check_sum+=temp->deep;
		check_sum+=temp->sflag;
		check_sum+=(!(temp->end));
		temp=temp->next;
	}
}


void make_tree(void)
{
	tree *cur;
	int x, y;

	x=E_x+(E_lx-10)*4;
	y=E_y+(E_ly-30)/2;
	mc_off();
	get_image(x, y, x+100, y+40, emenu_buf);
	put_panel(x, y, x+80, y+30, 0);
	put_string(x+15, y+3, (byte far *)"ó°ùB…°ü°üi ∑™â°∑∂∑sì°îa.", 0);
	mc_on();
	tot_dir=1;
	init_tree();
	strcpy(path, begin->name);
	cur=begin;
	tot_dir+=scan_path(1, path, cur)-1;
	tree_mody=ON;

	while(cur->next) {
		cur=cur->next;
		strcpy(path, cur->name);
		get_full_path(path, cur);
		if(strcmp(path, cur_dir)==0) {
			def_tree=cur_tree=cur;
			strcpy(cur_dir_name, cur_tree->name);
		}
		strcat(path, "\\");
		tot_dir+=scan_path(cur->deep+1, path, cur)-1;
	}
	mc_off();
	put_image(x, y, x+100, y+40, emenu_buf);
	mc_on();
	make_check_sum();
}

void put_tree(void)
{
	tree *cur;
	tree *temp;
	int i, j, tot;
	byte len;

	mc_off();
	cur=begin;
	clear_space(0);
	clear_space(E_scrx+14);
	for(i=0; i<14; i++) {
		put_string(E_x+24+i*16, E_y+E_ly-4, " ", 0);
	}
	for(i=0; cur;) {
		if(cur->deep==0) {
			cur->x=0;
			cur->y=1;
			if(E_scrx==0 && E_scry==0) {
				put_tree_name(0, 1, cur, 1);
//				put_str(0, 1, cur->name, 1);
			}
		}
		else {
			cur->x=i;
			cur->y=6+(cur->deep-1)*15;
			if(i>E_scrx-1 && i<E_scrx+14 && cur->deep>E_scry && cur->deep<E_scry+5) {
				put_char(cur->x, cur->y-1, 196, 7);
				if(cur->sflag)
					put_char(cur->x, cur->y-3, 196, 7);
				if(cur->sflag && cur->end)
					put_char(cur->x, cur->y-2, 196, 7);
				else if(cur->sflag)
					put_char(cur->x, cur->y-2, 194, 7);
				else if(cur->end)
					put_char(cur->x, cur->y-2, 192, 7);
				else
					put_char(cur->x, cur->y-2, 195, 7);
				if(cur->sflag) {
					len=strlen(cur->mother->name);
					if(cur->deep!=1) {
						for(j=0;j<12-len;j++) {
							put_char(cur->x, cur->y-4-j, 196, 7);
						}
					}
				}
				else if(cur->deep>1) {
					temp=cur;
					for(j=1;j<cur->deep;j++) {
						temp=temp->mother;
						if(!temp->end)
							put_char(cur->x, 4+(temp->deep-1)*15, 179, 7);
					}
				}
//				put_str(cur->x, cur->y, cur->name, 1);
				put_tree_name(cur->x, cur->y, cur, 1);
			}
		}
		if(!cur->next->sflag) {
			i++;
			if(i<14+E_scrx) clear_space(i);
		}
		cur=cur->next;
	}
	get_ecd_max();
	tot=ecd_h-14;
	if(tot<0) tot=1;
	loc_hbar(Hx, Hex, Hy, tot, E_scrx, E_ox);

	tot=ecd_w-4;
	if(tot<0) tot=1;
	loc_vbar(Wx, Wy, Wey, tot, E_scry, E_oy);
	gliney(Wx, Wy, Wey, 1);
	mc_on();
}

void reset_scrbar()
{
	int t, t2;

	t=ecd_w;
	t2=ecd_h;
	get_ecd_max();
	if(ecd_w<t && E_scry>0) {
		E_scry--;
		E_oy=E_scry;
	}
	if(ecd_h<t2 && E_scrx>0) {
		E_scrx--;
		E_ox=E_scrx;
	}
	put_hbar(Hx, Hex, Hy);
	put_vbar(Wx, Wy, Wey);
	gliney(Wx, Wy, Wey, 1);
}

void rev_branch(byte flag, tree *node)
{
	byte t=0;

	mc_off();
	if(node->x < E_scrx) {
		E_ox=E_scrx;
		E_scrx=node->x;
		t=1;
	}
	else if(node->x > E_scrx+13) {
		E_ox=E_scrx;
		E_scrx=node->x-13;
		t=1;
	}
	if(node->deep < E_scry) {
		E_oy=E_scry;
		E_scry=node->deep;
		if(E_scry>0) E_scry--;
		t=1;
	}
	else if(node->deep==E_scry) {
		if(E_scry>0) {
			E_oy=E_scry;
			E_scry--;
			t=1;
		}
	}
	else if(node->deep >E_scry+3) {
		E_oy=E_scry;
		E_scry=node->deep-3;
		t=1;
	}
	if(t==1) {
		put_tree();
		mc_off();
	}
	if(flag) {
		if(node->name[0]!='\\')
			put_str2(node->x, node->y-1, "              ", 0x70);
//		put_str(node->x, node->y, node->name, 0);
			put_tree_name(node->x, node->y, node, 0);
	}
	else {
		if(node->name[0]!='\\') {
			if(node->next->mother==node)
				put_str2(node->x, node->y-1, "ƒƒƒƒƒƒƒƒƒƒƒƒƒƒ", 7);
			else
				put_str2(node->x, node->y-1, "ƒ             ", 7);
		}
//		put_str(node->x, node->y, node->name, 1);
		put_tree_name(node->x, node->y, node, 1);
	}
	mc_on();
}

void read_info(void)
{
	FILE *fp;
	word total=1;
	tree *temp;
	tree *mo;
	byte name[80];
	byte deep;
	byte end;
	byte sflag;
	word i;

	if((fp=fopen("\\TREEINFO.NCD", "rb"))==NULL) {
RESCAN:
		make_tree();
		return;
	}
	fread(path, 5, 1, fp);
	path[4]=0;
	if(strcmp(header, path)!=0) {
		fclose(fp);
		goto RESCAN;
	}
	tot_dir=getw(fp);
	end=fgetc(fp);
	sflag=tot_dir>>8;
	deep=tot_dir & 0x00ff;
	if(end!=(sflag+deep+0x2a)) {
		fclose(fp);
		goto RESCAN;
	}
	init_tree();
	fread(path, 17, 1, fp);
	mo=begin;
	temp=begin;

	for(i=1;i<tot_dir;i++) {
		fread(name, 13, 1, fp);
		deep=fgetc(fp);
		sflag=fgetc(fp);
		end=!fgetc(fp);
		if(sflag) {
			mo=temp;
		}
		if(!insert_tree(deep, sflag, name, temp, mo)) {
			 error("°A°°ü°àa ¶Åπ¢–sì°îa!!");
			 temp->end=end;
			 fclose(fp);
			 return;
		}
		total++;
		temp=temp->next;
		temp->end=end;
		if(end) {
			while(mo->end && mo!=begin) mo=mo->mother;
			if(mo->mother) mo=mo->mother;
		}
	}
	i=getw(fp);
	fclose(fp);
	make_check_sum();
	if(i!=check_sum) goto RESCAN;
	def_tree=cur_tree=find_cur();
	strcpy(cur_dir_name, cur_tree->name);
	if(!cur_tree) goto RESCAN;
}

void save_info()
{
	FILE *fp;
	tree *temp;

	if((fp=fopen("\\TREEINFO.NCD", "wb+"))==0) {
		error("TREEINFO.NCD µiÆÅ¥Ù∑q!!");
		ecd_end();
	}
	fwrite(header, 5, 1, fp);
	putw(tot_dir, fp);
	fputc((char)((tot_dir>>8)+(tot_dir&0xff)+0x2a), fp);
	fputc(1, fp);
	temp=begin;
	while(temp) {
		fwrite(temp->name, 13, 1, fp);
		fputc(temp->deep, fp);
		if(temp->sflag) fputc(1, fp);
		else fputc(0, fp);
		fputc(!(temp->end), fp);
		temp=temp->next;
	}
	make_check_sum();
	putw(check_sum, fp);
	tree_mody=OFF;
	fclose(fp);
}

void set_cur_tree(int code)
{
	tree *temp;
	int i, j;

	temp=begin;
	while(temp) {
		if(code==0) {
			if(temp->x>=E_scrx && temp->deep>=E_scry) {
				cur_tree=temp;
				return;
			}
		}
		else if(code==1) {
			if(temp->deep>E_scry && temp->x>=E_scrx) {
				cur_tree=temp;
				return;
			}
		}
		temp=temp->next;
	}
}

int get_tree_mouse(int mx, int my)
{
	tree *temp;
	int x, y, tx, ty, add;

	x=E_scrx;
	y=E_scry*15;
	temp=begin;
	while(temp) {
		tx=(temp->x-x)*16+E_x+24;
		ty=(temp->y-y+E_y+1)*8;
		if(temp->deep==0) {
			add=1;
		}
		else {
			add=15;
		}
		if(mx>tx && my>ty && mx<tx+16 && my<ty+8*add) {
			cur_tree=temp;
			return 1;
		}
		temp=temp->next;
	}
	return 0;
}

int ecd_tx, ecd_ty;
byte keyin_flag=0;

int ecd_mouse(int mx, int my)
{
	tree *temp, *old;
	int x, y, lx, ly, t, tot;

	x=E_x;
	y=E_y;
	lx=E_lx;
	ly=E_ly;

	if(keyin_flag) goto L;
	t=get_ecd_menu(mx, my);
	if(t>-1) {
		rev_ecd_menu(t);
		while(lbp);
		rev_ecd_menu(t);
		return t-10;
	}
	if(mx>x && mx<x+20 && my>y*8 && my<(y+ly)*8) {
		if(my<E_y*8+20) {  // close
			return -3;
		}
		else {
			t=move_window(&x, &y, &lx, &ly);
			mc_on();
			if(t==1) {
				ecd_tx=x;
				ecd_ty=y;
				return -2;
			}
			else return -1;
		}
	}
	else if(mx>Hx && mx<Hex && my>Hy && my<(y+ly)*8) {
		tot=ecd_h-14;
		if(tot<0) tot=1;
		if(ecd_h>14) {
			t=set_hbar(Hx, Hex, Hy, tot, E_scrx, E_scrx, 0, tot);
			if(t!=-100) {
				rev_branch(1, cur_tree);
				E_ox=E_scrx;
				E_scrx=t;
				put_tree();
				set_cur_tree(0);
				rev_branch(1, cur_tree);
			}
		}
		return -1;
	}
	else if(mx>Wx && mx<Wx+16 && my>Wy && my<Wey) {
		tot=ecd_w-4;
		if(tot<0) tot=1;
		t=set_vbar(Wx, Wy, Wey, tot, E_scry, E_scry, 0, tot);
		if(t!=100) {
			if(t>-1) {
				rev_branch(1, cur_tree);
				E_oy=E_scry;
				E_scry=t;
				put_tree();
				set_cur_tree(1);
				rev_branch(1, cur_tree);
			}
			return -1;
		}
	}
	else if(mx>x+24 && mx<x+lx*8-54 && my>y*8+8 && my<(y+ly-2)*8) {
L:
		temp=(tree far *)cur_tree;
		old=(tree far *)cur_tree;
		while(lbp) {
			getpos(mx, my);
			t=get_tree_mouse(mx, my);
			if(lbp) {
				if(mx<E_x+24 && cur_tree!=begin) {
					keyin_flag=1;
					key_input(UP, 0);
					return -1;
				}
				else if(mx>E_x+E_lx*8-52 && cur_tree->x!=ecd_h-1) {
					keyin_flag=1;
					key_input(DOWN, 0);
					return -1;
				}
				if(temp!=cur_tree) {
					rev_branch(0, temp);
					rev_branch(1, cur_tree);
					temp=(tree far *)cur_tree;
				}
			}
			else {
				if(old==cur_tree) {
					if(t) {
						return 1;
					}
					else return 0;
				}
				else return -1;
			}
		}
	}
	return -1;
}

byte my_buf[20];

int search_tree(byte far *str)
{
	tree *temp;
	int len, i=0;

	temp=begin;
	while(temp) {
		for(i=0; str[i]; i++) {
			if(str[i]==temp->name[i]) {
				if(str[i+1]==0) {
					cur_tree=temp;
					return 1;
				}
			}
			else break;
		}
		temp=temp->next;
	}
	return 0;
}

int put_search(int code, byte far *str)
{
	int t;
	if(code==-1) {
		put_string(E_x+E_lx*8-36-16, E_y+1, str, 0);
	}
	else {
		if(str[0]>96 && str[0]<123) {
			str[0]-=32;
		}
		my_buf[code]=str[0];
		my_buf[code+1]=0;
		t=search_tree(my_buf);
		if(t==1) {
			put_string(E_x+E_lx*8-36-16, E_y+1+code, str, 0);
			return 1;
		}
		else {
			my_buf[code]=0;
			return 0;
		}
	}
	return 0;
}

int get_name(byte far *head, byte far *buf, int len)
{
	int x, y, t, l;

	mc_off();
	x=E_x+(E_lx-10)*4;
	y=E_y+(E_ly-30)/2;
	get_image(x, y, x+100, y+40, emenu_buf);
	put_panel(x, y, x+80, y+30, 0);
	l=strlen(head);
	put_string(x+2, y+15-l/2, head, 0);
	gliney(x+2+18, y*8, (y+30)*8, 1);

	put_string(x+40, y+2, " ∑≥ùb :  ", 0);
	gbox(x+40-2, (y+12)*8-2, x+40+16+2, (y+27)*8+2, 1);
	buf[0]=0;
	t=get_string(0, x+40, y+12, len, 15, buf);
	put_image(x, y, x+100, y+40, emenu_buf);
	mc_on();
	if(t!=1) {
		while(lbp || rbp);
		return 0;
	}
	else return 1;
}

byte check_drv(byte drv)
{
	union REGS r;

	drv-='A';
	r.h.ah=0x15;
	r.h.dl=drv;
	int86(0x13, &r, &r);
	return r.h.ah;
}

void able_disk(byte far *str)
{
	 int save, disk, disks, i=0;

	 save = getdisk();

	 for (disk=0 ; disk<26 ; ++disk)
	 {
			setdisk(disk);
			if(disk == getdisk()) {
				if(disk==1) {
					if(check_drv('B')!=0) {
						str[i]= disk + 'A';
						i++;
					}
				}
				else {
					str[i]= disk + 'A';
					i++;
				}
			}
	 }
	 setdisk(save);
}

int get_cur(byte far *str)
{
	int i=0;
	for(i=0; str[i]; i++) {
		if(str[i]-'A'==work_drv) {
			return i;
		}
	}
	return 0;
}

int disk_mouse(int x, int y, int tot)
{
	int i, mx, my;

	getpos(mx, my);
	for(i=0; i<tot+1; i++) {
		if(mx>x+30+i*20+1 && my>(y+3)*8-1 && mx< x+30+(i+1)*20-4
				&& mx< (y+17)*8+1) {
			 return i;
		}
	}
	return -1;
}

int get_drive(byte far *head)
{
	int x, y, t, i, j, mx, my;
	int choice=0, rtn=0;
	byte disk[26];
	INKEY c;

	mc_off();
	setmem(disk, 26, 0);
	x=E_x+E_lx*4-150/2-10;
	y=E_y+(E_ly-20)/2;
	get_image(x, y, x+200, y+30, emenu_buf);
	put_panel(x, y, x+150, y+20, 0);
	i=strlen(head);
	put_string(x+2, y+10-i/2, head, 0);
	gliney(x+2+18, y*8, (y+20)*8, 1);
	able_disk(disk);

	j=strlen(disk);
	for(i=0; i<j; i++) {
		gprintf(x+30+i*20, y+3, 0, "    %c :    ", disk[i]);
		gbox(x+30+i*20, (y+3)*8-2, x+30+(i+1)*20-3, (y+17)*8+2, 1);
	}
	j--;
	choice=get_cur(disk);
	gbar(x+30+choice*20+1, (y+3)*8-1, x+30+(choice+1)*20-4, (y+17)*8+1, -1);
	mc_on();
	for(;;) {
		while(!key_hit()) {
			if(rbp) {
				while(rbp);
				rtn=-1;
				goto OUT;
			}
			if(lbp) {
				t=disk_mouse(x, y, j);
				mc_on();
				switch(t) {
					case -1 : break;
					default :
						mc_off();
						gbar(x+30+choice*20+1, (y+3)*8-1, x+30+(choice+1)*20-4, (y+17)*8+1, -1);
						choice=t;
						gbar(x+30+choice*20+1, (y+3)*8-1, x+30+(choice+1)*20-4, (y+17)*8+1, -1);
						mc_on();
						while(lbp);
						goto OUT;
				}
			}
		}
		mc_off();
		gbar(x+30+choice*20+1, (y+3)*8-1, x+30+(choice+1)*20-4, (y+17)*8+1, -1);
		mc_on();
		c.i=bios_key(0);
		if(c.ch[0]) {
			switch(c.ch[0]) {
				case ESC :
					rtn=-1;
					goto OUT;
				case RETURN :
					rtn=choice;
					goto OUT;
			}
		}
		else if(c.ch[1]) {
			switch(c.ch[1]) {
				case UP :
					choice--;
					if(choice<0) choice=j;
					break;
				case DOWN :
					choice++;
					if(choice>j) choice=0;
					break;
			}
		}
		mc_off();
		gbar(x+30+choice*20+1, (y+3)*8-1, x+30+(choice+1)*20-4, (y+17)*8+1, -1);
		mc_on();
	}
OUT :
	mc_off();
	put_image(x, y, x+200, y+30, emenu_buf);
	if(rtn > -1) {
		i=work_drv;
		j=disk[choice]-'A';
		if(i!=j) {
			work_drv=j;
			setdisk(work_drv);
			return j;
		}
	}
	return -1;
}

void page_dnkey()
{
	int i;
	tree *temp;

	if(E_scrx==ecd_h-14) {
		temp=begin;
		while(temp->x!=ecd_h-1) {
			temp=temp->next;
		}
		cur_tree=temp;
		return;
	}
	else {
		E_ox=E_scrx;
		E_scrx+=13;
		if(E_scrx>ecd_h-14) E_scrx=ecd_h-14;
		set_cur_tree(0);
		put_tree();
	}
}

void page_upkey()
{
	if(E_scrx==0) {
		cur_tree=begin;
		return;
	}
	else {
		E_ox=E_scrx;
		E_scrx-=13;
		if(E_scrx<0) E_scrx=0;
		set_cur_tree(0);
		put_tree();
	}
}

//extern unsigned long CURSORSPEED;

void get_path(void)
{
	INKEY c;
	int x, deep, code;
	byte i, search_flag=0;
	int t;
	byte str[2], t_name[80];
	tree *temp;
	long int time;

	str[1]=0;
	setmem(t_name, 80, 0);
	rev_branch(1, cur_tree);
	while(1) {
		mc_on();
		while(!key_hit()) {
			if(rbp) {
				while(rbp);
				goto OUT;
			}
			if(lbp) {
				code=ecd_mouse(mousex, mousey);
				if(keyin_flag==0) {
					while(lbp);
				}
				else {
//				for(time=0; time<CURSORSPEED; time++);
//          edelay(1);
				}
				switch(code) {
					case -10: goto _2;
					case -9 : goto _3;
					case -8 : goto _6;
					case -7 : goto _7;
					case -6 : goto _8;
					case -3 :
						while(lbp);
						goto OUT;
					case -2 :
						off_ecd_win();
						E_x=ecd_tx;
						E_y=ecd_ty;
						put_ecd_win();
						rev_branch(1, cur_tree);
						break;
					case -1 :
						break;
					case  1 :
						goto RN;
				}
				if(keyin_flag==0) while(lbp);
			}
			else keyin_flag=0;
		}
		x=cur_tree->x;
		deep=cur_tree->deep;
		c.i=bioskey(0);
		rev_branch(0, cur_tree);
		if(!c.ch[0]) {
			switch(c.ch[1]) {
				case UP :
					if(x>0) x--;
					else if(deep) deep--;
					else break;
					temp=find_deep_back(x, deep);
					if(!temp) {
						if(cur_tree->mother) cur_tree=cur_tree->mother;
					}
					else cur_tree=temp;
					search_flag=0;
					break;
				case DOWN :
					if(x<ecd_h-1) x++;
					else if(deep<ecd_w-1) deep++;
					else break;
					temp=find_deep(x, deep);
					if(!temp) {
						if(cur_tree->next) cur_tree=cur_tree->next;
					}
					else cur_tree=temp;
					search_flag=0;
					break;
				case RIGHT :
					if(cur_tree->next) cur_tree=cur_tree->next;
					search_flag=0;
					break;
				case LEFT :
					if(cur_tree->mother) cur_tree=cur_tree->mother;
					search_flag=0;
					break;
				case HOME : case CTRL_PGUP :
					cur_tree=begin;
					search_flag=0;
					break;
				case END :  case CTRL_PGDN :
					while(cur_tree->next) cur_tree=cur_tree->next;
					search_flag=0;
					break;
				case PGDN :
					page_dnkey();
					break;
				case PGUP :
					page_upkey();
					break;
				case F2 :
_2:
					make_tree();
					E_scrx=0;
					E_scry=0;
					put_tree();
					search_flag=0;
					break;
				case F3 :
_3:
					if((get_drive("§aéâ óaúa∑°ßaìe?")) >= 0) {
						getcwd(cur_dir, 80);
						movmem(cur_dir+2, cur_dir, 78);
						read_info();
						E_ox=E_scrx=0;
						E_oy=E_scry=0;
						mc_off();
						for(t=0; t<14; t++) {
							clear_space(t);
						}
						t=ecd_h-14;
						if(t<0) t=1;
						put_hbar(Hx, Hex, Hy);
						loc_hbar(Hx, Hex, Hy, t, E_scrx, E_ox);

						t=ecd_w-4;
						if(t<0) t=1;
						put_vbar(Wx, Wy, Wey);
						loc_vbar(Wx, Wy, Wey, t, E_scry, E_oy);
						gliney(Wx, Wy, Wey, 1);
						E_oy=E_ox=0;
						put_tree();
					}
					search_flag=0;
					break;
				case F6 :
_6:
					if(strcmp(cur_tree->name, cur_dir_name)==0) {
						error("—e∏Å ó°ùB…°ü°∑°üq∑e §aéâÆÅ ¥ÙØsì°îa!");
						break;
					}
					t=get_name("ó°ùB…°ü° ∑°üq§aéÅã°", t_name, 13);
					if(t==1) strcpy(path, cur_tree->name);
					else break;
					for(i=0;t_name[i];i++) t_name[i]=toupper(t_name[i]);
					strcpy(cur_dir, t_name);
					if(strlen(t_name)==0) break;
					get_full_path(path, cur_tree);
					get_full_path(t_name, cur_tree);
					if(rename(path, t_name)==0) {
						strcpy(cur_tree->name, cur_dir);
						tree_mody=ON;
						put_tree();
					}
					else strcpy(cur_dir, cur_tree->name);
					search_flag=0;
					break;
				case F7 :  case INSERT :
_7:
					t=get_name("¨Å ó°ùB…°ü° †eóiã°", t_name, 13);
					if(t==1) strcpy(path, t_name);
					else break;
					for(i=0;path[i];i++) path[i]=toupper(path[i]);
					if(strlen(path)==0) break;
					if(cur_tree->next->mother==cur_tree) {
						cur_tree->next->sflag=0;
						i=OFF;
					}
					else i=ON;
					insert_tree(cur_tree->deep+1, 1, path, cur_tree, cur_tree);
					if(i==ON) cur_tree->next->end=ON;
					get_full_path(path, cur_tree->next);
					if(mkdir(path)==0) {
						tot_dir++;
						reset_scrbar();
						tree_mody=ON;
					}
					else {
						del_tree(cur_tree);
						error("ó°ùB…°ü°üi †eóiÆÅ ¥ÙØsì°îa!");
					}
					put_tree();
					search_flag=0;
					break;
				case F8 : case DELETE :
_8:				strcpy(t_name, cur_tree->name);
					get_full_path(t_name, cur_tree);
					if(rmdir(t_name)==0) {
						cur_tree=prev_tree(cur_tree);
						del_tree(cur_tree);
						tot_dir--;
						reset_scrbar();
						tree_mody=ON;
						put_tree();
					}
					else error("ó°ùB…°ü°üi ª°∂âÆÅ ¥ÙØsì°îa!");
					search_flag=0;
					break;
			}
		}
		else {
			switch(c.ch[0]) {
				case ESC :
OUT:			if(tree_mody) save_info();
					ecd_end();
				case RETURN :
RN:				strcpy(cur_dir, cur_tree->name);
					if(cur_dir[0]!='\\') get_full_path(cur_dir, cur_tree);
					chdir(cur_dir);
					if(tree_mody) save_info();
					ecd_end();
				case BS :
					if(search_flag>1) {
						search_flag--;
						my_buf[search_flag]=0;
						str[0]=my_buf[search_flag-1];
						search_flag--;
						put_search(-1, "             ");
						put_search(-1, my_buf);
						goto S;
					}
					else search_flag=0;
					break;
				default :
					str[0]=c.ch[0];
S:				if(put_search(search_flag, str)==1) {
						search_flag++;
					}
					break;
			}
		}
		if(search_flag==0) {
			put_search(-1, "            ");
		}
		rev_branch(1, cur_tree);
		if(tree_mody==ON) save_info();
	}
}

void ecd_end()
{
	off_ecd_win();
	clear_tree();
	free(ecd_buf);
	free(emenu_buf);
	exit_21();
}

byte far *ecd_menu[]={
  "F2:ñA∑™ã°",
	"F3:ó°Øa«a",
  "F6:¨Å∑°üq",
	"F7:†eóiã°",
	"F8:ª°∂Åã°",
};

void put_emenu()
{
	int x, y, i;

	x=E_x+E_lx*8-16-32+14;
	y=E_y+1;

	gliney(E_x+E_lx*8-36-18, E_y*8+3, (E_y+E_ly)*8-16, 1);
	for(i=0; i<5; i++) {
		gbox(x-1, y*8-2+i*80, x+16, y*8-4+(i+1)*80, 1);
		put_string(x, y+i*10, ecd_menu[i], 0);
	}
}

int get_ecd_menu(int mx, int my)
{
	int x, y, i;

	x=E_x+E_lx*8-16-32+14;
	y=E_y+1;
	for(i=0; i<5; i++) {
		if(x-1<mx &&  y*8-2+i*80 <my && mx<x+16 && my<y*8-4+(i+1)*80) {
			return i;
		}
	}
	return -1;
}

void rev_ecd_menu(int i)
{
	int x, y;

	x=E_x+E_lx*8-16-32+14;
	y=E_y+1;
	mc_off();
	gbar(x, y*8-1+i*80, x+15, y*8-5+(i+1)*80, -1);
	mc_on();
}

void off_ecd_win()
{
	mc_off();
	put_image(E_x, E_y, E_x+E_lx*8, E_y+E_ly, ecd_buf);
	mc_on();
}

void put_ecd_win()
{
	int x, y, ex, ey, tot;

	mc_off();
	get_image(E_x, E_y, E_x+E_lx*8, E_y+E_ly, ecd_buf);
	put_ewin(E_x, E_y, E_lx, E_ly, 0, "âwù°§aéÅã°");
	put_emenu();
	Hx=E_x+24;
	Hex=E_x+E_lx*8-16;
	Hy=(E_y+E_ly-2)*8;
	Wx=E_x+E_lx*8-16;
	Wy=E_y*8+4;
	Wey=(E_y+E_ly-2)*8;
	put_tree();
	mc_off();
	put_hbar(Hx, Hex, Hy);
	tot=ecd_h-14;
	if(tot<0) tot=1;
	loc_hbar(Hx, Hex, Hy, tot, E_scrx, 0);
	put_vbar(Wx, Wy, Wey);
	tot=ecd_w-4;
	if(tot<0) tot=1;
	loc_vbar(Wx, Wy, Wey, tot, E_scry, 0);
	gliney(Wx, Wy, Wey, 1);
	put_search(-1, my_buf);
	mc_on();
}

void main(void)
{
	if(is_in_21()==0) {
		printf("Must executed in 21!!!\n");
		exit_21();
	}

	ecd_buf=(byte far *)malloc(20000);
	emenu_buf=(byte far *)malloc(10000);
	if(ecd_buf==0 || emenu_buf==0) {
		error("°A°°ü°àa ¶Åπ¢–sì°îa!!");
		exit_21();
	}

	E_lx=40;
	E_ly=53;
	E_x=(MaxX-E_lx*8)/2;
	E_y=(MaxY-E_ly*8)/16;
	work_drv=getdisk();
	getcwd(cur_dir, 80);
	movmem(cur_dir+2, cur_dir, 78);
	read_info();
	setmem(my_buf, 20, 0);
	put_ecd_win();
	get_path();
}
