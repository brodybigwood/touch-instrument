#include "instrument.h"

Instrument::Instrument(SDL_Window* window, Synth* synth) :
    window(window), synth(synth)
{
    renderer = SDL_CreateRenderer(window, nullptr);
}

Instrument::~Instrument() {
    SDL_DestroyRenderer(renderer);
}

void Instrument::render() {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void Instrument::handleInput(SDL_Event& e) {

}
