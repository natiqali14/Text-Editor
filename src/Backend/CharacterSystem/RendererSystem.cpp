#include "RendererSystem.h"
#include "FontSystem.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  // For glm::ortho

namespace Cold
{
    static RendererSystem* instance = nullptr;
    void RendererSystem::initialise(const std::string& path)
    {
        instance = new RendererSystem;
        FontSystem::initialise(path);

        std::ifstream file ("/Users/frio/Desktop/text_editor/Shaders/text_vert.vert"); // TODO make it relative
        std::stringstream ss;
        ss << file.rdbuf();
        instance->vertex_shader = ss.str();
        std::ifstream file2 ("/Users/frio/Desktop/text_editor/Shaders/text_frag.frag");
        std::stringstream ss2;
        ss2 << file2.rdbuf();
        instance->fragment_shader = ss2.str();

        instance->current_program_id = OpenGLUtility::compile_program(instance->vertex_shader.c_str(), instance->fragment_shader.c_str());

        instance->rect = new Sqaure(instance->current_program_id);
        
        instance->projection = glm::ortho(0.0f,(float)SCR_WIDTH,0.f,(float)SCR_HEIGHT,-1.0f,1.0f);

        i32 loc = glGetUniformLocation(instance->current_program_id, "projection");
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(instance->projection));
        file.close();
        file2.close();
    }

    void RendererSystem::shutdown()
    {
        FontSystem::shutdown();
    }

    void RendererSystem::prepare_data(Character character)
    {
        FontSystem::prepare_font_data(character);
    }

    void RendererSystem::draw_character(Character character, glm::vec2 position, glm::vec2 scale)
    {
        u32 tex_id = FontSystem::get_texture(character);
        instance->rect->draw(tex_id, position, scale);
    }

} // namespace Cold
