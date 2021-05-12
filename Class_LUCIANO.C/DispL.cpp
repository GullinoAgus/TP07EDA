/***************************************************************************//**
@file     +DispL.cpp+
@brief    ++
@author   ++
******************************************************************************/

/*******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include "DispL.h"


#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <cstring>

using namespace std;

/*******************************************************************************
* CONSTANT AND MACRO DEFINITIONS USING #DEFINE
******************************************************************************/
#define LCD_FILE	"Class_LUCIANO.C/resources/background.jpg"
#define LCD_FONT_FILE "Class_LUCIANO.C/resources/Blanka-Regular.TTF"

#define D_WIDTH     500
#define D_HEIGHT    250

#define FONT_SIZE    30

ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_FONT* fontL = NULL;
ALLEGRO_BITMAP* LCD_image = NULL;

/*******************************************************************************
* ENUMERATIONS AND STRUCTURES AND TYPEDEFS
******************************************************************************/


/*******************************************************************************
* FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
******************************************************************************/



/******************************************************************************
* MEMBER FUNTIONS
******************************************************************************/
DispL::DispL() {
	curr.row = 0;
	curr.column = 0;
	clearLCD();
}

DispL::~DispL() {
	if(LCD_image!=nullptr)
		al_destroy_bitmap(LCD_image);
	if (display != nullptr)
		al_destroy_display(display);
	if (fontL != nullptr)
		al_destroy_font(fontL);
}

//Inicializa el display
bool DispL::lcdInitOk() {
	display = al_create_display(D_WIDTH, D_HEIGHT);
	if (display) {
		LCD_image = al_load_bitmap(LCD_FILE);
		if (LCD_image) {
			fontL = al_load_ttf_font(LCD_FONT_FILE, FONT_SIZE, 0);
			if (fontL) {
				updateDisplay();
				return true;
			}else{
				err = lcdError("Font", "ERROR: failed to load FONT!", FontError);
				cout << "ERROR: failed to load font!" << endl;
			}
			al_destroy_bitmap(LCD_image);
		}else{
			err = lcdError("Bitmap", "ERROR: failed to load Bitmap!", ImgError);
			cout << "ERROR: failed to load Bitmap!" << endl;
		}
		al_destroy_display(display);
	}else {
		err = lcdError("Display", "ERROR: failed to load display!", DispError);
		cout << "ERROR: failed to load display!" << endl;
	}
	return false;
}

lcdError& DispL::lcdGetError() {
	return err;
}

bool DispL::lcdClear() {
	clearLCD();
	curr.row = 0;
	curr.column = 0;
	updateDisplay();
	return true;
}

bool DispL::lcdClearToEOL() {
	clearCursor2EOL();
	updateDisplay();
	return true;
}

basicLCD& DispL::operator<<(const unsigned char c) {
	msg_buffer[curr.row][curr.column] = c;
	if (curr.column < MAX_COL) {
		curr.column++;
	}
	else if (curr.column == MAX_COL && curr.row == MAX_ROW) {
		//Do nothing
	}
	else {
		curr.row++;
		curr.column = 0;
	}
	updateDisplay();
	return *this;
}

basicLCD& DispL::operator<<(const unsigned char* c) {	
	int i_start = strlen((char*)c) - 33;
	if (i_start < 0) {
		i_start = 0;
	}
	for (int i = i_start; c[i] != '\0'; i++) {
		msg_buffer[curr.row][curr.column] = c[i];
		if (curr.column < MAX_COL) {
			curr.column++;
		}
		else if (curr.column == MAX_COL && curr.row == MAX_ROW) {
			break;
		}
		else {
			curr.row++;
			curr.column = 0;
		}
	}
	updateDisplay();
	return *this;
}

bool DispL::lcdMoveCursorUp() {
	if (curr.row == 0)
		return false;
	curr.row--;
	updateDisplay();
	return true;
}

bool DispL::lcdMoveCursorDown() {
	if (curr.row == MAX_ROW)
		return false;
	curr.row++;
	updateDisplay();
	return true;
}

bool DispL::lcdMoveCursorRight() {
	if (curr.column == MAX_COL)
		return false;
	curr.column++;
	updateDisplay();
	return true;
}

bool DispL::lcdMoveCursorLeft() {
	if (curr.column == 0)
		return false;
	curr.column--;
	updateDisplay();
	return true;
}

bool DispL::lcdSetCursorPosition(const cursorPosition pos) {
	if ((pos.column > MAX_COL) || (pos.row > MAX_ROW)|| (pos.column < 0) ||(pos.row < 0))
		return false;
	curr.column = pos.column;
	curr.row = pos.row;
	updateDisplay();
	return true;
}

cursorPosition DispL::lcdGetCursorPosition() {
	return curr;
}


/*******************************************************************************
*******************************************************************************
            LOCAL FUNCTION DEFINITIONS
*******************************************************************************
******************************************************************************/

void DispL::updateDisplay() {
	//TODO: poner un if que checkee que el prev_disp no sea NULL
	ALLEGRO_DISPLAY*prev_disp = al_get_current_display();
	al_set_target_backbuffer(display);
	al_draw_scaled_bitmap(LCD_image, 0, 0,al_get_bitmap_width(LCD_image), al_get_bitmap_height(LCD_image),0, 0, D_WIDTH, D_HEIGHT, 0);

	for (int i = 0; i <= MAX_ROW; i++) {
		for (int ii = 0; ii <= MAX_COL; ii++) { //Imprimo todos los caracteres en el display
			char aux[2];
			aux[0] = msg_buffer[i][ii];
			aux[1] = '\0';
			al_draw_text(fontL, al_map_rgb(200, 200, 0), 71 + (23.8 * ii), 90 + (40 * i), ALLEGRO_ALIGN_CENTER, aux);
		}
	}

	char curr_hilig[2];
	if (msg_buffer[curr.row][curr.column] == ' ' || msg_buffer[curr.row][curr.column] == 0) {
		curr_hilig[0] = '_';
	}
	else {
		curr_hilig[0] = msg_buffer[curr.row][curr.column];
	}
	curr_hilig[1] = '\0';
	al_draw_filled_rectangle(71 + (23.8 * curr.column)+12, 90 + (40 * curr.row)+6, 71+8 + (23.8 * curr.column)-20, 90+6 + (40 * curr.row)+30, al_map_rgb(200, 200, 0));
	al_draw_text(fontL, al_map_rgb(0, 0, 0), 71 + (23.8 * curr.column), 90 + (40 * curr.row), ALLEGRO_ALIGN_CENTER, curr_hilig); 
	

	al_flip_display();
	al_set_target_backbuffer(prev_disp);
}

void DispL::clearLCD() {
	for (int i = 0; i <= MAX_ROW; i++) {
		for (int ii = 0; ii <= MAX_COL; ii++) {
			msg_buffer[i][ii] = ' ';
		}
	}
}

void DispL::clearCursor2EOL() {
	for (int i = curr.column; i <= MAX_COL; i++) {
		msg_buffer[curr.row][i] = ' ';
	}
}


/******************************************************************************
* CALLBACKS
******************************************************************************/