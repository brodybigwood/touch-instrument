#include "instrument.h"
#include "color.h"
#include <cmath>
#include <iostream>

Instrument::Instrument(SDL_Window* window, Synth* synth) :
    window(window), synth(synth)
{
    renderer = SDL_CreateRenderer(window, nullptr);
    makeMapping();
    SDL_SetRenderTarget(renderer, 0);
}

Instrument::~Instrument() {
    SDL_DestroyRenderer(renderer);
}

void Instrument::render() {
    if (!buttons) return;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer,  buttons, 0, 0);
    SDL_RenderPresent(renderer);
}

void Instrument::handleInput(SDL_Event& e) {
    switch (e.type) {
        case SDL_EVENT_FINGER_DOWN: {
            int x = static_cast<int>(e.tfinger.x * WINDOW_W);
            int y = static_cast<int>(e.tfinger.y * WINDOW_H);
            auto pitch = pitches[x][y];
            std::cout << "x,y: " << x << "," << y<< std::endl;
            finger f {
                .x = x,
                .y = y,
                .noteID = synth->noteOn(pitch)
            };
            fingers.push_back(f);
            break;
        }
        case SDL_EVENT_FINGER_UP: {
            int x = static_cast<int>(e.tfinger.x * WINDOW_W);
            int y = static_cast<int>(e.tfinger.y * WINDOW_H);

            auto dist2 = [](int x1,int y1,int x2,int y2){
                int dx = x1 - x2;
                int dy = y1 - y2;
                return dx*dx + dy*dy;
            };
            
            finger* closest = nullptr;
            int closest_index = -1;
            int index = 0;
            
            for (auto& f : fingers) {
                if (!closest ||
                    dist2(f.x,f.y,x,y) < dist2(closest->x,closest->y,x,y))
                {
                    closest = &f;
                    closest_index = index;
                }
                index++;
            }

            if (closest_index != -1) {
                synth->noteOff(closest->noteID);
                fingers.erase(fingers.begin() + closest_index);
            }

            break;
        }
        default:
            break;
    }
}

void Instrument::makeMapping() {
    SDL_SetRenderTarget(renderer, buttons);
    for(int x = 0; x < WINDOW_W; ++x)
        for(int y = 0; y < WINDOW_H; ++y)
            pitches[x][y] = -1;
            
    float column_pitch = 30;
    
    for (int column_x = 0; column_x < WINDOW_W; column_x += column_pixels) {
        float pitch = column_pitch;
        for (int y = WINDOW_H - 1; y > -1; y--) {
            pitch += y_pixel_interval;

            auto frequency = 440 * std::pow(2, (pitch - 69) / 12.0f);
            auto color = color_from_freq(frequency);
            
            for (int x = column_x; x < column_pixels + column_x && x < WINDOW_W; x++) {
                uint8_t alpha = 255 * std::sin(M_PI * (x - column_x) / column_pixels);
                volumes[x][y] = alpha;
                pitches[x][y] = pitch;
                
                Uint32 px = (color.r << 24) | (color.g << 16) | (color.b << 8) | alpha;
                colors[x][y] = px;
            }
        }

        column_pitch += column_interval;
    }

    SDL_Surface* surface = SDL_CreateSurface(WINDOW_W, WINDOW_H, SDL_PIXELFORMAT_RGBA8888);
    Uint32* pixels = (Uint32*)surface->pixels;
    int surface_row_size = surface->pitch / sizeof(Uint32);
    
    for (int y = 0; y < WINDOW_H; ++y) {
        for (int x = 0; x < WINDOW_W; ++x) {
            pixels[y * surface_row_size + x] = colors[x][y]; // convert [x][y] → row-major
        }
    }

    buttons = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
}
