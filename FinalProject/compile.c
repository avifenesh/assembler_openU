#include "compile.h"
#include "general_functions.h"
#include "list.h"
#include "structures.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

/* a "table" of the information of each kind of command */
CommandInfo commandInfos[] =
{
	{MOV, "mov", 0, 0, 2,
		{{3, {IMMEDIETE, DIRECT, REGISTER}, SOURCE},
		{2, {DIRECT, REGISTER}, TARGET}}},
	{CMP, "cmp", 1, 0, 2,
		{{3, {IMMEDIETE, DIRECT, REGISTER}, SOURCE},
		{3, {IMMEDIETE, DIRECT, REGISTER}, TARGET}}},
	{ADD, "add", 2, 1, 2,
		{{3, {IMMEDIETE, DIRECT, REGISTER}, SOURCE},
		{2, {DIRECT, REGISTER}, TARGET}}},
	{SUB, "sub", 2, 2, 2,
		{{3, {IMMEDIETE, DIRECT, REGISTER}, SOURCE},
		{2, {DIRECT, REGISTER}, TARGET}}},
	{LEA, "lea", 4, 0, 2,
		{{1, {DIRECT}, SOURCE},
		{2, {DIRECT, REGISTER}, TARGET}}},
	{CLR, "clr", 5, 10, 1,
		{{2, {DIRECT, REGISTER}, TARGET}}},
	{NOT, "not", 5, 11, 1,
		{{2, {DIRECT, REGISTER}, TARGET}}},
	{INC, "inc", 5, 12, 1,
		{{2, {DIRECT, REGISTER}, TARGET}}},
	{DEC, "dec", 5, 13, 1,
		{{2, {DIRECT, REGISTER}, TARGET}}},
	{JMP, "jmp", 9, 10, 1,
		{{2, {DIRECT, RELATIVE}, TARGET}}},
	{BNE, "bne", 9, 11, 1,
		{{2, {DIRECT, RELATIVE}, TARGET}}},
	{JSR, "jsr", 9, 12, 1,
		{{2, {DIRECT, RELATIVE}, TARGET}}},
	{RED, "red", 12, 0, 1,
		{{2, {DIRECT, REGISTER}, TARGET}}},
	{PRN, "prn", 13, 0, 1,
		{{3, {IMMEDIETE, DIRECT, REGISTER}, TARGET}}},
	{RTS, "rts", 14, 0, 0},
	{STOP, "stop", 15, 0, 0}
};

void compile_files(char *file_names[], int n[umer_of_files)
{
	int i;
	/* start from 1 because the first one is the name of this program */
	for (i = 1; i < numer_of_files; i++)
	{
		compile_file(file_names[i]);
	}
}

void compile_file(char *file_name)
{
	/* file names */
	char assembler_file_name[MAXIMUM_FILE_NAME];
	char ob_file_name[MAXIMUM_FILE_NAME];
	char ext_file_name[MAXIMUM_FILE_NAME];
	char ent_file_name[MAXIMUM_FILE_NAME];
	char line[LINE_LEN], cleaned_line[LINE_LEN];
	/* file pointers */
	FILE* assembler_file, *ob_file, *ext_file, *ent_file;
	/* lists of code, data and symbols */
	HEAD code = NULL, data = NULL, symbols = NULL;
	int line_number = 0, address = 100, good = 1;
	command_struct* temp_command;
	ELM _node;
	sprintf(assembler_file_name, "%s.as", file_name);
	assembler_file = fopen(assembler_file_name, "r");
	if (assembler_file == NULL)
	{
		printf("Assembler file %s not found\n", assembler_file_name);
		return;
	}
	printf("Start processing assembler file %s\n", assembler_file_name);
	/* create the lists */
	code = CreateList(create_command_struct, free_command_struct);
	data = CreateList(create_data_struct, free_data_struct);
	symbols = CreateList(create_symbol_struct, free_symbol_struct);
	/* read the files line after line */
	while (fgets(line, sizeof(line), assembler_file) != NULL)
	{
		/* update the line number */
		line_number++;
		/* clean the line */
		clean(line, cleaned_line);
		if ((strlen(cleaned_line) == 0) || /* empty line */
			(cleaned_line[0] == ';')) /* comment line */
			continue;
		good = good && analyze_line(assembler_file_name, line_number, cleaned_line, code, data, symbols, &address);
	}
	fclose(assembler_file);
	/* go over each command and verify that its OK */
	temp_command = GetFirstFromList(code, &_node);
	if (temp_command != NULL)
	{
		good = good && check_command(assembler_file_name, temp_command, symbols);
		while (HasMoreInList(&_node))
		{
			temp_command = GetNextFromList(&_node);
			good = good && check_command(assembler_file_name, temp_command, symbols);
		}
	}
	/* write the output files only if there wasn't any error */
	if (good)
	{
		/* write the ob file */
		sprintf(ob_file_name, "%s.ob", file_name);
		ob_file = fopen(ob_file_name, "w");
		if (ob_file != NULL)
		{
			write_ob_file(ob_file, code, data, symbols);
			fclose(ob_file);
		}
		else
		{
			printf("OB file %s cannot be created\n", ob_file_name);
		}
		/* write the ext file */
		sprintf(ext_file_name, "%s.ext", file_name);
		ext_file = fopen(ext_file_name, "w");
		if (ext_file != NULL)
		{
			write_ext_file(ext_file, code, symbols);
			fclose(ext_file);
		}
		else
		{
			printf("EXT file %s cannot be created\n", ext_file_name);
		}
		/* write the ent file */
		sprintf(ent_file_name, "%s.ent", file_name);
		ent_file = fopen(ent_file_name, "w");
		if (ent_file != NULL)
		{
			write_ent_file(ent_file, code, symbols);
			fclose(ent_file);
		}
		else
		{
			printf("ENT file %s cannot be created\n", ent_file_name);
		}
		printf("Finish processing assembler file %s successfully\n", assembler_file_name);
	}
	else
	{
		printf("Finish processing assembler file %s with errors\n", assembler_file_name);
	}
	/* free the lists */
	DestroyList(code);
	DestroyList(data);
	DestroyList(symbols);
}

void write_ent_file(FILE* ent_file, HEAD code, HEAD symbols)
{
	ELM _node;
	symbol_struct* cs = GetFirstFromList(symbols, &_node);
	if (cs == NULL)
		return;
	if (cs->kinds & ENTRY_SYMBOLKIND)
		fprintf(ent_file, "%s %07d\n", cs->name, cs->value);
	while (HasMoreInList(&_node))
	{
		cs = GetNextFromList(&_node);
		if (cs->kinds & ENTRY_SYMBOLKIND)
			fprintf(ent_file, "%s %07d\n", cs->name, cs->value);
	}
}

void write_symbol_to_ext_file_from_command(FILE* ext_file, symbol_struct* symbol, command_struct* command)
{
	int i;
	if (!(symbol->kinds & EXERNAL_SYMBOLKIND))
		return;
	for (i = 0; i < command->arguments_num; i++)
	{
		if ((command->arguments[i].addressingMode == DIRECT) ||
			(command->arguments[i].addressingMode == RELATIVE))
		{
			if (strcmp(symbol->name, command->arguments[i].argument_str) == 0)
			{
				fprintf(ext_file, "%s %07d\n", symbol->name, command->address + i + 1);
			}
		}
	}
}

void write_symbol_to_ext_file(FILE* ext_file, symbol_struct* symbol, HEAD code)
{
	ELM node;
	command_struct* command = GetFirstFromList(code, &node);
	if (command != NULL)
	{
		write_symbol_to_ext_file_from_command(ext_file, symbol, command);
	}
	while (HasMoreInList(&node))
	{
		command = GetNextFromList(&node);
		write_symbol_to_ext_file_from_command(ext_file, symbol, command);
	}
}

void write_ext_file(FILE* ext_file, HEAD code, HEAD symbols)
{
	ELM node;
	symbol_struct* cs = GetFirstFromList(symbols, &node);
	if (cs == NULL)
		return;
	write_symbol_to_ext_file(ext_file, cs, code);
	while (HasMoreInList(&node))
	{
		cs = GetNextFromList(&node);
		write_symbol_to_ext_file(ext_file, cs, code);
		if (cs == NULL)
			return;
	}
}

/* return the size of the code part */
int get_code_size(HEAD code)
{
	int code_size = 0;
	ELM node;
	command_struct* command = GetFirstFromList(code, &node);
	if (command != NULL)
	{
		code_size += get_command_size(command);
	}
	while (HasMoreInList(&node))
	{
		command = GetNextFromList(&node);
		code_size += get_command_size(command);
	}
	return code_size;
}

/* return the size of the code part */
int get_data_size(HEAD data)
{
	int data_size = 0;
	ELM node;
	data_struct* ds = GetFirstFromList(data, &node);
	if (ds != NULL)
	{
		if (ds->kind == STRING_DATAKIND)
		{
			data_size += strlen(ds->str_value) + 1;
		}
		else if (ds->kind == DATA_DATAKIND)
		{
			data_size += ds->int_values_num;
		}
	}
	while (HasMoreInList(&node))
	{
		ds = GetNextFromList(&node);
		if (ds->kind == STRING_DATAKIND)
		{
			data_size += strlen(ds->str_value) + 1;
		}
		else if (ds->kind == DATA_DATAKIND)
		{
			data_size += ds->int_values_num;
		}
	}
	return data_size;
}

void write_ob_file(FILE* ob_file, HEAD code, HEAD data, HEAD symbols)
{
	ELM code_node, data_node;
	int code_size = get_code_size(code), data_size = get_data_size(data);
	command_struct* command = GetFirstFromList(code, &code_node);
	data_struct* ds = GetFirstFromList(data, &data_node);
	fprintf(ob_file, "%d %d\n", code_size, data_size);
	if (command != NULL)
	{
		write_command_to_ob_file(ob_file, command, symbols);
	}
	while (HasMoreInList(&code_node))
	{
		command = GetNextFromList(&code_node);
		write_command_to_ob_file(ob_file, command, symbols);
	}
	if (ds != NULL)
	{
		write_data_to_ob_file(ob_file, ds);
	}
	while (HasMoreInList(&data_node))
	{
		ds = GetNextFromList(&data_node);
		write_data_to_ob_file(ob_file, ds);
	}
}

/* write a word to the ob file */
void write_word(FILE* file, int address, unsigned int word)
{
	/* the word & 0xffffff is because we write only 6 bytes */
	fprintf(file, "%07d %06x\n", address, word & 0xffffff);
}

void write_command_to_ob_file(FILE* ob_file, command_struct* command, HEAD symbols)
{
	int num, succeded, address = command->address, i;
	unsigned int word = 0;
	symbol_struct* symbol;
	word |= command->commandInfo->oppCode << 18;
	word |= command->commandInfo->funct << 3;
	word |= 1 << 2;
	if (command->arguments_num == 1)
	{
		word |= command->arguments[0].addressingMode << 11;
		if (command->arguments[0].addressingMode == REGISTER)
		{
			num = get_number_from_string(command->arguments[0].argument_str, &succeded);
			word |= num << 8;
		}
	}
	else if (command->arguments_num == 2)
	{
		word |= command->arguments[0].addressingMode << 16;
		word |= command->arguments[1].addressingMode << 11;
		if (command->arguments[0].addressingMode == REGISTER)
		{
			num = get_number_from_string(command->arguments[0].argument_str, &succeded);
			word |= num << 13;
		}
		if (command->arguments[1].addressingMode == REGISTER)
		{
			num = get_number_from_string(command->arguments[1].argument_str, &succeded);
			word |= num << 8;
		}
	}
	write_word(ob_file, address, word);
	/* write the arguments */
	for (i = 0; i < command->arguments_num; i++)
	{
		if (command->arguments[i].addressingMode != REGISTER)
		{
			word = 0;
			address++;
			switch (command->arguments[i].addressingMode)
			{
			case REGISTER:
				/* nothing to do */
				break;
			case IMMEDIETE:
				num = get_number_from_string(command->arguments[i].argument_str, &succeded);
				word = num << 3;
				word |= 1 << 2;
				break;
			case DIRECT:
				symbol = find_symbol(command->arguments[i].argument_str, symbols);
				if (symbol->kinds & EXERNAL_SYMBOLKIND)
				{
					word = 1;
				}
				else
				{
					word = symbol->value << 3;
					word |= 1 << 1;
				}
				break;
			case RELATIVE:
				symbol = find_symbol(command->arguments[i].argument_str, symbols);
				if (symbol->kinds & EXERNAL_SYMBOLKIND)
				{
					word = 1;
				}
				else
				{
					word = (symbol->value - command->address) << 3;
					word |= 1 << 2;
				}
				break;
			}
			write_word(ob_file, address, word);
		}
	}
}

void write_data_to_ob_file(FILE* ob_file, data_struct* ds)
{
	unsigned int word = 0;
	int i, str_len;
	switch (ds->kind)
	{
	case STRING_DATAKIND:
		str_len = strlen(ds->str_value);
		for (i = 0; i <= str_len; i++)
		{
			word = ds->str_value[i];
			write_word(ob_file, ds->address + i, word);
		}
		break;
	case DATA_DATAKIND:
		for (i = 0; i < ds->int_values_num; i++)
		{
			word = ds->int_values[i];
			write_word(ob_file, ds->address + i, word);
		}
		break;
	default:
		break;
	}
}

/*
Verify the following:
	1. number of arguments
	2. addressing mode of each argument
	3. all symbols exist
	4. go over the arguments with DIRECT and RELATIVE
		and update the appropriate symbols as CODE_SYMBOLKIND
*/
int check_command(char* file_name, command_struct* command, HEAD symbols)
{
	/* the message is static so it will not be allocated
	each time that this fnction is being called */
	static char message[200];
	int i, j, found;
	symbol_struct* symbol;
	if (command->commandInfo == NULL)
	{
		/* no need to give an error as this was checked already */
		return 0;
	}
	if (command->arguments_num !=
		command->commandInfo->argumentInfosNum)
	{
		print_error(file_name, command->line_number, "Arguments number is wrong");
		return 0;
	}
	for (i = 0; i < command->arguments_num; i++)
	{
		found = 0;
		for (j = 0; j < command->commandInfo->argumentInfos[i].legalAddressingModesNum; j++)
		{
			if (command->arguments[i].addressingMode ==
				command->commandInfo->argumentInfos[i].legalAddressingModes[j])
			{
				found = 1;
				break;
			}
		}
		if (!found)
		{
			print_error(file_name, command->line_number, "Bad addressing mode for argument");
			return 0;
		}
		if ((command->arguments[i].addressingMode == DIRECT) ||
			(command->arguments[i].addressingMode == RELATIVE))
		{
			symbol = find_symbol(command->arguments[i].argument_str, symbols);
			if (symbol == NULL)
			{
				sprintf(message, "Symbol %s not found", command->arguments[i].argument_str);
				print_error(file_name, command->line_number, message);
				return 0;
			}
			symbol->kinds |= CODE_SYMBOLKIND;
		}
	}
	return 1;
}

/* return 1 if there is a label in the line.
if so, the label string will contain the label. */
int has_label(char* line, char* label)
{
	size_t i;
	for (i = 0; i < strlen(line); i++)
	{
		if (line[i] == ':')
		{
			label[i] = '\0';
			return 1;
		}
		label[i] = line[i];
	}
	label[0] = '\0';
	return 0;
}

/* return the command info associated with the command_name */
CommandInfo* getCommandInfo(char* command_name)
{
	int num = sizeof(commandInfos) / sizeof(CommandInfo), i;
	for (i = 0; i < num; i++)
	{
		if (strcmp(commandInfos[i].commandName, command_name) == 0)
			return &commandInfos[i];
	}
	return NULL;
}

int analyze_line(char* file_name, int line_number, char* line, HEAD code, HEAD data, HEAD symbols, int* address)
{
	symbol_struct* symbol = NULL;
	char label[SYMBOL_MAX_LEN], command_name[20];
	int i;
	label[0] = '\0';
	if (has_label(line, label))
	{
		/* the +1 is for the : */
		line += strlen(label) + 1;
		symbol = AppendElementToList(symbols);
		if (symbol == NULL)
		{
			print_error(file_name, line_number, "Failed to allocate symbol");
			return 0;
		}
		strcpy(symbol->name, label);
		symbol->kinds |= CODE_SYMBOLKIND;
		symbol->value = *address;
	}
	while ((*line) && spaceOrTab(*line))
		line++;
	i = 0;
	while ((*line) && !spaceOrTab(*line))
	{
		command_name[i] = *line;
		i++;
		line++;
	}
	command_name[i] = '\0';
	if (strlen(command_name) == 0)
	{
		print_error(file_name, line_number, "Bad command with empty name");
		return 0;
	}
	if (command_name[0] == '.')
	{
		return analyze_data(file_name, line_number, label, command_name, line, data, symbols, address);
	}
	return analyze_cmd(file_name, line_number, label, command_name, line, code, data, symbols, address);
}

int analyze_cmd(char* file_name, int line_number, char* label, char* command_name, char* line, HEAD code, HEAD data, HEAD symbols, int* address)
{
	/* the message is static so it will not be allocated
	each time that this fnction is being called */
	static char message[200];
	CommandInfo* commandInfo;
	command_struct* command = NULL;
	commandInfo = getCommandInfo(command_name);
	if (commandInfo == NULL)
	{
		sprintf(message, "Command %s not found", command_name);
		print_error(file_name, line_number, message);
		return 0;
	}
	command = AppendElementToList(code);
	if (command == NULL)
	{
		print_error(file_name, line_number, "Failed to allocate command");
		return 0;
	}
	command->line_number = line_number;
	command->commandInfo = commandInfo;
	if (strlen(label) > 0)
	{
		strcpy(command->label, label);
	}
	else
	{
		/* no label */
		command->label[0] = '\0';
	}
	if (!fill_arguments(file_name, line_number, line, command))
		return 0;
	command->address = *address;
	(*address) += get_command_size(command);
	return 1;
}

int get_command_size(command_struct* command)
{
	int size;
	if (command->arguments_num == 0)
	{
		return 1;
	}
	if (command->arguments_num == 1)
	{
		if (command->arguments[0].addressingMode == REGISTER)
			return 1;
		return 2;
	}
	if (command->arguments_num == 2)
	{
		size = 1;
		if (command->arguments[0].addressingMode != REGISTER)
			size++;
		if (command->arguments[1].addressingMode != REGISTER)
			size++;
		return size;
	}
	return 1;
}

int fill_arguments(char* file_name, int line_number, char* line, command_struct* command)
{
	int i = 0, j = 0, k;
	while (line[i])
	{
		while ((line[i] != '\0') && spaceOrTab(line[i]))
			i++;
		if (line[i] == '\0')
			break;
		if (line[i] == ',')
		{
			if (j == 0)
			{
				print_error(file_name, line_number, "Extra comma before arguments");
				return 0;
			}
			else
			{
				i++;
			}
			while ((line[i] != '\0') && spaceOrTab(line[i]))
				i++;
		}
		k = 0;
		while ((line[i] != '\0') && !spaceOrTab(line[i]) && (line[i] != ','))
		{
			command->arguments[j].argument_str[k] = line[i];
			k++;
			i++;
		}
		command->arguments[j].argument_str[k] = '\0';
		if (strlen(command->arguments[j].argument_str) == 0)
		{
			print_error(file_name, line_number, "Empty argument");
			return 0;
		}
		if (!fill_addressing_mode(&command->arguments[j]))
		{
			print_error(file_name, line_number, "Bad addressing mode for argument");
			return 0;
		}
		j++;
	}
	if (j > 0)
	{
		command->arguments_num = j;
	}
	return 1;
}

int fill_addressing_mode(argument_struct* argument)
{
	if (fill_immediete_addressing_mode(argument))
	{
		return 1;
	}
	if (fill_register_addressing_mode(argument))
	{
		return 1;
	}
	if (fill_direct_addressing_mode(argument))
	{
		return 1;
	}
	if (fill_relative_addressing_mode(argument))
	{
		return 1;
	}
	return 0;
}

int fill_immediete_addressing_mode(argument_struct* argument)
{
	int succeded;
	if (strlen(argument->argument_str) < 2)
		return 0;
	if (argument->argument_str[0] != '#')
		return 0;
	/* remove the # from the begining */
	shift_left(argument->argument_str, 1);
	/* ignore the returned value because this is just a test
	if there is a number in the argument */
	get_number_from_string(argument->argument_str, &succeded);
	if (succeded)
		argument->addressingMode = IMMEDIETE;
	return succeded;
}

int fill_register_addressing_mode(argument_struct* argument)
{
	int num, succeded;
	if (strlen(argument->argument_str) != 2)
		return 0;
	if (argument->argument_str[0] != 'r')
		return 0;
	num = get_number_from_string(&argument->argument_str[1], &succeded);
	if (!succeded)
		return 0;
	if ((num < 0) || (num > 7))
		return 0;
	/* leave only the number of the register */
	shift_left(argument->argument_str, 1);
	argument->addressingMode = REGISTER;
	return 1;
}

int fill_direct_addressing_mode(argument_struct* argument)
{
	if (!symbol_is_legal(argument->argument_str))
		return 0;
	argument->addressingMode = DIRECT;
	return 1;
}

int fill_relative_addressing_mode(argument_struct* argument)
{
	if (strlen(argument->argument_str) < 2)
		return 0;
	if (argument->argument_str[0] != '&')
		return 0;
	if (!symbol_is_legal(&argument->argument_str[1]))
		return 0;
	/* remove the & */
	shift_left(argument->argument_str, 1);
	argument->addressingMode = RELATIVE;
	return 1;
}

int analyze_data(char* file_name, int line_number, char* label, char* command_name, char* line, HEAD data, HEAD symbols, int* address)
{
	/* the message is static so it will not be allocated
	each time that this fnction is being called */
	static char message[200];
	if (strcmp(command_name, STRING_CMD) == 0)
	{
		return analyze_string_cmd(file_name, line_number, label, line, data, symbols, address);
	}
	if (strcmp(command_name, DATA_CMD) == 0)
	{
		return analyze_data_cmd(file_name, line_number, label, line, data, symbols, address);
	}
	if (strcmp(command_name, ENTRY_CMD) == 0)
	{
		return analyze_entry_cmd(file_name, line_number, label, line, data, symbols, address);
	}
	if (strcmp(command_name, EXTERN_CMD) == 0)
	{
		return analyze_extern_cmd(file_name, line_number, label, line, data, symbols, address);
	}
	sprintf(message, "Bad data command %s", command_name);
	print_error(file_name, line_number, message);
	return 0;
}

void print_error(char* file_name, int line_number, char* error_message)
{
	printf("Error in file %s, line %d: %s\n", file_name, line_number, error_message);
}

int symbol_is_legal(char* name)
{
	int symbol_length = strlen(name), i;
	if (symbol_length == 0)
		return 0;
	if (!is_letter(name[0]))
		return 0;
	for (i = 1; i < symbol_length; i++)
	{
		if (!is_letter(name[i]) && !is_number(name[i]))
			return 0;
	}
	return 1;
}

int fill_symbol_name(char* file_name, int line_number, char* line, char* name)
{
	int i = 0, j = 0;
	while ((line[i] != '\0') && spaceOrTab(line[i]))
		i++;
	while ((line[i] != '\0') && !spaceOrTab(line[i]))
	{
		name[j] = line[i];
		i++;
		j++;
	}
	name[j] = '\0';
	while (line[i] != '\0')
	{
		/* check the case that there are extra
		 characters after the symbol */
		if (!spaceOrTab(line[i]))
		{
			print_error(file_name, line_number, "Redundant data after symbol");
			return 0;
		}
		i++;
	}
	return symbol_is_legal(name);
}

symbol_struct* find_symbol(char* name, HEAD symbols)
{
	ELM _node;
	symbol_struct* cs = GetFirstFromList(symbols, &_node);
	if (cs == NULL)
		return NULL;
	if (strcmp(name, cs->name) == 0)
		return cs;
	while (HasMoreInList(&_node))
	{
		cs = GetNextFromList(&_node);
		if (strcmp(name, cs->name) == 0)
			return cs;
	}
	return NULL;
}

symbol_struct* get_or_create_symbol_struct(char* file_name, int line_number, char* line, HEAD symbols)
{
	char name[SYMBOL_MAX_LEN];
	symbol_struct* symbol;
	if (!fill_symbol_name(file_name, line_number, line, name))
	{
		print_error(file_name, line_number, "Symbol not legal");
		return NULL;
	}
	symbol = find_symbol(name, symbols);
	if (symbol == NULL)
	{
		symbol = AppendElementToList(symbols);
		if (symbol == NULL)
		{
			print_error(file_name, line_number, "Failed to allocate symbol");
			return NULL;
		}
		strcpy(symbol->name, name);
	}
	if (symbol == NULL)
	{
		print_error(file_name, line_number, "Symbol not found");
		return NULL;
	}
	return symbol;
}

int analyze_entry_cmd(char* file_name, int line_number, char* label, char* line, HEAD data, HEAD symbols, int* address)
{
	symbol_struct* symbol = get_or_create_symbol_struct(file_name, line_number, line, symbols);
	if (symbol == NULL)
		return 0;
	symbol->kinds |= ENTRY_SYMBOLKIND;
	return 1;
}

int analyze_extern_cmd(char* file_name, int line_number, char* label, char* line, HEAD data, HEAD symbols, int* address)
{
	symbol_struct* symbol = get_or_create_symbol_struct(file_name, line_number, line, symbols);
	if (symbol == NULL)
		return 0;
	symbol->kinds |= EXERNAL_SYMBOLKIND;
	symbol->value = 1;
	return 1;
}

/* fill the numbers of .data */
int fill_numbers(data_struct* ds, char* file_name, int line_number, char* line)
{
	char num_str[LINE_LEN];
	int i = 0, j, num, succeded;
	while (line[i])
	{
		while (line[i] && spaceOrTab(line[i]))
			i++;
		j = 0;
		while (line[i] && (line[i] != ',') && !spaceOrTab(line[i]))
		{
			num_str[j] = line[i];
			i++;
			j++;
		}
		while (line[i] && spaceOrTab(line[i]))
			i++;
		/* go to the next */
		if (line[i] && (line[i] == ','))
			i++;
		while (line[i] && spaceOrTab(line[i]))
			i++;
		num_str[j] = '\0';
		num = get_number_from_string(num_str, &succeded);
		if (!succeded)
		{
			print_error(file_name, line_number, "Bad number format");
			return 0;
		}
		ds->int_values[ds->int_values_num++] = num;
	}
	return 1;
}

int analyze_data_cmd(char* file_name, int line_number, char* label, char* line, HEAD data, HEAD symbols, int* address)
{
	symbol_struct* symbol = NULL;
	data_struct* ds = NULL;
	if (strlen(label) > 0)
	{
		symbol = find_symbol(label, symbols);
		if (symbol == NULL)
		{
			symbol = AppendElementToList(symbols);
			if (symbol == NULL)
			{
				print_error(file_name, line_number, "Failed to allocate symbol");
				return 0;
			}
			strcpy(symbol->name, label);
			symbol->value = *address;
		}
		if (symbol == NULL)
		{
			print_error(file_name, line_number, "Symbol not found");
			return 0;
		}
		symbol->value = *address;
		symbol->kinds |= DATA_SYMBOLKIND;
	}
	ds = AppendElementToList(data);
	if (ds == NULL)
	{
		print_error(file_name, line_number, "Failed to allocate data");
		return 0;
	}
	ds->kind = DATA_DATAKIND;
	if (!fill_numbers(ds, file_name, line_number, line))
	{
		return 0;
	}
	if (ds->int_values_num == 0)
	{
		print_error(file_name, line_number, "No numbers were found");
		return 0;
	}
	ds->address = *address;
	(*address) += ds->int_values_num;
	return 1;
}

int analyze_string_cmd(char* file_name, int line_number, char* label, char* line, HEAD data, HEAD symbols, int* address)
{
	int first_quot = 0, last_quot = 0, i = 0, j = 0;
	symbol_struct* symbol = NULL;
	data_struct* ds = NULL;
	if (strlen(label) > 0)
	{
		symbol = find_symbol(label, symbols);
		if (symbol == NULL)
		{
			symbol = AppendElementToList(symbols);
			if (symbol == NULL)
			{
				print_error(file_name, line_number, "Failed to allocate symbol");
				return 0;
			}
			strcpy(symbol->name, label);
			symbol->value = *address;
		}
		if (symbol == NULL)
		{
			print_error(file_name, line_number, "Symbol not found");
			return 0;
		}
		symbol->kinds |= DATA_SYMBOLKIND;
	}
	while ((line[i] != '\0') && spaceOrTab(line[i]))
		i++;
	if (line[i] != '"')
	{
		print_error(file_name, line_number, "No quotation mark in string");
		return 0;
	}
	first_quot = i;
	i++;
	while ((line[i] != '\0') && (line[i] != '"'))
		i++;
	if (line[i] != '"')
	{
		print_error(file_name, line_number, "No quotation mark in string");
		return 0;
	}
	last_quot = i;
	i++;
	while ((line[i] != '\0') && spaceOrTab(line[i]))
		i++;
	if (line[i] != '\0')
	{
		print_error(file_name, line_number, "Extra characters in string");
		return 0;
	}
	ds = AppendElementToList(data);
	if (ds == NULL)
	{
		print_error(file_name, line_number, "Failed to allocate data");
		return 0;
	}
	first_quot++;
	while (first_quot < last_quot)
	{
		ds->str_value[j++] = line[first_quot++];
	}
	ds->str_value[j] = '\0';
	ds->kind = STRING_DATAKIND;
	ds->address = *address;
	(*address) += strlen(ds->str_value) + 1;
	return 1;
}
