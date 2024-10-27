// Define all helper functions for hw2 in this file
#include "helpers2.h"
#define NULL ((void*) 0x0)


int myStrLen(char *str)
{
	// returns len of string
	int counter = 0;
	char *val = str;
	while(1)
	{
		if (*val == '\0')
		{
			return counter;
		}
		else
		{
			++counter;
			++val; 
		}
	}
}

int myStrCmp(char *str1, char *str2)
{
	// returns 0 if true 
	// positive if c1 is greater than c2
	// negative if c1 i less than c2
	char *c1 = str1;
	char *c2 = str2;
	if (c1 == NULL || c2 == NULL)
	{
		return 1;
	}

	while(1)
	{
		if (*c1 != *c2)
		{
			return *c1 - *c2;
		}
		else
		{
			if (*c1 == '\0')
			{
				return 0;
			}
			++c1;
			++c2;
		}
	}
}


int isin(char c1, char *str)
{
	// checks if char is in str and returns 1 if so and 0 if not
	char *s = str;
	while(1)
	{
		if (s == NULL)
		{
			return 0;
		}
		else if (c1 == *s)
		{
			return 1;
		}
		else if (*s == '\0')
		{
			return 0;
		}
		else
		{
			++s;
		}
	}

}


void clear_buffer(char *s)
{
	char *i = s;
	while (1)
	{
		if (*i == '\0')
		{
			break;
		}
		*i = '\0';
		++i;
	}

}


int has_4_commas(char *s)
{
	// returns 1 if the string has 5 commas and 0 if not
	int count = 0;
	char *ptr = s;
	while(*ptr != '\0')
	{
		if (*ptr == ',')
		{
			count++;
		}
		ptr += 1;
	}
	if (count >= 4)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}



