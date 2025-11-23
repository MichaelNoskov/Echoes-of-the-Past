#include "Bunker.h"
#include "LightFurniture.h"
#include <algorithm>


Bunker::Bunker(int startEnergy, int energyCapacity, Game* gameRef) : game(gameRef) {
    resources.push_back(std::make_unique<PercentageResource>("Energy", "res/textures/resources/energy.png", startEnergy, energyCapacity));
}

Bunker::~Bunker() {
    roomList.clear();
    resources.clear();
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

void Bunker::AddResource(std::unique_ptr<Resource> resource) {
    resources.push_back(std::move(resource));
}

Resource* Bunker::GetResource(const std::string& name) {
    auto it = std::find_if(resources.begin(), resources.end(),
        [&name](const auto& resource) { 
            return resource->GetName() == name;
        });
    return it != resources.end() ? it->get() : nullptr;
}

void Bunker::Update() {
    Room* currentRoom = GetCurrentRoom();
    if (currentRoom == nullptr) {
        return;
    }

    currentRoom->Update();

    Resource* PercentageResource = GetResource("Energy");

    for (const auto& room : roomList) {
        if (room && room->AreLightsOn()) {
            std::vector<std::string> furnitureNames = room->GetFurnitureNames();
            
            int lightCount = 0;
            for (const auto& name : furnitureNames) {
                Furniture* furniture = room->GetFurniture(name);
                if (furniture) {
                    LightFurniture* lightFurniture = dynamic_cast<LightFurniture*>(furniture);
                    if (lightFurniture) {
                        lightCount++;
                    }
                }
            }
            if (lightCount > 0) {
                if (PercentageResource) {
                    PercentageResource->Subtract(lightCount);
                }
            }
        }
    }

    if (PercentageResource->GetValue() <= 0) {
        for (const auto& room : roomList) {
            room->SetLights(false);
        }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        currentRoom->ToggleFlashLight();
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        Rectangle drawArea = currentRoom->GetDrawArea();

        float edgeZoneWidth = 50.0f;
    
        Rectangle leftEdgeZone = {
            drawArea.x,
            drawArea.y,
            edgeZoneWidth,
            drawArea.height
        };

        Rectangle rightEdgeZone = {
            drawArea.x + drawArea.width - edgeZoneWidth,
            drawArea.y,
            edgeZoneWidth,
            drawArea.height
        };
    
        if (CheckCollisionPointRec(mousePos, leftEdgeZone) && currentRoomIndex > 0) {
            GoToPreviousRoom();
        }
        else if (CheckCollisionPointRec(mousePos, rightEdgeZone) && 
                 currentRoomIndex < static_cast<int>(roomList.size()) - 1) {
            GoToNextRoom();
        }
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
