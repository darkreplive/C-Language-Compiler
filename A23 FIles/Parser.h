/*
Tyler King 040979598
Daniel Gribanov 40938906
*/
/*************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2021
*************************************************************
* File name: Parser.h
* Compiler: MS Visual Studio 2019
* Author: Paulo Sousa (based on prof. Svillen Ranev work)
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A3.
* Date: Dec 11 2021
* Purpose: This file is the main header for Parser (.h)
*************************************************************/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef BUFFER_H_
#include "Buffer.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */

/* TO_DO: Adjust all datatypes to your language */
static Token lookahead;
decartes_int syntaxErrorNumber = 0;
extern Buffer* stringLiteralTable;
extern decartes_int line;
extern Token tokenizer();
extern decartes_chr* keywordTable[];

/* TO_DO: Adjust your language name here */
#define STR_LANGNAME "Decartes"

/* TO_DO: Create ALL constants for keywords (sequence given in table.h) */

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,
	STATIC,
	SCANF,
	PRINTF,
	INT,
	FX,
	CONST,
	CUBIC,
	RATIONAL,
	QUADRATIC,
	FLOAT,
	STRING,
	IF,
	ELSE,
	WHITE,
	DO
};

/* Function definitions */
decartes_nul startParser();
decartes_nul matchToken(decartes_int, decartes_int);
decartes_nul syncErrorHandler(decartes_int);
decartes_nul printError();

/* TO_DO: Place ALL non-terminal function declarations */
decartes_nul additiveArithmeticExpression();
decartes_nul additiveArithmeticExpressionPrime();
decartes_nul arithmeticExpression();
decartes_nul assignmentExpression();
decartes_nul assignmentStatement();
decartes_nul codeSession();
decartes_nul dataSession();
decartes_nul fltVariableIdentifier();
decartes_nul fltVarList();
decartes_nul fltVarListPrime();
decartes_nul inputStatement();
decartes_nul intVarIdentifier();
decartes_nul intVarList();
decartes_nul intVarListPrime();
decartes_nul multiplicativeArithmeticExpression();
decartes_nul multiplicativeArithmeticExpressionPrime();
decartes_nul optVarListDeclarations();
decartes_nul optionalStatements();
decartes_nul outputStatement();
decartes_nul outputVariableList();
decartes_nul primaryArithmeticExpression();
decartes_nul primaryStringExpression();
decartes_nul program();
decartes_nul statement();
decartes_nul statements();
decartes_nul statementsPrime();
decartes_nul strVarList();
decartes_nul strVarListPrime();
decartes_nul strVariableIdentifier();
decartes_nul variableIdentifier();
decartes_nul variableList();
decartes_nul variableListPrime();
decartes_nul varListDeclarations();
decartes_nul varListDeclaration();
decartes_nul varListDeclarationsPrime();

#endif
