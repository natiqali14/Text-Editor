#include "Page.h"
#include "RendererSystem.h"
namespace Cold
{
    Page::Page()
    {
        character_grid.push_back(RowData());
        current_position = {100.0f, 300.0f};
        current_scale = {10, 10};
    }

    Page::~Page()
    {
        
    }

    void Page::add_character(Character entry)
    {
        character_grid[cursor.current_row].buffer.push_back(entry);
        RendererSystem::prepare_data(entry);
    }

    void Page::add_row()
    {
        character_grid.push_back(RowData());
        cursor.current_row++;
        cursor.current_index = 0;
    }

    void Page::run_loop()
    {
        for (auto row : character_grid) {
            for (auto c : row.buffer) {
                RendererSystem::draw_character(c, current_position, current_scale);
            }
        }
    }

} // namespace Cold
