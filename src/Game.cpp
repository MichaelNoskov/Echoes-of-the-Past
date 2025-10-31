#include "Game.h"
#include "Room.h"
#include "Furniture.h"
#include <string>


Game::Game(int startDay) : day(startDay) {
    curentRoom = std::make_unique<Room>(600*5, 600);
    curentRoom->AddFurniture(std::make_unique<Furniture>(
        "res/textures/furniture/монитор.PNG",
        "res/textures/furniture/монитор_л.PNG",
        "res/textures/furniture/монитор_п.PNG",
        250.0f, 177.0f, 100.0f, 600.0f - 50.0f, "Monitor"
    ));
    curentRoom->AddFurniture(std::make_unique<Furniture>(
        "res/textures/furniture/стол.PNG",
        "res/textures/furniture/стол_л.PNG",
        "res/textures/furniture/стол_п.PNG",
        500.0f, 300.0f, 700.0f, 600.0f - 50.0f, "Table"
    ));
}

Game::Game(int startDay, Room& room) : day(startDay), curentRoom(&room) {}

Game::~Game() {
}

void Game::Update() {
    curentRoom->Update();

}

void Game::Draw(){
    curentRoom->Draw();
}

void Game::ChangeRoom(std::unique_ptr<Room> newRoom) {
    curentRoom = std::move(newRoom);
}
