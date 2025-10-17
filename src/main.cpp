#include "raylib.h"
#include "Room.h"
#include "Furniture.h"

int main() {
    int windowHeight = 600;
    int windowWidth = 800;
    InitWindow(windowWidth, windowHeight, "Game with Random Room Background");

    SetTargetFPS(60);

    Room room(windowWidth*3, windowHeight);

    room.AddFurniture(std::make_unique<Furniture>("res/textures/furniture/chair.png", 250.0f, 250.0f, 100.0f, windowHeight - 50.0f, "Chair"));
    room.AddFurniture(std::make_unique<Furniture>("res/textures/furniture/table.png", 500.0f, 300.0f, 700.0f, windowHeight, "Table"));

    while (!WindowShouldClose()) {
        room.Update();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        room.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}