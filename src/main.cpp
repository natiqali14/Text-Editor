
#include "../includes/glad/glad.h"
#include <iostream>

#include <WindowSystemUtility.h>
#include <GWindow.h>
#include "Backend/CharacterSystem/RendererSystem.h"
#include <filesystem>
#include <Clock.h>
#include "EditorSettings.h"
#include "FileHelper.h"
int main(int args, char* argv[]) {


    std::string file;
    std::string input_file;
    if (args > 2) {
        file  = argv[1];
        input_file = argv[2];
    }
    else {
        std::cerr << "Invalid arguments\n";
        return 1;
    }
    // --------------------------------  Creating main window  --------------------------------

    WindowSystemUtility::initialise_glfw();
    std::unique_ptr<GWindow> main_window = std::make_unique<GWindow>(editor_settings.screen_width, editor_settings.screen_height, "Text Editor");
    main_window->should_hide_cursor(false);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

   
    // --------------------------------  Creating main window  --------------------------------
    auto ret = Cold::RendererSystem::initialise(file);
    if (!ret) return 1;

    main_window->create_page(input_file);
    
    glEnable(GL_BLEND);  //enable blinding
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  //to handel transperancy images
    
    Cold::Clock main_clock = Cold::Clock();
    auto start_time = main_clock.get_time_in_us();
    auto poll_event_time = main_clock.get_time_in_us();
    while (!main_window->should_close_window()) 
    {
        auto loop_start_time = main_clock.get_time_in_us();
        glClearColor(editor_settings.background_color.x,editor_settings.background_color.y,editor_settings.background_color.z,1);
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