#include "App.h"
#include "House.h"
#include "Sun.h"

#include <cmath>
#include <cstdlib>
#include <iostream>

App::App(const std::string& title,
         const Config& config)
       : window(nullptr),
         surface(nullptr),
         running(false),
         config(config) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow(title.c_str(),
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              getScreenWidth(),
                              getScreenHeight(),
                              SDL_WINDOW_RESIZABLE);

    if (!window) {
        std::cerr << "Error creating window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }

    surface = SDL_GetWindowSurface(window);
}

App::~App() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void App::run() {
    running = true;

    Color backgroundColor = Color::fromName(surface, getBackgroundColor());

    while (running) {

        clearGradient(surface,
                      backgroundColor.getSecondary(),
                      backgroundColor.getPrimary());

       for (auto& obj : config.objects) {
            switch (obj.type) {
                case ObjectType::House: {
                    auto& houseConfig = std::get<HouseConfig>(obj.data);
                    House house(houseConfig.x,
                                houseConfig.y,
                                houseConfig.width,
                                houseConfig.height,
                                houseConfig.angle,
                                houseConfig.wallColor,
                                houseConfig.roofColor,
                                houseConfig.doorColor,
                                config,
                                surface);
                    house.draw(surface);
                    break;
                }
                case ObjectType::Sun: {
                    auto& sunConfig = std::get<SunConfig>(obj.data);
                    Sun sun(sunConfig.x,
                            sunConfig.y,
                            sunConfig.width,
                            sunConfig.height,
                            sunConfig.angle,
                            sunConfig.color,
                            config,
                            surface);
                    sun.draw(surface);
                    break;
                }
            }
        }

        handleEvents();

        applyVignette(surface, 0.2f);

        present();
    }
}

void App::handleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            running = false;

        if (event.type == SDL_WINDOWEVENT &&
            event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
            surface = SDL_GetWindowSurface(window);
            setScreenWidth(surface->w);
            setScreenHeight(surface->h);
        }
    }
}

void App::clearGradient(SDL_Surface* surface, Uint32 topColor, Uint32 bottomColor) {
    SDL_LockSurface(surface);

    Uint8 r1, g1, b1;
    Uint8 r2, g2, b2;

    SDL_GetRGB(topColor, surface->format, &r1, &g1, &b1);
    SDL_GetRGB(bottomColor, surface->format, &r2, &g2, &b2);

    for (int y = 0; y < surface->h; y++) {
        float t = static_cast<float>(y) / (surface->h - 1);

        Uint8 r = static_cast<Uint8>(r1 + t * (r2 - r1));
        Uint8 g = static_cast<Uint8>(g1 + t * (g2 - g1));
        Uint8 b = static_cast<Uint8>(b1 + t * (b2 - b1));

        Uint32 color = SDL_MapRGB(surface->format, r, g, b);

        SDL_Rect line = {0, y, surface->w, 1};
        SDL_FillRect(surface, &line, color);
    }

    SDL_UnlockSurface(surface);
}

void App::applyVignette(SDL_Surface* surface, float strength) {
    SDL_LockSurface(surface);

    int w = surface->w;
    int h = surface->h;

    float cx = w / 2.0f;
    float cy = h / 2.0f;
    float maxDistance = std::sqrt(cx*cx + cy*cy);

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            float dx = x - cx;
            float dy = y - cy;
            float dist = std::sqrt(dx*dx + dy*dy) / maxDistance;

            float vignette = std::pow(dist, 2.0f) * strength;

            Uint32* pixel = (Uint32*)((Uint8*)surface->pixels + y * surface->pitch + x * 4);
            Uint8 r, g, b;
            SDL_GetRGB(*pixel, surface->format, &r, &g, &b);

            r = static_cast<Uint8>(r * (1.0f - vignette));
            g = static_cast<Uint8>(g * (1.0f - vignette));
            b = static_cast<Uint8>(b * (1.0f - vignette));

            *pixel = SDL_MapRGB(surface->format, r, g, b);
        }
    }

    SDL_UnlockSurface(surface);
}

void App::present() {
    SDL_UpdateWindowSurface(window);
}
