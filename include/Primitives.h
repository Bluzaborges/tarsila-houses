#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <SDL2/SDL.h>
#include "Point.h"

#include <list>

class Primitives {
public:
    static void setPixel(SDL_Surface* surface, int x, int y, Uint32 color);

    static Uint32 getPixel(SDL_Surface* surface, int x, int y);

    static void drawLine(SDL_Surface* surface, int x1, int y1, int x2, int y2, Uint32 color);

    static void scanlineFill(SDL_Surface* surface, const std::list<Point>& points, Uint32 color);

    static float toRadians(float degrees);

    static float distance(const Point& p1, const Point& p2);

    static float convertX(float x, int screenWidth, float worldWidth);

    static float convertY(float y, int screenHeight, float worldHeight);

    static void convertPoints(std::list<Point>& points, int screenWidth, int screenHeight, float worldWidth, float worldHeight);

    static void translatePolygon(std::list<Point>& points, float tx, float ty);

    static void scalePolygon(std::list<Point>& points, float sx, float sy, const Point& pivot);

    static void rotatePolygon(std::list<Point>& points, float angle, const Point& pivot);
};

#endif
