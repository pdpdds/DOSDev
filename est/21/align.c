static BYTE hs_align[128];
static BYTE dum[]={".,?!)}]>\"'"};

void vResetSpace(NODE *temp)
{
	WORD i=0;
	BYTE *buf;
	WORD tloc;

	_CHECKNODE(temp);
	if(ex_err==0) {
		vErrorMsgfDlg(1, 0, 0);
		return;
	}
	if(line_state>0) return;

	if(temp==CNode) tloc=wTL;
	else tloc=0;
	buf=temp->line;
	while(buf[i]!='\r') {
		if(buf[i]==0) {
			movmem(buf+i+1, buf+i,  temp->bn-i-1);
			temp->bn--;
			if(i<tloc) tloc--;
		}
		else if(buf[i]>127) i+=5;
		else if((buf[i+3] & 0x0f)==0x0f) {
			movmem(buf+i+4, buf+i,  temp->bn-i-4);
			temp->bn-=4;
			if(i<tloc) tloc-=4;
		}
		else i+=4;
	}
	if(temp==CNode && !block_task) wTL=tloc;
}

static int iResizeLHB(BYTE tlnum, NODE *n)
{
	BYTE *nhs;
//	int i;

	tlnum++;
	if(tlnum>n->lc[5]) {
		if(!block_task) tw[cur_w].total+=(tlnum-n->lc[5]);
		nhs=_REGETHS(tlnum, n);
	}
	else if(tlnum<n->lc[5]) {
		if(!block_task) tw[cur_w].total-=(n->lc[5]-tlnum);
		nhs=_REGETHS(tlnum, n);
	}
	else nhs=n->hs;
	if(!nhs) return 0;
	n->hs=nhs;
//	for(i=0;i<tlnum;i++) n->hs[i]=((n->hs[i] & 1) | hs_align[i]);
	memcpy(n->hs, hs_align, tlnum);
	n->lc[5]=tlnum;
	return 1;
}

static void vMarkEOL(int loc, BYTE *str)
{
	if(str[loc]>127) str[loc+2]|=0x80;
	else str[loc+1]|=0x80;
}

static void vAnalWord(NODE *n)
{
	int l;
	int c=1;
	int width;
	int loc=0;
	int split_loc=0;
	BYTE flag=1;
	BYTE hsize;
	BYTE *str;
	BYTE space_flag=ON;

	str=n->line;
	width=n->lc[3]-n->lc[2]+2;
	c=(int)((char)n->lc[1])+1;
	while(str[loc]==32) {
		c+=str[loc+2] & 0x0f;
		loc+=4;
	}
	for(l=0;;l++) {
		if(flag) hs_align[l]=0x10;
		while(1) {
			if(str[loc]>127) {
				if(c!=1) str[loc+2] &= 0x7f;
				c+=((str[loc+3] & 0x0f)<<1);
				if(c>width) break;
				hsize=str[loc+3] & 0xf0;
				if(hsize>(hs_align[l] & 0x70))	hs_align[l]=(hs_align[l] & 0x80) | hsize;
				loc+=5;
			}
			else {
				if(str[loc]=='\r') {
					if(!iResizeLHB(l, n))	vErrorMsgfDlg(1, 0, 0);
					return;
				}
				if(str[loc]==32) {
					split_loc=loc+4;
					space_flag=OFF;
				}
				if(c!=1) str[loc+1] &= 0x7f;
				c+=(str[loc+2] & 0x0f);
				if(c>width && str[loc]!=32) {
					if(space_flag) split_loc=loc;
					break;
				}
				hsize=str[loc+2] & 0xf0;
				if(hsize>(hs_align[l] & 0x70))	hs_align[l]=(hs_align[l] & 0x80) | hsize;
				loc+=4;
			}
		}
		if(split_loc) {
			vMarkEOL(split_loc, str);
			loc=split_loc;
			split_loc=0;
			c=1;
			flag=1;
			space_flag=ON;
		}
		else {
			if(str[loc]>127) loc+=5;
			else loc+=4;
			l--;
			flag=0;
		}
	}
}

static int near iCheckLineEnd(int wors, WORD loc, BYTE *str)
{
	if(str[loc+wors]<128 && str[loc+wors+3]==0x40) {
		if(strchr(dum, str[loc+wors])!=0) return str[loc+wors+2] & 0x0f;
	}
	return 0;
}

static void vAnalChar(NODE *n)
{
	int l;
	int c=1;
	int width;
	int loc=0;
	int dumc;
	BYTE hsize;
	BYTE *str;

	str=n->line;
	width=n->lc[3]-n->lc[2]+2;
	c=(int)((char)n->lc[1])+1;
	for(l=0;;l++) {
		hs_align[l]=0x10;
		while(1) {
			if(str[loc]>127) {
				if(c!=1) str[loc+2] &= 0x7f;
				c+=((str[loc+3] & 0x0f)<<1);
				dumc=iCheckLineEnd(5, loc, str);
				if(c+dumc>width) {
					str[loc+2]|=0x80;
					break;
				}
				hsize=str[loc+3] & 0xf0;
				if(hsize>(hs_align[l] & 0x70))	hs_align[l]=(hs_align[l] & 0x80) | hsize;
				loc+=5;
			}
			else {
				if(str[loc]=='\r') {
					if(!iResizeLHB(l, n))	vErrorMsgfDlg(1, 0, 0);
					return;
				}
				if(c!=1) str[loc+1] &= 0x7f;
				c+=(str[loc+2] & 0x0f);
				dumc=iCheckLineEnd(4, loc, str);
				if(c+dumc>width && str[loc]!=32) {
					str[loc+1]|=0x80;
					break;
				}
				hsize=str[loc+2] & 0xf0;
				if(hsize>(hs_align[l] & 0x70))	hs_align[l]=(hs_align[l] & 0x80) | hsize;
				loc+=4;
			}
		}
		c=1;
	}
}

static void vAnalMix(NODE *n)
{
	int l;
	int c=1;
	int width;
	int loc=0;
	int split_loc=0;
	int dumc;
	BYTE flag=1;
	BYTE hsize;
	BYTE *str;

	str=n->line;
	width=n->lc[3]-n->lc[2]+2;
	c=(int)((char)n->lc[1])+1;
	while(str[loc]==32) {
		c+=str[loc+2] & 0x0f;
		loc+=4;
	}
	for(l=0;;l++) {
		if(flag) hs_align[l]=0x10;
		while(1) {
			if(str[loc]>127) {
				if(c!=1) str[loc+2] &= 0x7f;
				c+=((str[loc+3] & 0x0f)<<1);
				dumc=iCheckLineEnd(5, loc, str);
				if(c+dumc>width) {
					str[loc+2]|=0x80;
					c=1;
					goto ENDLINE;
				}
				hsize=str[loc+3] & 0xf0;
				if(hsize>(hs_align[l] & 0x70))	hs_align[l]=(hs_align[l] & 0x80) | hsize;
				loc+=5;
				split_loc=loc;
			}
			else {
				if(str[loc]=='\r') {
					if(!iResizeLHB(l, n))	vErrorMsgfDlg(1, 0, 0);
					return;
				}
				if(str[loc]==32) split_loc=loc+4;
				if(c!=1) str[loc+1] &= 0x7f;
				c+=(str[loc+2] & 0x0f);
				if(c>width && str[loc]!=32) break;
				hsize=str[loc+2] & 0xf0;
				if(hsize>(hs_align[l] & 0x70))	hs_align[l]=(hs_align[l] & 0x80) | hsize;
				loc+=4;
			}
		}
		if(split_loc) {
			vMarkEOL(split_loc, str);
			loc=split_loc;
			split_loc=0;
			c=1;
			flag=1;
		}
		else {
			if(str[loc]>127) loc+=5;
			else loc+=4;
			l--;
			flag=0;
		}
ENDLINE:
	}
}



static void vGetLineWidth(BYTE *str)
{
	int loc=0;
	int sum=0;
	int lnum;
	int space_sum=0;

	for(lnum=0;;lnum++) {
		while(1) {
			if(str[loc]>127) {
				if(sum && str[loc+2]>127) break;
				sum+=((str[loc+3] & 0x0f)<<1);
				space_sum=sum;
				loc+=5;
			}
			else {
				if(str[loc]=='\r') {
					hs_align[lnum]=space_sum;
					return;
				}
				if(sum && str[loc+1]>127) break;
				sum+=(str[loc+2] & 0x0f);
				if(str[loc]!=32) space_sum=sum;
				loc+=4;
			}
		}
		hs_align[lnum]=space_sum;
		sum=0;
	}
}

BYTE null_code2[4]={32, 0x80, 0x11, 0x4f};

static int iInsertNullSpace1(int flag, NODE *n)
{
	BYTE *str;
	int lnum, elnum;
	int width, swidth;
	int rest;
	int insert_loc;
	int tloc;

	if(n==CNode) tloc=wTL;
	str=n->line;
	elnum=n->lc[5];
	vGetLineWidth(str);
	width=n->lc[3]-n->lc[2]+1;
	swidth=width-(int)((char)n->lc[1]);
	for(lnum=0;lnum<elnum;lnum++) {
		if(hs_align) {
			rest=(swidth-hs_align[lnum])/flag;
			if(rest>0) {
				rest--;
				insert_loc=wLine2Loc(lnum, n);
				str=_REGETLINE(n->bn+rest+4, str);
				if(!str) {
					vErrorMsgfDlg(1, 0, 0);
					return 0;
				}
				if(str[insert_loc]>127) str[insert_loc+2] &= 0x7f;
				else str[insert_loc+1] &= 0x7f;
				movmem(str+insert_loc, str+insert_loc+rest+4, n->bn-insert_loc);
				memcpy(str+insert_loc, null_code2, 4);
				setmem(str+insert_loc+4, rest, 0);
				n->bn+=rest+4;
				if(insert_loc<=tloc) tloc+=rest+4;
			}
		}
		swidth=width;
		n->line=str;
	}
	if(n==CNode && !block_task) wTL=tloc;
	return 1;
}

static int iInsertNullSpace2(NODE *n)
{
	BYTE *str;
	int lnum, elnum;
	int width, swidth;
	int rest;
	int loc;
	int line_end_loc=0, line_start_loc;
	int tloc;
	int rest_space_loc;

	if(n==CNode) tloc=wTL;
	str=n->line;
	elnum=n->lc[5]-1;
	vGetLineWidth(str);
	width=n->lc[3]-n->lc[2]+1;
	swidth=width-(int)((char)n->lc[1]);
	for(lnum=0;lnum<elnum;lnum++) {
		if(hs_align[lnum]) {
			rest=swidth-hs_align[lnum];
			if(rest>0) {
				str=_REGETLINE(n->bn+rest, str);
				if(!str) {
					vErrorMsgfDlg(1, 0, 0);
					return 0;
				}
				line_start_loc=loc=line_end_loc;
				if(line_start_loc==0) {
					while(str[line_start_loc]==32) line_start_loc+=4;
				}
				swidth=0;
				while(1) {
					if(str[loc]>127) {
						if(str[loc+2]>127 && swidth) break;
						loc+=5;
					}
					else {
						if(str[loc+1]>127 && swidth) break;
						loc+=4;
					}
					swidth=1;
				}
				line_end_loc=loc;
				while(loc && str[loc-1]<128 && str[loc-4]==32) loc-=4;
				rest_space_loc=loc;
				swidth=1;
				while(rest) {
					loc=rest_space_loc;
					while(rest) {
						if(line_start_loc==loc) break;
						if(str[loc-1]==0) loc--;
						else {
							if(str[loc-1]>127) loc-=5;
							else {
								if(str[loc-4]==32 && str[loc]!=32) {
									movmem(str+loc, str+loc+1, n->bn-loc);
									str[loc]=0;
									n->bn++;
									rest--;
									rest_space_loc++;
									line_end_loc++;
									if(loc<=tloc) tloc++;
									swidth=0;
								}
								loc-=4;
							}
						}
					}
					if(swidth) break;
				}
			}
			else line_end_loc=wLine2Loc(lnum+1, n);
		}
		else line_end_loc=wLine2Loc(lnum+1, n);
		swidth=width;
		n->line=str;
	}
	if(n==CNode && !block_task) wTL=tloc;
	return 1;
}

void vDelOverSpace(NODE *n)
{
	int lnum;
	BYTE far *str;
	WORD loc=0;
	BYTE flag=0;
	int c;

	c=(int)((char)n->lc[1])+n->lc[2];
	str=n->line;
	for(lnum=0;;lnum++) {
		while(1) {
			if(str[loc]>127) {
				if(str[loc+2]>127 && flag) break;
				flag=1;
				c+=(str[loc+3] & 0x0f )<<1;
				if(hs_align[lnum]<str[loc+3] & 0xf0) hs_align[lnum]=str[loc+3] & 0xf0;
				if(c>250) {
					str[loc+2]|=0x80;
					movmem(hs_align+lnum, hs_align+lnum+1, n->lc[5]-lnum);
					hs_align[lnum+1]=0x10;
					iResizeLHB(n->lc[5], n);
					break;
				}
				loc+=5;
			}
			else {
				if(str[loc]=='\r') return;
				else {
					if(str[loc+1]>127 && flag) break;
					flag=1;
					c+=str[loc+2] & 0x0f;
					if(hs_align[lnum]<str[loc+2] & 0xf0) hs_align[lnum]=str[loc+2] & 0xf0;
					if(c>250) {
						str[loc+1]|=0x80;
						movmem(hs_align+lnum, hs_align+lnum+1, n->lc[5]-lnum);
						hs_align[lnum+1]=0x10;
						iResizeLHB(n->lc[5], n);
						break;
					}
					loc+=4;
				}
			}
		}
		c=n->lc[2];
		flag=0;
	}
}

void vAnalText(NODE *n)
{
	BYTE method=n->lc[4];

	_CHECKNODE(n);
	if(ex_err==0) {
		vErrorMsgfDlg(1, 0, 0);
		return;
	}
	tw[cur_w].modify=ASV_MODIF=ON;
	if(line_state>0) return;
	vResetSpace(n);
	if(method==2 || method==3) {
		vAnalWord(n);
//		vDelOverSpace(n);
		iInsertNullSpace1(method-1, n);
		return;
	}
	if(method==6) vAnalMix(n);
	else if(method==5 | method==0) vAnalChar(n);
	else vAnalWord(n);
//	vDelOverSpace(n);
	if(method>3) iInsertNullSpace2(n);
}
