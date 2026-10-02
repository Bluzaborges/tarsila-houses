#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <SDL2/SDL.h>
#include "Shape.h"
#include "Primitives.h"

#include <iterator>
#include <list>

class Triangle : public Shape {
private:
    std::list<Point> points;
    Uint32 color;

public:
    Triangle(const std::list<Point>& pts,
             Uint32 color)
           : points(pts),
             color(color) {}

    void draw(SDL_Surface* surface) override {
        if (!surface || points.size() != 3)
            return;

        Primitives::scanlineFill(surface, points, color);

        auto it = points.begin();
        for (int i = 0; i < 3; i++) {
            auto p1 = *it;
            auto p2 = *(std::next(it) == points.end() ? points.begin() : std::next(it));
            Primitives::drawLine(surface, p1.x, p1.y, p2.x, p2.y, color);
            ++it;
        }
    }
};

#endif
