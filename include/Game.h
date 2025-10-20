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

public:
    Game(float startDay);
    Game(float startDay, Room& room);
    ~Game();

    void ChangeRoom(std::unique_ptr<Room> newRoom) ;
    void Update();
    void Draw();
};

#endif