#include "FontSystem.h"
#include <iostream>


namespace Cold
{
    static FontSystem* instance = nullptr;;
    void FontSystem::initialise(const std::string& path)
    {
        instance = new FontSystem;
        instance->current_path = path;
        if (FT_Init_FreeType(&instance->font_lib)) {
            std::cerr << "Could not initialize FreeType library" << std::endl;
        }

        if (FT_New_Face(instance->font_lib, path.c_str(), 0, &instance->font_face)) {
            std::cerr << "Could not load font" << std::endl;
        }

        // Set the font size (e.g., 48 pixels)
        FT_Set_Pixel_Sizes(instance->font_face, 0, instance->current_pixel_size);

        prepare_all_characters();
    }

    void FontSystem::shutdown()
    {
        FT_Done_Face(instance->font_face);       // When you're done with the specific font face
        FT_Done_FreeType(instance->font_lib);     // When you're done with FreeType entirely
    }

    

    void FontSystem::prepare_font_data(Character character)
    {
        if (instance->font_data.count(character)) return;

        if (FT_Load_Char(instance->font_face, character, FT_LOAD_RENDER)) {
            std::cerr << "Failed to load glyph" << std::endl;
        }
        auto bitmap = instance->font_face->glyph->bitmap;
        auto g = instance->font_face->glyph;
        auto metric = g->metrics;
        
        f32 size = instance->current_pixel_size;
        auto data = std::shared_ptr<CharacterData>(new CharacterData(
                        bitmap.width,
                        bitmap.rows,
                        bitmap.buffer,
                        0, 
                        (metric.width/64.f)/size, 
                        (metric.height/64.f)/size, 
                        (metric.horiBearingX/64)/size, 
                        (metric.horiBearingY/64)/size, 
                        (metric.horiAdvance/64)/size
                    ));

        std::cout<< character << " Width " << data->width << " Height " << data->height << " glyph " << data->advance  << " " << data->bearing_x << " " << data->bearing_y << " "<< data->relative_width << " " <<data->relative_height <<"\n";


        instance->font_data.insert({character, std::move(data)});

    }

    void FontSystem::prepare_texture(Character character)
    {
        if (!instance->font_data.count(character)) prepare_font_data(character);
        auto& data = instance->font_data[character];
        data->tex_id = OpenGLUtility::create_texture(data->data, data->width, data->height);
    }

    u32 FontSystem::get_texture(Character character)
    {
        if (!instance->font_data.count(character)) prepare_font_data(character);
        if (instance->font_data[character]->tex_id == 0) prepare_texture(character);
        return instance->font_data[character]->tex_id;
    }

    CharacterDataSPtr FontSystem::get_character_data(Character character)
    {
        return instance->font_data.at(character);
    }

    void FontSystem::prepare_all_characters()
    {
        for(u8 i = 32; i < 127; i++) {
            FontSystem::prepare_texture(i);
        }
    }

} // namespace Cold
