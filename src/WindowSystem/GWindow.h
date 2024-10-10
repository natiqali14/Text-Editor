#pragma once
#include "../includes.h"
#define GLFW_INCLUDE_NONE
#include "../includes/GLFW/glfw3.h"

#include "../Backend/CharacterSystem/Page.h"
#include <string>
class GWindow {
    GLFWwindow* window;
    u16 width;
    u16 height;
    std::string window_name;
public:
    GWindow(u16 w, u16 h, std::string w_name);
    void make_context_current();
    bool should_close_window();
    GLFWwindow* get_window_ptr();
    void key_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void poll_input_events();
    void should_hide_cursor(bool should_hide);
    void swap_buffer();
    ~GWindow();

    // TODO move from here
    void create_page();
private:
    bool b_shit_pressed{false};
    bool b_caps_lock_on{false};
    bool b_some_key_pressed{false};

    void mouse_input_callback(GLFWwindow* window, f64 x_pos, f64 y_pos);

    Cold::Page* page;


};