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

    curentBunker = std::make_unique<Bunker>(3000, this);

    std::unique_ptr<Room> room = std::make_unique<Room>(roomWidth, roomHeight, "res/config.json", roomArea, curentBunker.get());

    room->AddFurniture(std::make_unique<Furniture>(
        "res/textures/furniture/монитор.PNG",
        "res/textures/furniture/монитор_л.PNG",
        "res/textures/furniture/монитор_п.PNG",
        250.0f, 177.0f, 200.0f, roomHeight, "Monitor"
    ));
    room->AddFurniture(std::make_unique<Furniture>(
        "res/textures/furniture/стол.PNG",
        "res/textures/furniture/стол_л.PNG",
        "res/textures/furniture/стол_п.PNG",
        425.0f, 255.0f, 700.0f, roomHeight, "Table", true
    ));
    room->AddFurniture(std::make_unique<Furniture>(
        "res/textures/furniture/шкаф.PNG",
        "res/textures/furniture/шкаф_л.PNG",
        "res/textures/furniture/шкаф_п.PNG",
        400.0f, 550.0f, roomWidth - 650, roomHeight, "???", true
    ));
    room->AddFurniture(std::make_unique<Clock>(
        "res/textures/furniture/часы.PNG",
        "res/textures/furniture/часы_л.PNG",
        "res/textures/furniture/часы_п.PNG",
        "res/textures/furniture/маятник.PNG",
        "res/textures/furniture/маятник_л.PNG",
        "res/textures/furniture/маятник_п.PNG",
        15.0f, 46.0f, Vector2{0.454545f, 0.547169f}, Vector2{16.5f, 23.5f}, 4.0f,
        170.0f, 250.0f, roomWidth - 650, roomHeight, "Clock", false, true
    ));
    room->AddFurniture(std::make_unique<LightFurniture>(
        "res/textures/furniture/лампа.PNG",
        90.0f, 108.0f, (roomWidth-108.0f) / 2, 108, "Lamp"
    ));

    std::unique_ptr<Room> room2 = std::make_unique<Room>(roomWidth, roomHeight, "res/config.json", roomArea, curentBunker.get());
    room2->AddFurniture(std::make_unique<LightFurniture>(
        "res/textures/furniture/лампа.PNG",
        90.0f, 108.0f, (roomWidth-108.0f) / 2, 108, "Lamp"
    ));
    room2->AddFurniture(std::make_unique<BunkerDoor>(
        "res/textures/furniture/шкаф.PNG",
        "res/textures/furniture/стол.PNG",
        400.0f, 550.0f, 100, roomHeight, "Door"
    ));
    curentBunker->AddRoom(std::move(room));
    curentBunker->AddRoom(std::move(room2));

}

Game::Game(int startDay, Bunker& bunker) : day(startDay), curentBunker(&bunker) {}

Game::~Game() {
}

void Game::Update() {
    float timeScale = 60.0f / (24.0f * 60.0f);
    AddTime(GetFrameTime() * timeScale);

    curentBunker->Update();
}

void Game::Draw() {
    ClearBackground(BLACK);
    curentBunker->Draw();

    DrawText("Right click to toggle flashlight", 10, 40, 20, LIGHTGRAY);
    DrawText("Press left to move furniture", 10, 70, 20, LIGHTGRAY);
}

int Game::GetHours() const {
    return static_cast<int>(time * 24) % 24;
}

int Game::GetMinutes() const {
    float totalMinutes = time * 24 * 60;
    return static_cast<int>(totalMinutes) % 60;
}

std::string Game::GetTimeString() const {
    int hours = GetHours();
    int minutes = GetMinutes();
    
    std::ostringstream timeStr;
    timeStr << std::setw(2) << std::setfill('0') << hours << ":"
            << std::setw(2) << std::setfill('0') << minutes;
    
    return timeStr.str();
}

void Game::AddTime(float delta) {
    time += delta;
    if (time >= 1.0f) {
        time -= 1.0f;
        day++;
    }
}