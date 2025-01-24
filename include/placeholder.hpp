#ifndef PLACEHOLDER_HPP
#define PLACEHOLDER_HPP

class Placeholder
{
public:
    virtual ~Placeholder(){}
    virtual Placeholder* clone() const = 0;
};

#endif