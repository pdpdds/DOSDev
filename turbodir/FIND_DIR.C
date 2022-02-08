/* --------------------------------------------------------------------
	 ���α׷� : �־��� ��θ����� ȭ�ϰ� ���丮 ����� ã�Ƴ� ��,
				��Ʈ�Ͽ� �迭�� ����Ų��
	 �� �� �� : ȭ�ϰ�θ�
	 �� �� �� : �̽��׸� / 1995.02
----------------------------------------------------------------------- */

void sort(struct dirfile files[], int left, int right);

void find_dir(char *path)
{
	struct ffblk fblock;
	unsigned long usedBytes=0L;
	char buffer[16];
	char buf[11];
	int j,t;

	total=t=0;

	for(j=0; j<300; j++) {
		memset(FILE_dir[j].filename,'\0',12);
		FILE_dir[j].attrib='\0';
	}

	if(!findfirst(path,&fblock,FA_DIREC))
	{
		while(!findnext(&fblock))
		{
			if(fblock.ff_attrib==FA_DIREC)
			{
				strcpy(FILE_dir[total].filename,fblock.ff_name);
				FILE_dir[total++].attrib=fblock.ff_attrib;
					      usedBytes+=fblock.ff_fsize;
				t++;
			}
		}
	}
	sort(FILE_dir, 0, t-1);  /* total : �������� ���� */

	if(!findfirst(path, &fblock, FA_ARCH))
	{
		strcpy(FILE_dir[total].filename, fblock.ff_name);
		FILE_dir[total++].attrib =fblock.ff_attrib;
			      usedBytes +=fblock.ff_fsize;
		while(!findnext(&fblock))
		{	/* ��Ű�� ������ �迭�� ���� */
			strcpy(FILE_dir[total].filename, fblock.ff_name);
			FILE_dir[total++].attrib =fblock.ff_attrib;
		      /* ������ ũ�� ���� */
				      usedBytes +=fblock.ff_fsize;
		}
	}
	sort(FILE_dir,t,total-1);
//	sprintf(buffer,"�� %3d �� ȭ��,", total);
	sprintf(buffer," %-3d File(s) ", total);
	memset(buf,'\0',11);
	ultoa(usedBytes,buf,10);
	put_str(21,18,buffer,3<<4|15);
	put_str(37,18,buf,   3<<4|15);
//	put_str(49,18,"����Ʈ ���",3<<4|15);
	put_str(49,18," Bytes Used",3<<4|15);
}

/* ------- Quick Sort �˰����� �̿� ��Ʈ �Լ� ---------- */

void sort(struct dirfile files[], int left, int right)
{

	char x[13];
	struct dirfile temp;
	register int i=left;
	register int j=right;

	strcpy(x,files[(left+right)/2].filename);

	do {
		while(strncmp(files[i].filename,x,12)<0 && i<right)i++;
		while(strncmp(files[j].filename,x,12)>0 && j> left)j--;
		if(i <= j)
		{
			    temp=files[i];
			files[i]=files[j];
			files[j]=temp;
			i++;
			j--;
		}
	} while(i<=j);

	if(left < j ) sort(files,left, j);
	if(i < right) sort(files, i, right);
}