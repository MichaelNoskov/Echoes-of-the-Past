#include "Bunker.h"

Bunker::Bunker(int startEnergy, Game* gameRef) : energy(startEnergy), game(gameRef) {
}

Bunker::~Bunker() {
    roomList.clear();
}

void Bunker::AddRoom(std::unique_ptr<Room> newRoom) {
    roomList.push_back(std::move(newRoom));
}

void Bunker::SetCurrentRoom(int index) {
    if (index >= 0 && index < static_cast<int>(roomList.size())) {
        currentRoomIndex = index;
    }
}

Room* Bunker::GetCurrentRoom() {
    if (roomList.empty()) {
        return nullptr;
    }
    return roomList[currentRoomIndex].get();
}

int Bunker::GetCurrentRoomIndex() const {
    if (roomList.empty()) {
        return -1;
    }
    return currentRoomIndex;
}

bool Bunker::GoToNextRoom() {
    if (currentRoomIndex < static_cast<int>(roomList.size()) - 1) {
        currentRoomIndex++;
        return true;
    }
    return false;
}

bool Bunker::GoToPreviousRoom() {
    if (currentRoomIndex > 0) {
        currentRoomIndex--;
        return true;
    }
    return false;
}

void Bunker::Update() {
    Room* currentRoom = GetCurrentRoom();
    if (currentRoom == nullptr) {
        return;
    }

    currentRoom->Update();
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
        currentRoom->ToggleFlashLight();
    }
    
}

void Bunker::Draw() {
    Room* currentRoom = GetCurrentRoom();
    if (currentRoom == nullptr) {
        return;
    }

    ClearBackground(BLACK);
    currentRoom->Draw();

    DrawText("Right click to toggle flashlight", 10, 40, 20, LIGHTGRAY);
    DrawText("Press left to move furniture", 10, 70, 20, LIGHTGRAY);
}
