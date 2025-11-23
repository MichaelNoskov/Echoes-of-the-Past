#ifndef PERCENTAGE_RESOURCE_H
#define PERCENTAGE_RESOURCE_H

#include "Resource.h"
#include <string>

struct PercentageResource : public Resource {
private:
    std::string name;
    Texture2D icon;
    int currentValue;
    int maxValue;

public:
    PercentageResource(const std::string& resourceName, const std::string& texturePath, int startValue = 0, int maximumValue = 1000);
    ~PercentageResource();

    std::string GetName() const override { return name; }
    void Draw(const Rectangle& area) const override;
    int GetValue() const override { return currentValue; }
    void SetValue(int newValue) override;
    std::string GetDisplayText() const override;

    void Add(int value) override;
    void Subtract(int value) override;
    void SetMaxValue(int newMaxValue);
    int GetMaxValue() const { return maxValue; }
    float GetPercentage() const { return static_cast<float>(currentValue) / maxValue; }
};

#endif