#include "stopwatch.hpp"

void Stopwatch::set_start()
{
    reset();
    start_ = std::chrono::steady_clock::now();
}

void Stopwatch::set_finish()
{
    finish_ = std::chrono::steady_clock::now();
}

Stopwatch::TimeType Stopwatch::time() const noexcept
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(finish_ - start_);
}

void Stopwatch::reset() noexcept
{
    start_ = std::chrono::steady_clock::time_point{};
    finish_ = std::chrono::steady_clock::time_point{};
}