#ifndef BUNKERDOOR_H
#define BUNKERDOOR_H

#include "Furniture.h"

class Room;

class BunkerDoor : public Furniture {
 
private:
    bool closed = true;

    void SetClosed(bool isClosed);

public:
    BunkerDoor(
        const std::string& textureOpenedPath,
        const std::string& textureClosedPath,
        float width, float height, float x = 0.0f, float roomHeight = 0.0f,
        const std::string& furnitureName = ""
    );

    void Close() {
        SetClosed(false);
    }
    void Open() {
        SetClosed(true);
    }
    void Toggle() {
        SetClosed(!closed);
    }
    bool isClosed() {
        return closed;
    }

    bool GetFreeze() override { return true; };
    
    void Update(bool interact) override;
};

#endif