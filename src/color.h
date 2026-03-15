#include <SDL3/SDL.h>
#include <math.h>
#include <iostream>

SDL_Color color_from_freq(double f) {
    double fMin = 400.0;
    double fMax = 800.0;

    while (f < fMin) f *= 2.0;
    while (f > fMax) f /= 2.0;

    double t = (std::log2(f / fMin)) / (std::log2(fMax / fMin));
    const double pi = M_PI;

    double R = std::pow(std::sin(pi * t + 0.0), 2);
    double G = std::pow(std::sin(pi * t + pi / 3.0), 2);
    double B = std::pow(std::sin(pi * t + 2.0 * pi / 3.0), 2);

    SDL_Color color;
    color.r = static_cast<Uint8>(R * 255.0);
    color.g = static_cast<Uint8>(G * 255.0);
    color.b = static_cast<Uint8>(B * 255.0);
    color.a = 255;

    return color;
}

