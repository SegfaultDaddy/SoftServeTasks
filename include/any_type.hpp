#ifndef ANY_TYPE_HPP
#define ANY_TYPE_HPP

#include <type_traits>

#include "placeholder.hpp"
#include "holder.hpp"

class AnyType
{
public:
    AnyType()
        : data_{nullptr}
    {
    }

    template<typename ValueType>
    AnyType(const ValueType& value)
        : data_{new Holder<typename std::remove_cv<typename std::decay<const ValueType>::type>::type>(value)}
    {
    }

    AnyType(const AnyType& other)
        : data_{(other.data_ != nullptr)? other.data_->clone() : nullptr}
    {
    }

    AnyType(AnyType&& other)
        : data_{other.data_}
    {
    }

private:
    Placeholder* data_;
};


#endif