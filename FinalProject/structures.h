#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "constants.h"
#include "list.h"

/* struct for the general information about
a specific argument in a specific command */
typedef struct
{
	/* the number of legal addressing modes for this argument */
	int legalAddressingModesNum;
	/* the legal addressing modes for this argument */
	int legalAddressingModes[ADDRESSING_MODES_NUM];
	/* the kind of the argument (source or target) */
	ArgumentKind argumentKind;
} ArgumentInfo;

/* struct for the general information about a specific
command type */
typedef struct
{
	/* the type of the command */
	CommandType commandType;
	/* the name of the command */
	char* commandName;
	/* the opp code */
	int oppCode;
	/* the funct */
	int funct;
	/* the legal number of arguments */
	int argumentInfosNum;
	/* the information about the arguments */
	ArgumentInfo argumentInfos[MAX_ARGUMENTS_NUMBER];
} CommandInfo;

/* struct for holding an argument that was read from a file */
typedef struct argument_struct {
	char argument_str[MAX_ARGUMENT_LEN];
	AddressingMode addressingMode;
} argument_struct;

/* struct for holding a command that was read from a file */
typedef struct command_struct {
	char label[SYMBOL_MAX_LEN];
	/* the legal info about this command */
	CommandInfo* commandInfo;
	/* the number of arguments */
	int arguments_num;
	/* the address of the command */
	int address;
	/* the line number of the command in the file */
	int line_number;
	/* the arguments */
	struct argument_struct arguments[MAX_ARGUMENTS_NUMBER];
} command_struct;

/* struct for a symbol */
typedef struct symbol_struct {
	char name[SYMBOL_MAX_LEN];
	/* the address of the symbol */
	int value;
	/* a bit array of kinds of this symbol */
	int kinds;
} symbol_struct;

/* struct for .data and .string */
typedef struct data_struct {
	/* the name */
	char name[SYMBOL_MAX_LEN];
	/* the string for .string*/
	char str_value[LINE_LEN];
	/* the int values for .data */
	int int_values[MAX_INTS_IN_DATA];
	/* the number of int values for .data */
	int int_values_num;
	/* the address of this data */
	int address;
	/* the kind of this data */
	DataKind kind;
} data_struct;

/* functions for the lists */
/* create and initialize a command_struct */
ELM create_command_struct();
/* free a command_struct */
void free_command_struct(ELM);
/* create and initialize a symbol_struct */
ELM create_symbol_struct();
/* free a symbol_struct */
void free_symbol_struct(ELM);
/* create and initialize a data_struct */
ELM create_data_struct();
/* free a data_struct */
void free_data_struct(ELM);

#endif
