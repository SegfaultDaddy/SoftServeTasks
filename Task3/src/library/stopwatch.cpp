#include <cassert>

#include "stopwatch.hpp"

void Stopwatch::set_start()
{
    reset();
    start_ = std::chrono::steady_clock::now();
}

void Stopwatch::set_finish()
{
    assert((start_ != std::chrono::steady_clock::time_point{}) && "Start is not initialized");
    finish_ = std::chrono::steady_clock::now();
}

std::chrono::milliseconds Stopwatch::time() const noexcept
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(finish_ - start_);
}

void Stopwatch::reset() noexcept
{
    start_ = std::chrono::steady_clock::time_point{};
    finish_ = std::chrono::steady_clock::time_point{};
}