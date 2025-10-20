#include "Game.h"
#include "Room.h"
#include "Furniture.h"
#include <string>


Game::Game(float startDay=1) : day(startDay) {
    curentRoom = std::make_unique<Room>(600*5, 600);
    curentRoom->AddFurniture(std::make_unique<Furniture>("res/textures/furniture/chair.png", 250.0f, 250.0f, 100.0f, 100 - 50.0f, "Chair"));
    curentRoom->AddFurniture(std::make_unique<Furniture>("res/textures/furniture/table.png", 500.0f, 300.0f, 700.0f, 100, "Table"));
}

Game::Game(float startDay, Room& room) : day(startDay), curentRoom(&room) {}

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
