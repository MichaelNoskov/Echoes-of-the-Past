#include "LightFurniture.h"
#include "Room.h"

LightFurniture::LightFurniture(
    const std::string& texturePath,
    float width, float height, float x, float y, 
    const std::string& furnitureName
) : Furniture(texturePath, width, height, x, y, furnitureName, false) {
}

LightFurniture::LightFurniture(
    const std::string& textureFrontPath,
    const std::string& textureLeftPath,
    const std::string& textureRightPath,
    float width, float height, float x, float y, 
    const std::string& furnitureName
) : Furniture(textureFrontPath, textureLeftPath, textureRightPath, width, height, x, y, furnitureName, false) {
}

void LightFurniture::Update(Room* room, bool interact) {
    Furniture::Update(room, interact);

    if (!interact) {
        return;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        room -> ToggleLights();
    }

}