#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define MAXCHAR 80

int ch, dloc=0, error_flag;
char cdata[MAXCHAR];

double factor(void);
double term(void);
double expression(void);
void Cerror(char *s);

int ch_from_buf(void)
{
	int t;

	do {
		t=(int)cdata[dloc];
		dloc++;
	}
	while(t==' ' || t=='\t');
	return t;
}

void loc_back(void)
{
	int t;

	do {
		dloc--;
		t=(int)cdata[dloc];
	}
	while(t==' ' || t=='\t');
	ch=cdata[dloc];
	dloc++;
}

void readch(void)
{
	ch=ch_from_buf();
}

double power(double x)
{
	double y;

	readch();
	y=factor();
	if(y==0) x=1;
	else x=pow(x, y);
	return x;
}

const char *Funcs[]={
	"e",
	"pi",
	"sin",
	"cos",
	"tan",
	"asin",
	"acos",
	"atan",
	"sinh",
	"cosh",
	"tanh",
	"exp",
	"log",
	"log10",
	"pow10",
	"abs",
	"sqrt",
	"toup",   // ceil
	"todown", // floor
	"sqr",
	"ln",
};

int what_func(char *str)
{
	int i;

	for(i=0; i<21; i++) {
		if(strcmp(Funcs[i], str)==0) return i;
	}
	return -1;
}

double math_func(void)
{
	int i=0, j;
	char str[100];
	double x;

	while(isalpha(ch) || isdigit(ch)) {
		str[i++]=tolower(ch);
		readch();
	}
	str[i]=0;
	loc_back();

	j=what_func(str);
	if(j>1)
		x=factor();
	switch(j) {
		case -1 :  default :
			Cerror("Ðq®ˆa ËiŸ³“¡”a!!");
			break;
		case 0  : return M_E;
		case 1  : return M_PI;
		case 2  : return sin(x);
		case 3  : return cos(x);
		case 4  : return tan(x);
		case 5  : return asin(x);
		case 6  : return acos(x);
		case 7  : return atan(x);
		case 8  : return sinh(x);
		case 9  : return cosh(x);
		case 10 : return tanh(x);
		case 11 : return exp(x);
		case 12 : case 20:
			return log(x);
		case 13 : return log10(x);
		case 14 : return pow10(x);
		case 15 : return abs(x);
		case 16 : return sqrt(x);
		case 17 : return ceil(x);
		case 18 : return floor(x);
		case 19 : return pow(x, x);
	}
	return 0;
}

double number(void)
{
	double x, a;
	int sign;

	if(ch=='+' || ch =='-') {
		sign = ch; readch();
	}
	if(isalpha(ch)) {
		return (sign=='-') ? -math_func() : math_func();
	}
//  if(!isdigit(ch)) Cerror("'('ˆa ´ô¯s“¡”a !!");
	x = ch - '0';
	while(readch(), isdigit(ch)) x = 10*x+(ch -'0');
	if(ch == '.') {
		a=1;
		while(readch(), isdigit(ch)) x+=(a/=10)*(ch - '0');
	}
	if(sign == '-') x=-x;

	if(ch=='^') {
		x=power(x);
	}
	return x;
}


double factor(void)
{
	double x;
	if(ch !='(') return number();
	readch();
	x=expression();
	if(ch != ')') {
    Cerror("')'ˆa ´ô¯s“¡”a !!");
		return 0;
	}
	readch(); return x;
}

double term(void)
{
	double x, y;

	x=factor();
	for(;;) {
		if(ch == '*') {
			readch();
			x*=factor();
		}
		else if(ch == '^') {
			x=power(x);
		}
		else if(ch == '/') {
			readch();
			y=factor();
			if(y==0) {
        Cerror("'0'·a¡ a’‰® ´ô¯s“¡”a!!");
				return 0;
			}
			x/=y;
		}
		else break;
	}
	return x;
}

double expression(void)
{
	double x;

	x=term();
	for(;;) {
		if(ch == '+') {
			readch();
			x+=term();
		}
		else if(ch =='-') {
			readch();
			x-=term();
		}
		else break;
	}
	return x;
}

double Result=0;

int matherr(struct exception *a)
{
  Cerror("‰¬e¤ñ¶áŸi ¤õ´ás“¡”a!!");
	return 1;
}

int calc_main(char *str)
{
	double x;

	x=dloc=error_flag=0;
	strcpy(cdata, str);
	readch();
	x=expression();
	if(ch != '\0') {
    Cerror("¢…¤ó·¡ ËiŸ³“¡”a!!");
		return 0;
	}
	else if(error_flag==0) {
		Result=x;
		return 1;
	}
	return 0;
}
