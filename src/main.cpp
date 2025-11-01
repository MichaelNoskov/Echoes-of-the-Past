#include "raylib.h"
#include "Game.h"

int main() {
    SetTargetFPS(60);
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    int windowHeight = 600 * 3;
    int windowWidth = 800 * 3;
    InitWindow(windowWidth, windowHeight, "Game with Random Room Background");

    float loadingTime = 0;
    while (loadingTime < 1.0f) {
        loadingTime += GetFrameTime();

        BeginDrawing();
        ClearBackground(BLACK);
        
        const char* loadingText = "Loading";
        int fontSize = 40;
        int textWidth = MeasureText(loadingText, fontSize);
        int textX = (windowWidth - textWidth) / 2;
        int textY = (windowHeight - fontSize) / 2;
        
        DrawText(loadingText, textX, textY, fontSize, WHITE);

        int dotCount = (int)(loadingTime * 3) % 4;
        for (int i = 0; i < dotCount; i++) {
            DrawText(".", textX + textWidth + i * 15, textY, fontSize, WHITE);
        }
        
        EndDrawing();
    }

    Game game;

    while (!WindowShouldClose()) {
        game.Update();

        BeginDrawing();
            game.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}