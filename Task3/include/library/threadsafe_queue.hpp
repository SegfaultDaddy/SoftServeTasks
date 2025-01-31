#ifndef THREAD_SAFE_QUEUE
#define THREAD_SAFE_QUEUE

#include <memory>
#include <mutex>
#include <condition_variable>

#include "threadsafe_node.hpp"

template<typename T>
class ThreadsafeQueue
{
public:
    using NodeType = ThreadsafeNode<T>;

    ThreadsafeQueue()
        : head_{std::make_unique<NodeType>()}, tail_{head_.get()}
    {
    }

    ThreadsafeQueue(const ThreadsafeQueue& other) = delete;
    ThreadsafeQueue& operator=(const ThreadsafeQueue& other) = delete;

    std::shared_ptr<T> try_pop()
    {
        auto oldHead{try_pop_head()};
        return oldHead? oldHead->data : std::shared_ptr<T>{};
    }

    bool try_pop(T& value)
    {
        const auto oldHead{try_pop_head(value)};
        const bool complete{oldHead};
        return complete;
    }

    std::shared_ptr<T> wait_and_pop()
    {
        const auto oldHead{wait_pop_head()};
        return oldHead->data;
    }

    void wait_and_pop(T& value)
    {
        const auto oldHead{wait_pop_head(value)};
    }

    void push(T value)
    {
        auto data{std::make_shared<T>(std::move(value))};
        std::unique_ptr<NodeType> ptr{std::make_unique<NodeType>()};
        {
            std::lock_guard<std::mutex> tailLock{tailMutex_};
            tail_->data = data;
            NodeType* newTail{ptr.get()};
            tail_->next = std::move(ptr);
            tail_ = newTail;
        }
        dataCondition_.notify_one();       
    }

    bool empty()
    {
        std::lock_guard<std::mutex> headLock{headMutex_};
        return head_.get() == tail();
    }
private:
    NodeType* tail()
    {
        std::lock_guard<std::mutex> tailLock_{tailMutex_};
        return tail_;
    }

    std::unique_ptr<NodeType> pop_head()
    {
        std::unique_ptr<NodeType> oldHead{std::move(head_)};
        head_ = std::move(oldHead->next);
        return oldHead;
    }

    std::unique_lock<std::mutex> wait_for_data()
    {
        std::unique_lock<std::mutex> headLock{headMutex_};
        dataCondition_.wait(headLock, [&](){return head_.get() != tail();});
        return std::move(headLock);
    }

    std::unique_ptr<NodeType> wait_pop_head()
    {
        auto headLock{wait_for_data()};
        return pop_head();
    }

    std::unique_ptr<NodeType> wait_pop_head(T& value)
    {
        auto headLock{wait_for_data()};
        value = std::move(*(head_->data));
        return pop_head();
    }

    std::unique_ptr<NodeType> try_pop_head()
    {
        std::lock_guard<std::mutex> headLock{headMutex_};
        if(head_.get() == tail())
        {
            return {};
        }
        return pop_head();
    }

    std::unique_ptr<NodeType> try_pop_head(T& value)
    {
        std::lock_guard<std::mutex> headLock{headMutex_};
        if(head_.get() == tail())
        {
            return {};
        }
        value = std::move(*(head_->data));
        return pop_head();
    }

    std::mutex headMutex_;
    std::mutex tailMutex_;
    std::unique_ptr<NodeType> head_;
    NodeType* tail_;
    std::condition_variable dataCondition_;
};
#endif