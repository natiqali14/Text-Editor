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
        static Sqaure* get_surface();
        static Sqaure* get_cursor_surface();
    private:
        Sqaure* rect = nullptr;
        u32 program_id;
        std::string vertex_shader;
        std::string fragment_shader;
        u32 current_program_id;
        u32 cursor_program_id;
        Sqaure* cursor_surface;

        glm::mat4 projection;
    };


} // namespace Cold
