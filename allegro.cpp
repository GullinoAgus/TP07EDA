#include "allegro.h"
#include<string>
#include<iostream>


using namespace std;


int init_allegro(ALLEGRO_DISPLAY *& display) {            // incio allegro

    if (!al_init()) {
        cout << "failed to initialize allegro!!" << endl;
        return -1;
    }
    else if (!al_init_font_addon()) {
        std::cout << "failed to init font addon!" << std::endl;
        return -1;
    }
    else if (!al_init_ttf_addon()) {
        std::cout << "failed to init ttf addon!" << std::endl;
        return -1;
    }
    else if (!(display = al_create_display(W_PIXELS, H_PIXELS))) {
        cout << "failed to create display!" << endl;
        return -1;
    }

    al_clear_to_color(al_map_rgb(0, 0, 0));

    return 0;
}

void destroy_allegro(ALLEGRO_DISPLAY*& display) {

    al_destroy_display(display);

}

//aldrawtext