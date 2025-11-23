#include "PercentageResource.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>

PercentageResource::PercentageResource(const std::string& resourceName, const std::string& texturePath, float startValue, float maximumValue) 
    : name(resourceName), currentValue(startValue), maxValue(maximumValue) {
    
    if (!texturePath.empty()) {
        icon = LoadTexture(texturePath.c_str());
    }
}

PercentageResource::~PercentageResource() {
    UnloadTexture(icon);
}

void PercentageResource::Draw(const Rectangle& area) const {
    float scale = std::min(area.width / icon.width, area.height / icon.height) * 0.7f;
    float scaledWidth = icon.width * scale;
    float scaledHeight = icon.height * scale;
    float iconX = area.x + (area.width - scaledWidth) / 2;
    float iconY = area.y + (area.height - scaledHeight) / 2;
    
    DrawTextureEx(icon, {iconX, iconY}, 0.0f, scale, WHITE);
}

void PercentageResource::SetValue(float newValue) {
    if (newValue < 0) {
        throw std::invalid_argument("Energy value cannot be negative");
    }
    currentValue = (newValue > maxValue) ? maxValue : newValue;
}

std::string PercentageResource::GetDisplayText() const {
    std::ostringstream text;
    text << std::fixed << std::setprecision(1) << (GetPercentage() * 100) << "%";
    return text.str();
}

void PercentageResource::Add(float value) {
    if (value < 0) {
        throw std::invalid_argument("Cannot add negative value");
    }
    
    if (currentValue + value > maxValue) {
        currentValue = maxValue;
    }
    
    currentValue += value;
}

void PercentageResource::Subtract(float value) {
    if (value < 0) {
        throw std::invalid_argument("Cannot subtract negative value");
    }
    
    if (currentValue >= value) {
        currentValue -= value;
    }
}

void PercentageResource::SetMaxValue(float newMaxValue) {
    if (newMaxValue <= 0) {
        throw std::invalid_argument("Max value must be positive");
    }
    maxValue = newMaxValue;
    if (currentValue > maxValue) {
        currentValue = maxValue;
    }
}
