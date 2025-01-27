#ifndef LINE_TYPE_HPP
#define LINE_TYPE_HPP

#include <atomic>

struct LineType
{
    std::atomic_uint64_t any;
    std::atomic_uint64_t blank;
    std::atomic_uint64_t comment;
    std::atomic_uint64_t code;
};

#endif