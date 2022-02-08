/* --------------------------------------------------------------------
	 프로그램 : 주어진 경로명내의 화일과 디렉토리 목록을 찾아낸 후,
				소트하여 배열에 기억시킨다
	 인 자 값 : 화일경로명
	 작 성 자 : 미스테리 / 1995.02
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
	sort(FILE_dir, 0, t-1);  /* total : 총파일의 갯수 */

	if(!findfirst(path, &fblock, FA_ARCH))
	{
		strcpy(FILE_dir[total].filename, fblock.ff_name);
		FILE_dir[total++].attrib =fblock.ff_attrib;
			      usedBytes +=fblock.ff_fsize;
		while(!findnext(&fblock))
		{	/* 아키브 파일을 배열에 저장 */
			strcpy(FILE_dir[total].filename, fblock.ff_name);
			FILE_dir[total++].attrib =fblock.ff_attrib;
		      /* 파일의 크기 저장 */
				      usedBytes +=fblock.ff_fsize;
		}
	}
	sort(FILE_dir,t,total-1);
//	sprintf(buffer,"총 %3d 개 화일,", total);
	sprintf(buffer," %-3d File(s) ", total);
	memset(buf,'\0',11);
	ultoa(usedBytes,buf,10);
	put_str(21,18,buffer,3<<4|15);
	put_str(37,18,buf,   3<<4|15);
//	put_str(49,18,"바이트 사용",3<<4|15);
	put_str(49,18," Bytes Used",3<<4|15);
}

/* ------- Quick Sort 알고리즘 이용 소트 함수 ---------- */

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
