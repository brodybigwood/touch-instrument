#include <iostream>
#include "instrument.h"
#include "synth.h"
#include <emscripten/emscripten.h>
#include "window.h"

Instrument* inst;

void loop() {
    inst->render();
}

const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS;

int main() {

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Touch Accordion",
        WINDOW_W, WINDOW_H,
        0
    );

    auto synth = new Synth;
    inst = new Instrument(window, synth);

/*
    bool running = true;
    SDL_Event e;

    while (running) {
        Uint32 frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = false;
            inst->handleInput(e);
        }

        inst->render();

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY) SDL_Delay(FRAME_DELAY - frameTime);
    }
*/
    
    emscripten_set_main_loop(loop, 0, 1);

    delete inst;
    delete synth;

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
