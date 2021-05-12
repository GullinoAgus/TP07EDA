#pragma once
#ifndef PANALCD_H
#define PANALCD_H

#include "basicLCD.h"
#include "allegro.h"

#define FONTPATH "7SDD.ttf"

class PanaLCD :
    public basicLCD
{
public:
    // Inherited via basicLCD

    PanaLCD(int rows, int columns, int offsetX, int offsetY);
    ~PanaLCD();

    virtual bool lcdInitOk() override;
    virtual lcdError& lcdGetError() override;
    virtual bool lcdClear() override;
    virtual bool lcdClearToEOL() override;
    virtual basicLCD& operator<<(const unsigned char c) override;
    virtual basicLCD& operator<<(const char* c) override;
    virtual bool lcdMoveCursorUp() override;
    virtual bool lcdMoveCursorDown() override;
    virtual bool lcdMoveCursorRight() override;
    virtual bool lcdMoveCursorLeft() override;
    virtual bool lcdSetCursorPosition(const cursorPosition pos) override;
    virtual cursorPosition lcdGetCursorPosition() override;

private:

    bool redraw();

    ALLEGRO_FONT* font;
    ALLEGRO_DISPLAY* display;
    cursorPosition cursorPos;
    lcdError error;
    int rowQuant;
    int columnQuant;
    int screenOffsetX;
    int screenOffsetY;
    std::string lcdText;
    
};

#endif // !PANALCD_H
