#include "Game.h"
#include "Room.h"
#include "Furniture.h"
#include <string>


Game::Game(int startDay) : day(startDay) {
    roomShader = LoadShader(0, "res/shaders/room_shader.fs");
    int flashlightPosLoc = GetShaderLocation(roomShader, "flashlightPos");
    // int flashlightRadiusLoc = GetShaderLocation(roomShader, "flashlightRadius");
    // float flashlightRadius = 0.2f;
    // SetShaderValue(roomShader, flashlightRadiusLoc, &flashlightRadius, SHADER_UNIFORM_FLOAT);

    roomTarget = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

    float roomWidth = 600*4;
    float roomHeight = 800;

    float windowWidth = std::min(GetScreenWidth() * 0.5f, roomWidth);

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
        400.0f, 550.0f, roomWidth - 650, roomHeight, "???"
    ));
}

Game::Game(int startDay, Room& room) : day(startDay), curentRoom(&room) {}

Game::~Game() {
}

void Game::Update() {
    curentRoom->Update();
    if (IsKeyPressed(KEY_L)) {
        ToggleRoomLights();
    } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
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

    DrawText("Press L to toggle lights", 10, 10, 20, LIGHTGRAY);
    DrawText("Right click to toggle flashlight", 10, 40, 20, LIGHTGRAY);
    DrawText("Press left to move furniture", 10, 70, 20, LIGHTGRAY);
}

void Game::ChangeRoom(std::unique_ptr<Room> newRoom) {
    curentRoom = std::move(newRoom);
}

void Game::ToggleRoomLights() {
    curentRoom->ToggleLights();
}

void Game::ToggleRoomFlashLight() {
    curentRoom->ToggleFlashLight();
}
