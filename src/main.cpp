#include <iostream>
#include "instrument.h"
#include "synth.h"
#include <emscripten/emscripten.h>
#include "window.h"

Instrument* inst;
bool running = true;

void loop() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) running = false;
        inst->handleInput(e);
    }
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

    auto synth = new JSSynth;
    inst = new Instrument(window, synth);

    emscripten_set_main_loop(loop, 0, 1);

    delete inst;
    delete synth;

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
