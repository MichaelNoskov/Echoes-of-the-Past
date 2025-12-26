#include "Game.h"
#include "Bunker.h"
#include "Room.h"
#include "Furniture.h"
#include "LightFurniture.h"
#include "BunkerDoor.h"
#include "Clock.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <random>
#include <filesystem>

namespace fs = std::filesystem;

Game::Game(int startDay) : day(startDay) {
    float roomWidth = 600*4;
    float roomHeight = 1000;
    float windowWidth = std::min(GetScreenWidth() * 0.5f, roomWidth);

    Rectangle roomArea = {
        (GetScreenWidth() - windowWidth) / 2.0f,
        (GetScreenHeight() - roomHeight) / 3.5f,
        windowWidth,
        roomHeight
    };

    curentBunker = std::make_unique<Bunker>(3000, 6000, this);

    std::unique_ptr<Room> room = std::make_unique<Room>(roomWidth, roomHeight, "res/config.json", roomArea, curentBunker.get());
    room->AddFurniture(std::make_unique<Furniture>("res/textures/furniture/монитор.PNG", "res/textures/furniture/монитор_л.PNG", "res/textures/furniture/монитор_п.PNG", 250.0f, 177.0f, 200.0f, roomHeight, "Monitor"));
    room->AddFurniture(std::make_unique<Furniture>("res/textures/furniture/стол.PNG", "res/textures/furniture/стол_л.PNG", "res/textures/furniture/стол_п.PNG", 425.0f, 255.0f, 700.0f, roomHeight, "Table", true));
    room->AddFurniture(std::make_unique<Furniture>("res/textures/furniture/шкаф.PNG", "res/textures/furniture/шкаф_л.PNG", "res/textures/furniture/шкаф_п.PNG", 400.0f, 550.0f, roomWidth - 650, roomHeight, "???", true));
    room->AddFurniture(std::make_unique<LightFurniture>("res/textures/furniture/лампа.PNG", 90.0f, 108.0f, (roomWidth-108.0f) / 2, 108, "Lamp"));

    std::unique_ptr<Room> room2 = std::make_unique<Room>(roomWidth, roomHeight, "res/config.json", roomArea, curentBunker.get());
    room2->AddFurniture(std::make_unique<LightFurniture>("res/textures/furniture/лампа.PNG", 90.0f, 108.0f, (roomWidth-108.0f) / 2, 108, "Lamp"));
    room2->AddFurniture(std::make_unique<BunkerDoor>("res/textures/furniture/шкаф.PNG", "res/textures/furniture/стол.PNG", 400.0f, 550.0f, 100, roomHeight, "Door"));
    
    curentBunker->AddRoom(std::move(room));
    curentBunker->AddRoom(std::move(room2));

    dialogueSystem = CreateDialogueSystem();
}

Game::Game(int startDay, Bunker& bunker) : day(startDay), curentBunker(&bunker) {
    dialogueSystem = CreateDialogueSystem();
}

Game::~Game() {}

void Game::Update() {
    AddTime(GetFrameTime() * (60.0f / (24.0f * 60.0f)));
    curentBunker->Update();
    hoveredResource = GetResourceAtMousePosition();
    
    // Обновляем диалог, если он активен
    if (dialogueSystem && dialogueSystem->IsActive()) {
        dialogueSystem->Update();
        if (dialogueSystem->IsFinished()) {
            EndDialogue();
        }
    }
}

void Game::Draw() {
    ClearBackground(BLACK);
    curentBunker->Draw();
    DrawResources();

    DrawText("Right click to toggle flashlight", 10, 40, 20, LIGHTGRAY);
    DrawText("Press left to move furniture", 10, 70, 20, LIGHTGRAY);

    if (dialogueSystem && dialogueSystem->IsActive()) {
        dialogueSystem->Draw();
    }
}

int Game::GetHours() const {
    return static_cast<int>(time * 24) % 24;
}

int Game::GetMinutes() const {
    return static_cast<int>(time * 24 * 60) % 60;
}

std::string Game::GetTimeString() const {
    std::ostringstream timeStr;
    timeStr << std::setw(2) << std::setfill('0') << GetHours() << ":"
            << std::setw(2) << std::setfill('0') << GetMinutes();
    return timeStr.str();
}

void Game::AddTime(float delta) {
    time += delta;
    if (time >= 1.0f) {
        time -= 1.0f;
        day++;
    }
}

void Game::AddResource(std::unique_ptr<Resource> resource) {
    resources.push_back(std::move(resource));
}

Resource* Game::GetResource(const std::string& name) {
    auto it = std::find_if(resources.begin(), resources.end(),
        [&name](const auto& resource) { 
            return resource->GetName() == name;
        });
    return it != resources.end() ? it->get() : nullptr;
}

std::vector<Resource*> Game::GetAllResources() const {
    std::vector<Resource*> allResources;

    for (const auto& resource : resources) {
        allResources.push_back(resource.get());
    }

    if (curentBunker) {
        for (const auto& resource : curentBunker->GetResources()) {
            allResources.push_back(resource.get());
        }
    }
    
    return allResources;
}

Resource* Game::GetResourceAtMousePosition() {
    Vector2 mousePos = GetMousePosition();
    Rectangle resourcesArea = GetResourcesArea();
    
    if (!CheckCollisionPointRec(mousePos, resourcesArea)) return nullptr;

    std::vector<Resource*> allResources = GetAllResources();
    if (allResources.empty()) return nullptr;

    const float iconSize = 160.0f;
    const float spacing = 30.0f;
    const int maxColumns = CalculateOptimalColumns(resourcesArea.width, iconSize, spacing);
    
    float startX = resourcesArea.x;
    float startY = resourcesArea.y;
    
    for (size_t i = 0; i < allResources.size(); ++i) {
        Resource* resource = allResources[i];

        int row = i / maxColumns;
        int col = i % maxColumns;
        
        float totalRowWidth = maxColumns * iconSize + (maxColumns - 1) * spacing;
        float rowStartX = startX + (resourcesArea.width - totalRowWidth) / 2;
        
        Rectangle iconRect = {
            rowStartX + col * (iconSize + spacing),
            startY + row * (iconSize + spacing),
            iconSize,
            iconSize
        };
        
        if (CheckCollisionPointRec(mousePos, iconRect)) {
            return resource;
        }
    }
    
    return nullptr;
}

void Game::DrawResources() {
    std::vector<Resource*> allResources = GetAllResources();
    if (allResources.empty()) return;

    Rectangle area = GetResourcesArea();
    const float iconSize = 160.0f;
    const float spacing = 30.0f;
    const int fontSize = 40;
    const int textPadding = 8;
    
    const int maxColumns = CalculateOptimalColumns(area.width, iconSize, spacing);
    const int rows = (allResources.size() + maxColumns - 1) / maxColumns;
    const float contentHeight = rows * iconSize + (rows - 1) * spacing;
    
    float startX = area.x;
    float startY = area.y;
    
    for (size_t i = 0; i < allResources.size(); ++i) {
        Resource* resource = allResources[i];

        int row = i / maxColumns;
        int col = i % maxColumns;
        
        float totalRowWidth = maxColumns * iconSize + (maxColumns - 1) * spacing;
        float rowStartX = startX + (area.width - totalRowWidth) / 2;
        
        float x = rowStartX + col * (iconSize + spacing);
        float y = startY + row * (iconSize + spacing);
        
        Rectangle iconRect = {x, y, iconSize, iconSize};

        resource->Draw(iconRect);
        
        std::string amountText = resource->GetDisplayText();
        Vector2 textSize = MeasureTextEx(GetFontDefault(), amountText.c_str(), fontSize, 1);
        
        float textX = x + iconSize + textPadding * 0.01;
        float textY = y + iconSize - textSize.y - textPadding;
        textX = std::max(textX, x + textPadding);
        textY = std::max(textY, y + textPadding);
        
        DrawText(amountText.c_str(), textX, textY, fontSize, WHITE);
    }
    
    if (hoveredResource) {
        Vector2 mousePos = GetMousePosition();
        std::string tooltipText = hoveredResource->GetName();
        
        int padding = 8;
        Vector2 textSize = MeasureTextEx(GetFontDefault(), tooltipText.c_str(), fontSize, 1);
        
        float tooltipX = mousePos.x + 15;
        float tooltipY = mousePos.y - textSize.y - padding;
        
        if (tooltipX + textSize.x + padding * 2 > GetScreenWidth()) {
            tooltipX = mousePos.x - textSize.x - padding * 2 - 15;
        }
        if (tooltipY < 0) tooltipY = mousePos.y + 20;
        
        Rectangle bgRect = {tooltipX, tooltipY, textSize.x + padding * 2, textSize.y + padding * 2};
        DrawRectangleRec(bgRect, Fade(BLACK, 0.8f));
        DrawRectangleLinesEx(bgRect, 1, WHITE);
        DrawTextEx(GetFontDefault(), tooltipText.c_str(), {tooltipX + padding, tooltipY + padding}, fontSize, 1, WHITE);
    }
}

Rectangle Game::GetResourcesArea() const {
    Rectangle roomArea = curentBunker->GetCurrentRoom()->GetDrawArea();
    
    const float margin = 20.0f;
    const float iconSize = 80.0f;
    const float spacing = 15.0f;
    
    return {
        margin,
        roomArea.y,
        roomArea.x - margin * 2,
        roomArea.height
    };
}

int Game::CalculateOptimalColumns(float availableWidth, float iconSize, float spacing) const {
    if (availableWidth <= 0 || iconSize <= 0) return 1;
    
    std::vector<Resource*> allResources = GetAllResources();
    int totalResources = allResources.size();
    
    int maxPossibleColumns = std::max(1, static_cast<int>((availableWidth + spacing) / (iconSize + spacing)));
    return std::min(maxPossibleColumns, totalResources);
}


void Game::StartRandomDialogue() {
    dialogueSystem = CreateDialogueSystem(true);
}

void Game::EndDialogue() {
    if (dialogueSystem) {
        // Получаем награду в зависимости от настроения
        int mood = dialogueSystem->GetNpcMood();
        int reward = 100; // Базовая награда
        
        // Рассчитываем награду на основе настроения
        if (mood > 0) {
            reward = mood * 50; // Положительное настроение = положительная награда
            curentBunker->GetResource("Energy")->Add(abs(reward));
        } else if (mood < 0) {
            reward = mood * 50; // Отрицательное настроение = отрицательная награда (меньше)
            curentBunker->GetResource("Energy")->Subtract(abs(reward));
        }
        
        TraceLog(LOG_INFO, "Dialogue ended. Mood: %d, Reward: %d", mood, reward);

        dialogueSystem->EndDialogue();
    }
}

std::unique_ptr<DialogueSystem> Game::CreateDialogueSystem(bool active) {
    json dialogueJson = LoadRandomDialogue();
    Texture2D texture_neutral = LoadTexture("res/characters/hom-d.png");
    Texture2D texture_happy = LoadTexture("res/characters/hom-i.png");
    Texture2D texture_angry = LoadTexture("res/characters/hom-l.png");
    if (!dialogueJson.is_null()) {
        return std::make_unique<DialogueSystem>(dialogueJson, active);
    }
    return nullptr;
}

json Game::LoadRandomDialogue() const {
    const std::string dialoguesFile = "res/dialogues.json";
    
    if (!fs::exists(dialoguesFile)) {
        TraceLog(LOG_WARNING, "Dialogues file not found: %s", dialoguesFile.c_str());
        return json();
    }
    
    try {
        // Загружаем JSON файл
        std::ifstream file(dialoguesFile);
        json dialoguesJson;
        file >> dialoguesJson;
        
        // Проверяем, есть ли диалоги в файле
        if (!dialoguesJson.contains("dialogues") || !dialoguesJson["dialogues"].is_array()) {
            TraceLog(LOG_WARNING, "Invalid dialogues format");
            return json();
        }
        
        const auto& dialoguesArray = dialoguesJson["dialogues"];
        size_t arraySize = dialoguesArray.size();
        
        if (arraySize == 0) {
            TraceLog(LOG_WARNING, "No dialogues found");
            return json();
        }
        
        // Выбираем случайный диалог
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dis(0, arraySize - 1);  // Используем size_t
        
        size_t randomIndex = dis(gen);
        json randomDialogue = dialoguesArray[randomIndex];
        
        // Проверяем, что диалог содержит необходимые поля
        if (!randomDialogue.contains("dialogue_tree") || !randomDialogue["dialogue_tree"].is_object()) {
            TraceLog(LOG_WARNING, "Invalid dialogue structure");
            return json();
        }
        
        TraceLog(LOG_INFO, "Loaded dialogue: %s", randomDialogue.value("name", "unnamed").c_str());
        
        return randomDialogue;
        
    } catch (const std::exception& e) {
        TraceLog(LOG_ERROR, "Failed to load dialogues: %s", e.what());
        return json();
    }
}
