/***************************************************************************//**
@file     +basicLCD.cpp+
@brief    +Funciones miembro no virtuales+
@author   +Grupo 2+
******************************************************************************/

/******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include "basicLCD.h"

using namespace std;
/******************************************************************************
* MEMBER FUNTIONS CLASS: LCDERROR
******************************************************************************/
lcdError::lcdError() {
	this->name = "No error";
	this->description = "No errors so far";
	this->code = NOERROR;
}

lcdError::lcdError(string name, string description, unsigned long code) {
	this->name = name;
	this->description = description;
	this->code = code;
}

string lcdError::getErrorName() {
	return name;
}

string lcdError::getErrorDescription() {
	return description;
}

unsigned long lcdError::getErrorCode() {
	return code;
}

/******************************************************************************
* MEMBER FUNTIONS CLASS: BASICLCD
******************************************************************************/
basicLCD::basicLCD() 
{
}

basicLCD::~basicLCD() 
{
}