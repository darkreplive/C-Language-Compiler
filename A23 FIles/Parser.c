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
* File name: Parser.c
* Compiler: MS Visual Studio 2019
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: Sep 01 2021
* Professor: Paulo Sousa
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

/* TO_DO: For all functions adjust the datatypes used in your language */

/*************************************************************
 * Process Parser
 ************************************************************/
 /* TO_DO: This is the function to start the parser - check your program definition */

decartes_nul startParser() {
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*************************************************************
 * Match Token
 ************************************************************/
 /* TO_DO: This is the main code for match - check your definitions */

decartes_nul matchToken(decartes_int tokenCode, decartes_int tokenAttribute) {
	decartes_int matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType != tokenAttribute)
			matchFlag = 0;
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*************************************************************
 * Syncronize Error Handler
 ************************************************************/
 /* TO_DO: This is the function to handler error - adjust basically datatypes */

decartes_nul syncErrorHandler(decartes_int syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*************************************************************
 * Print Error
 ************************************************************/
 /* TO_DO: This is the function to error printing - adjust basically datatypes */

decartes_nul printError() {
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case VID_T:
		printf("%s\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("%s\n", getContent(stringLiteralTable, t.attribute.contentString));
		break;
	case KW_T:
		printf("%s\n", keywordTable[t.attribute.codeType]);
		break;
	case LPR_T:
	case RPR_T:
	case LBR_T:
	case RBR_T:
	case COM_T:
	case EOS_T:
		printf("NA\n");
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
	}
}

/*************************************************************
 * Program statement
 * BNF: <program> -> MAIN { <opt_statements> }
 * FIRST(<program>)= { MNID_T }.
 ************************************************************/
 /* TO_DO: Adjust this function to respect your grammar */

decartes_nul program() {
	switch (lookahead.code) {
	case KW_T:
		matchToken(KW_T, INT);
		//printf("1\n");
		//matchToken(VID_T, NO_ATTR);
		//printf(lookahead.attribute.idLexeme);
		if (strncmp(lookahead.attribute.idLexeme, "main", 4) == 0) {
			matchToken(VID_T, NO_ATTR);
			//printf("\nA\n");
			matchToken(LPR_T, NO_ATTR);
			//printf("B\n");
			matchToken(RPR_T, NO_ATTR);
			//printf("C\n");
			matchToken(LBR_T, NO_ATTR);
			dataSession();
			codeSession();
			//printf("D\n");
			matchToken(RBR_T, NO_ATTR);
			//printf("E\n");
		}
		else {
			printError();
		}
		break;
	case SEOF_T:
		; // Empty
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}


/*************************************************************
 * dataSession
 * BNF: <dataSession> -> DATA { <opt_varlist_declarations> }
 * FIRST(<program>)= {KW_T (DATA)}.
 ************************************************************/
 /* TO_DO: Adjust this function to respect your grammar */

decartes_nul dataSession() {
	//matchToken(LBR_T, NO_ATTR);
	optVarListDeclarations();
	//matchToken(RBR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Data Session parsed");
}

/*************************************************************
 * Optional Var List Declarations
 * BNF: <opt_varlist_declarations> -> <varlist_declarations> | e
 * FIRST(<opt_varlist_declarations>) = { e, KW_T (FLOAT) }.
 ************************************************************/
 /* TO_DO: Adjust this function to respect your grammar */

decartes_nul optVarListDeclarations() {
	varListDeclarations();
	/*switch (lookahead.code) {
		//printf("optVarListDeclarations\n");
		varListDeclarations();
	default:
		; // Empty
	}*/
	printf("%s%s\n", STR_LANGNAME, ": Optional Variable List Declarations parsed");
}

/*************************************************************
 * codeSession statement
 * BNF: <codeSession> -> CODE { <opt_statements> }
 * FIRST(<???>) = { ??? }.
 ************************************************************/
 /* TO_DO: Adjust this function to respect your grammar */

decartes_nul codeSession() {
	//matchToken(KW_T, INT);
	//matchToken(LBR_T, NO_ATTR);
	optionalStatements();
	//matchToken(RBR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Code Session parsed");
}

/*************************************************************
 * Optional statement
 * BNF: <opt_statements> -> <statements> | ϵ
 * FIRST(<???>) = { ??? }.
 ************************************************************/
 /* TO_DO: Adjust this function to respect your grammar */

decartes_nul optionalStatements() {
	switch (lookahead.code) {
	case KW_T:
		
		switch (lookahead.attribute.codeType) {
		case PRINTF:
			statements();
			break;

		case INT:
			printf("in optional int");
			matchToken(KW_T, INT);
			intVarIdentifier();
			assignmentStatement();
			break;
		}
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}

/*************************************************************
 * Statements
 * BNF: <statements> -> <statement><statementsPrime>
 * FIRST(<???>) = { ??? }.
 ************************************************************/
 /* TO_DO: Adjust this function to respect your grammar */

decartes_nul statements() {
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*************************************************************
 * Statements Prime
 * BNF: <statementsPrime>  <statement><statementsPrime> | ϵ
 * FIRST(<???>) = { ??? }.
 ************************************************************/
 /* TO_DO: Adjust this function to respect your grammar */

decartes_nul statementsPrime() {
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType == PRINTF) {
			statement();
			statementsPrime();
			break;
		}
	default:
		; //empty string
	}
}

/*************************************************************
 * Single statement
 * BNF: <statement> ->  <assignment statement> | <input statement> | <output statement>
 * FIRST(<???>) = { ??? }.
 ************************************************************/
 /* TO_DO: Adjust this function to respect your grammar */

decartes_nul statement() {
	switch (lookahead.code) {
	case KW_T://output
		//printf("In statement KW\n");
		switch (lookahead.attribute.codeType) {
		case PRINTF:
			outputStatement();
			break;
		case INT:
			printf("in statement int");
			break;
		default:
			printError();
		}
		break;
	case ASS_OP_T://assignment
		printf("in statement art");
		assignmentStatement();
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/*************************************************************
 * Output Statement
 * BNF: <output statement> -> WRITE (<output statementPrime>);
 * FIRST(<???>) = { ??? }.
 ************************************************************/
 /* TO_DO: Adjust this function to respect your grammar */

decartes_nul outputStatement() {
	matchToken(KW_T, PRINTF);
	matchToken(LPR_T, NO_ATTR);
	outputVariableList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");

}

/*************************************************************
 * Output Variable List
 * BNF: <opt_variable list> -> <variable list> | ϵ
 * FIRST(<???>) = { ??? }.
 ************************************************************/
 /* TO_DO: Adjust this function to respect your grammar */

decartes_nul outputVariableList() {
	variableList();
	printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}

/* TO_DO: Continue the development (all non-terminal functions) */
decartes_nul inputStatement() {
	matchToken(KW_T, SCANF);
	matchToken(LPR_T, NO_ATTR);
	variableList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": input statement parsed");
}

decartes_nul assignmentStatement() {
	assignmentExpression();
}

decartes_nul assignmentExpression() {
	printf("%d\n", lookahead.code);
	switch (lookahead.attribute.codeType) {
	case ART_OP_T:
		//matchToken(ASS_OP_T, NO_ATTR);
		arithmeticExpression();
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Assignment expression parsed");
}

decartes_nul arithmeticExpression() {
	switch (lookahead.code) {
	case ART_OP_T:
		additiveArithmeticExpression();
		break;
	case SCC_OP_T:
		break;
	}
}

decartes_nul additiveArithmeticExpression() {
	multiplicativeArithmeticExpression();
	additiveArithmeticExpressionPrime();
	printf("%s%s\n", STR_LANGNAME, ": Additive arithmetic expression parsed");
}

decartes_nul additiveArithmeticExpressionPrime() {
	switch (lookahead.code) {
	case ART_OP_T:
		switch (lookahead.attribute.codeType) {
		case OP_ADD:
			matchToken(ART_OP_T, OP_ADD);
			additiveArithmeticExpressionPrime();
			break;
		case OP_SUB:
			matchToken(ART_OP_T, OP_SUB);
			additiveArithmeticExpressionPrime();
			break;
		}
	default:
		;//nothing
	}
}

decartes_nul primaryArithmeticExpression() {
	switch (lookahead.code) {
	case VID_T:
		matchToken(VID_T, NO_ATTR);
		break;
	case ART_OP_T:
		arithmeticExpression();
		break;
	default:
		;//nothing
	}
}

decartes_nul multiplicativeArithmeticExpression() {
	primaryArithmeticExpression();
	multiplicativeArithmeticExpressionPrime();
}

decartes_nul multiplicativeArithmeticExpressionPrime() {
	switch (lookahead.code) {
	case ART_OP_T:
		switch (lookahead.attribute.codeType) {
		case OP_DIV:
			matchToken(ART_OP_T, OP_DIV);
			primaryArithmeticExpression();
			multiplicativeArithmeticExpressionPrime();
			break;
		case OP_MUL:
			matchToken(ART_OP_T, OP_MUL);
			primaryArithmeticExpression();
			multiplicativeArithmeticExpressionPrime();
			break;
		}
	default:
		;//nothing
	}
}

decartes_nul variableIdentifier() {
	switch (lookahead.code)
	{
	case INL_T:
		matchToken(INL_T, NO_ATTR);
		break;
	case FPL_T:
		matchToken(FPL_T, NO_ATTR);
		break;
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	default:
		;//nothing
	}
}

decartes_nul variableList() {
	variableIdentifier();
	variableListPrime();
}

decartes_nul variableListPrime() {

	switch (lookahead.code) {
	case VID_T:
		matchToken(VID_T, NO_ATTR);
		variableIdentifier();
		variableListPrime();
		break;
	default:
		;//nothing
	}
}

decartes_nul varListDeclaration() {
	//printf("In varListDeclaration\n");
	//printf(lookahead.code);
	switch (lookahead.code)
	{

	case KW_T:
		//printf("main switch\n");
		switch (lookahead.attribute.codeType) {
		case INT:
			//printf("in int\n");
			matchToken(KW_T, INT);
			intVarList();
			//check for ART_OP_T
			break;
		case FLOAT:
			//printf("in float\n");
			//printf("%s",lookahead.attribute.idLexeme);
			matchToken(KW_T, FLOAT);
			fltVarList();
			break;
		case STRING:
			//printf("in string\n");
			matchToken(KW_T, STRING);
			strVarList();
			break;
		default:
			break;
		}
		break;
	default:
		;
	}

}

decartes_nul varListDeclarations() {
	//printf("In varListDeclarations\n");
	varListDeclaration();
	varListDeclarationsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Variable list declaration parsed");
}

decartes_nul varListDeclarationsPrime() {
	//printf("In varListDeclarationsPrime\n");
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType != PRINTF) {
			//printf("In VPrime switch\n");
			varListDeclaration();
			varListDeclarationsPrime();
		}
	}
}
//int methods
decartes_nul intVarList() {
	intVarIdentifier();
	intVarListPrime();
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Integer Var list parsed");
}

decartes_nul intVarListPrime() {
	//printf("In intVarListPrime\n");
	switch (lookahead.code) {
	case VID_T:
		intVarIdentifier();
		intVarListPrime();
		break;
	default:
		;//nothing
	}
	
}

decartes_nul intVarIdentifier() {
	//printf("%s%s", lookahead.attribute.idLexeme, "\n");
	matchToken(VID_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Integer literal parsed");
}

//float methods
decartes_nul fltVarList() {
	fltVariableIdentifier();
	fltVarListPrime();
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Float Var list parsed");
}

decartes_nul fltVarListPrime() {
	switch (lookahead.code) {
	case VID_T:
		fltVariableIdentifier();
		fltVarListPrime();
		break;
	default:
		;//nothing
	}
}

decartes_nul fltVariableIdentifier() {
	//printf("%s%s", lookahead.attribute.idLexeme, "\n");
	matchToken(VID_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Float literal parsed");
}


//string methods
decartes_nul primaryStringExpression() {
	matchToken(STR_T, NO_ATTR);
}

decartes_nul strVarList() {
	strVariableIdentifier();
	strVarListPrime();
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": String Var list parsed");
}

decartes_nul strVarListPrime() {
	switch (lookahead.code) {
	case VID_T:
		strVariableIdentifier();
		strVarListPrime();
		break;
	default:
		;//nothing
	}
}

decartes_nul strVariableIdentifier() {
	//printf("%s%s", lookahead.attribute.idLexeme, "\n");
	matchToken(VID_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": String literal parsed");
}

