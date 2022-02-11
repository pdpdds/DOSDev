// VARIOUS TEMPLATES

#ifndef _TEMPLATE_H
#define _TEMPLATE_H

// ABS

template <class type>
inline type abs(type nr)
{
	return (nr>=0)?nr:-nr;
}

// SWAP

template <class type>
inline void swap(type& nr1,type& nr2)
{
	type temp=nr1;
	nr1=nr2;
	nr2=temp;
}

// MAX

template <class type>
inline type max(type nr1,type nr2)
{
	return nr1>nr2?nr1:nr2;
}

// MIN

template <class type>
inline type min(type nr1,type nr2)
{
	return nr1<nr2?nr1:nr2;
}

// SORT

template <class type>
inline void sort(type& nr1,type& nr2)
{
	if (nr1>nr2)
		swap(nr1,nr2);
}

// FIT

template <class type>
void fit(type nr1,type& nr2,type nr3)
{
	nr2=max(nr1,nr2);
	nr2=min(nr2,nr3);
}

#endif