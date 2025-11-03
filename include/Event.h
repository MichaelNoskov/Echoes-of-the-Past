#ifndef EVENT_H
#define EVENT_H

#include <Game.h>

class Event {
private:
    float chance = 1.0f;
    Sound sound;
    std::string title;
    std::string message = "";

public:
    Event(std::string title, std::string message = "", float chance, std::string& soundPath);
    ~Event();

    void Perform(Game* game);
    virtual bool Condition(Game* game) { return true; };
};

#endif