#ifndef CIRCLE_H
#define CIRCLE_H

#include <SDL2/SDL.h>
#include <cmath>
#include "Shape.h"
#include "Primitives.h"

class CircleFilledAA : public Shape {
private:
    int xc, yc;
    int radius;
    Uint32 color;

public:
    CircleFilledAA(int xc,
                   int yc,
                   int radius,
                   Uint32 color)
                 : xc(xc),
                   yc(yc),
                   radius(radius),
                   color(color) {}

    void draw(SDL_Surface* surface) override {
        if (!surface)
            return;

        Uint8 r, g, b;
        SDL_GetRGB(color, surface->format, &r, &g, &b);

        for (int y = -radius - 1; y <= radius + 1; y++) {
            for (int x = -radius - 1; x <= radius + 1; x++) {
                float dist = std::sqrt(float(x*x + y*y));
                float coverage = radius + 0.5f - dist;

                if (coverage > 0) {
                    if (coverage > 1) coverage = 1.0f;

                    int px = xc + x;
                    int py = yc + y;

                    if (px < 0 || px >= surface->w || py < 0 || py >= surface->h)
                        continue;

                    Uint32 pixelColor = Primitives::getPixel(surface, xc + x, yc + y);

                    Uint8 pr, pg, pb;
                    SDL_GetRGB(pixelColor, surface->format, &pr, &pg, &pb);

                    Uint8 nr = (Uint8)(pr * (1 - coverage) + r * coverage);
                    Uint8 ng = (Uint8)(pg * (1 - coverage) + g * coverage);
                    Uint8 nb = (Uint8)(pb * (1 - coverage) + b * coverage);

                    Uint32 newColor = SDL_MapRGB(surface->format, nr, ng, nb);
                    Primitives::setPixel(surface, xc + x, yc + y, newColor);
                }
            }
        }
    }
};

#endif
