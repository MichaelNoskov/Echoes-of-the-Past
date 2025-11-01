#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include <Room.h>
#include <string>

class Game {
private:
    float time = 0.0f;
    int day;
    std::unique_ptr<Room> curentRoom;
    Shader roomShader;
    RenderTexture2D roomTarget;

public:
    Game(int startDay=1);
    Game(int startDay, Room& room);
    ~Game();

    void ChangeRoom(std::unique_ptr<Room> newRoom) ;
    void Update();
    void Draw();

    void ToggleRoomLights();
    void ToggleRoomFlashLight();
};

#endif