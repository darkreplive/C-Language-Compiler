/*
Tyler King 040979598
Daniel Gribanov 40938906
*/
/*************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2021
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa / Daniel Cormier
*************************************************************/

/*************************************************************
* File name: Scanner.c
* Compiler: MS Visual Studio 2019
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22.
* Date: May 01 2021
* Professor: Paulo Sousa
* Purpose: This file contains all functionalities from Scanner.
* Function list: (...).
*************************************************************/

/* TODO: Adjust the function header */

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/* #define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Buffer.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/*
----------------------------------------------------------------
TO_DO 12: Global vars definitions
----------------------------------------------------------------
*/

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;		/* String literal table */
decartes_int line;									/* Current line number of the source code */
extern decartes_int errorNumber;					/* Defined in platy_st.c - run-time error number */

extern decartes_int stateType[];
extern decartes_chr* keywordTable[];
extern PTR_ACCFUN finalStateTable[];
extern decartes_int transitionTable[][TABLE_COLUMNS];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* pointer to input source buffer */

/*************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ************************************************************/
 /* TO_DO 13: Follow the standard and adjust datatypes */
decartes_int startScanner(BufferPointer psc_buf) {
	if (isEmpty(psc_buf)==DECARTES_TRUE)
		return EXIT_FAILURE; /*1*/
	/* in case the buffer has been read previously  */
	recover(psc_buf);
	clear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

/*************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states 
 *		in the Transition Diagram).
 ************************************************************/
Token tokenizer(void) {
	//printf("%s", "tokenizer ran\n");
	/* TO_DO 14: Follow the standard and adjust datatypes */

	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	decartes_chr c;	/* input symbol */
	decartes_int state = 0;		/* initial state of the FSM */
	decartes_int lexStart;		/* start offset of a lexeme in the input char buffer (array) */
	decartes_int lexEnd;		/* end offset of a lexeme in the input char buffer (array)*/

	decartes_int lexLength;		/* token length */
	decartes_int i;				/* counter */
	decartes_chr newc;			/* new char */
	
	while (1) { /* endless loop broken by token returns it will generate a warning */
		//printf("in loop\n");
		c = getChar(sourceBuffer);
		//printf("\n%s%c","value: ", c);
		/* ------------------------------------------------------------------------
			Part 1: Implementation of token driven scanner.
			Every token is possessed by its own dedicated code
			-----------------------------------------------------------------------
		*/

		/* TO_DO 15: All patterns that do not require accepting functions */
		switch (c) {

		/* Cases for spaces */
		case ' ':
			//break;
		case '\t':
		case '\f':
			break;
		case '\n':
			line++;
			break;

		/* Cases for symbols */
		case ';':
			currentToken.code = EOS_T;
			return currentToken;
		case '(':
			currentToken.code = LPR_T;
			return currentToken;
		case ')':
			currentToken.code = RPR_T;
			return currentToken;
		case '{':
			currentToken.code = LBR_T;
			return currentToken;
		case '}':
			currentToken.code = RBR_T;
			return currentToken;
		case '[':
			currentToken.code = LSB_T;
			return currentToken;
		case ']':
			currentToken.code = RSB_T;
			return currentToken;
		case '+':
			newc = getChar(sourceBuffer);
			if (newc == '+') {
				currentToken.code = SCC_OP_T;
				return currentToken;
			}
			retract(sourceBuffer);
			currentToken.code = ART_OP_T;
			currentToken.attribute.codeType = OP_ADD;
			return currentToken;
		case '-':
			currentToken.code = ART_OP_T;
			currentToken.attribute.codeType = OP_SUB;
			return currentToken;
		case '*':
			currentToken.code = ART_OP_T;
			currentToken.attribute.codeType = OP_MUL;
			return currentToken;
		case '/':
			currentToken.code = ART_OP_T;
			currentToken.attribute.codeType = OP_DIV;
			return currentToken;
		case '>':
			currentToken.code = REL_OP_T;
			currentToken.attribute.codeType = OP_GT;
			return currentToken;
		case '<':
			newc = getChar(sourceBuffer);
			if (newc == '>') {
				currentToken.code = REL_OP_T;
				currentToken.attribute.codeType = OP_NE;
				return currentToken;
			}
			retract(sourceBuffer);
			currentToken.code = REL_OP_T;
			currentToken.attribute.codeType = OP_LT;
			return currentToken;
		case '=':
			if (getChar(sourceBuffer) == '=') {
				currentToken.code = REL_OP_T;
				currentToken.attribute.codeType = OP_EQ;
				return currentToken;
			}
			retract(sourceBuffer);
			currentToken.code = ASS_OP_T;
			return currentToken;
		case ',':
			currentToken.code = COM_T;
			return currentToken;
		case '_':
			/* AND operator */
			setMark(sourceBuffer, getReadPos(sourceBuffer));
			if (getChar(sourceBuffer) == '&' &&
				getChar(sourceBuffer) == '_') {
				currentToken.code = LOG_OP_T;
				currentToken.attribute.codeType = OP_AND;
				return currentToken;
			}
			else
				restore(sourceBuffer);
			/* OR operator */
			if (getChar(sourceBuffer) == '|' &&
				getChar(sourceBuffer) == '_') {
				currentToken.code = LOG_OP_T;
				currentToken.attribute.codeType = OP_OR;
				return currentToken;
			}
			else
				restore(sourceBuffer);
			/* NOT operator */
			if (getChar(sourceBuffer) == '!' &&
				getChar(sourceBuffer) == '_') {
				currentToken.code = LOG_OP_T;
				currentToken.attribute.codeType = OP_NOT;
				return currentToken;
			}
			/* Error (no operator) */
			else {
				restore(sourceBuffer);
				currentToken.attribute.errLexeme[0] = c;
				currentToken.attribute.errLexeme[1] = CHARSEOF0;
				currentToken.code = ERR_T;
				return currentToken;
			}
		/* Comments */
		case '#':
			newc = getChar(sourceBuffer);
			do {
				c = getChar(sourceBuffer);
				if (c == CHARSEOF0 || c == CHARSEOF255) {
					retract(sourceBuffer);
					return currentToken;
				} else if (c=='\n') {
					line++;
				}
			} while (c != '#' && c!= CHARSEOF0 && c!= CHARSEOF255);
			currentToken.code = CMM_T;
			break;		
		/* Cases for END OF FILE */
		case CHARSEOF0:
			currentToken.code = SEOF_T;
			currentToken.attribute.seofType = SEOF_0;
			return currentToken; 
		case CHARSEOF255:
			currentToken.code = SEOF_T;
			currentToken.attribute.seofType = SEOF_255;
			return currentToken;

		/* ------------------------------------------------------------------------
			Part 2: Implementation of Finite State Machine (DFA) or Transition Table driven Scanner
			Note: Part 2 must follow Part 1 to catch the illegal symbols
			-----------------------------------------------------------------------
		*/

		/* TO_DO_16: Adjust / check the logic for your language */

		default: // general case
			//printf("\n%s%c","default: ",c);
			state = nextState(state, c);
			lexStart = getReadPos(sourceBuffer) - 1;
			setMark(sourceBuffer, lexStart);
			int pos = 0;
			while (stateType[state] == NOAS) {
				c = getChar(sourceBuffer);
				state = nextState(state, c);
				pos++;
			}
			if (stateType[state] == ASWR)
				retract(sourceBuffer);
			lexEnd = getReadPos(sourceBuffer);
			//printf("\n%s%c", "\nlex end", lexEnd);
			lexLength = lexEnd - lexStart;
			//printf("\n%s%c", "\nlex length",lexLength);
			lexemeBuffer = create((short)lexLength + 2, 0, MODE_FIXED);
			if (!lexemeBuffer) {
				fprintf(stderr, "Scanner error: Can not create buffer\n");
				exit(1);
			}
			restore(sourceBuffer);
			for (i = 0; i < lexLength; i++) {
				decartes_chr temp = getChar(sourceBuffer);
				addChar(lexemeBuffer, temp);
			}
				
			addChar(lexemeBuffer, BUFFER_EOF);
			currentToken = (*finalStateTable[state])(getContent(lexemeBuffer, 0));
			destroy(lexemeBuffer);
			return currentToken;
		} // switch

	} //while

} // tokenizer


/*************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ************************************************************/
 /* TO_DO 17: Just change the datatypes */

decartes_int nextState(decartes_int state, decartes_chr c) {
	//printf("next state called\n");
	decartes_int col;
	decartes_int next;
	col = nextClass(c);
	//printf("%s%c","col: ", col);
	next = transitionTable[state][col];
	//printf("\n%s%c", "next: ", next);
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != IS);
	if (DEBUG)
		if (next == IS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}

/*************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
************************************************************/
/* TO_DO 18: Use your column configuration */

decartes_int nextClass(decartes_chr c) {
	decartes_int val = -1;
	/* Adjust the logic to return next column in TT */
	/*			[A-z],	[0-9],	",		',		EF,		.,		Other, 
	/*			L(0),	D(1),	Q1(2),	Q2(3),	E(4),	P(5),	O(6), */
	switch (c) {
	case CHRCOL2:
		val = 2;//"
		break;
	case CHRCOL3:
		val = 3; //'
		break;

	case CHRCOL5:
		val = 5;// period
		break;

	case CHARSEOF0:
	case CHARSEOF255:
		val = 4;//eof
		break;
	default:
		if (isalpha(c))
			val = 0;//letter
		else if (isdigit(c))
			val = 1;//digit
		else
			val = 6;//other
	} //switch
	return val;
}


/*************************************************************
 * Acceptance State Method MID
 *		In this function, the pattern for MNID must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ************************************************************/
 /* TO_DO_19A: Adjust the function for MID */
Token funcMID(decartes_chr lexeme[]) {
	Token currentToken = { 0 };
	switch (lexeme[0]) {

	default:
		currentToken.code = ERR_T;
		break;
	}
	strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
	currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
	return currentToken;
}


/*************************************************************
 * Acceptance State Function VID
 *		In this function, the pattern for IVID must be recognized.
 *		Since keywords obey the same pattern, is required to test if 
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and 
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ************************************************************/
 /* TO_DO_19A: Adjust the function for VID */
Token funcVID(decartes_chr lexeme[]) {
	Token currentToken = { 0 };
	decartes_int kwindex = -1, j = 0;
	for (j = 0; j < KWT_SIZE; j++)
		if (!strcmp(lexeme, &keywordTable[j][0]))
			kwindex = j;
	if (kwindex != -1) {
		currentToken.code = KW_T;
		currentToken.attribute.codeType = kwindex;
	}
	else {
		currentToken.code = VID_T;
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
	}

	return currentToken;
}


/*************************************************************
 * Acceptance State Function IL
 *		Function responsible to identify IL (integer literals).
 * - It is necessary respect the limit (ex: 2-byte integer in C).
 * - In the case of larger lexemes, error shoul be returned.
 * - Only first ERR_LEN characters are accepted and eventually, 
 *   additional three dots (...) should be put in the output.
 ************************************************************/
/* TO_DO_19B: Adjust the function for IL */

Token funcIL(decartes_chr lexeme[]) {
	Token currentToken = { 0 };
	decartes_lng tlong;
	if (lexeme[0] != '\0' && strlen(lexeme) > NUM_LEN) {
		currentToken = (*finalStateTable[ES])(lexeme);
	}
	else {
		tlong = atol(lexeme);
		if (tlong >= 0 && tlong <= SHRT_MAX) {
			currentToken.code = INL_T;
			currentToken.attribute.intValue = (decartes_int)tlong;
		}
		else {
			currentToken = (*finalStateTable[ES])(lexeme);
		}
	}
	return currentToken;
}

/*************************************************************
 * Acceptance State Function FPL
 *		Function responsible to identify FPL (float point literals).
 * - It is necessary respect the limit (ex: 4-byte integer in C).
 * - In the case of larger lexemes, error shoul be returned.
 * - Only first ERR_LEN characters are accepted and eventually, 
 *   additional three dots (...) should be put in the output.
 ************************************************************/
/* TO_DO_19C: Adjust the function for FPL */

Token funcFPL(decartes_chr lexeme[]) {
	Token currentToken = { 0 };
	decartes_dbl tdouble = atof(lexeme);
	if (tdouble == 0.0 || tdouble >= FLT_MIN && tdouble <= FLT_MAX) {
		currentToken.code = FPL_T;
		currentToken.attribute.floatValue = (decartes_flt)tdouble;
	}
	else {
		currentToken = (*finalStateTable[ES])(lexeme);
	}
	return currentToken;
}

/*************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table 
 *   (stringLiteralTable). You need to include the literals in 
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ************************************************************/
/* TO_DO_19D: Adjust the function for SL */

Token funcSL(decartes_chr lexeme[]) {
	Token currentToken = { 0 };
	decartes_int i = 0, len = (decartes_int)strlen(lexeme);
	currentToken.attribute.contentString = getWritePos(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == '\n')
			line++;
		if (!addChar(stringLiteralTable, lexeme[i])) {
			currentToken.code = RTE_T;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
	}
	if (!addChar(stringLiteralTable, CHARSEOF0)) {
		currentToken.code = RTE_T;
		strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
		errorNumber = RTE_CODE;
		return currentToken;
	}
	currentToken.code = STR_T;
	return currentToken;
}


/*************************************************************
 * This function checks if one specific lexeme is a keyword.
 * - Tip: Remember to use the keywordTable to check the keywords.
 ************************************************************/
 /* TO_DO_19E: Adjust the function for Keywords */

Token funcKEY(decartes_chr lexeme[]) {
	Token currentToken = { 0 };
	decartes_int kwindex = -1, j = 0;
	for (j = 0; j < KWT_SIZE; j++)
		if (!strcmp(lexeme, &keywordTable[j][0]))
			kwindex = j;
	if (kwindex != -1) {
		currentToken.code = KW_T;
		currentToken.attribute.codeType = kwindex;
	}
	else {
		currentToken = funcErr(lexeme);
	}
	return currentToken;
}


/*************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ************************************************************/
 /* TO_DO_19F: Adjust the function for Errors */

Token funcErr(decartes_chr lexeme[]) {
	Token currentToken = { 0 };
	decartes_int i = 0, len = (decartes_int)strlen(lexeme);
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = CHARSEOF0;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == '\n')
			line++;
	currentToken.code = ERR_T;
	return currentToken;
}


/*************************************************************
 * The function prints the token returned by the scanner
 ************************************************************/

decartes_nul printToken(Token t) {
	extern decartes_chr* keywordTable[]; /* link to keyword table in */
	switch (t.code) {
	case RTE_T:
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		/* Call here run-time error handling component */
		if (errorNumber) {
			printf("%d", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case VID_T:
		printf("VID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case FPL_T:
		printf("FPL_T\t\t%f\n", t.attribute.floatValue);
		break;
	case INL_T:
		printf("INL_T\t\t%d\n", t.attribute.codeType);
		break;
	case STR_T:
		printf("STR_T\t\t%d\t ", (short)t.attribute.codeType);
		printf("%s\n", getContent(stringLiteralTable,
			(short)t.attribute.codeType));
		break;
	case SCC_OP_T:
		printf("SCC_OP_T\n");
		break;
	case ASS_OP_T:
		printf("ASS_OP_T\n");
		break;
	case ART_OP_T:
		printf("ART_OP_T\t%d\n", t.attribute.codeType);
		break;
	case REL_OP_T:
		printf("REL_OP_T\t%d\n", t.attribute.codeType);
		break;
	case LOG_OP_T:
		printf("LOG_OP_T\t%d\n", t.attribute.codeType);
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case LSB_T:
		printf("LSB_T\n");
		break;
	case RSB_T:
		printf("RSB_T\n");
		break;
	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case COM_T:
		printf("COM_T\n");
		break;
	//case CMM_T:
		//printf("CMM_T\n");
		//break;
	case EOS_T:
		printf("EOS_T\n");
		break;
	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
	}
}

/*
TO_DO: (If necessary): HERE YOU WRITE YOUR ADDITIONAL FUNCTIONS (IF ANY).
*/
