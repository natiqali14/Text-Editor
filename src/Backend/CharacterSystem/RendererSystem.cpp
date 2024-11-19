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
    bool RendererSystem::initialise(const std::string& path)
    {
        instance = new RendererSystem;
        auto ret = FontSystem::initialise(path);
        if (!ret) return false;

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
        
        file.close();
        file2.close();

        // -------- for cursor ----------

        std::ifstream file3 ("/Users/frio/Desktop/text_editor/Shaders/cursor_frag.frag");
        std::stringstream s3;
        s3 << file3.rdbuf();
        
        std::string cursor_shader = s3.str();
        instance->cursor_program_id = OpenGLUtility::compile_program(instance->vertex_shader.c_str(), cursor_shader.c_str());
        instance->cursor_surface = new Sqaure(instance->cursor_program_id);

        file3.close();
        return true;
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

    Sqaure *RendererSystem::get_surface()
    {
        return instance->rect;
    }

    Sqaure * RendererSystem::get_cursor_surface()
    {
    return instance->cursor_surface;
    }

    void RendererSystem::pass_uniform_to_cursor_float(f32 value, const std::string& name)
    {
        glUseProgram(instance->cursor_program_id);
        auto loc = glGetUniformLocation(instance->cursor_program_id, name.c_str());
        glUniform1f(loc, value);
    }

    void RendererSystem::pass_uniform_to_cursor_float_3(glm::vec3 color, const std::string &name)
    {
        glUseProgram(instance->cursor_program_id);
        auto loc = glGetUniformLocation(instance->cursor_program_id, name.c_str());
        glUniform3f(loc, color.x, color.y, color.z);
    }

    void RendererSystem::pass_uniform_float_3(glm::vec3 color, const std::string& name, u32 p_id) {
        glUseProgram(p_id);
        auto loc = glGetUniformLocation(p_id, name.c_str());
        glUniform3f(loc, color.x, color.y, color.z);
    }

    u32 RendererSystem::get_p_id_for_current() {
        return instance->current_program_id;
    }
    u32 RendererSystem::get_p_id_for_cursor() {
        return instance->cursor_program_id;
    }


} // namespace Cold
