#include "thread_pool.hpp"

thread_local ThiefQueue* ThreadPool::localThiefQueue_{nullptr};
thread_local std::size_t ThreadPool::index_{0};