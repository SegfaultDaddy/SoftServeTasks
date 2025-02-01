#include <print>
#include <thread>

#include "./thread_pool/thief_queue.hpp"

ThiefQueue::ThiefQueue()
{
}

void ThiefQueue::push(DataType data)
{
    std::lock_guard<std::mutex> lock{mutex_};
    queue_.push_front(std::move(data));
}

bool ThiefQueue::empty() const
{
    std::lock_guard<std::mutex> lock{mutex_};
    return queue_.empty();
}

bool ThiefQueue::try_pop(DataType& data)
{
    std::lock_guard<std::mutex> lock{mutex_};
    if(queue_.empty())
    {
        return false;
    }
    data = std::move(queue_.front());
    queue_.pop_front();
    return true;
}

bool ThiefQueue::try_steal(DataType& data)
{
    std::lock_guard<std::mutex> lock{mutex_};
    if(queue_.empty())
    {
        return false;
    }
    data = std::move(queue_.back());
    queue_.pop_back();
    return true;
}