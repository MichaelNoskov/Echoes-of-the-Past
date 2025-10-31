#include "Game.h"
#include "Room.h"
#include "Furniture.h"
#include <string>


Game::Game(int startDay) : day(startDay) {
    roomShader = LoadShader(0, "res/shaders/room_shader.fs");
    roomTarget = LoadRenderTexture(600 * 5, 600 * 2);

    curentRoom = std::make_unique<Room>(600*5, 600);
    curentRoom->AddFurniture(std::make_unique<Furniture>(
        "res/textures/furniture/монитор.PNG",
        "res/textures/furniture/монитор_л.PNG",
        "res/textures/furniture/монитор_п.PNG",
        250.0f, 177.0f, 100.0f, 600.0f, "Monitor"
    ));
    curentRoom->AddFurniture(std::make_unique<Furniture>(
        "res/textures/furniture/стол.PNG",
        "res/textures/furniture/стол_л.PNG",
        "res/textures/furniture/стол_п.PNG",
        500.0f, 300.0f, 700.0f, 600.0f, "Table"
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

void Game::Draw(){

    BeginTextureMode(roomTarget);
        curentRoom->Draw();
    EndTextureMode();

    int lightsOnLocation = GetShaderLocation(roomShader, "lightsOn");
    float lightsOnValue = curentRoom->AreLightsOn() ? 1.0f : 0.0f;
    SetShaderValue(roomShader, lightsOnLocation, &lightsOnValue, SHADER_UNIFORM_FLOAT);

    BeginShaderMode(roomShader);
        DrawTextureRec(roomTarget.texture, 
            (Rectangle){0, 0, (float)roomTarget.texture.width, (float)-roomTarget.texture.height},
            (Vector2){0, 0}, WHITE);
    EndShaderMode();
}

void Game::ChangeRoom(std::unique_ptr<Room> newRoom) {
    curentRoom = std::move(newRoom);
}

void Game::ToggleRoomLights() {
    curentRoom->ToggleLights();
}