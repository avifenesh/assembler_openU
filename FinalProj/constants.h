#ifndef CONSTANTS_H
#define CONSTANTS_H

/* max file name length */
#define MAXIMUM_FILE_NAME 1000
/* max line length */
#define LINE_LEN 80
/* max symbol name length */
#define SYMBOL_MAX_LEN 31
/* max argument length */
#define MAX_ARGUMENT_LEN 100
/* max number of arguments */
#define MAX_ARGUMENTS_NUMBER 2
/* max number of integers in .data */
#define MAX_INTS_IN_DATA 40

/*command types enum*/
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

/*kinds of the "data" commands*/
#define STRING_CMD ".string"
#define DATA_CMD ".data"
#define ENTRY_CMD ".entry"
#define EXTERN_CMD ".extern"

/*addressing modes enum*/
typedef enum AddressingMode
{
	IMMEDIETE,
	DIRECT,
	RELATIVE,
	REGISTER
} AddressingMode;

/*data kinds enum*/
typedef enum DataKind
{
	STRING_DATAKIND = 1,
	DATA_DATAKIND = STRING_DATAKIND <<1,
	ENTRY_DATAKIND = DATA_DATAKIND <<1,
	EXTERNAL_DATAKIND = ENTRY_DATAKIND <<1
} DataKind;

/*argument kind enum*/
typedef enum ArgumentKind
{
	SOURCE,
	TARGET
} ArgumentKind;
/*symbol kinds enum*/
typedef enum SymbolKind
{
	CODE_SYMBOLKIND = 1,
	DATA_SYMBOLKIND = CODE_SYMBOLKIND << 1,
	ENTRY_SYMBOLKIND = DATA_SYMBOLKIND << 1,
	EXERNAL_SYMBOLKIND = ENTRY_SYMBOLKIND << 1
} SymbolKind;

/*number of addressing modes*/
#define ADDRESSING_MODES_NUM 4

#endif
