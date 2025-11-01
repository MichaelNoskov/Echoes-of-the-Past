#include "Game.h"
#include "Room.h"
#include "Furniture.h"
#include <string>


Game::Game(int startDay) : day(startDay) {
    roomShader = LoadShader(0, "res/shaders/room_shader.fs");
    roomTarget = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

    float roomWidth = 600*7;
    float roomHeight = 800;

    float windowWidth = std::min(GetScreenWidth() * 0.7f, roomWidth);

    Rectangle roomArea = {
        (GetScreenWidth() - windowWidth) / 2.0f,
        (GetScreenHeight() - roomHeight) / 3.5f,
        windowWidth,
        roomHeight
    };

    curentRoom = std::make_unique<Room>(roomWidth, roomHeight, "res/config.json", roomArea);

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
        425.0f, 255.0f, 700.0f, roomHeight, "Table"
    ));
    curentRoom->AddFurniture(std::make_unique<Furniture>(
        "res/textures/furniture/шкаф.PNG",
        "res/textures/furniture/шкаф_л.PNG",
        "res/textures/furniture/шкаф_п.PNG",
        400.0f, 550.0f, 600.0f * 5 - 700, roomHeight, "???"
    ));
}

Game::Game(int startDay, Room& room) : day(startDay), curentRoom(&room) {}

Game::~Game() {
}

void Game::Update() {
    curentRoom->Update();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        ToggleRoomLights();
    }

}

void Game::Draw() {
    Vector2 roomSize = curentRoom->GetSize();
    ClearBackground(BLACK);
    BeginTextureMode(roomTarget);
        ClearBackground(BLACK);
        curentRoom->Draw();
    EndTextureMode();

    int lightsOnLocation = GetShaderLocation(roomShader, "lightsOn");
    float lightsOnValue = curentRoom->AreLightsOn() ? 1.0f : 0.0f;
    SetShaderValue(roomShader, lightsOnLocation, &lightsOnValue, SHADER_UNIFORM_FLOAT);

    Rectangle roomArea = curentRoom->GetDrawArea();

    BeginScissorMode((int)roomArea.x, (int)roomArea.y, (int)roomArea.width, (int)roomArea.height);
    
    BeginShaderMode(roomShader);
        DrawTextureRec(roomTarget.texture, 
            (Rectangle){0, 0, (float)roomTarget.texture.width, (float)-roomTarget.texture.height},
            (Vector2){0, 0}, WHITE);
    EndShaderMode();
    
    EndScissorMode();

    DrawText("Click to toggle lights", 10, 10, 20, LIGHTGRAY);
}

void Game::ChangeRoom(std::unique_ptr<Room> newRoom) {
    curentRoom = std::move(newRoom);
}

void Game::ToggleRoomLights() {
    curentRoom->ToggleLights();
}