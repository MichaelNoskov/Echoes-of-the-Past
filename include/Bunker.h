#ifndef BUNKER_H
#define BUNKER_H

#include "raylib.h"
#include <Room.h>
#include <string>

class Game;

class Bunker {
private:
    int energy;
    Game* game;

    std::vector<std::unique_ptr<Room>> roomList;
    int currentRoomIndex = 0; 

public:
    Bunker(int startEnergy = 0, Game* gameRef = nullptr);
    ~Bunker();

    Game* GetGame() const { return game; }
    void AddRoom(std::unique_ptr<Room> newRoom);

    void SetCurrentRoom(int index);
    Room* GetCurrentRoom();
    int GetCurrentRoomIndex() const;
    bool GoToNextRoom();
    bool GoToPreviousRoom();

    int GetEnergy() { return energy; }

    void Update();
    void Draw();

};

#endif