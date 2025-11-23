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
}

Game::Game(int startDay, Bunker& bunker) : day(startDay), curentBunker(&bunker) {
}

Game::~Game() {}

void Game::Update() {
    AddTime(GetFrameTime() * (60.0f / (24.0f * 60.0f)));
    curentBunker->Update();
    hoveredResource = GetResourceAtMousePosition();
}

void Game::Draw() {
    ClearBackground(BLACK);
    curentBunker->Draw();
    DrawResources();

    DrawText("Right click to toggle flashlight", 10, 40, 20, LIGHTGRAY);
    DrawText("Press left to move furniture", 10, 70, 20, LIGHTGRAY);
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
