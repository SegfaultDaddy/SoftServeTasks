#ifndef ANY_TO_ERROR_HPP
#define ANY_TO_ERROR_HPP

#include <string>
#include <exception>

class AnyToError : public std::exception
{
public:
    explicit AnyToError(const std::string& message) 
        : std::exception{message.c_str()}
    {
    }
    explicit AnyToError(const char* message) 
        : std::exception{message}
    {
    }
};

#endif