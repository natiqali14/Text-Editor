#pragma once

#include <includes.h>
#include <unordered_map>
#include <string>
#include <ft2build.h>
#include <memory>
#include FT_FREETYPE_H

#include "../Utils/OpenGLUtility.h"
namespace Cold
{
    struct CharacterData {
        u32 width;
        u32 height;
        void* data {nullptr};
        u32 tex_id {0};
        f32 relative_width;
        f32 relative_height;
        f32 bearing_x;
        f32 bearing_y;
        f32 advance;
        CharacterData(u32 w, u32 h, void* d, u32 id, f32 rw, f32 rh, f32 bx, f32 by, f32 a)
        : width(w), height(h), data(d), tex_id(id), relative_width(rw), relative_height(rh), bearing_x(bx), bearing_y(by), advance(a)
        {}
        CharacterData(){};
    };

    using CharacterDataSPtr = std::shared_ptr<CharacterData>;

    class FontSystem {
    public:
        static void initialise(const std::string& path);
        static void shutdown();
       
        static void prepare_font_data(Character character);
        static void prepare_texture(Character character);
        static u32 get_texture(Character character);
        static CharacterDataSPtr get_character_data(Character character);
    private:
        u32 current_pixel_size {100};
        static void prepare_all_characters();
        FT_Library font_lib;
        FT_Face font_face;
        std::unordered_map<Character, CharacterDataSPtr> font_data;
        std::string current_path;
    };
} // namespace Cold
