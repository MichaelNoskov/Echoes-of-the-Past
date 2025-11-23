#include "EnergyResource.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>

EnergyResource::EnergyResource(const std::string& texturePath, int startValue, int maximumValue) 
    : currentValue(startValue), maxValue(maximumValue) {
    
    if (!texturePath.empty()) {
        icon = LoadTexture(texturePath.c_str());
        iconLoaded = true;
    }
}

EnergyResource::~EnergyResource() {
    if (iconLoaded) {
        UnloadTexture(icon);
    }
}

void EnergyResource::Draw(const Rectangle& area) const {
    if (!iconLoaded) return;
    
    float scale = std::min(area.width / icon.width, area.height / icon.height) * 0.7f;
    float scaledWidth = icon.width * scale;
    float scaledHeight = icon.height * scale;
    float iconX = area.x + (area.width - scaledWidth) / 2;
    float iconY = area.y + (area.height - scaledHeight) / 2;
    
    DrawTextureEx(icon, {iconX, iconY}, 0.0f, scale, WHITE);
}

void EnergyResource::SetValue(int newValue) {
    if (newValue < 0) {
        throw std::invalid_argument("Energy value cannot be negative");
    }
    currentValue = (newValue > maxValue) ? maxValue : newValue;
}

std::string EnergyResource::GetDisplayText() const {
    std::ostringstream text;
    text << currentValue << "/" << maxValue << " " << "Вт" << " (" 
         << std::fixed << std::setprecision(1) << (GetPercentage() * 100) << "%)";
    return text.str();
}

bool EnergyResource::Add(int value) {
    if (value < 0) {
        throw std::invalid_argument("Cannot add negative value");
    }
    
    if (currentValue + value > maxValue) {
        currentValue = maxValue;
        return false;
    }
    
    currentValue += value;
    return true;
}

bool EnergyResource::Subtract(int value) {
    if (value < 0) {
        throw std::invalid_argument("Cannot subtract negative value");
    }
    
    if (currentValue >= value) {
        currentValue -= value;
        return true;
    }
    return false;
}

void EnergyResource::SetMaxValue(int newMaxValue) {
    if (newMaxValue <= 0) {
        throw std::invalid_argument("Max value must be positive");
    }
    maxValue = newMaxValue;
    if (currentValue > maxValue) {
        currentValue = maxValue;
    }
}
