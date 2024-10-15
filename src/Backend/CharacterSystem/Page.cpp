#include "Page.h"
#include "RendererSystem.h"
#include "../Camera.h"
#include "../../FileHelper.h"
#include <iostream>
#include <sstream>
namespace Cold
{
    static f32 get_y_axis_row_offset() {
        return (editor_settings.line_spacing + (editor_settings.font_pixel_size * editor_settings.char_scale_factor));
    }
    Page::Page(i32 fd): current_fd(fd)
    {
        character_grid.push_back(RowData(580));
        cursor.current_position = {editor_settings.start_x_point,580};
    }

    void Page::add_existing_content()
    {
        auto data = FileHelper::get_next_line(current_fd);
        bool content  = data.first;
        int i = 0;
        while (content) {
            if (i != 0) {
                add_row();
            }
            i++;
            for (auto c : data.second) {
                add_character(c);
            }
            data = FileHelper::get_next_line(current_fd);
            content = data.first;
        }
        FileHelper::reset_flags_for_writing(current_fd);
        set_cursor_position({editor_settings.start_x_point, 580});
        cursor.current_row = 0;
        cursor.current_index = 0;
        
    }

    void Page::write_back_data()
    {
        FileHelper::close_file_and_open_it_again(current_fd);
        for (auto& buffer : character_grid) {
            std::stringstream ss;
            for (auto c_buffer : buffer.buffer) {
                ss << c_buffer->get_character();
            }
            std::cout << ss.str() << "\n";
            ss << '\n';
            FileHelper::write_string(current_fd, ss.str());
        }
    }

    Page::~Page()
    {
        write_back_data();
    }

    void Page::add_character(Character entry)
    {
        std::cout << "Page::add_character\n";
        current_state = ADD_CHARACTER;
        auto current_row = character_grid[cursor.current_row];
        auto c = CharacterInfo::create_character_object(FontSystem::get_character_data(entry)
                                        , {10, 10}
                                        , {cursor.current_position.x, current_row.y_pos}
                                        , entry
                                        );
        u32 row_size = character_grid[cursor.current_row].buffer.size();

        /// Setting new character previous object. [Something like a linked list].
        if (cursor.current_index == 0) {
            c->set_prev_obj(nullptr);
        }
        else {
            auto prev_char = current_row.buffer[cursor.current_index - 1];
            c->set_prev_obj(prev_char);
        }

        /// checking if new character is adding between character not at the end
        if (cursor.current_index < row_size) {
            auto next = current_row.buffer[cursor.current_index];
            next->set_prev_obj(c);  // updating next char's previous character
        
            auto prev = c;
            // updating the positions of next characters in current row [Shifting them by 1 character].
            for (int i = cursor.current_index; i < row_size; i++) {
                auto next_char = current_row.buffer[i];
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
        f32 y_postion = cursor.current_position.y;
        y_postion -= get_y_axis_row_offset();
        if (cursor.current_row >= character_grid.size() -1) {
            character_grid.push_back(RowData(y_postion));
        }
        else {
            character_grid.insert(character_grid.begin() + cursor.current_row + 1, RowData(y_postion));
        }
        
        auto& buffer_to_move = character_grid[cursor.current_row+1].buffer;
        std::vector<CharacterSPtr> temp_buffer;
        auto& current_buffer = character_grid[cursor.current_row].buffer;
        if (cursor.current_index < current_buffer.size()) {
            auto it = current_buffer.begin();
            u32 current_buff_size = current_buffer.size();
            current_buffer[cursor.current_index]->set_prev_obj(nullptr);
            for (u32 i = cursor.current_index; i < current_buff_size; i++) {
                temp_buffer.push_back(current_buffer[i]);
            }
    
            current_buffer.erase(it + cursor.current_index, current_buffer.end());
            
            move_row(temp_buffer, buffer_to_move, y_postion);
        }

        f32 x_pos = editor_settings.start_x_point;
        if (!buffer_to_move.empty()) {
            x_pos = buffer_to_move[buffer_to_move.size() - 1]->get_next_position().x;
        }
        cursor.current_row++;
        cursor.current_index = buffer_to_move.size();
        set_cursor_position({x_pos, y_postion});

        u32 next_row = cursor.current_row+1;
        f32 row_offset = get_y_axis_row_offset();
        for (u32 i = next_row; i < character_grid.size(); i++) {
            f32 row_y_pos = character_grid[i].y_pos;
            character_grid[i].y_pos = row_y_pos - row_offset;
            for (auto& char_info : character_grid[i].buffer) {
                char_info->reset_character_y_position(character_grid[i].y_pos);
            }
        }
        
    }

        void Page::move_row(std::vector<CharacterSPtr>& buffer_to_move, std::vector<CharacterSPtr>& target_buffer, f32 y_pos)
        {
            auto& buf = target_buffer;
            auto& buf_to_move = buffer_to_move;
            u32 target_buffer_size = buf.size();
            u32 buf_to_move_size = buf_to_move.size();
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
        }

    void Page::cursor_go_one_row_up()
    {
        f32 y_pos = character_grid[cursor.current_row - 1].y_pos;
        move_row(character_grid[cursor.current_row].buffer, character_grid[cursor.current_row - 1].buffer, y_pos);
        u32 buffer_size = character_grid[cursor.current_row - 1].buffer.size();
        glm::vec2 new_pos;
        if (buffer_size > 0) {
            new_pos.x = character_grid[cursor.current_row-1].buffer[buffer_size-1]->get_next_position().x;   
        }
        else {
            new_pos.x = editor_settings.start_x_point;;
        }
        new_pos.y = y_pos;
        character_grid.erase( character_grid.begin() + cursor.current_row);
        cursor.current_row -= 1;
        cursor.current_index = buffer_size;
        set_cursor_position(new_pos);
        u32 row = cursor.current_row+1;
        f32 row_offset = get_y_axis_row_offset();
        for (u32 i = row; i <= character_grid.size() - 1; i++) {
            f32 row_y_pos = character_grid[i].y_pos;
            for (auto& char_info : character_grid[i].buffer) {
                f32 new_y_pos = row_offset + row_y_pos;
                char_info->reset_character_y_position(new_y_pos);
                character_grid[i].y_pos = new_y_pos;
            }
        }
    }

    void Page::run_loop()
    {

        for (auto row : character_grid) {
            for (auto c : row.buffer) {
               c->draw();
            }
        }
        f32 y_scale = editor_settings.font_pixel_size * editor_settings.char_scale_factor * 0.8;
        RendererSystem::get_cursor_surface()->draw(0,cursor.current_position, {.3, y_scale});
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
            f32 y_pos = character_grid[cursor.current_row - 1].y_pos;
            cursor.current_row--;
            cursor.current_index = buff.size();
            if (buff.size() == 0) {
                set_cursor_position({editor_settings.start_x_point , y_pos});
            }
            else {
                auto position = buff[buff.size() - 1]->get_next_position();
                set_cursor_position({position.x, y_pos});
            }
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
            //f32 points = editor_settings.line_spacing + (editor_settings.font_pixel_size * editor_settings.char_scale_factor);
            Camera::get_camera()->camera_move_down(position.y);
        }

        else if (cursor.current_position.y > (camera_y_start + editor_settings.screen_height)) {
            //f32 points = editor_settings.line_spacing + (editor_settings.font_pixel_size * editor_settings.char_scale_factor);
            Camera::get_camera()->camera_move_up(position.y);
        }

        std::cout << "x " << cursor.current_position.x << " y " << cursor.current_position.y << " current row " << cursor.current_row << " Current index " << cursor.current_index << "\n";
        
    }

} // namespace Cold
