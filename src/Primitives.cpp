#include "Primitives.h"
#include "Point.h"

#include <list>
#include <vector>
#include <cmath>
#include <algorithm>

// Set the color value of the pixel (x, y) on the surface
void Primitives::setPixel(SDL_Surface* surface,
                          int x,
                          int y,
                          Uint32 color) {
    if (!surface)
        return;

    if (x < 0 || y < 0 || x >= surface->w || y >= surface->h)
        return;

    Uint32* pixels = (Uint32*)surface->pixels;

    int pitch = surface->pitch / 4;

    pixels[y * pitch + x] = color;
}

// Get the color value of the pixel (x, y) from the surface
Uint32 Primitives::getPixel(SDL_Surface* surface,
                            int x,
                            int y) {
    int bytesPerPixel = surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bytesPerPixel;

    switch (bytesPerPixel) {
        case 1: return *p;
        case 2: return *(Uint16*)p;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4: return *(Uint32*)p;
        default: return 0;
    }
}

// Bresenham algorithm
void Primitives::drawLine(SDL_Surface* surface,
                          int x1,
                          int y1,
                          int x2,
                          int y2,
                          Uint32 color) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        setPixel(surface, x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

// Scanline-fill
void Primitives::scanlineFill(SDL_Surface* surface,
                              const std::list<Point>& points,
                              Uint32 color) {
    if (points.size() < 3)
        return;

    float minY = points.front().y;
    float maxY = points.front().y;

    for (const auto& p : points) {
        minY = std::min(minY, p.y);
        maxY = std::max(maxY, p.y);
    }

    int yStart = static_cast<int>(std::floor(minY));
    int yEnd   = static_cast<int>(std::ceil(maxY));

    const float EPS = 1e-5f;

    for (int y = yStart; y <= yEnd; ++y) {
        std::vector<float> intersections;

        auto it = points.begin();
        for (size_t i = 0; i < points.size(); ++i) {
            auto p1 = *it;
            auto p2 = *(std::next(it) == points.end() ? points.begin() : std::next(it));

            if ((p1.y <= y && p2.y > y) || (p2.y <= y && p1.y > y)) {
                if (std::fabs(p2.y - p1.y) > EPS) {
                    float x = p1.x + (y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
                    intersections.push_back(x);
                }
            }
            ++it;
        }

        std::sort(intersections.begin(), intersections.end());

        for (size_t i = 0; i + 1 < intersections.size(); i += 2) {
            int xStart = static_cast<int>(std::round(intersections[i]));
            int xEnd   = static_cast<int>(std::round(intersections[i + 1]));

            for (int x = xStart; x <= xEnd; ++x) {
                if (x >= 0 && x < surface->w && y >= 0 && y < surface->h) {
                    Primitives::setPixel(surface, x, y, color);
                }
            }
        }
    }
}

// Convert world coordinate X to screen coordinate X
float Primitives::convertX(float x,
                           int screenWidth,
                           float worldWidth) {
    return (x * screenWidth) / worldWidth;
}

// Convert world coordinate Y to screen coordinate Y
float Primitives::convertY(float y,
                           int screenHeight,
                           float worldHeight) {
    return ((y * -screenHeight) / worldHeight) + screenHeight;
}

// Convert a list of points
void Primitives::convertPoints(std::list<Point>& points,
                               int screenWidth,
                               int screenHeight,
                               float worldWidth,
                               float worldHeight) {
    for (auto& p : points) {
        p.x = convertX(p.x, screenWidth, worldWidth);
        p.y = convertY(p.y, screenHeight, worldHeight);
    }
}

// Convert degrees to radians
float Primitives::toRadians(float degrees)
{
    static constexpr float DEGREE_TO_RADIANS = M_PI / 180.0;
    return degrees * DEGREE_TO_RADIANS;
}

// Calculate the Euclidean distance between two points
float Primitives::distance(const Point& p1, const Point& p2)
{
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return std::sqrt(dx * dx + dy * dy);
}

// Translate a list of points by (tx, ty)
void Primitives::translatePolygon(std::list<Point>& points,
                                  float tx,
                                  float ty) {
    for (auto& p : points) {
        p.x += tx;
        p.y += ty;
    }
}

// Scale a list of points relative to an arbitrary pivot.
void Primitives::scalePolygon(std::list<Point>& points,
                              float sx,
                              float sy,
                              const Point& pivot) {
    if (points.empty())
        return;

    for (auto& p : points) {
        p.x = (p.x - pivot.x) * sx + (pivot.x * sx);
        p.y = (p.y - pivot.y) * sy + (pivot.y * sy);
    }
}

// Rotate a list of points around a pivot by a given angle
void Primitives::rotatePolygon(std::list<Point>& points,
                               float angle,
                               const Point& pivot) {
    if (points.empty())
        return;

    float radians = Primitives::toRadians(angle);
    float cosA = std::cos(radians);
    float sinA = std::sin(radians);

    for (auto& p : points) {
        float x = p.x - pivot.x;
        float y = p.y - pivot.y;

        float newX = x * cosA - y * sinA;
        float newY = x * sinA + y * cosA;

        p.x = newX + pivot.x;
        p.y = newY + pivot.y;
    }
}
