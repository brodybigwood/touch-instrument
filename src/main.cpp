#include <iostream>
#include "instrument.h"
#include "synth.h"

#ifdef __EMSCRIPTEN__
    #include <emscripten/emscripten.h>
    #define SDLFLAGS SDL_INIT_VIDEO | SDL_INIT_EVENTS
#else
    #define SDLFLAGS SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO
#endif

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

    if (!SDL_Init(SDLFLAGS)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Touch Accordion",
        WINDOW_W, WINDOW_H,
        0
    );

    #ifdef __EMSCRIPTEN__
        auto synth = new JSSynth;
    #else
        auto synth = new CSynth;
    #endif

    inst = new Instrument(window, synth);

    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(loop, 0, 1);
    #else
        bool running = true;
        while (running) {
            Uint64 start = SDL_GetTicksNS();    
            loop();
            Uint64 elapsed = SDL_GetTicksNS() - start;
            if (elapsed < (1000000000 / FPS)) {
                SDL_DelayNS((1000000000 / FPS) - elapsed);
            }
        }
    #endif

    delete inst;
    delete synth;

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
