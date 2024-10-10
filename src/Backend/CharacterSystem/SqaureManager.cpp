#include "SqaureManager.h"

namespace Cold
{
    static SqaureManager* instance = nullptr;
    void SqaureManager::initialise()
    {
        instance = new SqaureManager;
    }

    void SqaureManager::shutdown()
    {
        delete instance;
        instance = nullptr;
    }

    u32 SqaureManager::create_sqaure(u32 program)
    {
        if (!instance->sqaure_to_use.empty()) {
            u32 id = instance->sqaure_to_use.front();
            instance->sqaure_to_use.pop_front();
            instance->sqaure_uid_to_use.erase(id);
            return id;
        }
        auto s = std::make_shared<Sqaure>(program);
        instance->sqaures.insert({instance->unique_number, s});
        return instance->unique_number++;
    }

    void SqaureManager::draw_sqaure(u32 id)
    {
        for (auto& pairs : instance->sqaures) {
            if (!instance->sqaure_uid_to_use.count(pairs.first)) {
                pairs.second->draw(0,{},{});
            }
        }
    }

    void SqaureManager::delete_sqaure(u32 id)
    {
        instance->sqaures.at(id)->ref_count--;
        if (!instance->sqaures.at(id)->should_delete()) return;
        if (instance->sqaure_to_use.size() < MAX_SQAURE_CACHE) {
            instance->sqaure_to_use.push_back(id);
            instance->sqaure_uid_to_use.insert(id);
            return;
        }
        instance->sqaures.erase(id);
    }

    void SqaureManager::set_texture_id(u32 square_id, u32 id)
    {
        instance->sqaures.at(square_id)->current_texture_id = id;
    }

} // namespace Cold
