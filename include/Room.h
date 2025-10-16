#ifndef ROOM_H
#define ROOM_H

#include "raylib.h"
#include <string>

class Room {
private:
    Texture2D background;

public:
    Room(const std::string& configPath = "res/config.json");
    ~Room();

    void Draw();
};

#endif // ROOM_H
