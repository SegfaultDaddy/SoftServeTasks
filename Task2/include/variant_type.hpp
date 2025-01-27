#ifndef VARIANT_TYPE_HPP
#define VARIANT_TYPE_HPP

#include <type_traits>
#include <optional>
#include <format>

#include "./variant_type/max_size.hpp"
#include "./variant_type/variant_helper.hpp"
#include "./variant_type/variant_type_to_error.hpp"

template<typename... Types>
class VariantType
{
public:
    VariantType();

    template<typename T>
	VariantType(const T& value);

    template<typename T>
	VariantType(T&& value, std::enable_if_t<!std::is_same_v<VariantType&, T>>* = 0);
    VariantType(const VariantType& other);
    VariantType(VariantType&& other);
    ~VariantType();
    VariantType& swap(VariantType& other);
	VariantType& operator=(const VariantType& other);
	VariantType& operator=(VariantType&& other);

    template<typename T>
	VariantType<Types...>& operator=(T&& value);

    template<typename T>
    T to();

    template<typename T>
    std::optional<T> try_to();
    const std::type_info& type() const noexcept;
    bool has_value() const noexcept;
    void reset() noexcept;
private:
    using HelperType = VariantHelper<Types...>;

    constexpr static std::size_t size_{MaxSize<sizeof(Types)...>::size};
    constexpr static std::size_t align_{MaxSize<alignof(Types)...>::size};

    static constexpr const std::type_info* invalid_type();

    const std::type_info* type_;
    alignas(align_) std::byte data_[size_];
};

template<typename... Types>
void swap(VariantType<Types...>& first, VariantType<Types...>& second)
{
    first.swap(second);
}

template<typename... Types>
VariantType<Types...>::VariantType()
    : type_{invalid_type()}, data_{}
{
}

template<typename... Types>
template<typename T>
VariantType<Types...>::VariantType(const T& value)
    : type_{invalid_type()}
{
    using TransformedType = std::remove_cv_t<std::decay_t<const T>>;

    static_assert(HelperType::has_type(typeid(TransformedType)));
    HelperType::destroy(type(), &data_);      
    new (&data_) TransformedType(value);
    type_ = &typeid(TransformedType);
}

template<typename... Types>
template<typename T>
VariantType<Types...>::VariantType(T&& value, std::enable_if_t<!std::is_same_v<VariantType&, T>>*)
    : type_{invalid_type()}
{
    using TransformedType = std::decay_t<T>;

    static_assert(HelperType::has_type(typeid(TransformedType)));
    HelperType::destroy(type(), &data_);
    new (&data_) TransformedType(std::forward<T>(value));
    type_ = &typeid(TransformedType);
}

template<typename... Types>
VariantType<Types...>::VariantType(const VariantType& other)
    : type_{other.type_}
{
    HelperType::copy(type(), &other.data_, &data_);
}

template<typename... Types>
VariantType<Types...>::VariantType(VariantType&& other)
    : type_{other.type_}
{
    HelperType::move(type(), &other.data_, &data_);
    other.type_ = invalid_type();
}

template<typename... Types>
VariantType<Types...>::~VariantType()
{
    reset();
}

template<typename... Types>
VariantType<Types...>& VariantType<Types...>::swap(VariantType& other)
{
    std::swap(type_, other.type_);
    std::swap(data_, other.data_);
    return *this;
}

template<typename... Types>
VariantType<Types...>& VariantType<Types...>::operator=(const VariantType& other)
{
    VariantType{other}.swap(*this);
    return *this;
}

template<typename... Types>
VariantType<Types...>& VariantType<Types...>::operator=(VariantType&& other)
{
    other.swap(*this);
    VariantType{}.swap(other);
    return *this;
}

template<typename... Types>
template<typename T>
VariantType<Types...>& VariantType<Types...>::operator=(T&& value)
{
    VariantType{std::forward<T>(value)}.swap(*this);
    return *this;
}

template<typename... Types>
template<typename T>
T VariantType<Types...>::to()
{
    if(type() == typeid(T))
    {
        return *reinterpret_cast<T*>(&data_);
    }

    throw VariantTypeToError{std::format("Wrong type provided Contained: {} Provided: {}", type().name(), typeid(T).name())};
    return {};
}

template<typename... Types>
template<typename T>
std::optional<T> VariantType<Types...>::try_to()
{
    if(type() == typeid(T))
    {
        return *reinterpret_cast<T*>(&data_);
    }

    return {};
}

template<typename... Types>
const std::type_info& VariantType<Types...>::type() const noexcept
{
    return *type_;
}

template<typename... Types>
bool VariantType<Types...>::has_value() const noexcept
{
    return type() != *invalid_type();
}

template<typename... Types>
void VariantType<Types...>::reset() noexcept
{
    HelperType::destroy(type(), &data_);
    type_ = invalid_type();
}

template<typename... Types>
constexpr const std::type_info* VariantType<Types...>::invalid_type()
{
    return &typeid(void); 
}

#endif