#pragma once
#ifndef _ALLEGRO_H
#define _ALLEGRO_H
#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>

#define W_PIXELS	800				// tamaño del display
#define H_PIXELS	200

//Funcion que le da la bienvenida al usuario por allegro
int init_allegro(ALLEGRO_DISPLAY*& display);

//Funcion que destruye todo allegro, no recibe nada y no devuelve nada
void destroy_allegro(ALLEGRO_DISPLAY*& display);

#endif

