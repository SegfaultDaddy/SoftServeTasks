#include <cassert>

#include "library/timer.hpp"

namespace library
{
    void Timer::set_start()
    {
        reset();
        start_ = std::chrono::steady_clock::now();
    }

    void Timer::set_finish()
    {
        assert((start_ != std::chrono::steady_clock::time_point{}) && "Start is not initialized");
        finish_ = std::chrono::steady_clock::now();
    }

    std::chrono::milliseconds Timer::time() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(finish_ - start_);
    }

    void Timer::reset() noexcept
    {
        start_ = std::chrono::steady_clock::time_point{};
        finish_ = std::chrono::steady_clock::time_point{};
    }
}