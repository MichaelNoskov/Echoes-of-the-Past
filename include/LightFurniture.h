#ifndef LIGHTFURNITURE_H
#define LIGHTFURNITURE_H

#include "Furniture.h"

class Room;

class LightFurniture : public Furniture {

public:
    LightFurniture(
        const std::string& texturePath,
        float width, float height, float x = 0.0f, float y = 0.0f, 
        const std::string& furnitureName = ""
    );
    
    LightFurniture(
        const std::string& textureFrontPath,
        const std::string& textureLeftPath,
        const std::string& textureRightPath,
        float width, float height, float x = 0.0f, float y = 0.0f, 
        const std::string& furnitureName = ""
    );
    
    void Update(bool interact) override;
    void SetPosition(float x, float y) override { return; };
    void Drag(bool drag) override { return; };
};

#endif