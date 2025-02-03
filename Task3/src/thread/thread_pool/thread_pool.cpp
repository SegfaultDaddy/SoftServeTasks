#include "thread_pool.hpp"

thread_local ThiefQueue* ThreadPool::localThiefQueue_{nullptr};
thread_local std::size_t ThreadPool::index_{0};

ThreadPool::ThreadPool()
    : done_{false}
{
    const auto threadCount{std::thread::hardware_concurrency()};
    try
    {
        for(std::size_t i{0};i < threadCount; i += 1)
        {
            queues_.emplace_back(std::make_unique<ThiefQueue>());
        }
        for(std::size_t i{0}; i < threadCount; i += 1)
        {
            threads_.emplace_back(&ThreadPool::worker_thread, this, i);
        }
    }
    catch(...)
    {
        done_ = true;
        throw;
    }
    
}

ThreadPool::~ThreadPool()
{
    done_ = true;
}

void ThreadPool::run_pending_task()
{
    TaskType task{};
    if(pop_task_from_local_queue(task) || 
       pop_task_from_pool_queue(task) || 
       pop_task_from_other_thread_queue(task)) 
    {
        task();
    }
    else
    {
        std::this_thread::yield();
    }
} 

void ThreadPool::worker_thread(const std::size_t index)
{
    index_ = index;
    localThiefQueue_ = queues_[index_].get();
    while(!done_)
    {
        run_pending_task();
    }
}

bool ThreadPool::pop_task_from_local_queue(TaskType& task)
{
    return (localThiefQueue_ != nullptr) && 
            localThiefQueue_->try_pop(task);
}

bool ThreadPool::pop_task_from_pool_queue(TaskType& task)
{
    return workQueue_.try_pop(task);
}

bool ThreadPool::pop_task_from_other_thread_queue(TaskType& task)
{
    for(std::size_t i{0}; i < std::size(queues_); i += 1)
    {
        const auto index{(index_ + i + 1) % std::size(queues_)};
        if(queues_[index]->try_steal(task))
        {
            return true;
        }
    }
    return false;
}