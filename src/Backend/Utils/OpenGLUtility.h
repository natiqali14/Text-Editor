#pragma once
#include <includes.h>
namespace Cold {
    namespace OpenGLUtility {
        u32 compile_shader(const char* const shader, u32 type);
        u32 compile_program(const char* const vertex, const char* const frament);
        void link_program(u32 program_id);
        u32 create_texture(void* data, u32 width, u32 height);

    }
}