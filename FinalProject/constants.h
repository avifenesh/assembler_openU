#ifndef CONSTANTS_H
#define CONSTANTS_H

/* maximum file name length */
#define MAXIMUM_FILE_NAME 1000
/* maximum line length */
#define LINE_LEN 80
/* maximum symbol name length */
#define SYMBOL_MAX_LEN 31
/* maximum argument length */
#define MAX_ARGUMENT_LEN 100
/* maximum number of arguments */
#define MAX_ARGUMENTS_NUMBER 2
/* maximum number of integers in .data */
#define MAX_INTS_IN_DATA 40

/* enum for command types */
typedef enum CommandType
{
	MOV,
	CMP,
	ADD,
	SUB,
	LEA,
	CLR,
	NOT,
	INC,
	DEC,
	JMP,
	BNE,
	JSR,
	RED,
	PRN,
	RTS,
	STOP
} CommandType;

/* the names of the "data" commands */
#define STRING_CMD ".string"
#define DATA_CMD ".data"
#define ENTRY_CMD ".entry"
#define EXTERN_CMD ".extern"

/* enum for addressing modes */
typedef enum AddressingMode
{
	IMMEDIETE,
	DIRECT,
	RELATIVE,
	REGISTER
} AddressingMode;

/* enum for argument kind (source or target) */
typedef enum ArgumentKind
{
	SOURCE,
	TARGET
} ArgumentKind;

/* enum for data kinds */
typedef enum DataKind
{
	STRING_DATAKIND = 1,
	DATA_DATAKIND = STRING_DATAKIND*2,
	ENTRY_DATAKIND = DATA_DATAKIND*2,
	EXTERNAL_DATAKIND = ENTRY_DATAKIND*2
} DataKind;

/* enum for symbol kinds */
typedef enum SymbolKind
{
	CODE_SYMBOLKIND = 1,
	DATA_SYMBOLKIND = CODE_SYMBOLKIND*2,
	ENTRY_SYMBOLKIND = DATA_SYMBOLKIND*2,
	EXERNAL_SYMBOLKIND = ENTRY_SYMBOLKIND*2
} SymbolKind;

/* the number of addressing modes */
#define ADDRESSING_MODES_NUM 4

#endif
