#pragma once

#include <includes.h>
#include "FontSystem.h"
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace Cold
{
    class CharacterInfo {
    public:
        CharacterInfo(CharacterDataSPtr data, glm::vec2 scale, glm::vec2 postion);
        ~CharacterInfo();
    private:
        CharacterDataSPtr data;
        glm::vec2 current_position;
        glm::vec2 current_scale;
    };
} // namespace Cold