
#define MAXNODE 400
#define NODEBLK 5
BYTE *node_buf[NODEBLK]={0, 0, 0, 0, 0};
int node_use[NODEBLK]={0, 0, 0, 0, 0};
int blk_num=0;
int node_num=0;

NODE *npGetNode(void)
{
	BYTE i;
	int j;
	BYTE *t;

	for(i=0;i<NODEBLK;i++) {
		if(node_buf[i]==0) {
			node_buf[i]=(BYTE *)pGetMem((WORD)MAXNODE*sizeof(NODE));
			if(!node_buf[i]) goto ERR;
			setmem(node_buf[i], (WORD)MAXNODE*sizeof(NODE), 0);
		}
		if((int)node_use[i]<MAXNODE) {
			t=node_buf[i];
			for(j=0;j<MAXNODE;j++, t+=sizeof(NODE)) {
				if(t[0]==0) {
					node_use[i]++;
					t[0]=0x80;
					((NODE *)t)->tt=ttloc++;
					return (NODE *) t;
				}
			}
		}
	}
ERR:
	vErrorMsgfDlg(0, 0, "¢…”e· ˆ•®“e 2000ˆ¡ ¹AÐe–A ·¶¯s“¡”a");
	return 0;
}

static void vFreeNodeBuf(NODE *iDelKey)
{
	int i;
	BYTE *t;

	t=(BYTE *)iDelKey;
	for(i=0;i<NODEBLK;i++) {
		if(node_buf[i]) {
			if(FP_SEG(t)==FP_SEG(node_buf[i])) {
				node_use[i]--;
				t[0]=0;
				if(node_use[i]<1) {
					free(node_buf[i]);
					node_buf[i]=0;
				}
				return;
			}
		}
	}
	vErrorMsgfDlg(1, INTERNAL_ERR, 0);
}

NODE *npGetFullNode(WORD len, BYTE hs)
{
	NODE *temp;

	temp=_GETNODE;
	if(!temp) return 0;
	temp->line=_GETLINE(len);
	if(!temp->line) {
		vFreeNodeBuf(temp);
		return 0;
	}
	temp->hs=_GETHS(hs);
	if(!temp->hs) {
		free(temp->line);
		vFreeNodeBuf(temp);
		return 0;
	}
	return temp;
}

void vFreeNode(NODE *temp)
{
	vFreeNodeBuf(temp);
	if(temp->mh==0) free(temp->line);
	else vFreeMem(&(temp->mh));
	vFreeLHB(temp->hs, temp->lc[5]);
}

int	iInitFontCache(void);

void vInitNodeList(void)
{
	if(!hs_buf) {
		hs_buf=(BYTE *)pAllocUMB(20000);
		if(!hs_buf) {
			vErrorMsgfDlg(1, INTERNAL_ERR, 0);
			vExit();
		}
	}
	setmem(hs_buf, 20000, 0);
	iInitFontCache();
	SNode=_GETNODE;
	CNode=_GETNODE;
	CNode->line=_GETLINE(1);
	CNode->hs=_GETHS(1);
	(CNode->line)[0]='\r';
	memcpy(CNode->lc, def_lc, 6);
	CNode->lc[5]=1;
	CNode->hs[0]=0x10;
	CNode->bn=1;
	CNode->kd=0;
	SNode->prev=NULL;
	SNode->next=CNode;
	CNode->prev=SNode;
	CNode->next=NULL;
	ENode=CNode;
	tw[cur_w].total=1;
}

int iAppendNode(BYTE *str, BYTE *lc, BYTE *hs,
								WORD len, BYTE kd)
{
	NODE *temp;

	temp=_GETNODE;
	if(temp==0) {
		return 0;
	}
	temp->line=str;
	temp->hs=hs;
	memcpy(temp->lc, lc, 6);

	temp->kd=kd;
	temp->bn=len;
	ENode->next=temp;
	temp->prev=ENode;
	temp->next=0;
	ENode=temp;
	tw[cur_w].total+=lc[5];
	return 1;
}


int iInsertNode(NODE *over_node, BYTE *str, BYTE *lc, BYTE *hs,
								WORD len, BYTE kd)
{
	NODE *temp;

	temp=_GETNODE;
	if(temp==0) return 0;
	temp->line=str;
	temp->hs=hs;
	memcpy(temp->lc, lc, 6);
	temp->bn=len;
	temp->kd=kd;
	if(over_node->next) {
		over_node->next->prev=temp;
		temp->next=over_node->next;
		over_node->next=temp;
		temp->prev=over_node;
	}
	else {
		ENode->next=temp;
		temp->prev=ENode;
		temp->next=0;
		ENode=temp;
	}
	tw[cur_w].total+=lc[5];
	return 1;
}


void vDeleteNode(NODE *del_node)
{
	vFreeNodeBuf(del_node);
	tw[cur_w].total-=del_node->lc[5];
	if(del_node->next) {
		del_node->prev->next=del_node->next;
		del_node->next->prev=del_node->prev;
	}
	else {
		ENode=del_node->prev;
		ENode->prev=del_node->prev->prev;
		ENode->next=0;
	}
	if(del_node->mh==0) free(del_node->line);
	else vFreeMem(&(del_node->mh));
	vFreeLHB(del_node->hs, del_node->lc[5]);
}

void vClearNodeList(void)
{
	NODE *temp;

	if(!SNode) return;
	temp=ENode;
	while(temp->prev) {
		temp=temp->prev;
		vFreeNodeBuf(temp->next);
		if(temp->next->mh==0) free(temp->next->line);
		else vFreeMem(&(temp->next->mh));
		vFreeLHB(temp->next->hs, temp->next->lc[5]);
	}
	vFreeNodeBuf(SNode);
	if(hs_buf) {
		free(hs_buf);
		hs_buf=0;
	}
	ttloc=dtloc=0;
	SNode=0;
}

void vResetNodeList(void)
{
	vClearNodeList();
	iFreeFontBuffer(1);
	vInitNodeList();
}

void vInitLoad(void)
{
	vResetNodeList();
	vFreeNodeBuf(ENode);
	if(ENode->mh==0) free(ENode->line);
	else vFreeMem(&(ENode->mh));
	vFreeLHB(ENode->hs, ENode->lc[5]);
	SNode->next=NULL;
	ENode=CNode=SNode;
	tw[cur_w].total=0;
}

int iMergeNode(NODE *mnode)
{
	NODE *temp;
	BYTE *str;

	_CHECKNODE(mnode);
	if(ex_err==0) {
		vErrorMsgfDlg(1, 0, 0);
		return 0;
	}
	_CHECKNODE(mnode->next);
	if(ex_err==0) {
		vErrorMsgfDlg(1, 0, 0);
		return 0;
	}
	if(!mnode->next) return 1;
	temp=mnode;
	if(temp->lc[5]+temp->next->lc[5]>100) {
		vErrorMsgfDlg(0, 0, "¢…”e·¡ á¢ ‹³“¡”a");
		return 2;
	}
	str=temp->line;
	str=_REGETLINE(temp->bn+temp->next->bn-1, str);
	if(!str) return 0;
	temp->line=str;
	memcpy(temp->line+temp->bn-1, temp->next->line,
				 temp->next->bn);
	temp->bn=temp->bn+temp->next->bn-1;
	mnode=temp;

	vDeleteNode(mnode->next);
	return 1;
}

int iSplitNode(void)
{
	BYTE *ths;
	BYTE lc[6];
	BYTE *str;

	memcpy(lc, CNode->lc, 6);
	lc[5]=1;
	str=_GETLINE(CNode->bn-wTL);
	if(!str) return 0;
	ths=_GETHS(lc[5]);
	if(!str) {
		free(str);
		return 0;
	}
	memcpy(str, CNode->line+wTL, CNode->bn-wTL);
	ths[0]=0x10;

	if(!iInsertNode(CNode, str,
									lc, ths, CNode->bn-wTL, 0)) return 0;

	CNode->line[wTL]='\r';
	CNode->bn=wTL+1;
	return 1;
}

