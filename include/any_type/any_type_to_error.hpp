#ifndef ANY_TYPE_TO_ERROR_HPP
#define ANY_TYPE_TO_ERROR_HPP

#include <string>
#include <exception>

class AnyTypeToError : public std::exception
{
public:
    explicit AnyTypeToError(const std::string& message) 
        : std::exception{message.c_str()}
    {
    }
    explicit AnyTypeToError(const char* message) 
        : std::exception{message}
    {
    }
};

#endif