#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

namespace library
{
    class Timer
    {
    public:
        void set_start();
        void set_finish();
        void reset() noexcept;
        std::chrono::milliseconds time() const noexcept;
    private:
        std::chrono::steady_clock::time_point start_;
        std::chrono::steady_clock::time_point finish_;
    };
}

#endif