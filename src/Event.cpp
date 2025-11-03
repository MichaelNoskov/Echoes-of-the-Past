#include <Event.h>

Event::Event(std::string title, std::string message = "", float chance, std::string& soundPath) {
    this->title = title;
    this->message = message;
    this->chance = chance;
    this->sound = LoadSound(soundPath.c_str());
}

Event::~Event() {
    UnloadSound(sound);
}

void Event::Perform(Game* game) {
    if (sound.frameCount != 0) {
        PlaySound(sound);
    }
}
