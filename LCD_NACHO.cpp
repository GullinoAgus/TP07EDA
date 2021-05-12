#include "lcd.h"

lcd::lcd()
{
	errorinit = crear_lcd();
	for (int i = 0; i < FILAS ; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			matrizlcd[i][j] = ' ';
		}
	}
	cursor.column = 0;
	cursor.row = 0;
	escribir();
}

lcd::~lcd()
{

}
bool lcd::lcdInitOk()
{
	if (errorinit == ERROR)
	{
		return false;
	}
	else
	{
		return true;
	}
}
 bool lcd::lcdClear()
{
	
	 for (int i = 0;i < FILAS ; i++)
	 {
		 for (int j = 0; j < COLS; j++)
		 {
			 matrizlcd[i][j] = ' ';
		 }
	 }
	 cursor.column = 0;
	 cursor.row = 0;
	 escribir();
	 return 0;
}

 bool lcd:: lcdClearToEOL()
{
	 int flag = 0;
	 int x = cursor.column;
	
	 while ( (flag == 0))
	 {
		 
		 matrizlcd[cursor.row][x] = ' ';
		
		 if (( x == COLS - 1) )
		 {
			 flag = 1;
		 }
		 x++;

	 }
	 escribir();

	 return 1;
}

 basicLCD& lcd:: operator<<(const unsigned char c)
 {
	 
	matrizlcd[cursor.row][cursor.column] = c;
	lcdMoveCursorRight();
	escribir();
		 
 

	return *this;
}
 basicLCD& lcd:: operator<<(const unsigned char* c)
{
	 int sizestring = 0;
	 for (; c[sizestring] != '\0'; sizestring++)
	 {
		 
	 }
	 int espacios = (COLS - cursor.column) + (COLS * !cursor.row);
	 
	 if (espacios >= sizestring)
	 {
		 
		 int i = 0;
		 while ((c[i] != 0) )
		 {

			 *this << c[i];
			 i++;
		 }
	 }
	 else
	 {
		 for (int i = (sizestring - espacios) ; i < sizestring ; i++)
		 {
			 *this << c[i];
		 }
	 }
	 return *this;
}
 bool lcd::lcdMoveCursorUp()
{
	 if (cursor.row == 1)
	 {
		 cursor.row--;
		 escribir();
		 return true;
	 }
	 else
	 {
		 return false;
	 }
	 return 0;
	 
	 return 0;
 
}


 bool lcd::lcdMoveCursorDown()
{
	 if (cursor.row == 0)
	 {
		 cursor.row++;
		 escribir();
		 return true;
	 }
	 else
	 {
		 return false;
	 }
	 return 0;
	 
	
}
 bool lcd::lcdMoveCursorRight()
{
	 if (cursor.column < COLS-1 )
	 {
		 cursor.column++;
		 escribir();
		 
		 return true;
	 }
	 if ((cursor.column == COLS-1) && (cursor.row == 0))
	 {
		 cursor.column = 0;
		 cursor.row = 1;
		 escribir();
	 }
	 else
	 {
		 return false;
	 }
	 return 0;
	 
}
 bool lcd::lcdMoveCursorLeft()
{
	 if (cursor.column > 0)
	 {
		 cursor.column--;
		 escribir();
		 return true;
	 }
	 else if ( (cursor.column == 0) && (cursor.row == 1) )
	 {
		 cursor.column = COLS - 1;
		 cursor.row = 0;
		 escribir();
		 return true;
	 }
	 else
	 {
		 return false;
	 }
	 
	 
}
 bool  lcd::lcdSetCursorPosition(const cursorPosition pos)
{
	 if ((pos.column < COLS) && (pos.row < FILAS))
	 {
		 cursor.column = pos.column;
		 cursor.row = pos.row;
		 escribir();
		 return true;
	 }
	 else
	 {
		 return false;
	 }
	 
}
 cursorPosition lcd::lcdGetCursorPosition()
{
	 return cursor;
}



 void lcd::escribir()
 {

	 al_clear_to_color(al_map_rgb(0, 0, 0));

		 
		 
		 
	 for (int i = 0; i < FILAS ; i++)
	 {
		 for (int j = 0; j < COLS ; j++)
		 {
			 if (i == 1)
			 {
				 al_draw_textf(font, al_map_rgb(0, 255, 0), (j * SIZECARACT), (i * SIZECARACT) - 50 + SPACEY, 0, "%c", matrizlcd[i][j]);
			 }
			 else
			 {
				 al_draw_textf(font, al_map_rgb(0, 255, 0), (j * SIZECARACT), (i * SIZECARACT) - 50, 0, "%c", matrizlcd[i][j]);
			 }
		 }
	 }

		 al_draw_line(cursor.column * SIZECARACT, ((cursor.row + 1) * (SIZECARACT)) + (SPACEY*cursor.row) , ((cursor.column + 1) * (SIZECARACT)) - SPACEX, ((cursor.row + 1) * (SIZECARACT)) + (SPACEY * cursor.row),
			 al_map_rgb(255, 0, 0), 5);


	 al_flip_display();
 }
 int lcd::crear_lcd()
 {
	 if (!(display = al_create_display(W_PIXELS, H_PIXELS))) {
		 cout << "failed to create display!" << endl;
		 return -1;
	 }
	 font = al_load_ttf_font("fuente.ttf", 50, 0);
	 if (!font) {
		 fprintf(stderr, "Could not load 'fuente.ttf'.\n");
		 return -1;
	 }

	 al_clear_to_color(al_map_rgb(0, 0, 0));

	 al_flip_display();
	 return 1;
 }
