#include "raylib.h"
#include "Room.h"

int main() {
    InitWindow(800, 600, "Game with Random Room Background");

    Room room;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        room.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
