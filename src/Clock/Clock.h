#pragma once
#include <includes.h>
#include <chrono>
using namespace std::chrono;
namespace Cold
{
    class Clock {
        time_point<high_resolution_clock> start;
    public:
        Clock();
        ~Clock() = default;
        u64 get_time_in_us();
        u64 get_time_in_ms();
        f32 get_time_in_seconds();
        void reset_clock();

    };
} // namespace Cold
