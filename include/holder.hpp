#ifndef HOLDER_HPP
#define HOLDER_HPP

#include "placeholder.hpp"

template<typename ValueType>
class Holder : public Placeholder
{
public:
    Holder(const ValueType& value)
        : held{value}
    {
    }

    Holder(ValueType&& value)
        : held{static_cast<ValueType>(value)}
    {
    }

    Placeholder* clone() const override
    {
        return new Holder{held};
    }

    ValueType held;
};

#endif