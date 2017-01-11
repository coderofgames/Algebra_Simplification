#ifndef STRING_NUMBER_H
#define STRING_NUMBER_H

#include "StringUtils.h"
#include <cstring>

// found this on stack overflow, however currently not in use.

enum{
	STRING_IS_INVALID_NUMBER = 0,
	STRING_IS_HEXA,
	STRING_IS_INT,
	STRING_IS_DOUBLE
};

bool isDigit(char c){
	return (('0' <= c) && (c <= '9'));
}

bool isHexaDigit(char c){
	return ((('0' <= c) && (c <= '9')) || ((tolower(c) <= 'a') && (tolower(c) <= 'f')));
}


char *ADVANCE_DIGITS(char *aux_p){

	while (isDigit(*aux_p)) aux_p++;
	return aux_p;
}

char *ADVANCE_HEXADIGITS(char *aux_p){

	while (isHexaDigit(*aux_p)) aux_p++;
	return aux_p;
}


int isNumber(const string & test_str_number){
	bool isHexa = false;
	char *str = (char *)test_str_number.c_str();

	switch (*str){
	case '-': str++; // is negative number ...
		break;
	case '0':
		if (tolower(*str + 1) == 'x')  {
			isHexa = true;
			str += 2;
		}
		break;
	default:
		break;
	};

	char *start_str = str; // saves start position...
	if (isHexa) { // candidate to hexa ...
		str = ADVANCE_HEXADIGITS(str);
		if (str == start_str)
			return STRING_IS_INVALID_NUMBER;

		if (*str == ' ' || *str == 0)
			return STRING_IS_HEXA;

	}
	else{ // test if integer or float
		str = ADVANCE_DIGITS(str);
		if (*str == '.') { // is candidate to double
			str++;
			str = ADVANCE_DIGITS(str);
			if (*str == ' ' || *str == 0)
				return STRING_IS_DOUBLE;

			return STRING_IS_INVALID_NUMBER;
		}

		if (*str == ' ' || *str == 0)
			return STRING_IS_INT;

	}

	return STRING_IS_INVALID_NUMBER;


}


/* Found these on Stack Overflow, they are just ways of printing out long numbers ... however it got a -1 vote so I think it needs testing ...*/
#define ZERO 48
#define NINE 57
#define MINUS 45
#define DECPNT 46

long strtolng_n(char* str, int n)
{
	int sign = 1;
	int place = 1;
	long ret = 0;

	int i;
	for (i = n - 1; i >= 0; i--, place *= 10)
	{
		int c = str[i];
		switch (c)
		{
		case MINUS:
			if (i == 0) sign = -1;
			else return -1;
			break;
		default:
			if (c >= ZERO && c <= NINE) ret += (c - ZERO) * place;
			else return -1;
		}
	}

	return sign * ret;
}

double _double_fraction(char* str, int n)
{
	double place = 0.1;
	double ret = 0.0;

	int i;
	for (i = 0; i < n; i++, place /= 10)
	{
		int c = str[i];
		ret += (c - ZERO) * place;
	}
	return ret;
}
double strtodbl(char* str)
{
	int n = 0;
	int sign = 1;
	int d = -1;
	long ret = 0;

	char* temp = str;
	while (*temp != '\0')
	{
		switch (*temp)
		{
		case MINUS:
			if (n == 0) sign = -1;
			else return -1;
			break;
		case DECPNT:
			if (d == -1) d = n;
			else return -1;
			break;
		default:
			if (*temp < ZERO && *temp > NINE) return -1;
		}
		n++;
		temp++;
	}

	if (d == -1)
	{
		return (double)(strtolng_n(str, n));
	}
	else if (d == 0)
	{
		return _double_fraction((str + d + 1), (n - d - 1));
	}
	else if (sign == -1 && d == 1)
	{
		return (-1)*_double_fraction((str + d + 1), (n - d - 1));
	}
	else if (sign == -1)
	{
		ret = strtolng_n(str + 1, d - 1);
		return (-1) * (ret + _double_fraction((str + d + 1), (n - d - 1)));
	}
	else
	{
		ret = strtolng_n(str, d);
		return ret + _double_fraction((str + d + 1), (n - d - 1));
	}
}


#endif