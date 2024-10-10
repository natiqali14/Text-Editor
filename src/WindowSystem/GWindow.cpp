#include "GWindow.h"
#include "WindowSystemUtility.h"
#include <iostream>
GWindow::GWindow(u16 w, u16 h, std::string w_name)
: width(w)
, height(h)
, window_name(std::move(w_name))

{
    // window creation
    window = glfwCreateWindow(width, height, window_name.c_str(), nullptr, nullptr);
    // error on failure
    if(window == NULL) throw std::runtime_error("GLFW window creation failed");
    // for now when create a new GLFW window, this window will be current context for opneGL.
    glfwMakeContextCurrent(window); 
    // frame buffer size callback
    glfwSetFramebufferSizeCallback(window, &WindowSystemUtility::frame_buffer_size_callback);

    // For now following settings are for all windows
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE,1);
    glfwSetWindowUserPointer(window, this);
    auto lambda_ftn = [](GLFWwindow *w, int key, int scancode, int action, int mods) {
        GWindow* this_window = static_cast<GWindow*> (glfwGetWindowUserPointer(w));
        this_window->key_input_callback(w,key,scancode,action,mods);
    };

    auto mouse_input_ftn_ptr = [](GLFWwindow* w, f64 x_pos, f64 y_pos) {
        GWindow* my_window = static_cast<GWindow*> (glfwGetWindowUserPointer(w));
        my_window->mouse_input_callback(w,x_pos,y_pos);
    };

    glfwSetKeyCallback(window, lambda_ftn);
    glfwSetCursorPosCallback(window, mouse_input_ftn_ptr);

}

void GWindow::make_context_current()
{
    glfwMakeContextCurrent(window);
}

bool GWindow::should_close_window()
{
    return glfwWindowShouldClose(window);
}

GLFWwindow *GWindow::get_window_ptr()
{
    return window;
}

void GWindow::key_input_callback(GLFWwindow *w, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, 1);
    }
    switch (key)
    {
    case GLFW_KEY_RIGHT_SHIFT:
    case GLFW_KEY_LEFT_SHIFT:
    {
        if (action == GLFW_PRESS) b_shit_pressed = true;
        if (action == GLFW_RELEASE) b_shit_pressed = false;
        break;
    }


    case 32 ... 126: {
        if (action == GLFW_RELEASE) break;
        if (key >= 65 && key <= 90) {
            if (!(b_shit_pressed)) key += 32;
            page->add_character(key);
        }
        else
            page->add_character(key);
        break;
    }

    case GLFW_KEY_BACKSPACE: {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            page->delete_current_character();
        }
        
        break;
    }

    case GLFW_KEY_ENTER: {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            page->add_row();
        }
    }
    case GLFW_KEY_RIGHT: {
        if (action == GLFW_RELEASE) return;
        page->cursor_move_forward();
        break;
    }
    case GLFW_KEY_LEFT: {
        if (action == GLFW_RELEASE) return;
        page->cursor_move_backward();
        break;
    }
    
    default:
        break;
    }
}

void GWindow::poll_input_events()
{
    glfwPollEvents();
    // TODO
}

void GWindow::should_hide_cursor(bool should_hide)
{
    u32 value = should_hide ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
    glfwSetInputMode(window, GLFW_CURSOR, value);
}

void GWindow::swap_buffer()
{
    page->run_loop();
    glfwSwapBuffers(window);
}

GWindow::~GWindow()
{
    glfwSetWindowShouldClose(window, 1);
    glfwDestroyWindow(window);
}

void GWindow::create_page()
{
    page = new Cold::Page;
}

void GWindow::mouse_input_callback(GLFWwindow *window, f64 x_pos, f64 y_pos)
{
   // TODO
}
