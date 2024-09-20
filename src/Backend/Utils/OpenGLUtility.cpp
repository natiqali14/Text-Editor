#include "OpenGLUtility.h"
#include "../../includes/glad/glad.h"
#include <vector>
#include <iostream>
namespace Cold
{
    namespace OpenGLUtility
    {
        u32 compile_shader(const char *const shader, u32 type) 
        {
            GLuint id = glCreateShader(type);
            glShaderSource(id, 1, &shader, nullptr);
            glCompileShader(id);//compile shader
            //Error Handeling
            int result;
            glGetShaderiv(id, GL_COMPILE_STATUS, &result);
            if (result == GL_FALSE)
            {
                int length;
                glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
                if(length>=0) {
                    std::vector<char> message(length);  // vector char instead of alloca to avoid memory errors

                    glGetShaderInfoLog(id, length, &length, message.data());

                    if (type == GL_VERTEX_SHADER) {
                        std::cout << "Error vertex shader failed" << " " << message.data() << "\n";
                    } else {
                        std::cout << "Error fragment shader failed" << " " << message.data() << "\n";
                    }
                }
                glDeleteShader(id);
                return 0;
            }
            return id;
        }

        u32 compile_program(const char *const vertex, const char *const fragment)
        {
            //build a program to run on the cpu and attach video.shaders to it
            uint32_t program = glCreateProgram();
            //create vertex shader
            uint32_t vs = compile_shader(vertex, GL_VERTEX_SHADER);
            //create fragment shader
            uint32_t fs = compile_shader(fragment, GL_FRAGMENT_SHADER);
            //attach video.shaders to program
            glAttachShader(program, vs);
            glAttachShader(program, fs);
            glLinkProgram(program);
            i32 s;
            glGetProgramiv(program, GL_LINK_STATUS, &s);
            if (!s)
            {
                char info[512];
                glGetProgramInfoLog(program, 512, NULL, info);
                std::cout << "program linking failed. \n"
                        << info << "\n";
            }
         //   glValidateProgram(program);

            glDeleteShader(vs);
            glDeleteShader(fs);
            return program;
        }

        void link_program(u32 program_id)
        {
            glUseProgram(program_id);
        }

        u32 create_texture(void *data, u32 width, u32 height)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
            u32 id;
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  //minification filter
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  //magnification filter
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  //clamp horizontal
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  //clamp vertical  
            glTexImage2D(GL_TEXTURE_2D,0,GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);

            glBindTexture(GL_TEXTURE_2D,0);
            
            return id;

        }

    } // namespace OpenGLUtility
    
} // namespace Cold
