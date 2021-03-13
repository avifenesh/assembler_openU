#ifndef GENERAL_FUNCTIONS_H
#define GENERAL_FUNCTIONS_H

/*1 if the character is a space or a tab*/
int spaceOrTab(char c);
/*clean from space at the begin and at the end and copy from src to dst*/
void clean(char* src, char* dst);
/*1 if letter*/
int is_letter(char c);
/*return 1 if number*/
int is_number(char c);
/* check if num_str contain a number if it is the succeded will be 1. otherwise succeded will be 0.*/
int get_number_from_string(char* num_str, int* succeded);
/* move the str string number of steps to left */
void move_left(char* str, int num);

#endif
