#pragma once

#include "synth.h"
#include <SDL3/SDL.h>

class Instrument {
    SDL_Window* window;
    Synth* synth;
    SDL_Renderer* renderer;

public:
    Instrument(SDL_Window*, Synth*);
    ~Instrument();

    void render();
    void handleInput(SDL_Event&);
};
