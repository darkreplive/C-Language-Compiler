/*
Tyler King 040979598
Daniel Gribanov 40938906
*/
/*************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2021
*************************************************************
* File name: Scanner.h
* Compiler: MS Visual Studio 2019
* Author: Paulo Sousa (from original work of Svillen Ranev)
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A2.
* Date: May 01 2021
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 15   /* variable identifier length */
#define ERR_LEN 20  /* error message length */
#define NUM_LEN 15   /* maximum number of digits for IL */

#define RTE_CODE 1   /* Value for run-time error */

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/* 0: Error token */
	VID_T,		/* 1: Variable/Method Identifier */
	INL_T,		/* 5: Integer literal token */
	FPL_T,		/* 6: Floating point literal token */
	STR_T,		/* 7: String literal token */
	SCC_OP_T,	/* 8: String concatenation operator token: (++) */
	ASS_OP_T,	/* 9: Assignment operator token */
	ART_OP_T,	/* 10: Arithmetic operator token */
	REL_OP_T,	/* 11: Relational operator token */
	LOG_OP_T,	/* 12: Logical operator token */
	LPR_T,		/* 13: Left parenthesis token */
	RPR_T,		/* 14: Right parenthesis token */
	LBR_T,		/* 15: Left brace token */
	RBR_T,		/* 16: Right brace token */
	KW_T,		/* 17: Keyword token */
	COM_T,		/* 18: Comma token */
	EOS_T,		/* 19: End of statement (semicolon) */
	RTE_T,		/* 20: Run-time error token */
	SEOF_T,		/* 21: Source end-of-file token */
	LSB_T,		// 22; Left square bracket
	RSB_T,		// 23; Right square bracket
	CMM_T		// 24; Comment token
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	decartes_int codeType;      /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	decartes_int intValue;						/* integer literal attribute (value) */
	decartes_int keywordIndex;					/* keyword index in the keyword table */
	decartes_int contentString;				/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	decartes_flt floatValue;					/* floating-point literal attribute (value) */
	decartes_chr idLexeme[VID_LEN + 1];		/* variable identifier token attribute */
	decartes_chr errLexeme[ERR_LEN + 1];		/* error token attribite */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	decartes_flg flags;			/* Flags information */
	union {
		decartes_int intValue;				/* Integer value */
		decartes_flt floatValue;			/* Float value */
		decartes_nul* stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	decartes_int code;					    /* token code */
	TokenAttribute attribute;		/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner
 *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';', 
 *  white space, @comment@ , ',' , ';' , '-' , '+' , '*' , '/', ## , 
 *  .&., .|. , .!. , SEOF.
 */

/* TO_DO: Error states and illegal state */
#define ES  12		/* Error state with no retract */
#define ER  13		/* Error state with retract */
#define IS -1		/* Illegal state */

 /* TO_DO: State transition table definition */
#define TABLE_COLUMNS 7

/* TO_DO: Define lexeme FIXED classes */
/* These constants will be used on nextClass */
#define CHRCOL2 '"'
#define CHRCOL5 '.'
#define CHRCOL3 '\''
/* These constants will be used on VID function */
//#define IVIDPREFIX '#'
//#define FVIDPREFIX '%'
//#define SVIDPREFIX '$'
//#define MNIDPREFIX '&'

/* TO_DO: Transition table - type of states defined in separate table */
static decartes_int transitionTable[][TABLE_COLUMNS] = {
	/*			[A-z],	[0-9],	",		',		EF,		.,		Other,
	/*			L(0),	D(1),	Q1(2),	Q2(3),	E(4),	P(5),	O(6),
	/* S00 */	{8,		4,		1,		3,		ES,		ER,		ER},	/* NOAS */
	/* S01 */	{1,		1,		2,		1,		ES,		1,		1},		/* NOAS */
	/* S02 */	{IS,	IS,		IS,		IS,		IS,		IS,		IS},	/* ASWR (SL) */
	/* S03 */	{3,		3,		3,		2,		ES,		3,		3},		/* NOAS */
	/* S04 */	{7,		4,		12,		12,		ES,		5,		7},		/* NOAS */
	/* S05 */	{13,	5,		6,		6,		ES,		12,		6},		/* NOAS */
	/* S06 */	{IS,	IS,		IS,		IS,		IS,		IS,		IS},	/* ASWR (FPL) */
	/* S07 */	{IS,	IS,		IS,		IS,		IS,		IS,		IS},	/* ASWR (IL) */
	/* S08 */	{8,		9,		11,		11,		ES,		11,		11},	/* NOAS */
	/* S09 */	{9,		9,		10,		10,		ES,		10,		10},	/* NOAS */
	/* S10 */	{IS,	IS,		IS,		IS,		IS,		IS,		IS},	/* ASWR (VID)   */
	/* S11 */	{IS,	IS,		IS,		IS,		IS,		IS,		IS},	/* ASWR (KEY) */
	/* S12 */	{IS,	IS,		IS,		IS,		IS,		IS,		IS},	/* ASWR (ERR) */
	/* S13 */	{IS,	IS,		IS,		IS,		IS,		IS,		IS}		/* ASWR (Err2) */ };

/* Define accepting states types */
#define NOAS	0		/* not accepting state */
#define ASNR	1		/* accepting state with no retract */
#define ASWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static decartes_int stateType[] = {
	NOAS, /* 00 */
	NOAS, /* 01 */
	ASNR, /* 02 (SL - String Literal) */
	NOAS, /* 03 */
	NOAS, /* 04 */
	NOAS, /* 05 */
	ASWR, /* 06 (FPL - Floating Point Literal) */
	ASWR, /* 07 (Integer Literal)*/
	NOAS, /* 08  */
	NOAS, /* 09 */
	ASWR, /* 10 (VID) */
	ASWR, /* 11 (KEY)*/
	ASWR,  /* 12 (ERR1 - no retract)*/
	ASWR  /* 13 (ERR2 -  retract)*/
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
decartes_int startScanner(BufferPointer psc_buf);
static decartes_int nextClass(decartes_chr c);			/* character class function */
static decartes_int nextState(decartes_int, decartes_chr);		/* state machine function */

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(decartes_chr* lexeme);

/* Declare accepting states functions */
Token funcVID (decartes_chr lexeme[]);
Token funcIL(decartes_chr lexeme[]);
Token funcFPL(decartes_chr lexeme[]);
Token funcSL(decartes_chr lexeme[]);
Token funcKEY(decartes_chr lexeme[]);
Token funcErr(decartes_chr lexeme[]);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[] = {
	NULL,		/* 00 */
	NULL,		/* 01 */
	funcSL,		/* 02 */
	NULL,		/* 03 */
	NULL,		/* 04 */
	NULL,		/* 05 */
	funcFPL,	/* 06 */
	funcIL,		/* 07 */
	NULL,		/* 08 */
	NULL,		/* 09 */
	funcVID,	/* 10 */
	funcVID,	/* 11 */
	funcErr,	/* 12 */
	funcErr		/*13*/

};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 15

/* TO_DO: Define the list of keywords */
static decartes_chr* keywordTable[KWT_SIZE] = {
	"STATIC",
	"SCANF",
	"PRINTF",
	"INT",
	"FX",
	"CONST",
	"CUBIC",
	"RATIONAL",
	"QUADRATIC",
	"FLOAT",
	"STRING",
	"IF",
	"ELSE",
	"WHILE",
	"DO"
};

#endif
