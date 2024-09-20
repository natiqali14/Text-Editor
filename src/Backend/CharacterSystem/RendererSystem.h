#pragma once
#include <includes.h>
#include <string>
#include "Sqaure.h"

namespace Cold
{
    class RendererSystem {
    public:
        RendererSystem() = default;
        ~RendererSystem() = default;
        static void initialise(const std::string& path);
        static void shutdown();
        static void prepare_data(Character character);
        static void draw_character(Character character, glm::vec2 position, glm::vec2 scale);
    private:
        Sqaure* rect;
        u32 program_id;
        std::string vertex_shader;
        std::string fragment_shader;
        u32 current_program_id;

        glm::mat4 projection;
    };


} // namespace Cold
