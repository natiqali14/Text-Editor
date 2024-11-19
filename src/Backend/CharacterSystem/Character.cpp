#include "Character.h"
#include  "SqaureManager.h"
#include "RendererSystem.h"
#include <iostream>
namespace Cold
{
    CharacterInfo::CharacterInfo(CharacterDataSPtr data, glm::vec2 scale , glm::vec2 position, Character entry) 
    : data(std::move(data)), current_scale(scale), current_position(position), entry(entry)
    {
        size = editor_settings.char_scale_factor * editor_settings.font_pixel_size;
        position_to_ret = current_position;
        current_position.x += (this->data->bearing_x * size * 2);
        current_position.y -= ((this->data->relative_height - this->data->bearing_y) * size * 2);
        this->current_scale.x = this->data->width * editor_settings.char_scale_factor;
        this->current_scale.y = this->data->height * editor_settings.char_scale_factor; 
        position_to_ret.x += (this->data->advance * size * 2) + (size * x_space_factor);
        std::cout << "CharacterInfo Constructed\n";
    }

    CharacterInfo::~CharacterInfo()
    {
        std::cout << "CharacterInfo Destructed\n";
    }

    std::shared_ptr<CharacterInfo> CharacterInfo::create_character_object(CharacterDataSPtr data, glm::vec2 scale, glm::vec2 position, Character entry)
    {
        return std::make_shared<CharacterInfo>(data, scale, position, entry);
    }

    Character CharacterInfo::get_character()
    {
        return entry;   
    }

    void CharacterInfo::draw()
    {
        RendererSystem::get_surface()->draw(data->tex_id, current_position, current_scale);
    }

    glm::vec2 CharacterInfo::get_next_position()
    {    
        return position_to_ret;
    }

    f32 CharacterInfo::get_backward_position()
    {
        return (data->advance * size * 1.9);
    }

    void CharacterInfo::reset_character_position(glm::vec2 position)
    {
        current_position = position;
        position_to_ret = current_position;
        current_position.x += (this->data->bearing_x * size * 2);
        current_position.y -= ((this->data->relative_height - this->data->bearing_y) * size * 2);
        this->current_scale.x = this->data->width * editor_settings.char_scale_factor;
        this->current_scale.y = this->data->height * editor_settings.char_scale_factor;
        position_to_ret.x += (this->data->advance * size * 2)+ (size * x_space_factor);
    }

    void CharacterInfo::reset_character_y_position(f32 y_pos)
    {
        current_position.y = y_pos;
        reset_character_position(current_position);

    }

    void CharacterInfo::set_prev_obj(std::shared_ptr<CharacterInfo> object)
    {
        prev = object;
    }

    std::shared_ptr<CharacterInfo> CharacterInfo::get_prev_obj()
    {
        return prev;
    }

    
} // namespace Cold
