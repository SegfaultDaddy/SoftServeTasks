#include <gtest/gtest.h>

#include "variant_type.hpp"

TEST(VariantTypeTest, default_constructor)
{
    VariantType<int, bool, char> type{};
    EXPECT_FALSE(type.has_value());
    EXPECT_EQ(type.type(), typeid(void));
}

TEST(VariantTypeTest, copy_t_constructor)
{
    int x{10};
    int* y{&x};
    VariantType<int*, bool, char> type{y};
    EXPECT_TRUE(type.has_value());
    EXPECT_EQ(&x, type.to<int*>());
}

TEST(VariantTypeTest, move_t_constructor)
{
    int x{10};
    VariantType<int*, bool, char> type{&x};
    EXPECT_TRUE(type.has_value());
    EXPECT_EQ(&x, type.to<int*>());
}

TEST(VariantTypeTest, copy_variant_type_constructor)
{
    int x{10};
    int* y{&x};
    VariantType<int*, bool, char> type{y};
    VariantType<int*, bool, char> copy{type};
    EXPECT_TRUE(copy.has_value());
    EXPECT_EQ(&x, copy.to<int*>());
}

TEST(VariantTypeTest, move_variant_type_constructor)
{
    int x{10};
    int* y{&x};
    VariantType<int*, bool, char> type{y};
    VariantType<int*, bool, char> move{std::move(type)};
    EXPECT_TRUE(move.has_value());
    EXPECT_FALSE(type.has_value());
    EXPECT_EQ(&x, move.to<int*>());
}

TEST(VariantTypeTest, destructor_variant_type)
{
    VariantType<int*, bool, char> type{true};
    type.~VariantType();
    EXPECT_FALSE(type.has_value());
}

TEST(VariantTypeTest, swap)
{
    int x{10};
    VariantType<int*, bool, char> type{&x};
    VariantType<int*, bool, char> second{true};

    type.swap(second);
    EXPECT_EQ(&x, second.to<int*>());
    EXPECT_EQ(true, type.to<bool>());
}

TEST(VariantTypeTest, t_assignment)
{
    int x{20};
    int* y{&x};
    VariantType<int*, bool, char, int> type{true};
    EXPECT_EQ(true, type.to<bool>());
    type = 10;
    EXPECT_EQ(10, type.to<int>());
    type = y;
    EXPECT_EQ(&x, type.to<int*>());
    type = 'a';
    EXPECT_EQ('a', type.to<char>());
}

TEST(VariantTypeTest, copy_variant_type_assignment)
{
    int x{10};
    VariantType<int*, bool, char, int> type{true};
    VariantType<int*, bool, char, int> second{&x};
    EXPECT_EQ(true, type.to<bool>());
    type = second;
    EXPECT_EQ(&x, type.to<int*>());
}

TEST(VariantTypeTest, move_variant_type_assignment)
{
    int x{10};
    VariantType<int*, bool, char, int> type{true};
    VariantType<int*, bool, char, int> second{&x};
    EXPECT_EQ(true, type.to<bool>());
    type = std::move(second);
    EXPECT_FALSE(second.has_value());
    EXPECT_EQ(&x, type.to<int*>());
}

TEST(VariantTypeTest, to_no_except)
{
    VariantType<int*, bool, char, int> type{10};
    EXPECT_NO_THROW(type.to<int>());
    type = true;
    EXPECT_EQ(true, type.to<bool>());
}

TEST(VariantTypeTest, to_except)
{
    VariantType<int*, bool, char, int> type{10};
    EXPECT_THROW(type.to<bool>(), VariantTypeToError);
    type = true;
    EXPECT_EQ(true, type.to<bool>());
}

TEST(VariantTypeTest, try_to_has_type)
{
    VariantType<int*, bool, char, int> type{10};
    EXPECT_TRUE(type.try_to<int>().has_value());
    type = true;
    EXPECT_EQ(true, type.try_to<bool>().value());
}

TEST(VariantTypeTest, try_to_has_type_false)
{
    VariantType<int*, bool, char, int> type{10};
    EXPECT_FALSE(type.try_to<bool>().has_value());
    type = true;
    EXPECT_EQ(true, type.try_to<bool>().value());
}

TEST(VariantTypeTest, has_value_false)
{
    VariantType<int*, bool, char, int> type{};
    EXPECT_FALSE(type.has_value());
}

TEST(VariantTypeTest, has_value_true)
{
    VariantType<int*, bool, char, int> type{true};
    EXPECT_TRUE(type.has_value());
}

TEST(VariantTypeTest, type)
{
    VariantType<int*, bool, char, int, const char*> type{"Hello"};
    EXPECT_EQ(typeid(const char*), type.type());
}

TEST(VariantTypeTest, reset)
{
    VariantType<int*, bool, char, int, const char*> type{"Hello"};
    EXPECT_STREQ("Hello", type.to<const char*>());
    type.reset();
    EXPECT_THROW(type.to<const char*>(), VariantTypeToError);
    EXPECT_FALSE(type.has_value());
}