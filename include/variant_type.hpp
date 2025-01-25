#ifndef VARIANT_TYPE_HPP
#define VARIANT_TYPE_HPP

#include <type_traits>
#include <optional>

#include "./variant_type/max_size.hpp"
#include "./variant_type/variant_helper.hpp"

template<typename... Types>
class VariantType
{
public:
    VariantType();
    VariantType(const VariantType<Types...>& other);
    VariantType(VariantType<Types...>&& other);
    ~VariantType();
    VariantType<Types...>& swap(VariantType<Types...>& other);
	VariantType<Types...>& operator=(const VariantType<Types...>& other);
	VariantType<Types...>& operator=(VariantType<Types...>&& other);

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

    static constexpr const std::type_info& invalid_type();

    std::type_info type_;
    alignas(align) std::byte data_[size];
};

template<typename... Types>
VariantType<Types...>::VariantType()
{
}

template<typename... Types>
VariantType<Types...>::VariantType(const VariantType<Types...>& other)
{
}

template<typename... Types>
VariantType<Types...>::VariantType(VariantType<Types...>&& other)
{
}

template<typename... Types>
VariantType<Types...>::~VariantType()
{
}

template<typename... Types>
VariantType<Types...>& VariantType<Types...>::swap(VariantType<Types...>& other)
{
}

template<typename... Types>
VariantType<Types...>& VariantType<Types...>::operator=(const VariantType<Types...>& other)
{
}

template<typename... Types>
VariantType<Types...>& VariantType<Types...>::operator=(VariantType<Types...>&& other)
{
}

template<typename... Types>
template<typename T>
T VariantType<Types...>::to()
{
}

template<typename... Types>
template<typename T>
std::optional<T> VariantType<Types...>::try_to()
{
}

template<typename... Types>
const std::type_info& VariantType<Types...>::type() const noexcept
{
}

template<typename... Types>
bool VariantType<Types...>::has_value() const noexcept
{
}

template<typename... Types>
void VariantType<Types...>::reset() noexcept
{
}

template<typename... Types>
constexpr const std::type_info& VariantType<Types...>::invalid_type()
{
    return typeid(void); 
}

#endif