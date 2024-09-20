
#include "../includes/glad/glad.h"
#include <iostream>

#include <WindowSystemUtility.h>
#include <GWindow.h>
#include "Backend/CharacterSystem/RendererSystem.h"
int main() {


    // --------------------------------  Creating main window  --------------------------------

    WindowSystemUtility::initialise_glfw();
    std::unique_ptr<GWindow> main_window = std::make_unique<GWindow>(SCR_WIDTH, SCR_HEIGHT, "OPENGL WINDOW");
    main_window->should_hide_cursor(false);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    main_window->create_page();
   
    // --------------------------------  Creating main window  --------------------------------

    
    std::cout << "Hello World\n";
    Cold::RendererSystem::initialise("/Users/frio/Desktop/text_editor/Roboto-Light.ttf");
    //glViewport(0,0,SCR_WIDTH, SCR_HEIGHT);
    // glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);//enable blinding
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//to handel transperancy images
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);  // Or GL_FRONT
    // glFrontFace(GL_CCW);  // Or GL_CW based on your data
    while (!main_window->should_close_window()) 
    {
        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        main_window->swap_buffer();
        main_window->poll_input_events();
    }

    return 0;
}