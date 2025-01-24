#ifndef ANY_GET_ERROR_HPP
#define ANY_GET_ERROR_HPP

#include <string>
#include <exception>

class AnyGetError : public std::exception
{
public:
    explicit AnyGetError(const std::string& message) 
        : std::exception{message.c_str()}
        {
        }
    explicit AnyGetError(const char* message) 
        : std::exception{message}
        {
        }
};

#endif