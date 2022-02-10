
#define MYUNGJO  1
#define GOTH     2
#define UPHALF   3
#define DOWNHALF 4
#define PILKEE   5
#define LOMAN    6
#define NORM_VID 0
#define REV_VID  1

#define BIT_1 	0x80
#define BIT_2   0x40
#define BIT_3   0x20
#define BIT_4   0x10
#define BIT_5   0x08
#define BIT_6   0x04
#define BIT_7   0x02
#define BIT_8   0x01

BYTE near table_0[32]={
	0xff,0xff,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
	17,18,19,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};
BYTE near table_1[32]={
	0xff,0xff,0xff,0,1,2,3,4,0xff,0xff,5,6,7,8,9,10,0xff,
	0xff,11,12,13,14,15,16,0xff,0xff,17,18,19,20,0xff,0xff
};
BYTE near table_2[32]={
	0xff,0xff,0,1,2,3,4,5,6,7,8,9,10,11,12,13,
	14,15,0xff,16,17,18,19,20,21,22,23,24,25,26,0xff,0xff
};

BYTE near cho_type_1[]={0, 0, 0, 0, 0, 0, 0, 0, 1, 3,
									 3, 3, 1, 2, 4, 4, 4, 2, 1, 3, 0};
BYTE near cho_type_2[]={5, 5, 5, 5, 5, 5, 5, 5, 6, 7,
									 7, 7, 6, 6, 7, 7, 7, 6, 6, 7, 5};
BYTE near jong_type[]={0, 2, 0, 2, 1, 2, 1, 2, 3, 0,
									2, 1, 3, 3, 1, 2, 1, 3, 3, 1, 1};

WORD null_wimage[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
BYTE null_image[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

