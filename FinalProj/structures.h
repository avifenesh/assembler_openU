#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "constants.h"
#include "list.h"

/*struct for the information of arguments in command*/
typedef struct
{
	/*number of valid adrresing-mode for this argument*/
	int legalAddressingModesNum;
	/* the specific adreesing-mode for this argument*/
	int legalAddressingModes[ADDRESSING_MODES_NUM];
	/*argument is source or target*/
	ArgumentKind argumentKind;
} ArgumentInfo;

/* struct for information for each command type */
typedef struct
{
	/*type of the command*/
	CommandType commandType;
	/*name of command*/
	char* commandName;
	/*opp-code*/
	int oppCode;
	/*funct*/
	int funct;
	/*number of leagal args*/
	int argumentInfosNum;
	/*information of args*/
	ArgumentInfo argumentInfos[MAX_ARGUMENTS_NUMBER];
} CommandInfo;

/*struct for hold args that read from the file*/
typedef struct argument_struct {
	char argument_str[MAX_ARGUMENT_LEN];
	AddressingMode addressingMode;
} argument_struct;

/*struct for hold command that read from the file*/
typedef struct command_struct {
	char label[SYMBOL_MAX_LEN];
	/*valid info for this command*/
	CommandInfo* commandInfo;
	/*num of args*/
	int arguments_num;
	/*address of command*/
	int address;
	/*number of line the command take in file*/
	int line_number;
	/* the arguments */
	struct argument_struct arguments[MAX_ARGUMENTS_NUMBER];
} command_struct;

/*symbol struct*/
typedef struct symbol_struct {
	char name[SYMBOL_MAX_LEN];
	/*address of symbol*/
	int value;
	/*bits array for kind of symbol*/
	int kinds;
} symbol_struct;

/*data and string struct*/
typedef struct data_struct {
	/*name*/
	char name[SYMBOL_MAX_LEN];
	/*the string (if ist string)*/
	char str_value[LINE_LEN];
	/*the int value (if data)*/
	int int_values[MAX_INTS_IN_DATA];
	/*number of values for data*/
	int int_values_num;
	/*addres of data*/
	int address;
	/*kind of data*/
	DataKind kind;
} data_struct;

/*funcs for lists*/
/*create and initialize  command struct*/
ELM create_command_struct();
/*free command struct*/
void free_command_struct(ELM);
/*create and initialize symbol struct*/
ELM create_symbol_struct();
/*free symbol struct*/
void free_symbol_struct(ELM);
/*create and initialize data struct*/
ELM create_data_struct();
/*free data struct*/
void free_data_struct(ELM);

#endif
