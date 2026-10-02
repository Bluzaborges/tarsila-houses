#ifndef HOUSE_H
#define HOUSE_H

#include <SDL2/SDL.h>
#include "Config.h"
#include "Shape.h"
#include "Primitives.h"
#include "Point.h"
#include "Color.h"
#include "Rectangle.h"
#include "Triangle.h"

#include <list>
#include <memory>
#include <string>

class House : public Shape {
private:
    float x;
    float y;
    float width;
    float height;
    float angle;

    Color wallColor;
    Color roofColor;
    Color doorColor;

    std::list<std::unique_ptr<Shape>> shapes;

    const Config& config;
    SDL_Surface* surface;

    void addRectangle(const std::list<Point>& basePoints,
                      float angle,
                      Uint32 color)
    {
        std::list<Point> pts = basePoints;

        Primitives::scalePolygon(pts, width, height, {0, 0});
        Primitives::rotatePolygon(pts, angle, {0, 0});
        Primitives::translatePolygon(pts, x, y);
        Primitives::convertPoints(pts,
                                  config.screenWidth,
                                  config.screenHeight,
                                  config.worldWidth,
                                  config.worldHeight);

        shapes.push_back(std::make_unique<Rectangle>(pts, color));
    }

    void addTriangle(const std::list<Point>& basePoints,
                     float angle,
                     Uint32 color)
    {
        std::list<Point> pts = basePoints;

        Primitives::scalePolygon(pts, width, height, {0, 0});
        Primitives::rotatePolygon(pts, angle, {0, 0});
        Primitives::translatePolygon(pts, x, y);
        Primitives::convertPoints(pts,
                                  config.screenWidth,
                                  config.screenHeight,
                                  config.worldWidth,
                                  config.worldHeight);

        shapes.push_back(std::make_unique<Triangle>(pts, color));
    }

    void addWallWindows(float wallWidthMeters,
                        float wallHeightMeters,
                        float offsetX_world,
                        float offsetY_world,
                        const Uint32 color,
                        bool skipFirst = false)
    {
        float windowSize   = 0.8f;
        float spacingRule  = 2.0f;

        int numWindowsX = static_cast<int>(wallWidthMeters  / spacingRule);
        int numWindowsY = static_cast<int>(wallHeightMeters / spacingRule);

        if (numWindowsX <= 0 || numWindowsY <= 0)
            return;

        float windowWidthNorm  = windowSize / width;
        float windowHeightNorm = windowSize / height;

        float totalWindowsWidth  = (numWindowsX - 1) * spacingRule + windowSize;
        float totalWindowsHeight = (numWindowsY - 1) * spacingRule + windowSize;

        float offsetX = (wallWidthMeters  - totalWindowsWidth)  / 2.0f;
        float offsetY = (wallHeightMeters - totalWindowsHeight) / 2.0f;

        for (int ix = 0; ix < numWindowsX; ix++) {
            for (int iy = 0; iy < numWindowsY; iy++) {
                if (skipFirst && ix == 0 && iy == 0)
                    continue;

                float x0 = (offsetX_world + offsetX + ix * spacingRule) / width;
                float y0 = (offsetY_world + offsetY + iy * spacingRule) / height;
                float x1 = x0 + windowWidthNorm;
                float y1 = y0 + windowHeightNorm;

                Point A{x0, y0};
                Point B{x0, y1};
                Point C{x1, y1};
                Point D{x1, y0};

                addRectangle({A, B, C, D}, angle, color);
            }
        }
    }

    void addWindows(float maxFrontWallWidth, float maxWallHeight) {
        float frontWallWidthMeters  = maxFrontWallWidth * width;
        float frontWallHeightMeters = maxWallHeight * height;

        addWallWindows(frontWallWidthMeters,
                       frontWallHeightMeters,
                       0.0f, 0.0f,
                       roofColor.getVariant(surface, 0.5f));

        float sideWallWidthMeters  = (1.0f - maxFrontWallWidth) * width;
        float sideWallHeightMeters = maxWallHeight * height;

        addWallWindows(sideWallWidthMeters,
                       sideWallHeightMeters,
                       maxFrontWallWidth * width, 0.0f,
                       wallColor.getSecondary(),
                       true);
    }

    void addDoor(float maxFrontWallWidth) {
        float sideWallWidth = (1.0f - maxFrontWallWidth) * width;

        float doorWidth  = 0.8f;
        float doorHeight = 1.9f;

        float doorCenterX = sideWallWidth / 2.0f;

        float doorX0_world = doorCenterX - doorWidth / 2.0f;
        float doorX1_world = doorCenterX + doorWidth / 2.0f;

        float doorX0 = (maxFrontWallWidth * width + doorX0_world) / width;
        float doorX1 = (maxFrontWallWidth * width + doorX1_world) / width;
        float doorY0 = 0.0f;
        float doorY1 = doorHeight / height;

        Point A{doorX0, doorY0};
        Point B{doorX0, doorY1};
        Point C{doorX1, doorY1};
        Point D{doorX1, doorY0};

        addRectangle({A, B, C, D}, angle, doorColor.getPrimary());
    }


public:
    House(float x,
          float y,
          float width,
          float height,
          float angle,
          const std::string& wallColorName,
          const std::string& roofColorName,
          const std::string& doorColorName,
          const Config& config,
          SDL_Surface* surface)
        : x(x),
          y(y),
          width(width),
          height(height),
          angle(angle),
          wallColor(Color::fromName(surface, wallColorName)),
          roofColor(Color::fromName(surface, roofColorName)),
          doorColor(Color::fromName(surface, doorColorName)),
          config(config),
          surface(surface)
    {
        if (width < 5.0f || height < 4.0f)
            return;

        float normalizedWidth = 1.0f;
        float normalizedHeight = 1.0f;

        float maxWallHeight = (height >= 1.5f)
            ? (1.0f - (1.5f / height))
            : 0.65f;

        float maxFrontWallWidth = (width >= 3.0f)
            ? (1.0f - (3.0f / width))
            : 0.65f;

        float sideWallCenter = (maxFrontWallWidth + normalizedWidth) / 2.0f;
        float roofTriangleWidth = sideWallCenter - maxFrontWallWidth;

        float roofShadowOffset = 0.45f;
        float roofShadowMaxWidth = roofShadowOffset / height;

        Point A{0, 0};
        Point B{0, maxWallHeight};
        Point C{maxFrontWallWidth, maxWallHeight};
        Point D{maxFrontWallWidth, 0};
        Point E{normalizedWidth, 0};
        Point F{normalizedWidth, maxWallHeight};
        Point G{maxFrontWallWidth, maxWallHeight - roofShadowMaxWidth};
        Point H{normalizedWidth, maxWallHeight - roofShadowMaxWidth};
        Point I{roofTriangleWidth, maxWallHeight};
        Point J{roofTriangleWidth, normalizedHeight};
        Point K{sideWallCenter, maxWallHeight};
        Point L{sideWallCenter, normalizedHeight};
        Point M{sideWallCenter, normalizedHeight - roofShadowMaxWidth};

        addRectangle({A, B, C, D}, angle, wallColor.getSecondary());
        addRectangle({D, C, F, E}, angle, wallColor.getSecondary());
        addRectangle({D, G, H, E}, angle, wallColor.getPrimary());
        addRectangle({I, J, L, K}, angle, roofColor.getPrimary());
        addTriangle({C, L, K}, angle, wallColor.getSecondary());
        addTriangle({K, L, F}, angle, wallColor.getSecondary());
        addTriangle({G, H, M}, angle, wallColor.getPrimary());
        addTriangle({B, J, I}, angle, roofColor.getPrimary());

        addWindows(maxFrontWallWidth, maxWallHeight);
        addDoor(maxFrontWallWidth);
    }

    void draw(SDL_Surface* surface) override {
        if (!surface)
            return;

        for (auto& shape : shapes)
            shape->draw(surface);
    }
};

#endif
