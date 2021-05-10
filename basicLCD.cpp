#include "basicLCD.h"

basicLCD::basicLCD() {

}

basicLCD::~basicLCD() {

}

// error lcd
lcdError::lcdError(std::string errorname_, std::string errordescription_, unsigned long errorcode_) : errorcode(errorcode_),
																									errordesc(errordescription_),
																									errorname(errorname_)
{

}
std::string lcdError::getErrorName() {
	return errorname;
}
std::string lcdError::getErrorDescription() {
	return errordesc;
}
unsigned long lcdError::getErrorCode() {
	return errorcode;
}