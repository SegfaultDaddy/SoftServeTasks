#ifndef ANY_TYPE_HPP
#define ANY_TYPE_HPP

#include <cstring>

#include <type_traits>
#include <format>
#include <optional>

#include "./any_type/any_to_error.hpp"
#include "./any_type/placeholder.hpp"
#include "./any_type/banned_type.hpp"
#include "./any_type/holder.hpp"

class AnyType
{
public:
    AnyType();

    template<BannedType T>
    AnyType(const T& value);

    template<BannedType T>
    AnyType(T&& value, std::enable_if_t<!std::is_same_v<AnyType&, T>>* = 0);
    AnyType(const AnyType& other);
    AnyType(AnyType&& other);
    ~AnyType();
    AnyType& swap(AnyType& other);
    AnyType& operator=(const AnyType& other);
    AnyType& operator=(AnyType&& other);

    template<BannedType T>
    AnyType& operator=(T&& value);

    template<BannedType T>
    T to();

    template<BannedType T>
    std::optional<T> try_to();
    bool has_value() const noexcept;
    const std::type_info& type() const noexcept;
    void reset() noexcept;
private:
    Placeholder* data_;
};

void swap(AnyType& first, AnyType& second);

template<BannedType T>
AnyType::AnyType(const T& value)
    : data_{new Holder<std::remove_cv_t<std::decay_t<const T>>>(value)}
{
}

template<BannedType T>
AnyType::AnyType(T&& value, std::enable_if_t<!std::is_same_v<AnyType&, T>>*)
    : data_(new Holder<std::decay_t<T>>(std::forward<T>(value)))
{
}

template<BannedType T>
AnyType& AnyType::operator=(T&& value)
{
    AnyType{std::forward<T>(value)}.swap(*this);
    return *this;
}

template<BannedType T>
T AnyType::to() 
{
    if(type() == typeid(T))
    {
        return static_cast<Holder<T>*>(data_)->value;
    }

    throw AnyToError{std::format("Wrong type provided Contained: {} Provided: {}", type().name(), typeid(T).name())};
    return {};
}

template<BannedType T>
std::optional<T> AnyType::try_to() 
{
    if(type() == typeid(T))
    {
        return static_cast<Holder<T>*>(data_)->value;
    }

    return std::nullopt;
}

#endif