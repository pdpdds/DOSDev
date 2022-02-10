BYTE bCombine3(char ch, WORD *f)
{
	static HANKEY a={0, 0, 1};
	BYTE new_old=OLD;

	if(_CAPS_LOCK) {
		if((ch>64 && ch<91) || (ch>96 && ch<123)) ch=ch^0x0020;
	}
	if(start==ON && status<10) {
START :
		c.i=HANNULL;
		a.b=cho_jung_390[ch-32];
		if(a.key.flag==0) return a.b;
		if(a.key.code==0) {
			a.b=jong_jung_390[ch-32];
			c.c3.code3=a.key.code;
			switch(a.key.check) {
				case 0 : status=JONGSONG1; break;
				case 1 : status=JONGSONG2; break;
				case 2 : status=JUNGSONG1; break;
				case 3 : status=JUNGSONG2; break;
			}
		}
		else {
			switch(a.key.check) {
				case 0 : c.c3.code1=a.key.code;	status=CHOSONG1; break;
				case 1 : c.c3.code1=a.key.code;	status=CHOSONG2; break;
				case 2 : c.c3.code2=a.key.code;	status=JUNGSONG1;	break;
				case 3 : c.c3.code2=a.key.code;	status=JUNGSONG2;	break;
			}
		}
		c.c3.flag=1;
		start=OFF;
		return new_old;
	}
	else {
		if(status<10) {
			a.b=jong_jung_390[ch-32];
			if(a.key.flag==0) return a.b;
			switch(status) {
				case CHOSONG1 :
					switch(a.key.check) {
						case 0 : case 1 :
							*f=c.i; start=ON; new_old=NEW;
							goto START;
						case 2 : c.c3.code2=a.key.code; status=JUNGSONG1; break;
						case 3 : c.c3.code2=a.key.code;	status=JUNGSONG2;	break;
					}
					break;
				case CHOSONG2 :
					switch(a.key.check) {
						case 0 : case 1 : a.b=cho_jung_390[ch-32];
							if(a.key.check==1) {
								c.c3.code1=a.key.code+1;
								status=CHOSONG1;
							}
							else {
								*f=c.i; start=ON; new_old=NEW;
								status=CHOSONG1; goto START;
							}
							break;
						case 2 : c.c3.code2=a.key.code;	status=JUNGSONG1;	break;
						case 3 : c.c3.code2=a.key.code;	status=JUNGSONG2;	break;
					}
					break;
				case JUNGSONG1 :
					switch(a.key.check) {
						case 0 :
							if(a.key.code==NULL) {
								*f=c.i; start=ON; new_old=NEW;
								goto START;
							}
							else c.c3.code3=a.key.code; status=JONGSONG1; break;
						case 1 : c.c3.code3=a.key.code; status=JONGSONG2; break;
						case 2 : *f=c.i; start=ON; new_old=NEW;
							goto START;
						case 3 : *f=c.i; start=ON; new_old=NEW;
							goto START;
					}
					break;
				case JUNGSONG2 :
					switch(a.key.check) {
						case 0 :
							if(a.key.code==NULL) {
								*f=c.i; start=ON; new_old=NEW;
								goto START;
							}
							else c.c3.code3=a.key.code; status=JONGSONG1; break;
						case 1 : c.c3.code3=a.key.code; status=JONGSONG2; break;
						case 2 : case 3 :
							switch(c.c3.code2) {
								case 13 :
									if(ch=='f') c.c3.code2=14;
									else if(ch=='r') c.c3.code2=15;
									else if(ch=='d') c.c3.code2=18;
									else {
										*f=c.i; start=ON; new_old=NEW;
										goto START;
									}
									if(a.key.check==2) status=JUNGSONG1;
									else status=JUNGSONG2; break;
								case 20 :
									if(ch=='t') c.c3.code2=21;
									else if(ch=='c') c.c3.code2=22;
									else if(ch=='d') c.c3.code2=23;
									else {
										*f=c.i; start=ON; new_old=NEW;
										goto START;
									}
									if(a.key.check==2) status=JUNGSONG1;
									else status=JUNGSONG2; break;
								case 27 :
									if(ch=='d') c.c3.code2=28;
									else {
										*f=c.i; start=ON; new_old=NEW;
										goto START;
									}
									if(a.key.check==2) status=JUNGSONG1;
									else status=JUNGSONG2; break;
								default :
									*f=c.i; start=ON; new_old=NEW;
									goto START;
							}
							break;
					}
					break;
			}
		}
		else {
			a.b=cho_jung_390[ch-32];
			if(a.key.flag==0) return a.b;
			switch(status) {
				case JONGSONG1 :
					*f=c.i; start=ON; new_old=NEW;
					goto START;
				case JONGSONG2 :
					switch(a.key.check) {
						case 0 : case 1 :
							switch(c.c3.code3) {
								case 2 :
									if(ch=='x') c.c3.code3=3;
									else if(ch=='q') c.c3.code3=4;
									else {
										*f=c.i; start=ON; new_old=NEW;
										goto START;
									}
									break;
								case 5 :
									if(ch=='!') c.c3.code3=6;
									else if(ch=='1') c.c3.code3=7;
									else {
										*f=c.i; start=ON; new_old=NEW;
										goto START;
									}
									break;
								case 9 :
									switch(ch) {
										case 'x' : c.c3.code3=10; break;
										case 'z' : c.c3.code3=11; break;
										case '3' : c.c3.code3=12; break;
										case 'q' : c.c3.code3=13; break;
										case 'W' : c.c3.code3=14; break;
										case 'Q' : c.c3.code3=15; break;
										case '1' : c.c3.code3=16; break;
										default :
											*f=c.i; start=ON; new_old=NEW;
											goto START;
									}
									break;
								case 19 :
									if(ch=='q')c.c3.code3=20;
									else {
										*f=c.i; start=ON; new_old=NEW;
										goto START;
									}
									break;
								case 21 :
									if(ch=='q') c.c3.code3=22;
									else {
										*f=c.i; start=ON; new_old=NEW;
										goto START;
									}
									break;
								default :
									*f=c.i; start=ON; new_old=NEW;
									goto START;
							}
							break;
						case 2 : case 3 :
							*f=c.i; start=ON; new_old=NEW;
							goto START;
					}
					break;
			}
		}
		c.c3.flag=1;
	}
	return new_old;
}
