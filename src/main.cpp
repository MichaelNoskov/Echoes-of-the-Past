#include "raylib.h"
#include "Game.h"

int main() {
    SetTargetFPS(60);
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    int windowHeight = 600 * 2;
    int windowWidth = 800 * 2;
    InitWindow(windowWidth, windowHeight, "Game with Random Room Background");

    float loadingTime = 0;
    while (loadingTime < 2.0f) { // Показываем загрузку 2 секунды
        loadingTime += GetFrameTime();

        BeginDrawing();
        ClearBackground(BLACK);
        
        const char* loadingText = "Loading";
        int fontSize = 40;
        int textWidth = MeasureText(loadingText, fontSize);
        int textX = (windowWidth - textWidth) / 2;
        int textY = (windowHeight - fontSize) / 2;
        
        DrawText(loadingText, textX, textY, fontSize, WHITE);
        
        // Анимированные точки
        int dotCount = (int)(loadingTime * 3) % 4; // Меняем количество точек
        for (int i = 0; i < dotCount; i++) {
            DrawText(".", textX + textWidth + i * 15, textY, fontSize, WHITE);
        }
        
        EndDrawing();
    }

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