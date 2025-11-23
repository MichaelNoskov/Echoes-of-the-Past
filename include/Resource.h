#ifndef RESOURCE_H
#define RESOURCE_H

#include "raylib.h"
#include <string>

class Resource {
private:
    std::string name;
    std::string unit;
    int amount;
    Texture2D icon;
    bool iconLoaded;

public:
    Resource(const std::string& resourceName, const std::string& texturePath, const std::string& unitName = "", int startAmount = 0);
    ~Resource();

    std::string GetName() const { return name; }
    std::string GetUnit() const { return unit; }
    int GetAmount() const { return amount; }
    Texture2D GetIcon() const { return icon; }
    bool IsIconLoaded() const { return iconLoaded; }

    void Add(int value);
    bool Subtract(int value);
    void SetAmount(int newAmount);
};

#endif
