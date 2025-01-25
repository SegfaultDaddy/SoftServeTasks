#ifndef ANY_TYPE_HPP
#define ANY_TYPE_HPP

#include <cstring>

#include <type_traits>
#include <format>
#include <optional>

#include "any_get_error.hpp"
#include "placeholder.hpp"
#include "holder.hpp"

class AnyType
{
public:
    AnyType();

    template<typename T>
    AnyType(const T& value);

    template<typename T>
    AnyType(T&& value, std::enable_if_t<!std::is_same_v<AnyType&, T>>* = 0);
    AnyType(const AnyType& other);
    AnyType(AnyType&& other);
    ~AnyType();
    AnyType& swap(AnyType& other);
    AnyType& operator=(const AnyType& other);
    AnyType& operator=(AnyType&& other);

    template <class T>
    AnyType& operator=(T&& value);

    template<typename T>
    T to();

    template<typename T>
    std::optional<T> try_to();
    bool has_value() const noexcept;
    const std::type_info& type() const noexcept;
    void reset() noexcept;
private:
    Placeholder* data_;
};

template<typename T>
AnyType::AnyType(const T& value)
    : data_{new Holder<std::remove_cv_t<std::decay_t<const T>>>(value)}
{
}

template<typename T>
AnyType::AnyType(T&& value, std::enable_if_t<!std::is_same_v<AnyType&, T>>*)
    : data_(new Holder<std::decay_t<T>>(std::forward<T>(value)))
{
}

template<typename T>
AnyType& AnyType::operator=(T&& value)
{
    AnyType{std::forward<T>(value)}.swap(*this);
    return *this;
}

template<typename T>
T AnyType::to() 
{
    if(type() == typeid(T))
    {
        return static_cast<Holder<T>*>(data_)->value;
    }

    throw AnyGetError{std::format("Wrong type provided Contained: {} Provided: {}", type().name(), typeid(T).name())};
    return {};
}

template<typename T>
std::optional<T> AnyType::try_to() 
{
    if(type() == typeid(T))
    {
        return static_cast<Holder<T>*>(data_)->value;
    }

    return std::nullopt;
}

#endif