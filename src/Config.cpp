#include "Config.h"

#include <fstream>
#include <iostream>
#include <sstream>

Config loadConfig(const std::string& fileName) {
    Config config{640, 480, 40.0f, 30.0f, "White", {}};

    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Error opening " << fileName << ", using defaults.\n";
        return config;
    }

    std::string line;
    std::string section;

    HouseConfig currentHouse{};
    SunConfig currentSun{};

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string key;
        std::getline(ss, key, ';');

        if (key == "Screen" || key == "House" || key == "Sun") {
            if (section == "House" && !currentHouse.wallColor.empty()) {
                config.objects.push_back({ObjectType::House, currentHouse});
                currentHouse = HouseConfig{};
            } else if (section == "Sun" && !currentSun.color.empty()) {
                config.objects.push_back({ObjectType::Sun, currentSun});
                currentSun = SunConfig{};
            }

            section = key;
            continue;
        }

        if (section == "Screen") {
            if (key == "Resolution") {
                std::string width;
                std::string height;
                std::getline(ss, width, ';');
                std::getline(ss, height, ';');
                config.screenWidth = std::stoi(width);
                config.screenHeight = std::stoi(height);
            } else if (key == "WorldSize") {
                std::string width;
                std::string height;
                std::getline(ss, width, ';');
                std::getline(ss, height, ';');
                config.worldWidth = std::stof(width);
                config.worldHeight = std::stof(height);
            } else if (key == "Color") {
                std::getline(ss, config.backgroundColor, ';');
            }
        } else if (section == "House") {
            if (key == "Position") {
                std::string x;
                std::string y;
                std::getline(ss, x, ';');
                std::getline(ss, y, ';');
                currentHouse.x = std::stof(x);
                currentHouse.y = std::stof(y);
            } else if (key == "Height") {
                std::string height;
                std::getline(ss, height, ';');
                currentHouse.height = std::stof(height);
            } else if (key == "Width") {
                std::string width;
                std::getline(ss, width, ';');
                currentHouse.width = std::stof(width);
            } else if (key == "WallColor") {
                std::getline(ss, currentHouse.wallColor, ';');
            } else if (key == "RoofColor") {
                std::getline(ss, currentHouse.roofColor, ';');
            } else if (key == "DoorColor") {
                std::getline(ss, currentHouse.doorColor, ';');
            } else if (key == "Angle") {
                std::string angle;
                std::getline(ss, angle, ';');
                currentHouse.angle = std::stof(angle);
            }
        } else if (section == "Sun") {
            if (key == "Position") {
                std::string x;
                std::string y;
                std::getline(ss, x, ';');
                std::getline(ss, y, ';');
                currentSun.x = std::stof(x);
                currentSun.y = std::stof(y);
            } else if (key == "Height") {
                std::string height;
                std::getline(ss, height, ';');
                currentSun.height = std::stof(height);
            } else if (key == "Width") {
                std::string width;
                std::getline(ss, width, ';');
                currentSun.width = std::stof(width);
            } else if (key == "Color") {
                std::getline(ss, currentSun.color, ';');
            } else if (key == "Angle") {
                std::string angle;
                std::getline(ss, angle, ';');
                currentSun.angle = std::stof(angle);
            }
        }
    }

    if (!currentHouse.wallColor.empty())
        config.objects.push_back({ObjectType::House, currentHouse});

    if (!currentSun.color.empty())
        config.objects.push_back({ObjectType::Sun, currentSun});

    return config;
}
