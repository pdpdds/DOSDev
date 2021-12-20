// --------------------------------------------------------------------------
// Project: Tetris Game That Has A Flexible Framework For Further Extension
// Organization: None, Personal
// --------------------------------------------------------------------------
// Class   : String
// Platform: None
// Purpose : Represented Character Strings Like The Java's
// License : GNU General Public License
// Release : Free Software
// Date    : 1999/12/28
// Author  : Yu Byoung-Hak; Sejong University Computer Engineering Department
// E-Mail  : xover0@hitel.net
// --------------------------------------------------------------------------
// Constructor Summary:
// - String()
//        Initializes a newly created String object with empty character
//        sequence.
// - String(const char * string)
//        Constructs a new string object using the character sequence.
// - String(const String & string)
//        Copy constructor of this class.
// Method Summary:
// - int length()
//        Returns the length of this string.
// - const char * getChars() const
//        Returns the characters of this string.
// - boolean equals(const String & anotherString)
//        Compares this string to another string.
// --------------------------------------------------------------------------

#ifndef _STRING_
#define _STRING_

#include "boolean.h"

class String
{
	private:
		char * string;
		int size;
	public:
		String();
		String(const char * string);
		String(const String & string);
		~String();
		int length();
		const char * getChars() const;
		boolean equals(const String & anotherString);
		String operator+(const String & string);
		String & operator=(const String & string);
};

inline String::String()
{
	size = 0;
	string = new char[1];
	*(string + 0) = 0;
}

inline String::~String()
{
	delete [] string;
}

inline int String::length()
{
	return size;
}

inline const char * String::getChars() const
{
	return string;
}

#endif
