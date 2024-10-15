#include "Camera.h"
#include <iostream>
namespace Cold
{
    Camera *Camera::get_camera()
    {
        if (instance) return instance;
        instance = new Camera;
        return instance;
    }

    void Camera::delete_camera()
    {
        delete instance;
        instance = nullptr;
    }

    glm::mat4 Camera::get_camera_projection()
    {
        return projection;
    }

    void Camera::camera_move_left(f32 points)
    {
        std::cout << "[camera_move_left] " << x_start << " " << x_end << " "<< points <<  "\n";
        if ((x_start - points) < 0) {
            points = x_start;
        }
        x_start -= points;
        x_end -= points;
        
        projection = glm::ortho(x_start, x_end, y_start, y_end, -1.f, 1.f);

    }

    void Camera::camera_move_right(f32 points)
    {
        x_start += points;
        x_end += points;
        projection = glm::ortho(x_start, x_end, y_start, y_end, -1.f, 1.f);
        
    }

    void Camera::camera_move_up(f32 points)
    {
        if ((y_end + points > editor_settings.screen_height)) return;
        y_start += points;
        y_end += points;
        projection = glm::ortho(x_start, x_end, y_start, y_end, -1.f, 1.f);
    }

    void Camera::camera_move_down(f32 points)
    {
        y_start -= points;
        y_end -= points;
        projection = glm::ortho(x_start, x_end, y_start, y_end, -1.f, 1.f);
    }

    void Camera::move_camera_at_start_in_x_axis()
    {
        x_start = 0;
        x_end = editor_settings.screen_width;
        projection = glm::ortho(x_start, x_end, y_start, y_end, -1.f, 1.f);
    }

    glm::vec2 Camera::get_x_points()
    {
        return {x_start, x_end};
    }

    glm::vec2 Camera::get_y_points()
    {
        return {y_start, y_end};
    }

    Camera::Camera() : x_start(0), x_end(editor_settings.screen_width), y_start(0), y_end(editor_settings.screen_height)
    {
        projection = glm::ortho(x_start, x_end, y_start, y_end, -1.f, 1.f);
    }

} // namespace Cold
