#include <cstring>

#include <gtest/gtest.h>

#include <any_type.hpp>

struct Data
{
    int x;
    float y;
    double z;
};

TEST(AnyTypeTest, bool_copy_value_constructor)
{
    auto value{true};
    AnyType any{value};
    EXPECT_EQ(value, any.to<bool>());
}
    
TEST(AnyTypeTest, double_copy_value_constructor)
{
    auto value{1.2};
    AnyType any{value};
    EXPECT_EQ(value, any.to<double>());
}

TEST(AnyTypeTest, float_copy_value_constructor)
{
    auto value{1.2f};
    AnyType any{value};
    EXPECT_EQ(value, any.to<float>());
}
    
TEST(AnyTypeTest, int_copy_value_constructor)
{
    auto value{1};
    AnyType any{value};
    EXPECT_EQ(value, any.to<int>());
}

TEST(AnyTypeTest, const_char_pointer_copy_value_constructor)
{
    auto value{"Hello"};
    AnyType any{value};
    EXPECT_STREQ(value, any.to<const char*>());
}

TEST(AnyTypeTest, std_string_copy_value_constructor)
{
    std::string value{"AFDFDJSFksdkfdkjskjfdskfdkskfdksHello"};
    AnyType any{value};
    EXPECT_EQ(value, any.to<std::string>());
}

TEST(AnyTypeTest, int_pointer_copy_value_constructor)
{
    auto data{1};
    auto value{&data};
    AnyType any{value};
    EXPECT_EQ(value, any.to<int*>());
}

TEST(AnyTypeTest, array_int_copy_value_constructor)
{
    int value[]{1, 2, 3};
    AnyType any{value};
    EXPECT_EQ(value, any.to<int*>());
}

TEST(AnyTypeTest, custom_type_copy_value_constructor)
{
    Data data{1, 5.0f, 10};  
    AnyType any{data};
    auto[x, y, z]{any.to<Data>()};
    EXPECT_EQ(data.x, x);
    EXPECT_EQ(data.y, y);
    EXPECT_EQ(data.z, z);
}

TEST(AnyTypeTest, std_string_move_value_constructor)
{
    std::string value{"AFDFDJSFksdkfdkjskjfdskfdkskfdksHello"};
    std::string copy{"AFDFDJSFksdkfdkjskjfdskfdkskfdksHello"};
    AnyType any{std::move(value)};
    EXPECT_EQ(copy, any.to<std::string>());
}

TEST(AnyTypeTest, custom_type_move_value_constructor)
{
    Data data{1, 5.0f, 10};  
    Data copy{1, 5.0f, 10};  
    AnyType any{std::move(data)};
    auto[x, y, z]{any.to<Data>()};
    EXPECT_EQ(copy.x, x);
    EXPECT_EQ(copy.y, y);
    EXPECT_EQ(copy.z, z);
}

TEST(AnyTypeTest, any_type_copy_constructor_custom_type)
{
    Data data{1, 5.0f, 10};  
    AnyType any{data};
    AnyType copy{any};
    auto valFirst{any.to<Data>()};
    auto valSecond{copy.to<Data>()};
    EXPECT_EQ(valFirst.x, valSecond.x);
    EXPECT_EQ(valFirst.y, valSecond.y);
    EXPECT_EQ(valFirst.z, valSecond.z);
}

TEST(AnyTypeTest, any_type_move_constructor_custom_type)
{
    Data data{1, 5.0f, 10};  
    AnyType any{data};
    AnyType copy{std::move(any)};
    auto valSecond{copy.to<Data>()};
    EXPECT_EQ(data.x, valSecond.x);
    EXPECT_EQ(data.y, valSecond.y);
    EXPECT_EQ(data.z, valSecond.z);
}

TEST(AnyTypeTest, destructor)
{
    AnyType any{};
    any.~AnyType();
    EXPECT_FALSE(any.has_value());
}

TEST(AnyTypeTest, swap_copy_constructed_move_constructed_any_types)
{
    Data data{1, 5.0f, 10};  
    AnyType any{data};
    AnyType anyAny{"Hello"};
    swap(any, anyAny);
    EXPECT_STREQ("Hello", any.to<const char*>());
    auto value{anyAny.to<Data>()};
    EXPECT_EQ(data.x, value.x);
    EXPECT_EQ(data.y, value.y);
    EXPECT_EQ(data.z, value.z);
}

TEST(AnyTypeTest, swap_move_constructred_any_types)
{
    AnyType any{"Hi"};
    AnyType anyAny{"Hello"};
    any.swap(anyAny);
    EXPECT_STREQ("Hello", any.to<const char*>());
    EXPECT_STREQ("Hi", anyAny.to<const char*>());
}

TEST(AnyTypeTest, swap_self_any_type)
{
    AnyType any{"Hi"};
    any.swap(any);
    EXPECT_STREQ("Hi", any.to<const char*>());
}

TEST(AnyTypeTest, assignment_custom_type_multiple_fundamental_types)
{
    int a{21};
    float b{1.2};
    Data c{1, 2.0f, 3.0};
    AnyType type{a};
    EXPECT_EQ(a, type.to<int>());
    type = b;
    EXPECT_EQ(b, type.to<float>());
    type = c;
    EXPECT_EQ(c.x, type.to<Data>().x);
    EXPECT_EQ(c.y, type.to<Data>().y);
    EXPECT_EQ(c.z, type.to<Data>().z);
}

TEST(AnyTypeTest, assignment_any_type_from_moved_strings)
{
    AnyType any{"Hi"};
    AnyType anyAny{"Hello"};
    EXPECT_STREQ("Hi", any.to<const char*>());
    any = anyAny;
    EXPECT_STREQ("Hello", any.to<const char*>());
    EXPECT_STREQ("Hello", anyAny.to<const char*>());
}

TEST(AnyTypeTest, assignment_moved_value)
{
    AnyType any{"Hi"};
    any = 3.4;
    EXPECT_EQ(3.4, any.to<double>());
}

TEST(AnyTypeTest, has_value_default_constructed)
{
    AnyType any{};
    EXPECT_FALSE(any.has_value());
}

TEST(AnyTypeTest, has_value_after_assignment)
{
    AnyType any{};
    any = 1.24f;
    EXPECT_TRUE(any.has_value());
}

TEST(AnyTypeTest, reset_data_after_assignment)
{
    AnyType any{};
    any = 2.34554;
    any.reset();
    EXPECT_FALSE(any.has_value());
}

TEST(AnyTypeTest, to_throw)
{
    auto value{true};
    AnyType any{value};
    EXPECT_THROW(any.to<int>(), AnyTypeToError);
}

TEST(AnyTypeTest, to_no_throw)
{
    auto value{"Hello"};
    AnyType any{value};
    EXPECT_NO_THROW(any.to<const char*>());
    EXPECT_STREQ(value, any.to<const char*>());
}

TEST(AnyTypeTest, try_to_false)
{
    auto value{true};
    AnyType any{value};
    EXPECT_FALSE(any.try_to<int>().has_value());
}

TEST(AnyTypeTest, try_to_true)
{
    auto value{1.2};
    AnyType any{value};
    EXPECT_TRUE(any.try_to<double>().has_value());
}