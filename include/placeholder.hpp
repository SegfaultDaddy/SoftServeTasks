#ifndef PLACEHOLDER_HPP
#define PLACEHOLDER_HPP

#include <typeinfo>

class Placeholder
{
public:
    virtual ~Placeholder(){}
    virtual Placeholder* clone() const = 0;
    virtual const std::type_info& type() const noexcept = 0;
};

#endif