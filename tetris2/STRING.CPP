#include <string.h>
#include "string.h"

String::String(const char * string)
{
	size = strlen(string);
	this->string = new char[size + 1];
	strcpy(this->string, string);
}

String::String(const String & string)
{
	size = string.size;
	this->string = new char[size + 1];
	strcpy(this->string, string.string);
}

boolean String::equals(const String & anotherString)
{
	return strcmp(string, anotherString.string) == 0 ? true : false;
}

String String::operator+(const String & string)
{
	int newSize = size + string.size;
	char * tmp = new char[newSize + 1];
	strcpy(tmp, this->string);
	strcat(tmp, string.string);

	String newObject(tmp);
	delete [] tmp;
	return newObject;
}

String & String::operator=(const String & string)
{
	if (this == &string)
		return *this;

	delete [] this->string;
	size = string.size;
	this->string = new char[size + 1];
	strcpy(this->string, string.string);

	return *this;
}
