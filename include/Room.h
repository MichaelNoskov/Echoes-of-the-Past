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
    bool lightsOn;
    bool flashlightOn;
    Rectangle drawArea;
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
    Furniture* GetFurnitureAtMousePosition();
    Vector2 ScreenToRoomSpace(Vector2 screenPos) const;
    Vector2 RoomToScreenSpace(Vector2 roomPos) const;

public:
    Room(float sceneWidth, float sceneHeight, const std::string& configPath);
    Room(float sceneWidth, float sceneHeight, const std::string& configPath, Rectangle area);
    ~Room();
    
    void Update();
    void Draw();
    Camera2D& GetCamera() { return camera; }

    void SetDrawArea(Rectangle area);
    Rectangle GetDrawArea() const;
    void AddFurniture(std::unique_ptr<Furniture> furniture);
    bool RemoveFurniture(const std::string& name);
    bool RemoveFurniture(int index);
    void ClearAllFurniture();
    size_t GetFurnitureCount() const { return furnitureList.size(); }
    Furniture* GetFurniture(const std::string& name);
    Furniture* GetFurniture(int index);
    void MoveFurniture(const std::string& name, float newX, float newY);
    std::vector<std::string> GetFurnitureNames() const;

    void ToggleLights();
    void ToggleFlashLight();
    bool AreLightsOn() const { return lightsOn; }
    bool AreFlashLightOn() const { return flashlightOn; }
    void SetLights(bool on) { lightsOn = on; }

    Vector2 GetSize() const { return {width, height}; };
};
#endif