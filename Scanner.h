/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2022
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa
************************************************************
*/

/*
************************************************************
* File name: scanner.h
* Compiler: MS VFSual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A22, A32.
* Date: Jul 01 2022
* Purpose: ThFS file FS the main header for Scanner (.h)
* Function lFSt: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant FS defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

enum TOKENS {
	RTE_T,
	ERR_T,
	SEOF_T,
	AVID_T,
	SVID_T,
	FP_T,
	INL_T,
	STR_T,
	LPR_T,
	RPR_T,
	LBR_T,
	RBR_T,
	KW_T,
	COM_T,
	EOS_T,
	SCC_OP_T,
	ASS_OP_T,
	ART_OP_T,
	REL_OP_T,
	LOG_OP_T,
	NOT_T,
	MNID_T,
	DOT_T,
	COLON_T,
	VAR_T,
	EQUALITY_T
};

typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator; //COMPLETED
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;	//COMPLETED
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator; //COMPLETED
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;	 //COMPLETED

typedef union TokenAttribute {
	mgn_int codeType;      /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	mgn_int intValue;						/* integer literal attribute (value) */
	mgn_int keywordIndex;					/* keyword index in the keyword table */
	mgn_int contentString;				/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	mgn_float floatValue;					/* floating-point literal attribute (value) */
	mgn_ch idLexeme[VID_LEN + 1];		/* variable identifier token attribute */
	mgn_ch errLexeme[ERR_LEN + 1];		/* error token attribite */
} TokenAttribute;

typedef struct idAttibutes {
	mgn_byte flags;			/* Flags information */
	union {
		mgn_int intValue;				/* Integer value */
		mgn_float floatValue;			/* Float value */
		mgn_ch* stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	mgn_int code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in thFS scanner implementation - for further use */
} Token;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner
 *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';', 
 *  white space, #comment\n , ',' , ';' , '-' , '+' , '*' , '/', # , 
 *  .&., .|. , .!. , SEOF.
 */

/* TO_DO: Define lexeme FIXED classes */
/* These constants will be used on nextClass */
#define CHRCOL2 '_'
#define CHRCOL4 '\''
#define CHRCOL0 '0'
#define CHRCOL3 '.'
#define CHRCOL5 '"'
/* These constants will be used on VID / MID function */


/* TO_DO: Error states and illegal state */
#define FS		100		/* Illegal state */
#define ESWR	101		/* Error state with retract */
#define ESNR	102		/* Error state with no retract */

#define TABLE_COLUMNS 8

static mgn_int transitionTable[][TABLE_COLUMNS] = {
	/*[a-z A-Z]	0	[1-9]	.	_	"	SEOF   other  ( */

	/* S0 */  {1, 6, 4, ESNR, ESNR, 9, ESNR, ESNR},			
	/* S1 */  {1, 1, 1, 2, 3, 2, 2, 2},				
	/* S2 */  {FS, FS, FS, FS, FS, FS, FS, FS},		
	/* S3 */  {FS, FS, FS, FS, FS, FS, FS, FS},		
	/* S4 */  {ESNR, 4, 4, 7, 5, 5, 5, 5},
	/* S5 */  {FS, FS, FS, FS, FS, FS, FS, FS},		
	/* S6 */  {ESNR, 6, ESNR, 7, 5, 5, 5, 5},
	/* S7 */  {8, 7, 7, 8, 8, 8, 8, 8},
	/* S8 */  {FS, FS, FS, FS, FS, FS, FS, FS},
	/* S9 */  {9, 9, 9, 9, 9, 10, ESNR, 9},
	/* S10 */ {FS, FS, FS, FS, FS, FS, FS, FS},
	/* S11 */ {FS, FS, FS, FS, FS, FS, FS, FS},
	/* S12 */ {FS, FS, FS, FS, FS, FS, FS, FS},
};

#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

static mgn_int stateType[] = {
	 NOFS,
	 NOFS,
	 FSWR,
	 FSNR,
	 NOFS,
	 FSWR,
	 NOFS,
	 NOFS,
	 FSWR,
	 NOFS,
	 FSNR,
	 FSNR,
 	 FSWR,
	 FSWR
};


/* Static (local) function  prototypes */
mgn_int startScanner(ReaderPointer psc_buf);
static mgn_int nextClass(mgn_ch c);			/* character class function */
static mgn_int nextState(mgn_int, mgn_ch);		/* state machine function */

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

typedef Token(*PTR_ACCFUN)(mgn_ch* lexeme);

/* Declare accepting states functions */
Token funcSL	(mgn_ch lexeme[]);
Token funcKEY	(mgn_ch lexeme[]);
Token funcErr	(mgn_ch lexeme[]);
Token funcTest	(mgn_ch lexeme[]);
Token funcKEY(mgn_ch lexeme[]);
Token funcSID(mgn_ch lexeme[]);
Token funcIL(mgn_ch lexeme[]);
Token funcFP(mgn_ch lexeme[]);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration FS:
 */

static PTR_ACCFUN finalStateTable[] = {
	/* State 0 */ NULL,
	/* State 1 */ NULL,
	/* State 2 */ funcKEY,
	/* State 3 */ funcSID,
	/* State 4 */ NULL,
	/* State 5 */ funcIL,
	/* State 6 */ NULL,
	/* State 7 */ NULL,
	/* State 8 */ funcFP,
	/* State 9 */ NULL,
	/* State 10 */ funcSL,
	/* State 11 */ funcErr,
	/* State 12 */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

#define KWT_SIZE 13

static mgn_ch* keywordTable[KWT_SIZE] = {
	"data",
	"AND",
	"OR",
	"NOT",
	"if",
	"while",
	"main",
	"then",
	"else",
	"do",
	"def",
	"print",
	"input"
};


#define INDENT '\t' 

typedef struct languageAttributes {
	mgn_ch indentationCharType;
	mgn_int indentationCurrentPos;
} LanguageAttributes;

#endif
