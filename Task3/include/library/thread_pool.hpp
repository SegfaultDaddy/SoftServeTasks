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

    ThreadPool()
        : done_{false}
    {
        const auto threadCount{std::thread::hardware_concurrency()};
        try
        {
            for(std::size_t i{0};i< threadCount; i += 1)
            {
                queues_.emplace_back(std::make_unique<ThiefQueue>());
            }
            for(std::size_t i{0}; i < threadCount; i += 1)
            {
                threads_.emplace_back(&ThreadPool::worker_thread, this, index_);
            }
        }
        catch(...)
        {
            done_ = true;
            throw;
        }
        
    }

    ~ThreadPool()
    {
        done_ = true;
    }
    
    template<typename FutureType>
    std::future<std::invoke_result_t<FutureType>> submit(FutureType f)
    {
        std::packaged_task<std::invoke_result_t<FutureType>()> task{f};
        auto result{task.get_future()};
        if(localThiefQueue_)
        {
            localThiefQueue_->push(std::move(task));
        }
        else
        {
            workQueue_.push(std::move(task));
        }
        return result;
    }
private:
    void run_pending_task()
    {
        TaskType task;
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

    void worker_thread(const std::size_t index)
    {
        index_ = index;
        localThiefQueue_ = queues_[index_].get();
        while(!done_)
        {
            run_pending_task();
        }
    }

    bool pop_task_from_local_queue(TaskType& task)
    {
        return localThiefQueue_ && localThiefQueue_->try_pop(task);
    }

    bool pop_task_from_pool_queue(TaskType& task)
    {
        return workQueue_.try_pop(task);
    }

    bool pop_task_from_other_thread_queue(TaskType& task)
    {
        for(std::size_t i{0}; i < std::size(queues_); i+=1)
        {
            const auto index{(index_ + i + 1) % std::size(queues_)};
            if(queues_[index]->try_steal(task))
            {
                return true;
            }
        }
        return false;
    }

    ThreadsafeQueue<TaskType> workQueue_;
    std::vector<std::unique_ptr<ThiefQueue>> queues_;
    std::vector<std::jthread> threads_;
    std::atomic_bool done_;
    thread_local static ThiefQueue* localThiefQueue_;
    thread_local static std::size_t index_;
};

#endif