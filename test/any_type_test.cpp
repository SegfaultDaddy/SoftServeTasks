#include <cstring>

#include <gtest/gtest.h>

#include <any_type.hpp>

TEST(AnyType, constructors)
{
    {
        auto value{true};
        AnyType any{value};
        EXPECT_NO_THROW(AnyType{value});
        EXPECT_EQ(any.to<bool>(), value);
    }
    
    {
        auto value{1.2};
        AnyType any{value};
        EXPECT_NO_THROW(AnyType{value});
        EXPECT_EQ(any.to<double>(), value);
    }

    {
        auto value{1.2f};
        AnyType any{value};
        EXPECT_NO_THROW(AnyType{value});
        EXPECT_EQ(any.to<float>(), value);
    }
    
    {
        auto value{1};
        AnyType any{value};
        EXPECT_NO_THROW(AnyType{value});
        EXPECT_EQ(any.to<int>(), value);
    }

    {
        auto value{"Hello"};
        AnyType any{value};
        EXPECT_NO_THROW(AnyType{value});
        EXPECT_TRUE(std::strcmp(any.to<const char*>(), value) == 0);
    }

    {
        std::string value{"AFDFDJSFksdkfdkjskjfdskfdkskfdksHello"};

        AnyType any{value};
        EXPECT_NO_THROW(AnyType{value});
        EXPECT_EQ(any.to<std::string>(), value);
    }

    {
        auto data{1};
        auto value{&data};

        AnyType any{value};
        EXPECT_NO_THROW(AnyType{value});
        EXPECT_EQ(any.to<int*>(), value);
    }

    {
        int value[]{1, 2, 3};

        AnyType any{value};
        EXPECT_NO_THROW(AnyType{value});
        EXPECT_EQ(any.to<int*>(), value);
    }

    {
        std::string value{"AFDFDJSFksdkfdkjskjfdskfdkskfdksHello"};
        std::string copy{"AFDFDJSFksdkfdkjskjfdskfdkskfdksHello"};

        EXPECT_NO_THROW(AnyType{value});
        AnyType any{std::move(value)};
        EXPECT_EQ(any.to<std::string>(), copy);
    }

    {
        int value[]{1, 2, 3};
    
        EXPECT_NO_THROW(AnyType{value});
        AnyType any{value};
        EXPECT_EQ(any.to<int*>(), value);
    }

    struct Data
    {
        int x;
        float y;
        double z;
    };

    {
        Data data{1, 5.0f, 10};  
        EXPECT_NO_THROW(AnyType{data});
        AnyType any{data};
        auto[x, y, z]{any.to<Data>()};
        EXPECT_EQ(data.x, x);
        EXPECT_EQ(data.y, y);
        EXPECT_EQ(data.z, z);
    }

    {
        Data data{1, 5.0f, 10};  
        Data copy{1, 5.0f, 10};  
        EXPECT_NO_THROW(AnyType{data});
        AnyType any{std::move(data)};
        auto[x, y, z]{any.to<Data>()};
        EXPECT_EQ(copy.x, x);
        EXPECT_EQ(copy.y, y);
        EXPECT_EQ(copy.z, z);
    }

    {
        Data data{1, 5.0f, 10};  
        EXPECT_NO_THROW(AnyType{data});
        AnyType any{data};
        EXPECT_NO_THROW(AnyType{any});
        AnyType copy{any};

        auto valFirst{any.to<Data>()};
        auto valSecond{copy.to<Data>()};
        EXPECT_EQ(valFirst.x, valSecond.x);
        EXPECT_EQ(valFirst.y, valSecond.y);
        EXPECT_EQ(valFirst.z, valSecond.z);
    }

    {
        Data data{1, 5.0f, 10};  
        EXPECT_NO_THROW(AnyType{data});
        AnyType any{data};
        EXPECT_NO_THROW(AnyType{any});
        AnyType copy{std::move(any)};

        auto valSecond{copy.to<Data>()};
        EXPECT_EQ(data.x, valSecond.x);
        EXPECT_EQ(data.y, valSecond.y);
        EXPECT_EQ(data.z, valSecond.z);
    }

    {
        AnyType any{};
        any.~AnyType();
    }
}

TEST(AnyType, swap)
{
}

TEST(AnyType, assignment)
{
}

TEST(AnyType, has_value)
{
}

TEST(AnyType, reset)
{
}

TEST(AnyType, to)
{
}