#include "raylib.h"
#include "Game.h"

int main() {
    int windowHeight = 600;
    int windowWidth = 800;
    InitWindow(windowWidth, windowHeight, "Game with Random Room Background");

    SetTargetFPS(60);

    Game game;

    while (!WindowShouldClose()) {
        game.Update();

		BeginDrawing();
        ClearBackground(BLACK);
		game.Draw();
		EndDrawing();
	}

    CloseWindow();
    return 0;
}