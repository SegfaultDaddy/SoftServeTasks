#ifndef STOPWATCH_HPP
#define STOPWATCH_HPP

#include <chrono>

class Stopwatch
{
public:
    using TimeType = std::chrono::milliseconds;

    void set_start();
    void set_finish();
    void reset() noexcept;
    TimeType time() const noexcept;
private:
    std::chrono::steady_clock::time_point start_;
    std::chrono::steady_clock::time_point finish_;
};

#endif