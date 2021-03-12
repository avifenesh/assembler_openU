#include "structures.h"
#include <malloc.h>

/*comments of every method in structures.h*/

ELM create_command_struct()
{
	command_struct* cs = malloc(sizeof(command_struct));
	cs->arguments_num = 0;
	cs->label[0] = '\0';
	cs->address = 0;
	return cs;
}

void free_command_struct(ELM cs)
{
	free((command_struct*)cs);
}

ELM create_symbol_struct()
{
	symbol_struct* cs = malloc(sizeof(symbol_struct));
	cs->kinds = 0;
	cs->value = 0;
	cs->name[0] = '\0';
	return cs;
}

void free_symbol_struct(ELM cs)
{
	free((symbol_struct*)cs);
}

ELM create_data_struct()
{
	data_struct* ds = malloc(sizeof(data_struct));
	ds->name[0] = '\0';
	ds->str_value[0] = '\0';
	ds->int_values_num = 0;
	ds->kind = 0;
	ds->address = 0;
	return ds;
}

void free_data_struct(ELM ds)
{
	free((data_struct*)ds);
}
