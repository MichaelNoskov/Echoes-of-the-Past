#ifndef BUNKER_H
#define BUNKER_H

#include "raylib.h"
#include "Room.h"
#include "Resource.h"
#include "EnergyResource.h"
#include <string>
#include <vector>
#include <memory>

class Game;

class Bunker {
private:
    Game* game;
    std::vector<std::unique_ptr<Resource>> resources;

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

    void AddResource(std::unique_ptr<Resource> resource);
    Resource* GetResource(const std::string& name);
    const std::vector<std::unique_ptr<Resource>>& GetResources() const { return resources; }

    void Update();
    void Draw();

};

#endif
