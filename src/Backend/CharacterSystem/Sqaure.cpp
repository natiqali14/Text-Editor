#include "Sqaure.h"
#include <iostream>
#include "../Camera.h"
namespace Cold
{
    const f32 Sqaure::sqaure_data[24] ={
         // pos      // tex
            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            -1.0f, -01.0f, 0.0f, 0.0f,

            -01.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f
    };

    const u32 Sqaure::index_data[6] = {
        0, 1, 2,
        2, 3, 0
    };

    // void Sqaure::calculate_model()
    // {
    //     model = glm::translate(model, glm::vec3(position.x, position.y, 0));
    //     model = glm::translate(model, glm::vec3(scale.x, scale.y, 1));
    // }

    Sqaure::Sqaure(u32 program_id) : program_id(program_id)
    {
        glERROR("Constructor start");
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 24, sqaure_data, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, sizeof(f32) * 4, (void*) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,2,GL_FLOAT, GL_FALSE, sizeof(f32) * 4, (void*) (sizeof(f32) * 2));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * 6, index_data, GL_STATIC_DRAW);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glERROR("Constructor End");

        
    }

    Sqaure::~Sqaure() {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteVertexArrays(1, &vao);
    }

    bool Sqaure::should_delete()
    {
        if (ref_count <= 0) return true;
        return false;
    }

    void Sqaure::draw(u32 tex_id, glm::vec2 position, glm::vec2 scale)
    {
        glERROR("draw start");
        glBindVertexArray(vao);
       // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glUseProgram(program_id);
        if (tex_id  != 0) {
            glActiveTexture(GL_TEXTURE0 + 0);
            glBindTexture(GL_TEXTURE_2D, tex_id);
            u32 loci = glGetUniformLocation(program_id, "frameTexture");
            glUniform1i(loci+0, 0);
        }
        

        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
        model = glm::translate(model,glm::vec3(scale.x,scale.y,0));
        model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));
        auto m = Camera::get_camera()->get_camera_projection() * model;
        i32 loc = glGetUniformLocation(program_id, "model");
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));

      //  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT ,0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        // glBindBuffer(GL_ARRAY_BUFFER,0);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glERROR("Draw end")
    }
} // namespace Cold
