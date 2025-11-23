#ifndef BUNKER_H
#define BUNKER_H

#include "raylib.h"
#include <Room.h>
#include <string>
#include <vector>
#include <memory>
#include "Resource.h"

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

    void AddResource(const std::string& name, const std::string& texturePath, const std::string& unit = "", int startAmount = 0);
    Resource* GetResource(const std::string& name);
    bool AddToResource(const std::string& name, int amount);
    bool SubtractFromResource(const std::string& name, int amount);
    void SetResourceAmount(const std::string& name, int amount);
    const std::vector<std::unique_ptr<Resource>>& GetResources() const { return resources; }

    void Update();
    void Draw();

};

#endif