#include "Page.h"
#include "RendererSystem.h"
#include "../Camera.h"
#include <iostream>
namespace Cold
{
    static f32 get_y_axis_row_offset() {
        return (editor_settings.line_spacing + (editor_settings.font_pixel_size * editor_settings.char_scale_factor));
    }
    Page::Page()
    {
        character_grid.push_back(RowData(580));
        cursor.current_position = {editor_settings.start_x_point,580};
    }


    Page::~Page()
    {
        
    }

    void Page::add_character(Character entry)
    {
        current_state = ADD_CHARACTER;
        auto c = CharacterInfo::create_character_object(FontSystem::get_character_data(entry)
                                        , {10, 10}
                                        , cursor.current_position
                                        , entry
                                        );
        u32 row_size = character_grid[cursor.current_row].buffer.size();

        /// Setting new character previous object. [Something like a linked list].
        if (cursor.current_index == 0) {
            c->set_prev_obj(nullptr);
        }
        else {
            auto prev_char = character_grid[cursor.current_row].buffer[cursor.current_index - 1];
            c->set_prev_obj(prev_char);
        }

        /// checking if new character is adding between character not at the end
        if (cursor.current_index < row_size) {
            auto next = character_grid[cursor.current_row].buffer[cursor.current_index];
            next->set_prev_obj(c);  // updating next char's previous character
        
            auto prev = c;
            // updating the positions of next characters in current row [Shifting them by 1 character].
            for (int i = cursor.current_index; i < row_size; i++) {
                auto next_char = character_grid[cursor.current_row].buffer[i];
                next_char->reset_character_position(prev->get_next_position());
                prev = next_char;
            }
        }
       
        auto new_pos = c->get_next_position();
        auto it = character_grid[cursor.current_row].buffer.begin();
        if (cursor.current_index == character_grid[cursor.current_row].buffer.size()) {
            character_grid[cursor.current_row].buffer.push_back(c);
        }
        else {
            character_grid[cursor.current_row].buffer.insert(it + cursor.current_index,c);
        }
        //character_grid[cursor.current_row].buffer.push_back(c);
        RendererSystem::prepare_data(entry);
        set_cursor_position(new_pos);
        cursor.current_index++;
    }

    void Page::add_row()
    {
        if (cursor.current_row == character_grid.size() - 1) {
            f32 y_postion = cursor.current_position.y;
            y_postion -= get_y_axis_row_offset();
            character_grid.push_back(RowData());
            cursor.current_row++;
            cursor.current_index = 0;
            set_cursor_position({editor_settings.start_x_point, y_postion});
            character_grid[cursor.current_row].y_pos = cursor.current_position.y;
        }
        else {
            // TODO check this and update
            character_grid.insert(character_grid.begin()+ cursor.current_row+1, RowData());
            auto row = character_grid[cursor.current_row+1];
        }
        
    }

        void Page::move_row(u32 row_to_move, u32 target_row)
        {
            auto& buf = character_grid[target_row].buffer;
            auto& buf_to_move = character_grid[row_to_move].buffer;
            u32 target_buffer_size = buf.size();
            u32 buf_to_move_size = buf_to_move.size();
            f32 y_pos = character_grid[target_row].y_pos;
            f32 x_pos;
            if (!buf_to_move.empty()) {
                auto first_char_to_move = buf_to_move[0];
                CharacterSPtr prev_char = nullptr;
                glm::vec2 last_char_pos;
                if (target_buffer_size > 0) {
                    prev_char = buf[target_buffer_size - 1];
                    last_char_pos = prev_char->get_next_position();
                }
                else {
                    last_char_pos = {editor_settings.start_x_point, y_pos};
                }
                first_char_to_move->set_prev_obj(prev_char);
                first_char_to_move->reset_character_position(last_char_pos);
                buf.push_back(first_char_to_move);
                for(u32 i = 1; i < buf_to_move_size; i++) {
                    auto prev = buf_to_move[i-1];
                    auto next_pos = prev->get_next_position();
                    buf_to_move[i]->reset_character_position(next_pos);
                    buf.push_back(buf_to_move[i]);
                }
            }

            if (buf.empty()) {
                x_pos = editor_settings.start_x_point;
            }
            else {
                x_pos = buf[buf.size() - 1]->get_next_position().x;
            }

            character_grid.erase( character_grid.begin() +row_to_move);
            cursor.current_row = target_row;
            cursor.current_index = buf.size();
            set_cursor_position({x_pos, y_pos});

        }

    void Page::cursor_go_one_row_up()
    {
        move_row(cursor.current_row, cursor.current_row - 1);
    }

    void Page::run_loop()
    {

        for (auto row : character_grid) {
            for (auto c : row.buffer) {
               c->draw();
            }
        }
        f32 y_scale = editor_settings.font_pixel_size * editor_settings.char_scale_factor * 0.8;
        RendererSystem::get_cursor_surface()->draw(0,cursor.current_position, {2, y_scale});
    }

    void Page::delete_current_character()
    {
        auto it = character_grid[cursor.current_row].buffer.begin();
        current_state = DELETE_ENTRY;
        if (cursor.current_index <= 0) {
            if (cursor.current_row <= 0) return;
            cursor_go_one_row_up();
            return;
        }
        auto character_to_delete = character_grid[cursor.current_row].buffer[cursor.current_index - 1];
        auto position_to_minus = character_to_delete->get_backward_position();
        auto new_prev_character = character_to_delete->get_prev_obj();
        if (cursor.current_index >= character_grid[cursor.current_row].buffer.size()) {
            
            character_grid[cursor.current_row].buffer.erase(it + cursor.current_index - 1);
            cursor.current_index--;
            set_cursor_position({cursor.current_position.x - position_to_minus, cursor.current_position.y});
            return;
        }
        auto new_next_character = character_grid[cursor.current_row].buffer[cursor.current_index];
        character_grid[cursor.current_row].buffer.erase(it + cursor.current_index - 1);
        cursor.current_index--;
        for (int i = cursor.current_index; i < character_grid[cursor.current_row].buffer.size(); i++) {
            auto next = character_grid[cursor.current_row].buffer[i];
            next->set_prev_obj(new_prev_character);
            if (new_prev_character)
                next->reset_character_position({new_prev_character->get_next_position()});
            else {
                next->reset_character_position({editor_settings.start_x_point, cursor.current_position.y});
            }
            new_prev_character = next;
        }
        set_cursor_position({cursor.current_position.x - position_to_minus, cursor.current_position.y});

    }

    void Page::cursor_move_forward()
    {
        u32 current_row_size = character_grid[cursor.current_row].buffer.size();
        u32 total_columns = character_grid.size();
        if (cursor.current_index >= current_row_size && cursor.current_row >= total_columns-1) return; // no need to move

        if (cursor.current_index < current_row_size) {
            // move in same line
            auto current_character = character_grid[cursor.current_row].buffer[cursor.current_index];
            cursor.current_index++;
            set_cursor_position(current_character->get_next_position());
        }
        else {
            // go to next row
            cursor.current_index = 0;
            cursor.current_row++;
            set_cursor_position({editor_settings.start_x_point, cursor.current_position.y - get_y_axis_row_offset()});
            
        }
    }

    void Page::cursor_move_backward()
    {
        if (cursor.current_index <=  0 && cursor.current_row <= 0) return;
        if (cursor.current_index <= 0) {
            // go up
            auto buff = character_grid[cursor.current_row - 1].buffer;
            if (buff.size() == 0) {
                set_cursor_position({editor_settings.start_x_point ,cursor.current_position.y + get_y_axis_row_offset() });
            }
            else {
                auto position = buff[buff.size() - 1]->get_next_position();
                set_cursor_position(position);
            }
            cursor.current_row--;
            cursor.current_index = buff.size();
        }
        else {
            // go back same line
            auto buff = character_grid[cursor.current_row].buffer[cursor.current_index-1];
            auto position = buff->get_backward_position();
            set_cursor_position({cursor.current_position.x - position, cursor.current_position.y});
            cursor.current_index--;
        }
        
    }

    void Page::cursor_move_up()
    {
    }

    void Page::cursor_move_down()
    {
    }

    void Page::set_cursor_position(glm::vec2 position)
    {
        glm::vec2 prev = cursor.current_position;
        cursor.current_position = position;
        f32 camera_x_start = Camera::get_camera()->get_x_points().x;
        f32 camera_x_end = Camera::get_camera()->get_x_points().y;

        f32 offset = 0;
        if (current_state == DELETE_ENTRY && camera_x_start > 0.f) {
            offset = editor_settings.offset_on_delete_char;
        }
        if (cursor.current_position.x > camera_x_end) {
            Camera::get_camera()->camera_move_right(cursor.current_position.x - camera_x_end + cursor.current_scale.x+5); // 50 pixels
        }            
        else if (cursor.current_position.x < (camera_x_start + offset)) {
            Camera::get_camera()->camera_move_left(camera_x_start - cursor.current_position.x + offset + editor_settings.start_x_point);
        }

        f32 camera_y_start = Camera::get_camera()->get_y_points().x;
        if (cursor.current_position.y < camera_y_start) {
            f32 points = editor_settings.line_spacing + (editor_settings.font_pixel_size * editor_settings.char_scale_factor);
            Camera::get_camera()->camera_move_down(points);
        }

        else if (cursor.current_position.y > (camera_y_start + editor_settings.screen_height)) {
            f32 points = editor_settings.line_spacing + (editor_settings.font_pixel_size * editor_settings.char_scale_factor);
            Camera::get_camera()->camera_move_up(points);
        }
        
    }

} // namespace Cold
