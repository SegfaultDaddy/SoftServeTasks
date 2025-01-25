#ifndef MAX_SIZE_HPP
#define MAX_SIZE_HPP

#include <cstdlib>

template<std::size_t first, std::size_t... sizes>
struct MaxSize;

template<std::size_t first>
struct MaxSize<first>
{
    constexpr static std::size_t size{first};
};

template<std::size_t first, std::size_t second, std::size_t... sizes>
struct MaxSize<first, second, sizes...>
{
    constexpr static std::size_t size{(first >= second) ? MaxSize<first, sizes...>::size :
                                                          MaxSize<second, sizes...>::size};
};

#endif