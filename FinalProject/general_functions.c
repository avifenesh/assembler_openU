#include "general_functions.h"
#include <stdio.h>
#include <string.h>

int spaceOrTab(char c)
{
	return (c == ' ') || (c == '\t');
}

/* copy src to dst without spaces in the begining and in
the end */
void clean(char* src, char* dst)
{
	int i = 0, j = 0;
	while ((src[i] != '\0') && spaceOrTab(src[i]))
	{
		i++;
	}
	while ((src[i] != '\0') && (src[i] != '\n'))
	{
		dst[j++] = src[i++];
	}
	dst[j] = '\0';
}

/* return 1 if c is upper case letter */
int is_upper_letter(char c)
{
	if ((c >= 'A') && (c <= 'Z'))
		return 1;
	else
		return 0;
}

/* return 1 if c is lower case letter */
int is_lower_letter(char c)
{
	if ((c >= 'a') && (c <= 'z'))
		return 1;
	else
		return 0;
}

/* return 1 if c is a letter */
int is_letter(char c)
{
	return is_upper_letter(c) || is_lower_letter(c);
}

int is_number(char c)
{
	if ((c >= '0') && (c <= '9'))
		return 1;
	else
		return 0;
}

int get_number_from_string(char* num_str, int* succeded)
{
	int num = 0;
	*succeded = (sscanf(num_str, "%d", &num) == 1);
	return num;
}

void move_left(char* str, int num)
{
	int i, j, str_len;
	for (i = 0; i < num; i++)
	{
		str_len = strlen(str);
		for (j = 0; j < str_len; j++)
		{
			str[j] = str[j + 1];
		}
	}
}

