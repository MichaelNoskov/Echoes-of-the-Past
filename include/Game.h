#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "Bunker.h"
#include "Resource.h"
#include "Dialogue.h"
#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Game {
private:
    float time = 0.0f;
    int day = 1;
    std::unique_ptr<Bunker> curentBunker;
    std::vector<std::unique_ptr<Resource>> resources;
    Resource* hoveredResource = nullptr;
    
    // Диалоговая система
    std::unique_ptr<DialogueSystem> dialogueSystem;

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

    void AddResource(std::unique_ptr<Resource> resource);
    Resource* GetResource(const std::string& name);
    std::vector<Resource*> GetAllResources() const;
    
    int CalculateOptimalColumns(float availableWidth, float iconSize, float spacing) const;

    // Методы для работы с диалогами
    void StartRandomDialogue();
    void EndDialogue();

private:
    void DrawResources();
    Resource* GetResourceAtMousePosition();
    Rectangle GetResourcesArea() const;
    
    // Генерация диалоговой системы
    std::unique_ptr<DialogueSystem> CreateDialogueSystem(bool active = false);
    json LoadRandomDialogue() const;
};

#endif
