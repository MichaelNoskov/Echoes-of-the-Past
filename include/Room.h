#ifndef ROOM_H
#define ROOM_H

#include "raylib.h"
#include <string>

class Room {
private:
    float heigth;
    float width;
    Texture2D textureWallTop;
    Texture2D textureWallDown;
    Texture2D textureFloor;
    Camera2D camera;
    float cameraSensitivity;
    float cameraSmoothness;

public:
    Room(float sceneWidth = 2000.0f, float sceneHeigth = 1000.0f, const std::string& configPath = "res/config.json");
    ~Room();
    
    void Update();
    void Draw();
    Camera2D& GetCamera() { return camera; }
};

#endif