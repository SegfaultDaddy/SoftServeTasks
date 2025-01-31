#ifndef THIEF_QUEUE
#define THIEF_QUEUE

#include <functional>
#include <mutex>
#include <deque>

#include "function_wrapper.hpp"

class ThiefQueue
{
public:
    using DataType = FunctionWrapper;

    ThiefQueue();
    ThiefQueue(const ThiefQueue& other) = delete;
    ThiefQueue& operator=(const ThiefQueue& other) = delete;
    void push(DataType data);
    bool try_pop(DataType& data);
    bool try_steal(DataType& data);
    bool empty() const;
private:
    mutable std::mutex mutex_;
    std::deque<DataType> queue_;
};

#endif