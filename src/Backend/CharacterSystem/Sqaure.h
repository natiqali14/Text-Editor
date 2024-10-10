#pragma once

#include <includes.h>

#include "../../includes/glad/glad.h"

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Cold
{
    class Sqaure {
        friend class SqaureManager;
        u32 vbo;
        u32 ebo;
        u32 vao;
        const static f32 sqaure_data[24];
        const static u32 index_data[6];
        u32 program_id {0};
        u32 current_texture_id {0};
        u32 ref_count{1};
    public:
        Sqaure(u32 program_id);
        ~Sqaure();
        bool should_delete();
        void draw(u32 tex_id, glm::vec2 position, glm::vec2 scale);
    };
    
} // namespace Cold
