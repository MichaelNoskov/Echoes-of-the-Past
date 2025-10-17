#ifndef ROOM_H
#define ROOM_H

#include "raylib.h"
#include <string>
#include <vector>
#include <memory>
#include "Furniture.h"

class Room {
private:
    float height;
    float width;
    Texture2D textureWallTop;
    Texture2D textureWallDown;
    Texture2D textureFloor;
    Camera2D camera;
    float cameraSensitivity;
    float cameraSmoothness;
    bool texturesTiled = false;
    
    std::vector<std::unique_ptr<Furniture>> furnitureList;
    Furniture* hoveredFurniture;
    Font font;

public:
    Room(float sceneWidth = 2000.0f, float sceneHeight = 1000.0f, const std::string& configPath = "res/config.json");
    ~Room();
    
    void Update();
    void Draw();
    Camera2D& GetCamera() { return camera; }

    void AddFurniture(std::unique_ptr<Furniture> furniture);
    bool RemoveFurniture(const std::string& name);
    bool RemoveFurniture(int index);
    void ClearAllFurniture();
    size_t GetFurnitureCount() const { return furnitureList.size(); }
    Furniture* GetFurniture(const std::string& name);
    Furniture* GetFurniture(int index);
    void MoveFurniture(const std::string& name, float newX, float newY);
    std::vector<std::string> GetFurnitureNames() const;

private:
    Furniture* GetFurnitureAtMousePosition();
};

#endif