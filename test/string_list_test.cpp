#include <cstring>

#include <gtest/gtest.h>

#include "string_list.hpp"

TEST(StringList, init) 
{
    using namespace string_list;

    List list{nullptr};
    EXPECT_NO_THROW(init(&list));
    EXPECT_NE(list, nullptr);
    destroy(&list);
}

TEST(StringList, destroy) 
{
    using namespace string_list;

    List list{nullptr};
    init(&list);
    EXPECT_NO_THROW(destroy(&list));
    EXPECT_EQ(list, nullptr);
}

TEST(StringList, size) 
{
    using namespace string_list;

    List list{nullptr};
    init(&list);
    EXPECT_EQ(size(list), 0);
    push_back(list, "Hi");
    push_back(list, "Hello");
    push_back(list, "Good evening");
    push_back(list, "Nice to meet you");
    push_back(list, "Long time no see");
    EXPECT_EQ(size(list), 5);
    destroy(&list);
}

TEST(StringList, empty) 
{
    using namespace string_list;

    List list{nullptr};
    init(&list);
    EXPECT_TRUE(empty(list));
    EXPECT_EQ(size(list) == 0, empty(list));
    push_back(list, "Hi");
    EXPECT_FALSE(empty(list));
    destroy(&list);
}

TEST(StringList, index_of) 
{
    using namespace string_list;

    List list{nullptr};
    init(&list);
    EXPECT_DEATH(index_of(list, "Hello", true), "Failed to find specified string in list");
    EXPECT_EQ(index_of(list, "Hello"), 0);
    push_back(list, "Hi");
    push_back(list, "Hello");
    push_back(list, "Good evening");
    push_back(list, "Good afternoon");
    push_back(list, "Nice to meet you");
    push_back(list, "Long time no see");
    EXPECT_EQ(index_of(list, "Hello"), 1);
    EXPECT_EQ(index_of(list, "Long time no see"), 5);
    destroy(&list);
}

TEST(StringList, push_front) 
{
    using namespace string_list;
    using namespace implementation;

    List list{nullptr};
    init(&list);
    push_front(list, "Hi");
    push_front(list, "Hello");
    push_front(list, "Good evening");
    push_front(list, "Good afternoon");
    push_front(list, "Nice to meet you");
    push_front(list, "Long time no see");
    const auto conditionFirst{std::strcmp(value(head(list)), "Long time no see")};
    EXPECT_TRUE(conditionFirst == 0);
    destroy(&list);
    init(&list);
    push_front(list, "Hi");
    EXPECT_NO_THROW(push_back(list, "Hello"));
    EXPECT_NO_THROW(push_front(list, "Long time no see"));
    const auto conditionSecond{std::strcmp(value(head(list)), "Long time no see")};
    EXPECT_TRUE(conditionSecond == 0);
    destroy(&list);
}

TEST(StringList, push_back) 
{
    using namespace string_list;
    using namespace implementation;

    List list{nullptr};
    init(&list);
    push_back(list, "Hi");
    push_back(list, "Hello");
    push_back(list, "Good evening");
    push_back(list, "Good afternoon");
    push_back(list, "Nice to meet you");
    push_back(list, "Long time no see");
    const auto conditionFirst{std::strcmp(value(tail(list)), "Long time no see")};
    EXPECT_TRUE(conditionFirst == 0);
    destroy(&list);
    init(&list);
    push_back(list, "Hi");
    EXPECT_NO_THROW(push_front(list, "Hello"));
    EXPECT_NO_THROW(push_back(list, "Long time no see"));
    const auto conditionSecond{std::strcmp(value(tail(list)), "Long time no see")};
    EXPECT_TRUE(conditionSecond == 0);
    destroy(&list);
}

TEST(StringList, pop_front) 
{
    using namespace string_list;

    List list{nullptr};
    init(&list);
    destroy(&list);   
}

TEST(StringList, pop_back) 
{
    using namespace string_list;

    List list{nullptr};
    init(&list);
    destroy(&list);
}

TEST(StringList, remove) 
{
    using namespace string_list;

    List list{nullptr};
    init(&list);
    destroy(&list);
}

TEST(StringList, unique) 
{
    using namespace string_list;

}

TEST(StringList, replace) 
{
    using namespace string_list;

}

TEST(StringList, sort) 
{
    using namespace string_list;

}