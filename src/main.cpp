#include "raylib.h"
#include "Room.h"

int main() {
    int windowHeight = 600;
    int windowWidth = 800;
    InitWindow(windowWidth, windowHeight, "Game with Random Room Background");

    SetTargetFPS(60);

    Room room(windowWidth*3, windowHeight);

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