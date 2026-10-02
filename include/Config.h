#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>
#include <variant>

struct HouseConfig {
    float x;
    float y;
    float width;
    float height;
    float angle = 0.0f;
    std::string wallColor;
    std::string roofColor;
    std::string doorColor;
};

struct SunConfig {
    float x;
    float y;
    float width;
    float height;
    float angle = 0.0f;
    std::string color;
};

enum class ObjectType { House, Sun };

struct ObjectConfig {
    ObjectType type;
    std::variant<HouseConfig, SunConfig> data;
};

struct Config {
    int screenWidth;
    int screenHeight;
    float worldWidth;
    float worldHeight;
    std::string backgroundColor;

    std::vector<ObjectConfig> objects;
};

Config loadConfig(const std::string& fileName);

#endif
