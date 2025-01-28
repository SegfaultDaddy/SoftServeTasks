#ifndef LINE_TYPE_HPP
#define LINE_TYPE_HPP

#include <atomic>

template<typename T>
struct LineType
{
    T any;
    T blank;
    T comment;
    T code;
};

#endif