#pragma once
#include <includes.h>

struct EditorSettings {
    f32 screen_width {800};
    f32 screen_height {600};

    f32 font_pixel_size {100};

    f32 char_scale_factor {0.07};

    f32 line_spacing {10.f};
    f32 offset_on_delete_char{40.f};
    f32 start_x_point {30.f};
    f32 start_y_point {screen_height - (font_pixel_size * char_scale_factor  * 2)};

    glm::vec3 background_color{0,0,0};
    glm::vec3 font_color {1,1,1};
};

inline static EditorSettings editor_settings;