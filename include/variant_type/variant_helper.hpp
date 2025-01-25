#ifndef VARIANT_HELPER_HPP
#define VARIANT_HELPER_HPP

#include <typeinfo>

template<typename... Types>
struct VariantHelper;

template<typename T, typename... Types>
struct VariantHelper<T, Types...>
{
    static void copy(const std::type_info& prevType, void* prevData, void* nextData);
    static void move(const std::type_info& prevType, void* prevData, void* nextData);
    static void destroy(const std::type_info& type, void* data);
};

template<>
struct VariantHelper<>
{
    static void copy(const std::type_info& prevType, void* prevData, void* nextData){}
    static void move(const std::type_info& prevType, void* prevData, void* nextData){}
    static void destroy(const std::type_info& type, void* data){}
};

template<typename T, typename... Types>
void VariantHelper<T, Types...>::copy(const std::type_info& prevType, void* prevData, void* nextData)
{
    if(prevType == typeid(T))
    {
        new (nextData) T(*reinterpret_cast<const T*>(prevData));
    }
    else
    {
        VariantHelper<Types...>::copy(prevType, prevData, nextData);
    }
}

template<typename T, typename... Types>
void VariantHelper<T, Types...>::move(const std::type_info& prevType, void* prevData, void* nextData)
{
    if(prevType == typeid(T))
    {
        new (nextData) T(std::move(*reinterpret_cast<T*>(prevData)));
    }
    else
    {
        VariantHelper<Types...>::move(prevType, prevData, nextData);
    }
}

template<typename T, typename... Types>
void VariantHelper<T, Types...>::destroy(const std::type_info& type, void* data)
{
    if(type == typeid(T))
    {
        reinterpret_cast<T*>(data)->~T();
    }
    else
    {
        VariantHelper<Types...>::destroy(type, data);
    }
}

#endif