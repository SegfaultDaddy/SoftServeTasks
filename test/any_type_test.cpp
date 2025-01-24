#include <cstring>

#include <gtest/gtest.h>

#include <any_type.hpp>

TEST(AnyType, constructors)
{
    {
        auto value{true};
        AnyType any{value};
        EXPECT_NO_THROW(AnyType{value});
        EXPECT_EQ(any.get<bool>(), value);
    }
    
    {
        auto value{1.2};
        AnyType any{value};
        EXPECT_NO_THROW(AnyType{value});
        EXPECT_EQ(any.get<double>(), value);
    }

    {
        auto value{1.2f};
        AnyType any{value};
        EXPECT_NO_THROW(AnyType{value});
        EXPECT_EQ(any.get<float>(), value);
    }
    
    {
        auto value{1};
        AnyType any{value};
        EXPECT_NO_THROW(AnyType{value});
        EXPECT_EQ(any.get<int>(), value);
    }

    {
        auto value{"Hello"};
        AnyType any{value};
        EXPECT_NO_THROW(AnyType{value});
        EXPECT_TRUE(std::strcmp(any.get<const char*>(), value) == 0);
    }

    {
        std::string value{"Hello"};

        AnyType any{value};
        EXPECT_NO_THROW(AnyType{value});
        EXPECT_TRUE(any.get<std::string>() == value);
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

TEST(AnyType, get)
{
}