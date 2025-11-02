#include "Game.h"
#include "Room.h"
#include "Furniture.h"
#include "LightFurniture.h"
#include "Clock.h"
#include <string>
#include <sstream>
#include <iomanip>


Game::Game(int startDay) : day(startDay) {
    roomShader = LoadShader(0, "res/shaders/room_shader.fs");
    int flashlightPosLoc = GetShaderLocation(roomShader, "flashlightPos");
    // int flashlightRadiusLoc = GetShaderLocation(roomShader, "flashlightRadius");
    // float flashlightRadius = 0.2f;
    // SetShaderValue(roomShader, flashlightRadiusLoc, &flashlightRadius, SHADER_UNIFORM_FLOAT);

    roomTarget = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

    float roomWidth = 600*4;
    float roomHeight = 1000;

    float windowWidth = std::min(GetScreenWidth() * 0.5f, roomWidth);

    Rectangle roomArea = {
        (GetScreenWidth() - windowWidth) / 2.0f,
        (GetScreenHeight() - roomHeight) / 3.5f,
        windowWidth,
        roomHeight
    };

    curentRoom = std::make_unique<Room>(roomWidth, roomHeight, "res/config.json", roomArea, this);

    curentRoom->AddFurniture(std::make_unique<Furniture>(
        "res/textures/furniture/монитор.PNG",
        "res/textures/furniture/монитор_л.PNG",
        "res/textures/furniture/монитор_п.PNG",
        250.0f, 177.0f, 200.0f, roomHeight, "Monitor"
    ));
    curentRoom->AddFurniture(std::make_unique<Furniture>(
        "res/textures/furniture/стол.PNG",
        "res/textures/furniture/стол_л.PNG",
        "res/textures/furniture/стол_п.PNG",
        425.0f, 255.0f, 700.0f, roomHeight, "Table", true
    ));
    curentRoom->AddFurniture(std::make_unique<Furniture>(
        "res/textures/furniture/шкаф.PNG",
        "res/textures/furniture/шкаф_л.PNG",
        "res/textures/furniture/шкаф_п.PNG",
        400.0f, 550.0f, roomWidth - 650, roomHeight, "???", true
    ));
    curentRoom->AddFurniture(std::make_unique<Clock>(
        "res/textures/furniture/часы.PNG",
        "res/textures/furniture/часы_л.PNG",
        "res/textures/furniture/часы_п.PNG",
        "res/textures/furniture/маятник.PNG",
        "res/textures/furniture/маятник_л.PNG",
        "res/textures/furniture/маятник_п.PNG",
        15.0f, 46.0f, Vector2{0.454545f, 0.547169f}, Vector2{16.5f, 23.5f}, 4.0f,
        170.0f, 250.0f, roomWidth - 650, roomHeight, "Clock", false, true
    ));
    curentRoom->AddFurniture(std::make_unique<LightFurniture>(
        "res/textures/furniture/лампа.PNG",
        90.0f, 108.0f, (roomWidth-108.0f) / 2, 108, "Lamp"
    ));
}

Game::Game(int startDay, Room& room) : day(startDay), curentRoom(&room) {}

Game::~Game() {
    UnloadShader(roomShader);
}

void Game::Update() {
    float timeScale = 1.0f / (24.0f * 60.0f);
    AddTime(GetFrameTime() * timeScale);

    curentRoom->Update();
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
        ToggleRoomFlashLight();
    }

}

void Game::Draw() {
    Vector2 roomSize = curentRoom->GetSize();
    ClearBackground(BLACK);
    BeginTextureMode(roomTarget);
        ClearBackground(BLACK);
        curentRoom->Draw();
    EndTextureMode();

    int flashLightsOnLocation = GetShaderLocation(roomShader, "flashlightOn");
    float flashLightOnValue = curentRoom->AreFlashLightOn() ? 1.0f : 0.0f;
    SetShaderValue(roomShader, flashLightsOnLocation, &flashLightOnValue, SHADER_UNIFORM_FLOAT);

    int lightsOnLocation = GetShaderLocation(roomShader, "lightsOn");
    float lightsOnValue = curentRoom->AreLightsOn() ? 1.0f : 0.0f;
    SetShaderValue(roomShader, lightsOnLocation, &lightsOnValue, SHADER_UNIFORM_FLOAT);

    int flashlightPosLoc = GetShaderLocation(roomShader, "flashlightPos");
    Rectangle roomArea = curentRoom->GetDrawArea();
    Vector2 mousePos = GetMousePosition();

    Vector2 normalizedPos = {
        (mousePos.x - 100) / GetScreenWidth(),
        1 - (mousePos.y - 100) / GetScreenHeight()
    };
    
    SetShaderValue(roomShader, flashlightPosLoc, &normalizedPos, SHADER_UNIFORM_VEC2);
    float screenSize[2] = { (float)GetScreenWidth(), (float)GetScreenHeight() };
    SetShaderValue(roomShader, GetShaderLocation(roomShader, "screenSize"), screenSize, SHADER_UNIFORM_VEC2);

    BeginScissorMode((int)roomArea.x, (int)roomArea.y, (int)roomArea.width, (int)roomArea.height);
    
    BeginShaderMode(roomShader);
        DrawTextureRec(roomTarget.texture, 
            (Rectangle){0, 0, (float)roomTarget.texture.width, (float)-roomTarget.texture.height},
            (Vector2){0, 0}, WHITE);
    EndShaderMode();
    
    EndScissorMode();

    DrawText("Right click to toggle flashlight", 10, 40, 20, LIGHTGRAY);
    DrawText("Press left to move furniture", 10, 70, 20, LIGHTGRAY);
}

void Game::ChangeRoom(std::unique_ptr<Room> newRoom) {
    curentRoom = std::move(newRoom);
}

void Game::ToggleRoomFlashLight() {
    curentRoom->ToggleFlashLight();
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
