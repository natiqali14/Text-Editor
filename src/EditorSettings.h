#pragma once
#include <includes.h>

struct EditorSettings {
    f32 screen_width {800};
    f32 screen_height {600};

    f32 font_pixel_size {100};

    f32 char_scale_factor {0.12};

    f32 line_spacing {10.f};
    f32 offset_on_delete_char{40.f};
    f32 start_x_point {30.f};
};

inline static EditorSettings editor_settings;