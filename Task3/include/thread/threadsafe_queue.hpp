#ifndef THREAD_SAFE_QUEUE
#define THREAD_SAFE_QUEUE

#include <memory>
#include <mutex>
#include <condition_variable>

#include "./threadsafe_queue/threadsafe_node.hpp"

template<typename T>
class ThreadsafeQueue
{
public:
    using NodeType = ThreadsafeNode<T>;
    using NodeTypePtr = std::unique_ptr<NodeType>;

    ThreadsafeQueue();
    ThreadsafeQueue(const ThreadsafeQueue& other) = delete;
    ThreadsafeQueue& operator=(const ThreadsafeQueue& other) = delete; 
    ThreadsafeQueue(ThreadsafeQueue&& other) = delete;
    ThreadsafeQueue& operator=(ThreadsafeQueue&& other) = delete; 
    std::shared_ptr<T> try_pop();
    bool try_pop(T& value);
    std::shared_ptr<T> wait_and_pop();
    void wait_and_pop(T& value);
    void push(T value);
    bool empty();
private:
    NodeType* tail();
    NodeTypePtr pop_head();
    std::unique_lock<std::mutex> wait_for_data();
    NodeTypePtr wait_pop_head();
    NodeTypePtr wait_pop_head(T& value);
    NodeTypePtr try_pop_head();
    NodeTypePtr try_pop_head(T& value);

    std::mutex headMutex_;
    std::mutex tailMutex_;
    NodeTypePtr head_;
    NodeType* tail_;
    std::condition_variable dataCondition_;
};

template<typename T>
ThreadsafeQueue<T>::ThreadsafeQueue()
    : head_{std::make_unique<NodeType>()}, tail_{head_.get()}
{
}

template<typename T>
std::shared_ptr<T> ThreadsafeQueue<T>::try_pop()
{
    auto oldHead{try_pop_head()};
    return oldHead? oldHead->data : std::shared_ptr<T>{};
}

template<typename T>
bool ThreadsafeQueue<T>::try_pop(T& value)
{
    const auto oldHead{try_pop_head(value)};
    const bool complete{oldHead};
    return complete;
}

template<typename T>
std::shared_ptr<T> ThreadsafeQueue<T>::wait_and_pop()
{
    const auto oldHead{wait_pop_head()};
    return oldHead->data;
}

template<typename T>
void ThreadsafeQueue<T>::wait_and_pop(T& value)
{
    const auto oldHead{wait_pop_head(value)};
}

template<typename T>
void ThreadsafeQueue<T>::push(T value)
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

template<typename T>
bool ThreadsafeQueue<T>::empty()
{
    std::lock_guard<std::mutex> headLock{headMutex_};
    return head_.get() == tail();
}

template<typename T>
ThreadsafeQueue<T>::NodeType* ThreadsafeQueue<T>::tail()
{
    std::lock_guard<std::mutex> tailLock_{tailMutex_};
    return tail_;
}

template<typename T>
ThreadsafeQueue<T>::NodeTypePtr ThreadsafeQueue<T>::pop_head()
{
    NodeTypePtr oldHead{std::move(head_)};
    head_ = std::move(oldHead->next);
    return oldHead;
}

template<typename T>
std::unique_lock<std::mutex> ThreadsafeQueue<T>::wait_for_data()
{
    std::unique_lock<std::mutex> headLock{headMutex_};
    dataCondition_.wait(headLock, [&](){return head_.get() != tail();});
    return std::move(headLock);
}

template<typename T>
ThreadsafeQueue<T>::NodeTypePtr ThreadsafeQueue<T>::wait_pop_head()
{
    auto headLock{wait_for_data()};
    return pop_head();
}

template<typename T>
ThreadsafeQueue<T>::NodeTypePtr ThreadsafeQueue<T>::wait_pop_head(T& value)
{
    auto headLock{wait_for_data()};
    value = std::move(*(head_->data));
    return pop_head();
}

template<typename T>
ThreadsafeQueue<T>::NodeTypePtr ThreadsafeQueue<T>::try_pop_head()
{
    std::lock_guard<std::mutex> headLock{headMutex_};
    if(head_.get() == tail())
    {
        return {};
    }
    return pop_head();
}

template<typename T>
ThreadsafeQueue<T>::NodeTypePtr ThreadsafeQueue<T>::try_pop_head(T& value)
{
    std::lock_guard<std::mutex> headLock{headMutex_};
    if(head_.get() == tail())
    {
        return {};
    }
    value = std::move(*(head_->data));
    return pop_head();
}

#endif