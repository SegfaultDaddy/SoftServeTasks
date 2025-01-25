#ifndef VARIANT_TYPE_TO_ERROR_HPP
#define VARIANT_TYPE_TO_ERROR_HPP

#include <string>
#include <exception>

class VariantTypeToError : public std::exception
{
public:
    explicit VariantTypeToError(const std::string& message) 
        : std::exception{message.c_str()}
    {
    }
    explicit VariantTypeToError(const char* message) 
        : std::exception{message}
    {
    }
};

#endif