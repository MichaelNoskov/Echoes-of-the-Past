#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include <Bunker.h>
#include <string>

class Game {
private:
    float time = 0.0f;
    int day = 1;

    std::unique_ptr<Bunker> curentBunker;

public:
    Game(int startDay=1);
    Game(int startDay, Bunker& bunker);
    ~Game();

    void Update();
    void Draw();

    float GetTime() const { return time; }
    int GetHours() const;
    int GetMinutes() const;
    std::string GetTimeString() const;
    void AddTime(float delta);
};

#endif