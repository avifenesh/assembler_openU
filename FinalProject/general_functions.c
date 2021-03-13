#include "general_functions.h"
#include <stdio.h>
#include <string.h>

int spaceOrTab(char c)
{
	return (c == ' ') || (c == '\t');
}

/*clean begin and end from spaces and copy src to dst*/
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

/*1 if upper-case letter*/
int is_upper_letter(char c)
{
	if ((c >= 'A') && (c <= 'Z'))
		return 1;
	else
		return 0;
}

/*1 if lower-case letter*/
int is_lower_letter(char c)
{
	if ((c >= 'a') && (c <= 'z'))
		return 1;
	else
		return 0;
}

/*1 if letter*/
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

/* check if num_str contain a number if it is the succeded will be 1. otherwise succeded will be 0.*/
int get_number_from_string(char* num_str, int* succeded)
{
	int num = 0;
	*succeded = (sscanf(num_str, "%d", &num) == 1);
	return num;
}

/* move the str string number of steps to left */
void shift_left(char* str, int num)
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
