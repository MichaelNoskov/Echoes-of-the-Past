#ifndef PERCENTAGE_RESOURCE_H
#define PERCENTAGE_RESOURCE_H

#include "Resource.h"
#include <string>

struct PercentageResource : public Resource {
private:
    Texture2D icon;
    bool iconLoaded = false;
    int currentValue;
    int maxValue;

public:
    PercentageResource(const std::string& texturePath, int startValue = 0, int maximumValue = 1000);
    ~PercentageResource();

    void Draw(const Rectangle& area) const override;
    int GetValue() const override { return currentValue; }
    void SetValue(int newValue) override;
    std::string GetDisplayText() const override;

    bool Add(int value);
    bool Subtract(int value);
    void SetMaxValue(int newMaxValue);
    int GetMaxValue() const { return maxValue; }
    float GetPercentage() const { return static_cast<float>(currentValue) / maxValue; }
};

#endif
