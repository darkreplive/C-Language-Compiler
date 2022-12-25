/*
Tyler King 040979598
Daniel Gribanov 40938906
*/

/*************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2020
* Author: Paulo Sousa - Sep, 2021.
*************************************************************
* File name: compilers.h
* Compiler: MS Visual Studio 2019
* Author: Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A1, A2, A3.
* Date: Sep 01 2020
* Professor: Paulo Sousa / Haider Miraj
* Purpose: This file defines the functions called by main function.
* Function list: mainBuffer(), mainScanner(), mainParser().
*************************************************************/

#ifndef COMPILERS_H_
#define COMPILERS_H_

#define DEBUG 0

/* Logical constants - adapt for your language */
#define DECARTES_TRUE	1
#define DECARTES_FALSE 0

/*
------------------------------------------------------------
Programs:
1: Buffer - invokes MainBuffer code
2: Scanner - invokes MainScanner code
3: Parser - invokes MainParser code
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_BUFFER	= '1',
	PGM_SCANNER = '2',
	PGM_PARSER	= '3'
};

/*
------------------------------------------------------------
Main functions signatures
(Codes will be updated during next assignments)
------------------------------------------------------------
*/
int mainBuffer	(int argc, char** argv);
int mainScanner	(int argc, char** argv);
int mainParser	(int argc, char** argv);

/*
------------------------------------------------------------
Data types definitions
------------------------------------------------------------
*/

/* TO_DO: Define your typedefs */

typedef short			decartes_int;
typedef long			decartes_lng;
typedef char			decartes_chr;
typedef unsigned char	decartes_flg;
typedef char			decartes_bol;
typedef float			decartes_flt;
typedef double			decartes_dbl;
typedef void			decartes_nul;

#endif
