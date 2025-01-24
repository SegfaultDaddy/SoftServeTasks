#ifndef ANY_TYPE_HPP
#define ANY_TYPE_HPP

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
        : data_{new Holder<ValueType>(value)}
    {
    }

    AnyType(const AnyType& other)
        : data_{(other.data_ == nullptr)? other.data_->clone() : nullptr}
    {
    }

private:
    Placeholder* data_;
};


#endif