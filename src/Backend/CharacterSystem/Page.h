#pragma once

#include <includes.h>
#include <vector>

namespace Cold
{
    struct RowData {
        std::vector<Character> buffer; // TODO check if ok for performace or we can shift to linked list
    };
    struct Cursor {
        u32 current_index {0};
        u32 current_row{0};
    };
    class Page {
    public:
        Page();
        ~Page();
        void add_character(Character entry);
        void add_row();
        void run_loop();
    private:
        std::vector<RowData> character_grid;
        Cursor cursor;
        glm::vec2 current_position;
        glm::vec2 current_scale;
    };
} // namespace Cold
