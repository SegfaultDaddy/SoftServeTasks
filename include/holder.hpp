#ifndef HOLDER_HPP
#define HOLDER_HPP

#include "placeholder.hpp"

template<typename T>
class Holder final : public Placeholder
{
public:
    Holder(const T& value);
    Holder(T&& value);
    Placeholder* clone() const override;
    const std::type_info& type() const noexcept override;

    T value;
};

template<typename T>
Holder<T>::Holder(const T& value)
    : value{value}
{
}

template<typename T>
Holder<T>::Holder(T&& value)
    : value{static_cast<T>(value)}
{
}

template<typename T>
Placeholder* Holder<T>::clone() const
{
    return new Holder{value};
}

template<typename T>
const std::type_info& Holder<T>::type() const noexcept
{
    return typeid(T);
}

#endif