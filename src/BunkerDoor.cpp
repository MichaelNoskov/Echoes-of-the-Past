#include "BunkerDoor.h"
#include "Room.h"
# include "Game.h"

BunkerDoor::BunkerDoor(
    const std::string& textureOpenedPath,
    const std::string& textureClosedPath,
    float width, float height, float x, float roomHeight,
    const std::string& furnitureName
) : Furniture({{"default", textureOpenedPath}, {"close", textureClosedPath}}, width, height, x, roomHeight, furnitureName) {
}

void BunkerDoor::SetClosed(bool isClosed) {
        closed = isClosed;
        std::string state = "default";
        if (closed) {
            state = "close";
        }
        Furniture::SetState(state);
    }

void BunkerDoor::Update(bool interact) {
    Furniture::Update(interact);

    if (!interact) {
        return;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        Toggle();
    }
}
