#pragma once
#include <includes.h>

namespace Cold
{
    class Camera {
    public:
        static Camera* get_camera();
        static void delete_camera();
        glm::mat4 get_camera_projection();
        void camera_move_left(f32 points);
        void camera_move_right(f32 points);
        void camera_move_up(f32 points);
        void camera_move_down(f32 points);
        void move_camera_at_start_in_x_axis();
        glm::vec2 get_x_points();
        glm::vec2 get_y_points();
    private:
        Camera();
        ~Camera() = default;
        inline static Camera* instance = nullptr;
        glm::mat4 projection;
        f32 x_start;
        f32 x_end;
        f32 y_start;
        f32 y_end;
    };
} // namespace Cold
