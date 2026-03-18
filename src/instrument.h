#pragma once

#include "synth.h"
#include <SDL3/SDL.h>
#include <cmath>
#include "window.h"

struct finger {
    int x;
    int y;
    int noteID;
};

class Instrument {
    SDL_Window* window;
    Synth* synth;
    SDL_Renderer* renderer;
    SDL_Texture* buttons = nullptr;
    
    float pitches[WINDOW_W][WINDOW_H];
    float volumes[WINDOW_W][WINDOW_H];
    Uint32 colors[WINDOW_W][WINDOW_H];

    float ppi = WINDOW_W / 12.2062;
    float button_inches = 0.748;
    int button_pixels = button_inches * ppi;
    int column_pixels = button_pixels * std::cos(30 * M_PI / 180.0); 
    float column_interval = 3 * std::cos(30 * M_PI / 180.0);
    float y_pixel_interval = 1.0f / button_pixels; // buttons per pixel

    void makeMapping();

    std::vector<finger> fingers;
    int closestFinger(int, int);
public:
    Instrument(SDL_Window*, Synth*);
    ~Instrument();

    void render();
    void handleInput(SDL_Event&);
};
