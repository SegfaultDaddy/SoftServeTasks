#ifndef THREADSAFE_NODE
#define THREADSAFE_NODE

#include <memory>

template<typename T>
struct ThreadsafeNode
{
    std::shared_ptr<T> data;
    std::unique_ptr<ThreadsafeNode> next;
};

#endif