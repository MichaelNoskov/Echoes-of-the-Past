#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "Bunker.h"
#include "Resource.h"
#include <string>
#include <vector>
#include <memory>

class Game {
private:
    float time = 0.0f;
    int day = 1;
    std::unique_ptr<Bunker> curentBunker;
    std::vector<std::unique_ptr<Resource>> resources;
    Resource* hoveredResource = nullptr;

public:
    Game(int startDay=1);
    Game(int startDay, Bunker& bunker);
    ~Game();

    void Update();
    void Draw();

    float GetTime() const { return time; }
    int GetHours() const;
    int GetMinutes() const;
    std::string GetTimeString() const;
    void AddTime(float delta);

    void AddResource(const std::string& name, const std::string& texturePath, const std::string& unit = "", int startAmount = 0);
    Resource* GetResource(const std::string& name);
    bool AddToResource(const std::string& name, int amount);
    bool SubtractFromResource(const std::string& name, int amount);
    void SetResourceAmount(const std::string& name, int amount);

    std::vector<Resource*> GetAllResources() const;
    
    int CalculateOptimalColumns(float availableWidth, float iconSize, float spacing) const;

private:
    void DrawResources();
    Resource* GetResourceAtMousePosition();
    Rectangle GetResourcesArea() const;
};

#endif