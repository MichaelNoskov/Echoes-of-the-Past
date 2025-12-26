#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include "raylib.h"

using json = nlohmann::json;

struct DialogueOption {
    std::string text;
    int mood_change = 0;
    std::string next_node;
    
    static DialogueOption from_json(const json& j) {
        DialogueOption option;
        option.text = j.value("text", "");
        option.mood_change = j.value("mood_change", 0);
        
        if (j.contains("next") && !j["next"].is_null()) {
            option.next_node = j["next"].get<std::string>();
        }
        
        return option;
    }
};

struct DialogueButton {
    Rectangle bounds;
    DialogueOption option;
    
    bool IsMouseOver() const {
        return CheckCollisionPointRec(GetMousePosition(), bounds);
    }
    
    void Draw(bool is_hovered, Font& font) const {
        Color bg_color = is_hovered ? Color{70, 70, 90, 255} : Color{50, 50, 70, 255};
        DrawRectangleRec(bounds, bg_color);
        DrawRectangleLinesEx(bounds, 2, is_hovered ? YELLOW : GRAY);

        const char* button_text = option.text.c_str();
        Vector2 text_size = MeasureTextEx(font, button_text, 20, 2);
        Vector2 text_pos = {
            bounds.x + (bounds.width - text_size.x) / 2,
            bounds.y + (bounds.height - text_size.y) / 2
        };
        DrawTextEx(font, button_text, text_pos, 20, 2, WHITE);
    }
};

struct DialogueNode {
    std::string id;
    std::string text;
    std::vector<DialogueOption> options;
    std::vector<DialogueButton> buttons;
    bool is_end_node = false;

    DialogueNode(const std::string& node_id, const json& j) {
        id = node_id;
        text = j.value("text", "");
        
        if (j.contains("options") && j["options"].is_array()) {
            for (const auto& option_json : j["options"]) {
                options.push_back(DialogueOption::from_json(option_json));
            }
        }
        
        // Узел считается конечным, если у него нет вариантов выбора
        is_end_node = options.empty();
    }

    void create_buttons(float start_y, float button_height, float button_spacing, 
                       float available_width, float start_x = 30.0f) {
        buttons.clear();
        
        if (options.empty()) return;
        
        int option_count = static_cast<int>(options.size());
        float button_width = (available_width - (option_count - 1) * button_spacing) / option_count;
        
        for (int i = 0; i < option_count; i++) {
            DialogueButton button;
            button.option = options[i];
            
            button.bounds = {
                start_x + i * (button_width + button_spacing),
                start_y,
                button_width,
                button_height
            };
            
            buttons.push_back(button);
        }
    }
};

class DialogueSystem {
private:
    std::map<std::string, DialogueNode> dialogue_tree_;
    DialogueNode* current_node_ = nullptr;
    int npc_mood_ = 0;
    bool is_active_ = false;
    bool is_finished_ = false;
    Font font_;

    void load_dialogue_tree(const json& dialogue_json);
    void reset();
    void select_option_by_button(const DialogueButton& button);
    
public:
    DialogueSystem(const json& dialogue_json, bool active = true);
    ~DialogueSystem();
    
    void Update();
    void Draw();
    
    int GetNpcMood() const { return npc_mood_; }
    bool IsActive() const { return is_active_; }
    bool IsFinished() const { return is_finished_; }
    
    void Reload(const json& dialogue_json);
    
    // Метод для принудительного завершения диалога
    void EndDialogue();
};
