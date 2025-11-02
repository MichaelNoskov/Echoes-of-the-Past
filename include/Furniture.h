#ifndef FURNITURE_H
#define FURNITURE_H

#include "raylib.h"
#include "Drawable.h"
#include <string>

class Room;

class Furniture {
private:
    float posX;
    float posY;

    bool isPedestal = false;

    // перетаскиваем ли
    bool isDragging = false;

    // пересекается ли с другими предметами
    bool isCollisioning = false;

    // запрещено ли передвигать
    bool freezed = false;

    // кол-во предметов на нём
    int riders = 0;

    // мебель, на которой стоит
    Furniture* pedestal = nullptr;

    // это мы отрисовываем
    Drawable surface;

    // имя
    std::string name;

public:
    Furniture(
        const std::string& texturePath,
        float width, float height, float x = 0.0f, float y = 0.0f, const std::string& furnitureName = "", bool pedestal = false
    );
    Furniture(
        const std::string& textureFrontPath,
        const std::string& textureLeftPath,
        const std::string& textureRightPath,
        float width, float height, float x = 0.0f, float y = 0.0f, const std::string& furnitureName = "", bool pedestal = false
    );
    ~Furniture();

    // метод для обаботки событий внутри мебели
    virtual void Update(Room* room, bool interact);

    // отрисовка
    void Draw(int side = 1);

    // установить позицию
    virtual void SetPosition(float x, float y) { posX = x; posY = y; }

    // позиция
    Vector2 GetPosition() const { return {posX, posY}; }

    // размер
    Vector2 GetSize() const { return surface.GetSize(); };

    // имя
    std::string GetName() const { return name; }

    // хитбокс
    Rectangle GetBoundingBox() const;

    // попадает ли точка на мебель
    bool IsPointInside(float x, float y) const;

    // установить режим перетаскивания
    virtual void Drag(bool drag) { isDragging = drag; };

    // получить текущее состояние перетаскивания
    bool GetDragging() { return isDragging; };

    // установить режим пересечения
    void Collide(bool collision) { isCollisioning = collision; };

    // получить текущее состояние пересечения
    bool GetCollisioning() { return isCollisioning; };

    // установить опору
    void setPedestal(Furniture* newPedestal) { pedestal = newPedestal; };

    // получить опору
    Furniture* GetPedestal() { return pedestal; };

    // На полу на мебели
    bool GetOnPedestal() { return pedestal != nullptr; };

    // можно ли передвигать?
    bool GetFreeze() { return freezed; };

    bool IsPedestal() { return isPedestal; };

    // заблокировать передвижение
    void Freeze() { freezed = true; };

    // разблокировать передвижение
    void Unfreeze() { freezed = false; };

    // пересекается с областью?
    bool IntersectsWithArea(const Rectangle& area) const;

    // пересекается с мебелью?
    bool IntersectsWith(const Furniture& other) const;

    // добавить предмет наверх
    void addItem() {
        riders += 1;
        freezed = true;
        printf("%d\n", riders);
    }

    // снять предмет сверху
    void removeItem() {
        riders -= 1;
        if (riders <= 0) {
            freezed = false;
        }
        printf("%d\n", riders);
    }
};

#endif