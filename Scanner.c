/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2022
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa
************************************************************
*/

/*
************************************************************
* File name: scanner.h
* Compiler: MS Visual Studio 2022
* Author: Jack Johnston
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A22, A32.
* Date: Jul 01 2022
* Professor: Paulo Sousa
* Purpose: This file contains all functionalities from Scanner.
* Function list: (...).
************************************************************
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>  
#include <ctype.h>  
#include <stdlib.h>  
#include <string.h> 
#include <limits.h> 
#include <float.h>  


#include <assert.h> 


#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

extern ReaderPointer stringLiteralTable;	
extern ReaderPointer keywordLiteralTable;	

mgn_int line;								
extern mgn_int errorNumber;			

extern mgn_int stateType[];
extern mgn_ch* keywordTable[];
extern PTR_ACCFUN finalStateTable[];
extern mgn_int transitionTable[][TABLE_COLUMNS];

static ReaderPointer lexemeBuffer;			
static ReaderPointer sourceBuffer;			


mgn_int startScanner(ReaderPointer psc_buf) {
	readerRecover(psc_buf);
	readerClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

Token tokenizer(void) {
	Token currentToken = {0};
	LanguageAttributes indentation = { 0 };
	mgn_ch c;
	mgn_int state = 0;		
	mgn_int lexStart;		
	mgn_int lexEnd;		
	mgn_ch getChar;
	mgn_int lexLength;		
	mgn_int i;				
	mgn_ch newc;		
	mgn_ch logicalBufferArray[4] = {'0'};
	mgn_ch* sPointer;
	mgn_ch temp = ' ';
	
	while (1) {
		c = readerGetChar(sourceBuffer);
		switch (c) {
		case ' ':
		
		case '\t':
			indentation.indentationCurrentPos++;
			
		case '\f':
		
			
		
		case '\n':
			line++;
			break;
		

		case ':':
			currentToken.code = COLON_T;
			return currentToken;
	
		case '+':
			currentToken.code = ART_OP_T;
			currentToken.attribute.arithmeticOperator = OP_ADD;
			return currentToken;

		case '-':
			currentToken.code = ART_OP_T;
			currentToken.attribute.arithmeticOperator = OP_SUB;
			return currentToken;

		case '*':
			currentToken.code = ART_OP_T;
			currentToken.attribute.arithmeticOperator = OP_MUL;
			return currentToken;

		case '/':
			currentToken.code = ART_OP_T;
			currentToken.attribute.arithmeticOperator = OP_DIV;
			return currentToken;


		case '<':
			currentToken.code = SCC_OP_T;
			currentToken.attribute.relationalOperator = OP_LT;
			return currentToken;

		case '>':
			currentToken.code = REL_OP_T;
			currentToken.attribute.relationalOperator = OP_GT;
			return currentToken;

		case '=':
			temp = readerGetChar(sourceBuffer);

			if (temp == '=') {
				currentToken.code = EQUALITY_T;
				currentToken.attribute.relationalOperator = OP_EQ;
				return currentToken;
			}
			else {
				readerRetract(sourceBuffer);
				currentToken.code = ASS_OP_T;
				currentToken.attribute.relationalOperator = OP_EQ;
				return currentToken;


			}
		case '.':
			currentToken.code = DOT_T;
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

		case ',':
			currentToken.code = COM_T;
			return currentToken;

		case '[':
			i = 0;
			do {
				c = readerGetChar(sourceBuffer);
				if (c != 'A' || c != 'O' || c != 'N') {
					lexemeBuffer->position.wrte++;
					currentToken.attribute.logicalOperator = ERR_T;
					currentToken.code = ERR_T;
					strcpy(currentToken.attribute.errLexeme, "[");
				}
				logicalBufferArray[i] = c;
				i++;
			} while (c != ']' && c != CHARSEOF0 && c != CHARSEOF255);

			sPointer = strstr(logicalBufferArray, "AND]");

			if (sPointer != NULL)
			{
				currentToken.code = LOG_OP_T;
				currentToken.attribute.logicalOperator = OP_AND;
				return currentToken;
			}
			sPointer = strstr(logicalBufferArray, "OR]");

			if (sPointer != NULL)
			{
				currentToken.code = LOG_OP_T;
				currentToken.attribute.logicalOperator = OP_OR;
				return currentToken;
			}

			sPointer = strstr(logicalBufferArray, "NOT]");

			if (sPointer != NULL)
			{
				currentToken.code = LOG_OP_T;
				currentToken.attribute.logicalOperator = OP_NOT;
				return currentToken;
			}

			return currentToken;
			lexemeBuffer->position.wrte--;

			break;

		case ';':
			currentToken.code = EOS_T;
			return currentToken;

		case CHARSEOF0:
			currentToken.code = SEOF_T;
			currentToken.attribute.seofType = SEOF_0;
			return currentToken;

		case SEOF_255:
			currentToken.attribute.seofType = SEOF_255;
			currentToken.code = SEOF_T;
			return currentToken;

		case '#':
			newc = readerGetChar(sourceBuffer);
			do {
				c = readerGetChar(sourceBuffer);
				if (c == CHARSEOF0 || c == CHARSEOF255) {
					readerRetract(sourceBuffer);
				}
				else if (c == '\n') {
					line++;
				}
			} while (c != '#' && c != CHARSEOF0 && c != CHARSEOF255);
			break;

		default: 
			state = nextState(state, c);
			lexStart = readerGetPosRead(sourceBuffer) - 1;
			readerSetMark(sourceBuffer, lexStart);
			int pos = 0;
			while (stateType[state] == NOFS) {
				c = readerGetChar(sourceBuffer);
				state = nextState(state, c);
				pos++;
			}
			if (stateType[state] == FSWR)
				readerRetract(sourceBuffer);
			lexEnd = readerGetPosRead(sourceBuffer);
			lexLength = lexEnd - lexStart;
			lexemeBuffer = readerCreate((mgn_int)lexLength + 2, 0, MODE_FIXED);
			if (!lexemeBuffer) {
				fprintf(stderr, "Scanner error: Can not create buffer\n");
				exit(1);
			}
			readerRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++)
				readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
			readerAddChar(lexemeBuffer, READER_TERMINATOR);
			currentToken = (*finalStateTable[state])(readerGetContent(lexemeBuffer, 0));
			readerRestore(lexemeBuffer);
			return currentToken;
		}

	}

}

mgn_int nextState(mgn_int state, mgn_ch c) {
	mgn_int col;
	mgn_int next;
	col = nextClass(c);
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != FS);
	if (DEBUG)
		if (next == FS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}



mgn_int nextClass(mgn_ch c) {

	mgn_int val = -1;
	switch (c) {
		
		case '0':
			val = 1;
			break;

		case '.':
			val = 3;
			break;

	
		case '"':
			val = 5;
			break;

		case '(':
			val = 8;
			break;
		
		case SEOF_0:
			val = 8;
			break;
		
		case SEOF_255:
			val = 8;
			break;
		default:
			if (isalpha(c))
				val = 0;
			else if (isdigit(c) && c != '0')
				val = 2;
			else
				val = 7;
	}

		return val;
	}

Token funcSL(mgn_ch lexeme[]) {

	Token currentToken = { 0 };
	mgn_int i = 0, len = (mgn_int)strlen(lexeme);
	currentToken.code = STR_T;

	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);

	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == '\n')
			line++;
		
		if (!readerAddChar(stringLiteralTable, lexeme[i])) {
			currentToken.code = RTE_T;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
	}
	if (!readerAddChar(stringLiteralTable, CHARSEOF0)) {
		currentToken.code = RTE_T;
		strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
		errorNumber = RTE_CODE;
		return currentToken;
	}
	

	return currentToken;
}


Token funcKEY(mgn_ch lexeme[]) {
	Token currentToken = { 0 };
	mgn_int kwindex = -1, j = 0;
	for (j = 0; j < KWT_SIZE; j++)
		if (!strcmp(lexeme, &keywordTable[j][0]))
		kwindex = j;
	if (kwindex != -1) {
		
		currentToken.attribute.keywordIndex = j;
		currentToken.code = KW_T;
		currentToken.attribute.codeType = kwindex;
		readerAddChar(keywordLiteralTable, j);

	}
	else {
		currentToken.code = VAR_T;
		strcpy(currentToken.attribute.idLexeme, lexeme);

		if (strlen(lexeme) > VID_LEN) {
			for (mgn_int i = 0; i < VID_LEN; i++) {
				currentToken.attribute.idLexeme[i] = lexeme[i];
			}
			currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
		}

		else {
			for (mgn_int i = 0; i < VID_LEN; i++) {
				currentToken.attribute.idLexeme[i] = lexeme[i];
			}

		}
	}

	
	return currentToken;
}



Token funcSID(mgn_ch lexeme[]) {
	Token currentToken = { 0 };
	mgn_int i;
	size_t length = strlen(lexeme);
	mgn_ch lastch = lexeme[length - 1];


	switch (lastch) {




		if (length > VID_LEN) {
			for (i = 0; i < VID_LEN - 1; i++)
				currentToken.attribute.idLexeme[i] = lexeme[i];
			currentToken.attribute.idLexeme[VID_LEN - 1] = '$';
			currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF255;
			return currentToken;
		}

		strcpy(currentToken.attribute.idLexeme, lexeme);

		return currentToken;
	}
}


Token funcIL(mgn_ch lexeme[]) {
	Token currentToken = { 0 }; 
	int i; 
	mgn_long tlong; 
	tlong = atol(lexeme);

	if (tlong > SHRT_MAX || tlong < SHRT_MIN) {
		currentToken.code = ERR_T;
		if (strlen(lexeme) > ERR_LEN) {
			for (i = 0; i < ERR_LEN - 3; i++)
				currentToken.attribute.errLexeme[i] = lexeme[i];
			strcat(currentToken.attribute.errLexeme, "...");
		}
		else
			strcpy(currentToken.attribute.errLexeme, lexeme);
			currentToken.code = ERR_T;
			currentToken = (*finalStateTable[ESNR])(lexeme);
			return currentToken;
	}

	currentToken.code = INL_T;
	currentToken.attribute.intValue = (short)tlong;

	return currentToken;
}

Token funcFP(mgn_ch lexeme[]) {
	Token currentToken = { 0 };
	int i;
	double fpl; 
	mgn_ch* *endChar;
	fpl = strtod(lexeme, endChar);

	if (fpl > FLT_MAX || (fpl < FLT_MIN)) {
		currentToken.code = ERR_T;

		if (strlen(lexeme) > ERR_LEN) {
			for (i = 0; i < ERR_LEN - 3; i++) {
				currentToken.attribute.errLexeme[i] = lexeme[i];
				strcat(currentToken.attribute.errLexeme, "...");
			
			}
			
		}
		else
			strcpy(currentToken.attribute.errLexeme, lexeme);

			return currentToken;
	}

	currentToken.attribute.errLexeme[ERR_LEN - 1] = CHARSEOF0;
	currentToken.code = FP_T;
	currentToken.attribute.floatValue = (mgn_float)fpl;

	return currentToken;
}

Token funcErr(mgn_ch lexeme[]) {
	Token currentToken = { 0 };
	mgn_int i, len = (mgn_int)strlen(lexeme);								

	
	if (len > ERR_LEN) {
		for (i = 0; i < ERR_LEN - 3; i++)
			strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
			currentToken.attribute.errLexeme[ERR_LEN - 3] = CHARSEOF0;
			strcat(currentToken.attribute.errLexeme, "...");
	}
	
	else
		strcpy(currentToken.attribute.errLexeme, lexeme);

	for (i = 0; i < strlen(lexeme); i++) {
		if (lexeme[i] == '\n')
			line++;
	}

	currentToken.code = ERR_T;

	return currentToken;
}

mgn_void printToken(Token t) {
	extern char* kw_table[]; 
	switch (t.code) {
	case RTE_T:
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
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
	case MNID_T:
		printf("MNID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case  STR_T:
		printf("STR_T\t\t%d\t ", (mgn_int)t.attribute.codeType);
		printf("%s\n", readerGetContent(stringLiteralTable, (mgn_int)t.attribute.codeType));
		break;
	case  LPR_T:
		printf("LPR_T\n");
		break;
	case  RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);

		break;
	case COM_T:
		printf("COM_T\n");
		break;
	case EOS_T:
		printf("EOS_T\n");
		break;
	case  AVID_T:
		printf("AVID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case  SVID_T:
		printf("SVID_T\t\ t%s\n", t.attribute.idLexeme);
		break;
	case  FP_T:
		printf("FP_T\t\t%f\n", t.attribute.floatValue);
		break;
	case  INL_T:
		printf("INL_T\t\t%d\n", t.attribute.intValue);
		break;
	case  SCC_OP_T:
		printf("SCC_OP_T\n");
		break;
	case  ASS_OP_T:
		printf("ASS_OP_T\n");
		break;
	case  ART_OP_T:
		printf("ART_OP_T\t%d\n", t.attribute.intValue);
		break;
	case  REL_OP_T:
		printf("REL_OP_T\t%d\n", t.attribute.intValue);
		break;
	case  LOG_OP_T:
		printf("LOG_OP_T\t%d\n", t.attribute.intValue);
		break;
	case  COLON_T:
		printf("COLON_T\t \n");
		break;
	case  VAR_T:
		printf("VAR_T\t \t%s\n", t.attribute.idLexeme);
		break;
	case  EQUALITY_T:
		printf("EQUALITY_T\t\n");
		break;
	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
	}
}


