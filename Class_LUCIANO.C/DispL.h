/***************************************************************************//**
@file     +DispL.h+
@brief    ++
@author   ++
******************************************************************************/

#ifndef _DISPL_H_
#define _DISPL_H_

/*******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include "../basicLCD.h"

#include <iostream> 
#include <string>

/*******************************************************************************
* CONSTANT AND MACRO DEFINITIONS USING #DEFINE
******************************************************************************/



/*******************************************************************************
* ENUMERATIONS AND STRUCTURES AND TYPEDEFS
******************************************************************************/



/*******************************************************************************
* CLASS
******************************************************************************/
class DispL : public basicLCD {
public:
	DispL();

	~DispL();

	virtual bool lcdInitOk();

	virtual lcdError& lcdGetError();

	virtual bool lcdClear();

	virtual bool lcdClearToEOL();

	virtual basicLCD& operator<<(const unsigned char c);

	virtual basicLCD& operator<<(const unsigned char* c);

	virtual bool lcdMoveCursorUp();

	virtual bool lcdMoveCursorDown();

	virtual bool lcdMoveCursorRight();

	virtual bool lcdMoveCursorLeft();

	virtual bool lcdSetCursorPosition(const cursorPosition pos);

	virtual cursorPosition lcdGetCursorPosition();


private:

	cursorPosition curr;

	lcdError err;
	char msg_buffer[MAX_ROW + 1][MAX_COL + 1];
	void updateDisplay();
	void clearLCD();
	void clearCursor2EOL();


};

/*******************************************************************************
******************************************************************************/

#endif // _DISPL_H_