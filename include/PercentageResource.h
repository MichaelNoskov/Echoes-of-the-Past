#ifndef PERCENTAGE_RESOURCE_H
#define PERCENTAGE_RESOURCE_H

#include "Resource.h"
#include <string>

struct PercentageResource : public Resource {
private:
    std::string name;
    Texture2D icon;
    float currentValue;
    float maxValue;

public:
    PercentageResource(const std::string& resourceName, const std::string& texturePath, float startValue = 0, float maximumValue = 1000);
    ~PercentageResource();

    std::string GetName() const override { return name; }
    void Draw(const Rectangle& area) const override;
    float GetValue() const override { return currentValue; }
    void SetValue(float newValue) override;
    std::string GetDisplayText() const override;

    void Add(float value) override;
    void Subtract(float value) override;
    void SetMaxValue(float newMaxValue);
    float GetMaxValue() const { return maxValue; }
    float GetPercentage() const { return static_cast<float>(currentValue) / maxValue; }
};

#endif