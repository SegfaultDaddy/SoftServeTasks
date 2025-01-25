#ifndef VARIANT_TO_ERROR_HPP
#define VARIANT_TO_ERROR_HPP

#include <string>
#include <exception>

class VariantToError : public std::exception
{
public:
    explicit VariantToError(const std::string& message) 
        : std::exception{message.c_str()}
    {
    }
    explicit VariantToError(const char* message) 
        : std::exception{message}
    {
    }
};

#endif