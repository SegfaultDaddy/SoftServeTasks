#include <cstring>

#include <gtest/gtest.h>

#include <any_type.hpp>

struct Data
{
    int x;
    float y;
    double z;
};

TEST(AnyTypeTest, constructors)
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
        EXPECT_FALSE(any.has_value());
    }
}

TEST(AnyTypeTest, swap)
{
    {
        Data data{1, 5.0f, 10};  
        AnyType any{data};
        AnyType anyAny{"Hello"};
        swap(any, anyAny);
        EXPECT_TRUE(std::strcmp(any.to<const char*>(), "Hello") == 0);

        auto value{anyAny.to<Data>()};
        EXPECT_EQ(data.x, value.x);
        EXPECT_EQ(data.y, value.y);
        EXPECT_EQ(data.z, value.z);
    }

    {
        AnyType any{"Hi"};
        AnyType anyAny{"Hello"};
        any.swap(anyAny);
        EXPECT_TRUE(std::strcmp(any.to<const char*>(), "Hello") == 0);
        EXPECT_TRUE(std::strcmp(anyAny.to<const char*>(), "Hi") == 0);
    }

    {
        AnyType any{"Hi"};
        AnyType anyAny{"Hello"};
        anyAny.swap(any);
        EXPECT_TRUE(std::strcmp(any.to<const char*>(), "Hello") == 0);
        EXPECT_TRUE(std::strcmp(anyAny.to<const char*>(), "Hi") == 0);
    }

    {
        AnyType any{"Hi"};
        any.swap(any);
        EXPECT_TRUE(std::strcmp(any.to<const char*>(), "Hi") == 0);
    }
}

TEST(AnyTypeTest, assignment)
{
    {
        int a{21};
        float b{1.2};
        Data c{1, 2.0f, 3.0};
        AnyType type{a};

        EXPECT_EQ(type.to<int>(), a);
        type = b;
        EXPECT_EQ(type.to<float>(), b);
        type = c;
        EXPECT_EQ(type.to<Data>().x, c.x);
        EXPECT_EQ(type.to<Data>().y, c.y);
        EXPECT_EQ(type.to<Data>().z, c.z);
    }

    {
        Data c{1, 2.0f, 3.0};
        AnyType type{std::move(c)};
        EXPECT_EQ(type.to<Data>().x, 1);
        EXPECT_EQ(type.to<Data>().y, 2.0f);
        EXPECT_EQ(type.to<Data>().z, 3.0);
    }

    {
        AnyType any{"Hi"};
        AnyType anyAny{"Hello"};
        EXPECT_TRUE(std::strcmp(any.to<const char*>(), "Hi") == 0);
        any = anyAny;
        EXPECT_TRUE(std::strcmp(any.to<const char*>(), "Hello") == 0);
        EXPECT_TRUE(std::strcmp(anyAny.to<const char*>(), "Hello") == 0);
    }

    {
        AnyType any{"Hi"};
        any = 3.4;
        EXPECT_EQ(any.to<double>(), 3.4);
    }
}

TEST(AnyTypeTest, has_value)
{
    AnyType any{};
    EXPECT_FALSE(any.has_value());
    any = 1.24f;
    EXPECT_TRUE(any.has_value());
    any.reset();
    EXPECT_FALSE(any.has_value());
}

TEST(AnyTypeTest, reset)
{
    AnyType any{};
    any = 2.34554;
    EXPECT_TRUE(any.has_value());
    any.reset();
    EXPECT_FALSE(any.has_value());
}

TEST(AnyTypeTest, to)
{
    {
        auto value{true};
        AnyType any{value};
        EXPECT_THROW(any.to<int>(), AnyTypeToError);
        EXPECT_EQ(any.to<bool>(), value);
    }
    
    {
        auto value{1.2};
        AnyType any{value};
        EXPECT_THROW(any.to<int>(), AnyTypeToError);
        EXPECT_EQ(any.to<double>(), value);
    }

    {
        auto value{1.2f};
        AnyType any{value};
        EXPECT_THROW(any.to<int>(), AnyTypeToError);
        EXPECT_EQ(any.to<float>(), value);
    }
    
    {
        auto value{1};
        AnyType any{value};
        EXPECT_THROW(any.to<double>(), AnyTypeToError);
        EXPECT_EQ(any.to<int>(), value);
    }

    {
        auto value{"Hello"};
        AnyType any{value};
        EXPECT_THROW(any.to<char*>(), AnyTypeToError);
        EXPECT_TRUE(std::strcmp(any.to<const char*>(), value) == 0);
    }

    {
        std::string value{"AFDFDJSFksdkfdkjskjfdskfdkskfdksHello"};

        AnyType any{value};
        EXPECT_THROW(any.to<char*>(), AnyTypeToError);
        EXPECT_EQ(any.to<std::string>(), value);
    }

    {
        auto data{1};
        auto value{&data};

        AnyType any{value};
        EXPECT_THROW(any.to<char*>(), AnyTypeToError);
        EXPECT_EQ(any.to<int*>(), value);
    }

    {
        int value[]{1, 2, 3};

        AnyType any{value};
        EXPECT_THROW(any.to<char*>(), AnyTypeToError);
        EXPECT_EQ(any.to<int*>(), value);
    }

    {
        std::string value{"AFDFDJSFksdkfdkjskjfdskfdkskfdksHello"};
        std::string copy{"AFDFDJSFksdkfdkjskjfdskfdkskfdksHello"};

        AnyType any{std::move(value)};
        EXPECT_THROW(any.to<char*>(), AnyTypeToError);
        EXPECT_EQ(any.to<std::string>(), copy);
    }

    {
        int value[]{1, 2, 3};
    
        AnyType any{value};
        EXPECT_THROW(any.to<char*>(), AnyTypeToError);
        EXPECT_EQ(any.to<int*>(), value);
    } 

    {
        Data data{1, 5.0f, 10};  
        AnyType any{data};
        auto[x, y, z]{any.to<Data>()};
        EXPECT_THROW(any.to<char*>(), AnyTypeToError);
        EXPECT_EQ(data.x, x);
        EXPECT_EQ(data.y, y);
        EXPECT_EQ(data.z, z);
    }

    {
        Data data{1, 5.0f, 10};  
        Data copy{1, 5.0f, 10};  
        AnyType any{std::move(data)};
        auto[x, y, z]{any.to<Data>()};
        EXPECT_THROW(any.to<char*>(), AnyTypeToError);
        EXPECT_EQ(copy.x, x);
        EXPECT_EQ(copy.y, y);
        EXPECT_EQ(copy.z, z);
    }

    {
        Data data{1, 5.0f, 10};  
        AnyType any{data};
        AnyType copy{any};

        auto valFirst{any.to<Data>()};
        auto valSecond{copy.to<Data>()};
        EXPECT_THROW(any.to<char*>(), AnyTypeToError);
        EXPECT_EQ(valFirst.x, valSecond.x);
        EXPECT_EQ(valFirst.y, valSecond.y);
        EXPECT_EQ(valFirst.z, valSecond.z);
    }

    {
        Data data{1, 5.0f, 10};  
        AnyType any{data};
        AnyType copy{std::move(any)};

        auto valSecond{copy.to<Data>()};
        EXPECT_THROW(copy.to<char*>(), AnyTypeToError);
        EXPECT_EQ(data.x, valSecond.x);
        EXPECT_EQ(data.y, valSecond.y);
        EXPECT_EQ(data.z, valSecond.z);
    }
}

TEST(AnyTypeTest, try_to)
{
    {
        auto value{true};
        AnyType any{value};
        EXPECT_FALSE(any.try_to<int>().has_value());
        EXPECT_TRUE(any.try_to<bool>().has_value());
        EXPECT_EQ(any.try_to<bool>().value(), value);
    }
    
    {
        auto value{1.2};
        AnyType any{value};
        EXPECT_FALSE(any.try_to<int>().has_value());
        EXPECT_TRUE(any.try_to<double>().has_value());
        EXPECT_EQ(any.try_to<double>().value(), value);
    }

    {
        auto value{1.2f};
        AnyType any{value};
        EXPECT_FALSE(any.try_to<int>().has_value());
        EXPECT_TRUE(any.try_to<float>().has_value());
        EXPECT_EQ(any.try_to<float>().value(), value);
    }
    
    {
        auto value{1};
        AnyType any{value};
        EXPECT_FALSE(any.try_to<double>().has_value());
        EXPECT_TRUE(any.try_to<int>().has_value());
        EXPECT_EQ(any.try_to<int>().value(), value);
    }

    {
        auto value{"Hello"};
        AnyType any{value};
        EXPECT_FALSE(any.try_to<char*>().has_value());
        EXPECT_TRUE(any.try_to<const char*>().has_value());
        EXPECT_TRUE(std::strcmp(any.try_to<const char*>().value(), value) == 0);
    }

    {
        std::string value{"AFDFDJSFksdkfdkjskjfdskfdkskfdksHello"};

        AnyType any{value};
        EXPECT_FALSE(any.try_to<char*>().has_value());
        EXPECT_TRUE(any.try_to<std::string>().has_value());
        EXPECT_EQ(any.try_to<std::string>().value(), value);
    }

    {
        auto data{1};
        auto value{&data};

        AnyType any{value};
        EXPECT_FALSE(any.try_to<char*>().has_value());
        EXPECT_TRUE(any.try_to<int*>().has_value());
        EXPECT_EQ(any.try_to<int*>().value(), value);
    }

    {
        int value[]{1, 2, 3};

        AnyType any{value};
        EXPECT_FALSE(any.try_to<char*>().has_value());
        EXPECT_TRUE(any.try_to<int*>().has_value());
        EXPECT_EQ(any.try_to<int*>().value(), value);
    }

    {
        std::string value{"AFDFDJSFksdkfdkjskjfdskfdkskfdksHello"};
        std::string copy{"AFDFDJSFksdkfdkjskjfdskfdkskfdksHello"};

        AnyType any{std::move(value)};
        EXPECT_FALSE(any.try_to<char*>().has_value());
        EXPECT_TRUE(any.try_to<std::string>().has_value());
        EXPECT_EQ(any.try_to<std::string>().value(), copy);
    }

    {
        Data data{1, 5.0f, 10};  
        AnyType any{data};
        EXPECT_TRUE(any.try_to<Data>().has_value());
        EXPECT_FALSE(any.try_to<char*>().has_value());
        auto[x, y, z]{any.try_to<Data>().value()};
        EXPECT_EQ(data.x, x);
        EXPECT_EQ(data.y, y);
        EXPECT_EQ(data.z, z);
    }

    {
        Data data{1, 5.0f, 10};  
        Data copy{1, 5.0f, 10};  
        AnyType any{std::move(data)};
        EXPECT_TRUE(any.try_to<Data>().has_value());
        EXPECT_FALSE(any.try_to<char*>().has_value());
        auto[x, y, z]{any.try_to<Data>().value()};
        EXPECT_EQ(copy.x, x);
        EXPECT_EQ(copy.y, y);
        EXPECT_EQ(copy.z, z);
    }

    {
        Data data{1, 5.0f, 10};  
        AnyType any{data};
        AnyType copy{any};

        auto valFirst{any.try_to<Data>().value()};
        auto valSecond{copy.try_to<Data>().value()};
        EXPECT_FALSE(any.try_to<char*>().has_value());
        EXPECT_EQ(valFirst.x, valSecond.x);
        EXPECT_EQ(valFirst.y, valSecond.y);
        EXPECT_EQ(valFirst.z, valSecond.z);
    }

    {
        Data data{1, 5.0f, 10};  
        AnyType any{data};
        AnyType copy{std::move(any)};

        auto valSecond{copy.to<Data>()};
        EXPECT_FALSE(copy.try_to<char*>().has_value());
        EXPECT_EQ(data.x, valSecond.x);
        EXPECT_EQ(data.y, valSecond.y);
        EXPECT_EQ(data.z, valSecond.z);
    }
}