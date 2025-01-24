#ifndef ANY_TYPE_HPP
#define ANY_TYPE_HPP

#include <cstring>

#include <type_traits>
#include <format>

#include "any_get_error.hpp"
#include "placeholder.hpp"
#include "holder.hpp"

class AnyType
{
public:
    AnyType();

    template<typename T>
    AnyType(const T& value);
    AnyType(const AnyType& other);
    AnyType(AnyType&& other);
    ~AnyType();
    AnyType& swap(AnyType& other);
    AnyType& operator=(const AnyType& other);
    AnyType& operator=(AnyType&& other);

    template <class T>
    AnyType& operator=(T&& value);
    bool has_value() const noexcept;
    const std::type_info& type() const noexcept;
    void reset() noexcept;

    template<typename T>
    T get();
private:
    Placeholder* data_;
};

template<typename T>
AnyType::AnyType(const T& value)
    : data_{new Holder<T>(value)}
{
}

template <class T>
AnyType& AnyType::operator=(T&& value)
{
    AnyType(std::forward<T>(value)).swap(*this);
    return *this;
}

template<typename T>
T AnyType::get() 
{
    if(std::strcmp(typeid(T).name(), type().name()) == 0)
    {
        return static_cast<Holder<T>*>(data_)->value;
    }

    throw AnyGetError{std::format("Wrong type provided Contained: {} Provided: {}", typeid(T).name(), data_->type().name())};
    return {};
}

#endif