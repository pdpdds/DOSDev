// --------------------------------------------------------------------------
// Project: Tetris Game That Has A Flexible Framework For Further Extension
// Organization: None, Personal
// --------------------------------------------------------------------------
// Class   : Integer
// Platform: None
// Purpose : Represented Int Value Like The Java's
// License : GNU General Public License
// Release : Free Software
// Date    : 1999/12/28
// Author  : Yu Byoung-Hak; Sejong University Computer Engineering Department
// E-Mail  : xover0@hitel.net
// --------------------------------------------------------------------------
// Constructor Summary:
// - Integer(int value)
//        Constructs a newly allocated Integer object using the value.
// - Integer(const String & string)
//        Constructs a newly allocated Integer object using the value
//        represented by the string.
// Method Summary:
// - int intValue()
//        Returns the value of this Integer as an int.
// - static int parseInt(const String & string)
//        Parses the string argument as a signed decimal integer.
// - static String toString(int value)
//        Returns a new String object representing the specified value.
// - String toString()
//        Returns a String object representing this Integer's value.
// --------------------------------------------------------------------------

#ifndef _INTEGER_
#define _INTEGER_

#include "string.h"

class Integer
{
	private:
		int value;
	public:
		Integer(int value);
		Integer(const String & string);
		int intValue();
		static int parseInt(const String & string);
		static String toString(int value);
		String toString();
};

inline Integer::Integer(int value)
{
	this->value = value;
}

inline int Integer::intValue()
{
	return value;
}

#endif
