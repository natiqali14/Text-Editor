#pragma once

#include <includes.h>
#include "FontSystem.h"
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
namespace Cold
{

    class CharacterInfo {
    public:
        CharacterInfo(CharacterDataSPtr data, glm::vec2 scale, glm::vec2 postion, Character entry);
        ~CharacterInfo();
        
        static std::shared_ptr<CharacterInfo> 
        create_character_object(CharacterDataSPtr data, glm::vec2 scale, glm::vec2 postion, Character entry);

        Character get_character();
        void draw();
        glm::vec2 get_next_position();
        f32 get_backward_position();
        void reset_character_position(glm::vec2 position);
        void reset_character_y_position(f32 y_pos);
        void set_prev_obj(std::shared_ptr<CharacterInfo> object);
        std::shared_ptr<CharacterInfo> get_prev_obj();
    private:
        CharacterDataSPtr data;
        glm::vec2 current_position;
        glm::vec2 current_scale;
        glm::vec2 position_to_ret;
        Character entry;
        f32 size;
        std::shared_ptr<CharacterInfo> prev;
        static inline f32 x_space_factor {0.1};
        u8 selected {0};
    };
    using CharacterSPtr = std::shared_ptr<CharacterInfo>;
} // namespace Cold