#ifndef FUNCTION_WRAPPER_HPP
#define FUNCTION_WRAPPER_HPP

#include <memory>

class FunctionWrapper
{ 
public:

    FunctionWrapper() = default;
    FunctionWrapper(FunctionWrapper&& other)
        : impl(std::move(other.impl))
    {
    }

    FunctionWrapper& operator=(FunctionWrapper&& other)
    {
        impl=std::move(other.impl);
        return *this;
    }

    FunctionWrapper(const FunctionWrapper&)=delete;
    FunctionWrapper(FunctionWrapper&)=delete;
    FunctionWrapper& operator=(const FunctionWrapper&)=delete;

    template<typename F>
    FunctionWrapper(F&& f):
    impl(new impl_type<F>(std::move(f)))
    {
    }

    void operator()() 
    { 
        impl->call(); 
    }
private:
    struct impl_base 
    {
        virtual void call()=0;
        virtual ~impl_base() {}
    };

    template<typename F>
    struct impl_type: impl_base
    {
        F f;
        impl_type(F&& f_): f(std::move(f_)) 
        {
        }
        void call() 
        { 
            f(); 
        }
    };

    std::unique_ptr<impl_base> impl;
};

#endif