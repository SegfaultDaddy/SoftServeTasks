#include <gtest/gtest.h>

#include <any_type.hpp>

TEST(AnyType, constructors)
{
    AnyType any{true};
    EXPECT_EQ(any.get<bool>(), true);
    any = 1.2;

    EXPECT_EQ(any.get<double>(), 1.2);
}