#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <SDL2/SDL.h>
#include "Shape.h"
#include "Primitives.h"
#include "Point.h"

#include <iterator>
#include <list>

class Rectangle : public Shape {
private:
    std::list<Point> points;
    Uint32 color;

public:
    Rectangle(const std::list<Point>& pts,
              Uint32 color)
            : points(pts),
              color(color) {}

    void draw(SDL_Surface* surface) override {
        if (!surface || points.size() != 4)
            return;

        Primitives::scanlineFill(surface, points, color);

        auto it = points.begin();
        for (int i = 0; i < 4; i++) {
            auto p1 = *it;
            auto p2 = *(std::next(it) == points.end() ? points.begin() : std::next(it));
            Primitives::drawLine(surface, p1.x, p1.y, p2.x, p2.y, color);
            ++it;
        }
    }
};

#endif
