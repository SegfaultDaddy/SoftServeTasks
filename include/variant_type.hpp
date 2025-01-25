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
	VariantType(T&& value, std::enable_if_t<!std::is_same_v<VariantType<Types...>&, T>>* = 0);
    VariantType(const VariantType<Types...>& other);
    VariantType(VariantType<Types...>&& other);
    ~VariantType();
    VariantType<Types...>& swap(VariantType<Types...>& other);
	VariantType<Types...>& operator=(const VariantType<Types...>& other);
	VariantType<Types...>& operator=(VariantType<Types...>&& other);

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
    static constexpr std::size_t size{MaxSize<sizeof(Types)...>::size};
    static constexpr std::size_t align{MaxSize<alignof(Types)...>::size};

    using HelperType = VariantHelper<Types...>;

    static constexpr const std::type_info* invalid_type();

    const std::type_info* type_;
    alignas(align) std::byte data_[size];
};

template<typename... Types>
VariantType<Types...>::VariantType()
    : type_{invalid_type()}, data_{}
{
}

template<typename... Types>
template<typename T>
VariantType<Types...>::VariantType(const T& value)
{
    static_assert(HelperType::has_type(T));
    HelperType::destroy(type(), &data);      
    new (&data_) T(value);
    type_ = &typeid(T);
}

template<typename... Types>
template<typename T>
VariantType<Types...>::VariantType(T&& value, std::enable_if_t<!std::is_same_v<VariantType<Types...>&, T>>*)
{
    static_assert(HelperType::has_type(T));
    HelperType::destroy(type(), &data);
    new (&data_) T(std::forward(value));
    type_ = &typeid(T);
}

template<typename... Types>
VariantType<Types...>::VariantType(const VariantType<Types...>& other)
    : type_{other.type_}
{
    HelperType::copy(type(), &other.data_, &data_);
}

template<typename... Types>
VariantType<Types...>::VariantType(VariantType<Types...>&& other)
    : type_{other.type_}
{
    HelperType::move(type(), &other.data_, &data_);
}

template<typename... Types>
VariantType<Types...>::~VariantType()
{
    HelperType::destroy(type(), &data_);
}

template<typename... Types>
VariantType<Types...>& VariantType<Types...>::swap(VariantType<Types...>& other)
{
    std::swap(type_, other.type_);
    std::swap(data_, other.data_);
    return *this;
}

template<typename... Types>
VariantType<Types...>& VariantType<Types...>::operator=(const VariantType<Types...>& other)
{
    VariantType{other}.swap(*this);
    return *this;
}

template<typename... Types>
VariantType<Types...>& VariantType<Types...>::operator=(VariantType<Types...>&& other)
{
    other.swap(*this);
    VariantType{}.swap(other);
    return *this;
}

template<typename... Types>
template<typename T>
VariantType<Types...>& VariantType<Types...>::operator=(T&& value)
{
    VariantType<Types...>{std::forward<T>(value)}.swap(*this);
    return *this;
}

template<typename... Types>
template<typename T>
T VariantType<Types...>::to()
{
    if(type() == typeid(T))
    {
        return *reinterpret_cast<T*>(&data);
    }

    throw VariantTypeToError{"Wrong type provided Contained: {} Provided: {}", type().name(), typeid(T).name()};
    return {};
}

template<typename... Types>
template<typename T>
std::optional<T> VariantType<Types...>::try_to()
{
    if(type() == typeid(T))
    {
        return *reinterpret_cast<T*>(&data);
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