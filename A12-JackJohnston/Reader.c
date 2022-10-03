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
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Sep 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */


#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

/*
***********************************************************
* Function name: readerCreate
* Purpose: Creates the buffer reader according to capacity, increment
	factor and operational mode ('f', 'a', 'm')
* Author: Svillen Ranev / Paulo Sousa
* History/Versions: S22
* Called functions: calloc(), malloc()
* Parameters:
*   size = initial capacity
*   increment = increment factor
*   mode = operational mode
* Return value: bPointer (pointer to reader)
* Algorithm: Allocation of memory according to inicial (default) values.
* TODO ......................................................
*	- Adjust datatypes for your LANGUAGE.
*   - Use defensive programming
*	- Check boundary conditions
*	- Check flags.
*************************************************************
*/

ReaderPointer readerCreate(mgn_int size, mgn_int increment, mgn_int mode) {
	ReaderPointer readerPointer;
	/* defensive programming to check if the parameters are correct  */
if(increment == 0){
increment = READER_DEFAULT_INCREMENT;
mode = MODE_FIXED;
}

if(size == 0){
	size = READER_DEFAULT_SIZE;
}
	if((size <= 0 ) && (increment<= 0)){
	return NULL;
}


if(size <= 0 || size > READER_MAX_SIZE){
increment = READER_DEFAULT_INCREMENT;
size = READER_DEFAULT_SIZE;
}


if(mode != 'f' && mode != 'a' && mode != 'm'){


}
if(mode != 'f' && mode != 'a' && mode != 'm'){ //checks the mode whether if the one specified is available

	return NULL;
}
/* creates space for the pointer */
	readerPointer = (ReaderPointer)calloc(1, sizeof(BufferReader));
	readerPointer->content = (mgn_ch*)malloc(size);
	
	switch(mode){

	case 'f':
			readerPointer->mode = 'f';
	break;

	case 'a':
			readerPointer->mode = 'a';
	break;

	case 'm':
			readerPointer->mode = 'm';
	break;

	default:
	return NULL;
	break;
}
	
	if(!readerPointer){
		return NULL;
	}
	
for(int z = 0; z <= 128; z++){ //initializes the histograms with zeroes that can be used later on
    readerPointer->histogram[z] = 0;      
}
	
	
	readerPointer->size = size;
	readerPointer->increment = increment;
	readerPointer->flags = READER_DEFAULT_FLAG;
	return readerPointer;
	
}


/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

ReaderPointer readerAddChar(ReaderPointer const readerPointer, mgn_ch ch) {
	mgn_ch* tempReader = NULL;
	mgn_int newSize = 0;
	
	if(!ch && !readerPointer){
		return NULL;
	}

	if(isascii(ch)){
	readerPointer->histogram[ch] += 1; // updating the histogram by using the character ascii number as the array number and added by one
	}
/* checks to see if the buffer is less or more than the size  */
	if (readerPointer->position.wrte * (mgn_int)sizeof(mgn_ch) < readerPointer->size) {
		readerPointer->content[readerPointer->position.wrte++] = ch;
		return readerPointer;
	} 
		switch (readerPointer->mode) {
		case MODE_FIXED:
			return NULL;

		case MODE_ADDIT: 
		newSize = readerPointer->size + readerPointer->increment; 
		if(newSize > 0 && newSize <= READER_MAX_SIZE){ 
			readerPointer->size = newSize; 
			}
		 else{ 
			readerPointer->flags = READER_FUL_FLAG; 
			return NULL; 
		 } 
		 break;

		case MODE_MULTI:
			newSize = readerPointer->size * readerPointer->increment;
			
			if(newSize > 0 && newSize <= READER_MAX_SIZE){
				readerPointer->size = newSize;
			}	
			else{
				readerPointer->flags = READER_FUL_FLAG;
				return NULL;
			}
			break;
		default:
			return NULL;
		}
/* increments the position and sets the new size for the reader buffer          */
	readerPointer->flags &= READER_REL_FLAG;
	readerPointer->size = newSize;
	tempReader = (char*)realloc(readerPointer->content, newSize); //called a realloc as mentioned in the assignment by a temporary variable
	readerPointer->content = tempReader;
	readerPointer->content[readerPointer->position.wrte++] = ch;

	return readerPointer;
}

/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mgn_bool readerClear(ReaderPointer const readerPointer) {
	if(!readerPointer){
		return mgn_false;
	}
	/*  resets the content of the reader */
	readerPointer->flags &= READER_DEFAULT_FLAG; 
	readerPointer->position.mark = 0;
	readerPointer->position.read = 0;
	readerPointer->position.wrte = 0;
	return mgn_true;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mgn_bool readerFree(ReaderPointer const readerPointer) {
	/*  frees the readerPointer          */
	free(readerPointer); 
	return mgn_true;
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mgn_bool readerIsFull(ReaderPointer const readerPointer) {
	if(!readerPointer){
		return mgn_false;
	}
	/*    checks if the size is equal or is more than the allocated space        */
	if(readerPointer->position.wrte * (mgn_int)sizeof(mgn_ch) >= readerPointer->size)
	{
		readerPointer->flags &= READER_FUL_FLAG; 
		return mgn_true;
	}
	return mgn_false;
}


/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

/* checks if the reader is empty           */
mgn_bool readerIsEmpty(ReaderPointer const readerPointer) {
	if(!readerPointer){
		return mgn_false;
	}

	if(readerPointer->position.wrte * (mgn_int)sizeof(mgn_ch) == 0){

	  readerPointer->flags &= READER_EMP_FLAG;

	return mgn_true;
	}

	return mgn_false;
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mgn_bool readerSetMark(ReaderPointer const readerPointer, mgn_int mark) {
	
	if (!readerPointer || mark < 0 || mark > readerPointer->position.wrte){
		return mgn_false;
	}
	readerPointer->position.mark = mark;
	return mgn_true;
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mgn_int readerPrint(ReaderPointer const readerPointer) {
	if(!readerPointer){
		return mgn_false;
	}
	mgn_int cont = 0;
	mgn_ch c;
	c = readerGetChar(readerPointer);
	int flag = readerPointer->flags &= READER_FUL_FLAG;
/*  checks if the char is an ascii and if the flags indicate that it is full  */
	if(isascii(c) && flag != READER_FUL_FLAG){ 

	while (cont < readerPointer->position.wrte) {
		cont++;
		printf("%c", c);
		c = readerGetChar(readerPointer);
	}
	return cont;
	}
	cont++;
	return cont;
}
	
	


/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mgn_int readerLoad(ReaderPointer const readerPointer, FILE* const fileDescriptor) {
	mgn_int size = 0;
	mgn_ch c;
	if(!readerPointer || fileDescriptor == NULL){
		return mgn_false;
	}
	c = (mgn_ch)fgetc(fileDescriptor);
	while (!feof(fileDescriptor)) {
		if (!readerAddChar(readerPointer, c)) {
			ungetc(c, fileDescriptor);
			return READER_ERROR;
		}
		c = (char)fgetc(fileDescriptor);
		size++;
	}
	if(size <= 0){
		return mgn_false;
	}

	return size;
}


/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mgn_bool readerRecover(ReaderPointer const readerPointer) {
	if(!readerPointer){
		return mgn_false;
	}

	/* resets mark and read to zero           */
	readerPointer->position.mark = 0;
	readerPointer->position.read = 0;
	return mgn_true;
}


/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mgn_bool readerRetract(ReaderPointer const readerPointer) {
	if(!readerPointer){
		return mgn_false;
	}
	if(readerPointer->position.read > 0){
	readerPointer->position.read--;		//decrement the read value by one
	return mgn_true;
	}

	return mgn_false;			
}


/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mgn_bool readerRestore(ReaderPointer const readerPointer) {
	if(!readerPointer){
		return mgn_false;
	}
	/* sets read and mark equal to each other  */
	readerPointer->position.read = readerPointer->position.mark;
	return mgn_true;
}



/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mgn_ch readerGetChar(ReaderPointer const readerPointer) {
		if(!readerPointer){
		return mgn_false;
	}

	if(readerPointer->position.read == readerPointer->position.wrte){
		 readerPointer->flags &= READER_END_FLAG;
	}
	
	return readerPointer->content[readerPointer->position.read++]; //returns the char
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mgn_ch* readerGetContent(ReaderPointer const readerPointer, mgn_int pos) {
	if(!readerPointer || pos < 0){
		return mgn_false;
	} 
	char *pContent;
	pContent = &readerPointer->content[pos];

	return pContent;					//returns the value associated with the index location set by pos
}



/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mgn_int readerGetPosRead(ReaderPointer const readerPointer) {
		if(!readerPointer){
		return mgn_false;
	}
	return readerPointer->position.read; //returns the read position
}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mgn_int readerGetPosWrte(ReaderPointer const readerPointer) {	
	if(!readerPointer){
	return mgn_false;
	}
	
	return readerPointer->position.wrte; //return the wrte
}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mgn_int readerGetPosMark(ReaderPointer const readerPointer) {
if(!readerPointer){
		return mgn_false;
	}
	return readerPointer->position.mark; //returns the mark
}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mgn_int readerGetSize(ReaderPointer const readerPointer) {
	if(!readerPointer){
		return mgn_false;
	}
	return readerPointer->size; //returns the size
}

/*
***********************************************************
* Function name: readerGetInc
* Purpose: Returns the buffer increment.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The Buffer increment.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mgn_int readerGetInc(ReaderPointer const readerPointer) {
	if(!readerPointer){
		return mgn_false;
	}
	return readerPointer->increment; //returns the increment amount
}

/*
***********************************************************
* Function name: readerGetMode
* Purpose: Returns the operational mode
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Operational mode.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mgn_int readerGetMode(ReaderPointer const readerPointer) {
	if(!readerPointer){
		return mgn_false;
	}
	return readerPointer->mode; //returns the mode
}


/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
mgn_byte readerGetFlags(ReaderPointer const readerPointer) {
	if(!readerPointer){
		return mgn_false;
	}
	return readerPointer->flags; //returns flag
}


/*
***********************************************************
* Function name: readerShowStat
* Purpose: Returns the number of chars used.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars.
*************************************************************
*/
mgn_int readerShowStat(ReaderPointer const readerPointer) {
	if(!readerPointer){
		return mgn_false;
	}

	int differentChars = 0;
	
for(int z = 0; z < 127; z++){
	if(readerPointer->histogram[z] != 0){
		differentChars++; //counter used to count the numbers of unique chars
	}
}
	if(differentChars == 0){
		return 0;
	}

	return differentChars-1; //subtracted by 1 because \n is included in every sentence
}