#include "Resource.h"
#include <stdexcept>

Resource::Resource(const std::string& resourceName, const std::string& texturePath, const std::string& unitName, int startAmount) 
    : name(resourceName), unit(unitName), amount(startAmount), iconLoaded(false) {
    
    if (!texturePath.empty()) {
        icon = LoadTexture(texturePath.c_str());
        iconLoaded = true;
    }
}

Resource::~Resource() {
    if (iconLoaded) {
        UnloadTexture(icon);
    }
}

void Resource::Add(int value) {
    if (value < 0) {
        throw std::invalid_argument("Cannot add negative value");
    }
    amount += value;
}

bool Resource::Subtract(int value) {
    if (value < 0) {
        throw std::invalid_argument("Cannot subtract negative value");
    }
    
    if (amount >= value) {
        amount -= value;
        return true;
    }
    return false;
}

void Resource::SetAmount(int newAmount) {
    if (newAmount < 0) {
        throw std::invalid_argument("Amount cannot be negative");
    }
    amount = newAmount;
}
