#ifndef GENERAL_FUNCTIONS_H
#define GENERAL_FUNCTIONS_H

/* return 1 if the character c is a space or a tab */
int spaceOrTab(char c);
/* cleans spaces and tabs from the beginning and the end
of src into dst */
void clean(char* src, char* dst);
/* return 1 if c is a letter */
int is_letter(char c);
/* return 1 if c is a number */
int is_number(char c);
/* parse the num_str string and if it contains a number,
return the number.
the succeded will be 1 if the num_str string
contains a number. otherwise succeded will be 0.
*/
int get_number_from_string(char* num_str, int* succeded);
/* move the str string num steps to the left */
void shift_left(char* str, int num);

#endif
