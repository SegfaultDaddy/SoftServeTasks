#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <atomic>
#include <queue>
#include <thread>
#include <vector>
#include <functional>
#include <memory>
#include <future>
#include <type_traits>

#include "threadsafe_queue.hpp"
#include "thief_queue.hpp"

class ThreadPool
{
public:
    using TaskType = std::move_only_function<void()>;

    ThreadPool();
    ~ThreadPool();

    template<typename FutureType>
    std::future<std::invoke_result_t<FutureType>> submit(FutureType function);
private:
    void run_pending_task();
    void worker_thread(const std::size_t index);
    bool pop_task_from_local_queue(TaskType& task);
    bool pop_task_from_pool_queue(TaskType& task);
    bool pop_task_from_other_thread_queue(TaskType& task);

    ThreadsafeQueue<TaskType> workQueue_;
    std::vector<std::unique_ptr<ThiefQueue>> queues_;
    std::vector<std::jthread> threads_;
    std::atomic_bool done_;
    thread_local static ThiefQueue* localThiefQueue_;
    thread_local static std::size_t index_;
};

template<typename FutureType>
std::future<std::invoke_result_t<FutureType>> ThreadPool::submit(FutureType f)
{
    std::packaged_task<std::invoke_result_t<FutureType>()> task{f};
    auto result{task.get_future()};
    if(localThiefQueue_ != nullptr)
    {
        localThiefQueue_->push(std::move(task));
    }
    else
    {
        workQueue_.push(std::move(task));
    }
    return result;
}

#endif