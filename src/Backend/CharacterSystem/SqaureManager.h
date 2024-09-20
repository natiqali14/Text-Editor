#pragma once

#include <includes.h>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <deque>

#include "Sqaure.h"

#define MAX_SQAURE_CACHE 500
namespace Cold
{
    using SqaureSPtr = std::shared_ptr<Sqaure>;
    class SqaureManager {
    public:
        static void initialise();
        static void shutdown();
        static u32 create_sqaure(u32 program);
        static void draw_sqaure(u32 id);
        static void delete_sqaure(u32 id);
        static void set_texture_id(u32 square_id, u32 id);
    private:
        std::unordered_map<u32, SqaureSPtr> sqaures;
        std::deque<u32> sqaure_to_use;
        std::unordered_set<u32> sqaure_uid_to_use;
        u32 unique_number {0};

    }; 
} // namespace Cold

