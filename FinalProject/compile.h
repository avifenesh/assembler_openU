#ifndef COMPILE_H
#define COMPILE_H

#include "list.h"
#include "structures.h"
#include <stdio.h>

/* get an array of file names and compile them and write the output files */
void compile_files(char *file_names[], int numer_of_files);
/* get a file name and compile it and write the output files */
void compile_file(char *file_name);
/* analyze a line and update the code, data and symbols lists */
int analyze_line(char* file_name, int line_number, char* line, HEAD code, HEAD data, HEAD symbols, int* address);
/* analyze a command and update the code, data and symbols lists */
int analyze_cmd(char* file_name, int line_number, char* label, char* command_name, char* line, HEAD code, HEAD data, HEAD symbols, int* address);
/* analyze a data line and update the code, data and symbols lists */
int analyze_data(char* file_name, int line_number, char* label, char* command_name, char* line, HEAD data, HEAD symbols, int* address);
/* analyze a .string line and update the code, data and symbols lists */
int analyze_string_cmd(char* file_name, int line_number, char* label, char* line, HEAD data, HEAD symbols, int* address);
/* analyze a .data line and update the code, data and symbols lists */
int analyze_data_cmd(char* file_name, int line_number, char* label, char* line, HEAD data, HEAD symbols, int* address);
/* analyze a .entry line and update the code, data and symbols lists */
int analyze_entry_cmd(char* file_name, int line_number, char* label, char* line, HEAD data, HEAD symbols, int* address);
/* analyze a .extern line and update the code, data and symbols lists */
int analyze_extern_cmd(char* file_name, int line_number, char* label, char* line, HEAD data, HEAD symbols, int* address);
/* read the arguments and fill them in the command */
int fill_arguments(char* file_name, int line_number, char* line, command_struct* command);
/* fill the addrfessing modes */
int fill_addressing_mode(argument_struct* argument);

/* return the size of the command in words */
int get_command_size(command_struct* command);

/*
Verify the following:
	1. number of arguments
	2. addressing mode of each argument
	3. all symbols exist
	4. go over the arguments with DIRECT and RELATIVE
		and update the appropriate symbols as CODE_SYMBOLKIND
*/
int check_command(char* file_name, command_struct* command, HEAD symbols);
/* return a symbol if is exist */
symbol_struct* find_symbol(char* name, HEAD symbols);

/* fill immediete addressing mode */
int fill_immediete_addressing_mode(argument_struct* argument);
/* fill register addressing mode */
int fill_register_addressing_mode(argument_struct* argument);
/* fill direct addressing mode */
int fill_direct_addressing_mode(argument_struct* argument);
/* fill relative addressing mode */
int fill_relative_addressing_mode(argument_struct* argument);

/* return 1 if a symbol is legal */
int symbol_is_legal(char* name);

/* print an error that occured in a specific line
in a specific file */
void print_error(char* file_name, int line_number, char* error_message);

/* write the .ob file */
void write_ob_file(FILE* ob_file, HEAD code, HEAD data, HEAD symbols);
/* write a command to the .ob file */
void write_command_to_ob_file(FILE* ob_file, command_struct* command, HEAD symbols);
/* write data to the the .ob file */
void write_data_to_ob_file(FILE* ob_file, data_struct* ds);

/* write the .ext file */
void write_ext_file(FILE* ext_file, HEAD code, HEAD symbols, char* file_name);
/* write the .ent file */
void write_ent_file(FILE* ent_file, HEAD code, HEAD symbols, char* file_name);

#endif
