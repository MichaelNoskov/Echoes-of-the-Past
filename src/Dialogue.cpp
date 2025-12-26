#include "Dialogue.h"
#include <iostream>

// Загрузка шрифта (упрощенная)
Font LoadRussianFont() {
    const char* fontPath = "res/fonts/RobotoMono.ttf";
    
    if (!FileExists(fontPath)) {
        std::cerr << "Warning: Font file not found: " << fontPath << std::endl;
        return GetFontDefault();
    }
    
    // Базовые кодовые точки
    std::vector<int> codepoints;
    for (int i = 32; i < 127; i++) codepoints.push_back(i);    // ASCII
    for (int i = 1024; i < 1104; i++) codepoints.push_back(i); // Кириллица
    for (int i = 1105; i < 1106; i++) codepoints.push_back(i); // Ёё
    
    Font font = LoadFontEx(fontPath, 32, codepoints.data(), static_cast<int>(codepoints.size()));
    
    if (font.texture.id == 0) {
        std::cerr << "Error: Failed to load font" << std::endl;
        return GetFontDefault();
    }
    
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    return font;
}

// Конструктор
DialogueSystem::DialogueSystem(const json& dialogue_json, bool active) 
    : font_(LoadRussianFont()), is_finished_(false) {
    load_dialogue_tree(dialogue_json);
    is_active_ = active;
    is_finished_ = !active;
}

// Деструктор
DialogueSystem::~DialogueSystem() {
    if (font_.texture.id != 0 && font_.texture.id != GetFontDefault().texture.id) {
        UnloadFont(font_);
    }
}

// Загрузка дерева диалогов
void DialogueSystem::load_dialogue_tree(const json& dialogue_json) {
    dialogue_tree_.clear();
    npc_mood_ = 0; // Сбрасываем настроение при загрузке нового диалога
    is_finished_ = false;
    
    if (dialogue_json.contains("dialogue_tree")) {
        const auto& tree = dialogue_json["dialogue_tree"];
        
        for (auto it = tree.begin(); it != tree.end(); ++it) {
            dialogue_tree_.emplace(it.key(), DialogueNode(it.key(), it.value()));
        }
    }
    
    reset();
}

// Сброс диалога
void DialogueSystem::reset() {
    if (dialogue_tree_.empty()) {
        is_active_ = false;
        is_finished_ = true;
        return;
    }
    
    auto it = dialogue_tree_.find("start");
    if (it != dialogue_tree_.end()) {
        current_node_ = &it->second;
        is_active_ = true;
        is_finished_ = false;
        
        // Если стартовый узел уже конечный (нет вариантов выбора)
        if (current_node_->is_end_node) {
            is_active_ = false;
            is_finished_ = true;
        }
    } else {
        std::cerr << "Error: No 'start' node found" << std::endl;
        is_active_ = false;
        is_finished_ = true;
    }
}

// Выбор варианта
void DialogueSystem::select_option_by_button(const DialogueButton& button) {
    // Обновляем настроение NPC
    npc_mood_ += button.option.mood_change;
    
    // Если нет следующего узла - завершаем диалог
    if (button.option.next_node.empty()) {
        EndDialogue();
        return;
    }
    
    auto it = dialogue_tree_.find(button.option.next_node);
    if (it != dialogue_tree_.end()) {
        current_node_ = &it->second;
        
        // Если текущий узел конечный (нет вариантов выбора) - завершаем диалог
        if (current_node_->is_end_node) {
            EndDialogue();
        }
    } else {
        std::cerr << "Error: Next node '" << button.option.next_node << "' not found" << std::endl;
        EndDialogue();
    }
}

// Принудительное завершение диалога
void DialogueSystem::EndDialogue() {
    is_active_ = false;
    is_finished_ = true;
    current_node_ = nullptr;
}

// Обновление
void DialogueSystem::Update() {
    if (!is_active_ || is_finished_ || !current_node_) return;
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse_pos = GetMousePosition();
        
        for (const auto& button : current_node_->buttons) {
            if (CheckCollisionPointRec(mouse_pos, button.bounds)) {
                select_option_by_button(button);
                return;
            }
        }
    }
}

// Отрисовка
void DialogueSystem::Draw() {
    if (!is_active_ || is_finished_ || !current_node_) return;
    
    int screen_width = GetScreenWidth();
    int screen_height = GetScreenHeight();
    
    // === 1. Фон диалогового окна ===
    float window_height = 250.0f;
    
    // === 2. Текст NPC ===
    float text_x = 30.0f;
    float text_y = static_cast<float>(screen_height) - window_height + 20.0f;
    
    // Текст NPC
    DrawTextEx(font_, current_node_->text.c_str(), Vector2{text_x, text_y}, 48, 2.0f, WHITE);
    text_y += 60.0f;
    
    // === 3. Создание и отрисовка вариантов ответа ===
    float option_start_y = text_y;
    float option_spacing = 35.0f;
    float available_width = static_cast<float>(screen_width) - 60.0f;
    
    // Создаем области для клика
    current_node_->create_buttons(option_start_y, 30.0f, 
                                 option_spacing, available_width, 30.0f);
    
    // Отрисовываем варианты ответов как простой текст
    for (size_t i = 0; i < current_node_->buttons.size(); i++) {
        const auto& button = current_node_->buttons[i];
        bool is_hovered = button.IsMouseOver();
        
        // Цвет текста - подсвечиваем при наведении
        Color text_color = is_hovered ? YELLOW : LIGHTGRAY;
        
        // Просто текст, без фона и рамок
        DrawTextEx(font_, button.option.text.c_str(), 
                  Vector2{button.bounds.x, button.bounds.y}, 
                  40, 2.0f, text_color);
    }
}

// Перезагрузка
void DialogueSystem::Reload(const json& dialogue_json) {
    load_dialogue_tree(dialogue_json);
}
