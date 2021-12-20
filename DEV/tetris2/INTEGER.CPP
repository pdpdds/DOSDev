#include <stdlib.h>
#include "integer.h"

Integer::Integer(const String & string)
{
	value = atoi(string.getChars());
}

int Integer::parseInt(const String & string)
{
	return atoi(string.getChars());
}

String Integer::toString(int value)
{
	char string[10];
	itoa(value, string, 10);

	return String(string);
}

String Integer::toString()
{
	return toString(value);
}
