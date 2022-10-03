/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2022
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa
************************************************************
 _________________________________
|                                 |
| ........ BOA LANGUAGE ......... |
|     __    __    __    __        |
|    /  \  /  \  /  \  /  \       |
| __/  __\/  __\/  __\/  __\__    |
| _/  /__/  /__/  /__/  /_____|   |
|  \_/ \   / \   / \   / \  \___  |
|       \_/   \_/   \_/   \___o_> |
|                                 |
| .. ALGONQUIN COLLEGE - 2022F .. |
|_________________________________|

*/

/*
************************************************************
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Sep 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

/* TIP: Do not change pragmas, unless necessary .......................................*/
/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer reader) */
enum READER_MODE {
	MODE_FIXED = 'f', /* Fixed mode (constant size) */
	MODE_ADDIT = 'a', /* Additive mode (constant increment to be added) */
	MODE_MULTI = 'm'  /* Multiplicative mode (constant increment to be multiplied) */
};

/* Constants about controls (not need to change) */
#define READER_ERROR (-1)						/* General error message */
#define READER_TERMINATOR '\0'							/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE	INT_MAX-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE			250		/* default initial buffer reader capacity */
#define READER_DEFAULT_INCREMENT	10		/* default increment factor */

/* Add your bit-masks constant definitions here - Defined for BOA */
/* BITS                                (7654.3210) */
#define READER_DEFAULT_FLAG 0x00 	/* (0000.0000)_2 = (000)_10 */
#define READER_FUL_FLAG 0xF 
#define READER_EMP_FLAG 0x7
#define READER_REL_FLAG 0x3
#define READER_END_FLAG 0x1 

#define NCHAR				128			/* Chars from 0 to 127 */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* TODO: Adjust datatypes */

/* Offset declaration */
typedef struct position {
	mgn_int mark;			/* the offset to the mark position (in chars) */
	mgn_int read;			/* the offset to the get a char position (in chars) */
	mgn_int wrte;			/* the offset to the add chars (in chars) */
} Position;

/* Buffer structure */
typedef struct bufferReader {
	mgn_ch*	content;			/* pointer to the beginning of character array (character buffer) */
	mgn_int	size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	mgn_int	increment;			/* character array increment factor */
	mgn_int	mode;				/* operational mode indicator */
	mgn_byte	flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position	position;			/* Offset / position field */
	mgn_int	histogram[NCHAR];	/* Statistics of chars */
} BufferReader, * ReaderPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
ReaderPointer	readerCreate		(mgn_int, mgn_int, mgn_int);
ReaderPointer	readerAddChar		(ReaderPointer const, mgn_ch);
mgn_bool		readerClear		    (ReaderPointer const);
mgn_bool		readerFree		    (ReaderPointer const);
mgn_bool		readerIsFull		(ReaderPointer const);
mgn_bool		readerIsEmpty		(ReaderPointer const);
mgn_bool		readerSetMark		(ReaderPointer const, mgn_int);
mgn_int		readerPrint		    (ReaderPointer const);
mgn_int		readerLoad			(ReaderPointer const, FILE* const);
mgn_bool		readerRecover		(ReaderPointer const);
mgn_bool		readerRetract		(ReaderPointer const);
mgn_bool		readerRestore		(ReaderPointer const);
/* Getters */
mgn_ch		readerGetChar		(ReaderPointer const);
mgn_ch*		readerGetContent	(ReaderPointer const, mgn_int);
mgn_int		readerGetPosRead	(ReaderPointer const);
mgn_int		readerGetPosWrte	(ReaderPointer const);
mgn_int		readerGetPosMark	(ReaderPointer const);
mgn_int		readerGetSize		(ReaderPointer const);
mgn_int		readerGetInc		(ReaderPointer const);
mgn_int		readerGetMode		(ReaderPointer const);
mgn_byte		readerGetFlags		(ReaderPointer const);
mgn_int		readerShowStat		(ReaderPointer const);

#endif
