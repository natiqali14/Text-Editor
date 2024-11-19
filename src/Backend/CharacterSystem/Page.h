#pragma once

#include <includes.h>
#include <vector>
#include "Character.h"
#include <memory>
#include "../../Clock/Clock.h"
#include <array>
namespace Cold
{
    
    struct RowData {
        std::vector<CharacterSPtr> buffer;
        f32 y_pos {0.0f};
        RowData() = default;
        RowData(f32 y) : y_pos(y) {}
    };
    struct Cursor {
        glm::vec2 current_position;
        glm::vec2 current_scale;
        bool b_should_hide{false};
        u32 current_index {0};
        u32 current_row{0};
        u32 cursor_blink_time{500}; // in ms
        glm::vec3 color {1,1,1};
        Clock cursor_clock;
        bool should_blink{true};        
    };
    enum entry_state {
        START = 0,
        ADD_CHARACTER,
        DELETE_ENTRY,
        NEW_ROW,
    };
    class Page {
    public:
        Page(i32 fd);
        ~Page();
        void add_character(Character entry);
        void add_row();
        void cursor_go_one_row_up();
        void run_loop();
        void delete_current_character();
        void cursor_move_forward();
        void cursor_move_backward();
        void cursor_move_up();
        void cursor_move_down();
        void set_cursor_position(glm::vec2 position);
        void add_character_at_index(Character entry);
        void move_row(std::vector<CharacterSPtr>& buffer_to_move, std::vector<CharacterSPtr>& target_buffer, f32 y_pos);
        void add_existing_content();
        void write_back_data();
        void paste();
        void create_copy_buffer();
    private:
        std::vector<RowData> character_grid;
        Cursor cursor;
        f32 camera_to_move_right_value{editor_settings.screen_width};
        f32 camera_to_move_bottom_value{0};
        entry_state current_state{START};
        bool b_is_dirty {false};
        i32 current_fd;
        f32 row_offset {0};
        std::array<std::pair<u32, u32>, 2> select_range;
        std::vector<std::vector<Character>> copy_buffer;
        bool shift_pressed {false};
    };
} // namespace Cold
