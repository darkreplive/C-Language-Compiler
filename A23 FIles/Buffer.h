/*
Tyler King 040979598
Daniel Gribanov 40938906
*/

/*************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2021
*************************************************************
* File name: Buffer.h
* Compiler: MS Visual Studio 2019
* Author: Paulo Sousa (based on prof. Svillen Ranev work)
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: Sep 01 2021
* Purpose: This file is the main header for Parser (.h)
*************************************************************/


#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#define BUFFER_H_

/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer) */
enum BUFFERMODES {
	MODE_FIXED = 'f',
	MODE_ADDIT = 'a',
	MODE_MULTI = 'm'
};

/* TO_DO: Adjust all constants to your Language */

#define BUFFER_ERROR (-1)						/* General error message */
#define BUFFER_EOF '\0'							/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (decartes) .................................. */

#define BUFFER_DEFAULT_SIZE			200			/* default initial buffer capacity */
#define BUFFER_DEFAULT_INCREMENT	100			/* default increment factor */

/* You should add your own constant definitions here */
#define decartes_MAX_SIZE				SHRT_MAX-1	/*maximum capacity*/ 

/* Add your bit-masks constant definitions here - Defined for decartes */
/* BITS                                7654.3210 */
#define decartes_DEFAULT_FLAG 0xF0 	/* 1111.0000 = 240 */

/* TO_DO: BIT 0: EMP: Empty */
#define set_EMP_FLAG 0x01 // 0000.0001 = 1
#define reset_EMP_FLAG 0xFE // 1111.1110 = 254
#define check_EMP_FLAG 0x01 // 0000.0001 = 1
/* TO_DO: BIT 1: FUL = Full */
#define set_FUL_FLAG 0x02 // 0000.0010 = 2
#define reset_FUL_FLAG 0xFD // 1111.1101 = 253
#define check_FUL_FLAG 0x02 // 0000.0010 = 2
/* TO_DO: BIT 2: EOB = EndOfBuffer */
#define set_EOB_FLAG 0x04 // 0000.0100 = 4
#define reset_EOB_FLAG 0xFB // 1111.1011 = 251
#define check_EOB_FLAG 0x02 // 0000.0100 = 4
/* TO_DO: BIT 3: REL = Relocation */
#define set_REL_FLAG 0x08 // 0000.8000 = 8
#define reset_REL_FLAG 0xF7 // 1111.0111 = 247
#define check_REL_FLAG 0x08 // 0000.1000 = 8
/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (decartes) .................................. */

/* TO_DO: Adjust datatypes */

/* Offset declaration */
typedef struct position {
	decartes_int writePos;				/* the offset (in chars) to the add-character location */
	decartes_int readPos;				/* the offset (in chars) to the get-character location */
	decartes_int markPos;				/* the offset (in chars) to the mark location */
} Position;

/* Buffer structure */
typedef struct buffer {
	decartes_chr*	content;		/* pointer to the beginning of character array (character buffer) */
	decartes_int	size;		    /* current dynamic memory size (in bytes) allocated to character buffer */
	decartes_int	increment;		/* character array increment factor */
	decartes_int	mode;			/* operational mode indicator */
	decartes_flg	flags;			/* contains character array reallocation flag and end-of-buffer flag */
	Position	position;		/* Offset / position field */
} Buffer, * BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
BufferPointer create(decartes_int, decartes_int, decartes_int);
BufferPointer addChar(BufferPointer const, decartes_chr);
decartes_bol retract(BufferPointer const);
decartes_bol restore(BufferPointer const);
decartes_bol recover(BufferPointer const);
decartes_bol clear(BufferPointer const);
decartes_bol destroy(BufferPointer const);
decartes_bol setMark(BufferPointer const, decartes_int);
decartes_bol isEmpty(BufferPointer const);
decartes_bol isFull(BufferPointer const);
decartes_bol isRealocated(BufferPointer const);
decartes_int print(BufferPointer const);
decartes_int load(BufferPointer const, FILE* const);
/* Getters */
decartes_int getSize(BufferPointer const);
decartes_int getWritePos(BufferPointer const);
decartes_int getMarkPos(BufferPointer const);
decartes_int getReadPos(BufferPointer const);
decartes_int getIncrement(BufferPointer const);
decartes_int getMode(BufferPointer const);
decartes_chr getChar(BufferPointer const);
decartes_chr* getContent(BufferPointer const, decartes_int);
decartes_flg getFlags(BufferPointer const);

#endif
