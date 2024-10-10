#include "Clock.h"

namespace Cold
{
    Clock::Clock()
    {
        start = high_resolution_clock::now();
    }

    u64 Clock::get_time_in_us()
    {
        auto now = high_resolution_clock::now();
        return duration_cast<microseconds>(now - start).count();
    }

    u64 Clock::get_time_in_ms()
    {
        auto now = high_resolution_clock::now();
        return duration_cast<milliseconds>(now - start).count();
    }

    f32 Clock::get_time_in_seconds()
    {
        auto now = high_resolution_clock::now();
        return duration_cast<seconds>(now - start).count();
    }

} // namespace Cold
