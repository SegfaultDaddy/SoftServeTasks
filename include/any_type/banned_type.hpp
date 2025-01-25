#ifndef BANNED_TYPES_HPP
#define BANNED_TYPES_HPP

#include <type_traits>

template<typename T>
concept BannedType = requires(T type)
{
    requires(!std::is_same_v<T, void>);
    requires(!std::is_same_v<T, decltype(nullptr)>);
};

#endif