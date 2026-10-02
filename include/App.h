#ifndef APP_H
#define APP_H

#include "Config.h"

#include <SDL2/SDL.h>
#include <string>

class App {
private:
    SDL_Window* window;
    SDL_Surface* surface;

    bool running;

    Config config;

    // Setters
    void setScreenWidth(int width) { config.screenWidth = width; }
    void setScreenHeight(int height) { config.screenHeight = height; }

public:
    App(const std::string& title, const Config& config);
    ~App();

    void run();
    void handleEvents();
    void clearGradient(SDL_Surface* surface, Uint32 topColor, Uint32 bottomColor);
    void applyVignette(SDL_Surface* surface, float strength);
    void present();

    // Getters
    int getScreenWidth() const { return config.screenWidth; }
    int getScreenHeight() const { return config.screenHeight; }

    const std::string& getBackgroundColor() const { return config.backgroundColor; }
};

#endif
