
#include "../includes/glad/glad.h"
#include <iostream>

#include <WindowSystemUtility.h>
#include <GWindow.h>
#include "Backend/CharacterSystem/RendererSystem.h"
#include <Clock.h>
#include "FileHelper.h"
int main() {


    // --------------------------------  Creating main window  --------------------------------

    WindowSystemUtility::initialise_glfw();
    std::unique_ptr<GWindow> main_window = std::make_unique<GWindow>(editor_settings.screen_width, editor_settings.screen_height, "Text Editor");
    main_window->should_hide_cursor(false);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

   
    // --------------------------------  Creating main window  --------------------------------
    Cold::RendererSystem::initialise("/Users/frio/Desktop/text_editor/Roboto-Light.ttf");

    main_window->create_page();
    
    glEnable(GL_BLEND);  //enable blinding
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  //to handel transperancy images
    
    Cold::Clock main_clock = Cold::Clock();
    auto start_time = main_clock.get_time_in_us();
    auto poll_event_time = main_clock.get_time_in_us();
    while (!main_window->should_close_window()) 
    {
        auto loop_start_time = main_clock.get_time_in_us();
        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        main_window->swap_buffer();
        if (loop_start_time - poll_event_time > 1) {
            main_window->poll_input_events();
            poll_event_time = main_clock.get_time_in_us();
        }
        
    }
    Cold::FileHelper::close();
    return 0;
}