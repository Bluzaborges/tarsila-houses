#ifndef SUN_H
#define SUN_H

#include <SDL2/SDL.h>
#include "Config.h"
#include "Shape.h"
#include "Primitives.h"
#include "Point.h"
#include "Color.h"
#include "Circle.h"

#include <list>
#include <memory>
#include <cmath>
#include <string>

class Sun : public Shape {
private:
    float x;
    float y;
    float width;
    float height;
    float angle;
    Color color;

    const Config& config;

    std::list<std::unique_ptr<Shape>> shapes;

    void addCircle(float angle, Uint32 color)
    {
        Point center{0.5f, 0.5f};
        Point right{1.0f, 0.5f};

        std::list<Point> pts{center, right};

        Primitives::scalePolygon(pts, width, height, {0, 0});
        Primitives::rotatePolygon(pts, angle, {0, 0});
        Primitives::translatePolygon(pts, x, y);
        Primitives::convertPoints(pts,
                                  config.screenWidth,
                                  config.screenHeight,
                                  config.worldWidth,
                                  config.worldHeight);
        auto it = pts.begin();
        Point c = *it++;
        Point r = *it;

        int screenRadius = static_cast<int>(Primitives::distance(c, r));

        shapes.push_back(std::make_unique<CircleFilledAA>(
            c.x,
            c.y,
            screenRadius,
            color
        ));
    }

public:
    Sun(float x,
        float y,
        float width,
        float height,
        float angle,
        const std::string& colorName,
        const Config& config,
        SDL_Surface* surface)
      : x(x),
        y(y),
        width(width),
        height(height),
        angle(angle),
        color(Color::fromName(surface, colorName)),
        config(config),
        shapes()
    {
        if (width == 0.0f || height == 0.0f)
            return;

        addCircle(angle, color.getPrimary());
    }

    void draw(SDL_Surface* surface) override {
        if (!surface)
            return;

        for (auto& shape : shapes)
            shape->draw(surface);
    }
};

#endif
