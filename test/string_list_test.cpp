#include <cstring>

#include <random>
#include <ranges>

#include <gtest/gtest.h>

#include "string_list.hpp"

TEST(StringList, init) 
{
    using namespace string_list;

    List list{nullptr};
    EXPECT_DEATH(init(nullptr), "Provide viable address not nullptr");
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
    EXPECT_DEATH(destroy(nullptr), "Provide viable address not nullptr");
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
    push_back(list, "Hi");
    push_back(list, "Hello");
    push_back(list, "Good evening");
    push_back(list, "Good afternoon");
    push_back(list, "Nice to meet you");
    push_back(list, "Long time no see");
    pop_front(list);
    pop_front(list);
    pop_front(list);
    EXPECT_EQ(size(list), 3);
    pop_front(list);
    pop_front(list);
    pop_front(list);
    EXPECT_NO_THROW(push_front(list, "Hi"));
    push_back(list, "Hello");
    push_back(list, "Good evening");
    push_back(list, "Good afternoon");
    push_back(list, "Nice to meet you");
    push_back(list, "Long time no see");
    EXPECT_NO_THROW(pop_back(list));
    EXPECT_NO_THROW(pop_front(list));
    EXPECT_EQ(size(list), 4);
    destroy(&list);   
}

TEST(StringList, pop_back) 
{
    using namespace string_list;

    List list{nullptr};
    init(&list);
    push_back(list, "Hi");
    push_back(list, "Hello");
    push_back(list, "Good evening");
    push_back(list, "Good afternoon");
    push_back(list, "Nice to meet you");
    push_back(list, "Long time no see");
    pop_back(list);
    pop_back(list);
    pop_back(list);
    EXPECT_EQ(size(list), 3);
    pop_back(list);
    pop_back(list);
    pop_back(list);
    EXPECT_NO_THROW(push_front(list, "Hi"));
    push_back(list, "Hello");
    push_back(list, "Good evening");
    push_back(list, "Good afternoon");
    push_back(list, "Nice to meet you");
    push_back(list, "Long time no see");
    EXPECT_NO_THROW(pop_front(list));
    EXPECT_NO_THROW(pop_back(list));
    EXPECT_EQ(size(list), 4);
    destroy(&list);
}

TEST(StringList, remove) 
{
    using namespace string_list;

    List list{nullptr};
    init(&list);
    push_back(list, "Hi");
    push_back(list, "Hello");
    push_back(list, "Good evening");
    push_back(list, "Good evening");
    push_back(list, "Hi");
    push_back(list, "Good afternoon");
    push_back(list, "Nice to meet you");
    push_back(list, "Long time no see");
    push_back(list, "Hi");
    EXPECT_NO_THROW(remove(list, "Hi"));
    EXPECT_EQ(size(list), 6);
    EXPECT_DEATH(index_of(list, "Hi", true), "Failed to find specified string in list");
    EXPECT_NO_THROW(remove(list, "Good evening", true));
    EXPECT_EQ(size(list), 5);
    EXPECT_EQ(index_of(list, "Good evening"), 1);
    EXPECT_NO_THROW(remove(list, "Hello"));
    EXPECT_NO_THROW(remove(list, "Good evening"));
    EXPECT_NO_THROW(remove(list, "Good afternoon"));
    EXPECT_NO_THROW(remove(list, "Nice to meet you"));
    EXPECT_NO_THROW(remove(list, "Long time no see"));
    EXPECT_EQ(size(list), 0);
    EXPECT_NO_THROW(push_back(list, "Hi"));
    EXPECT_EQ(size(list), 1);
    EXPECT_EQ(index_of(list, "Hi"), 0);
    destroy(&list);
}

TEST(StringList, unique) 
{
    using namespace string_list;

    List list{nullptr};
    init(&list);
    push_back(list, "Hi");
    push_back(list, "Hi");
    push_back(list, "Hi");
    push_back(list, "Hi");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Good evening");
    push_back(list, "Good evening");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Hi");
    push_back(list, "Good afternoon");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Nice to meet you");
    push_back(list, "Nice to meet you");
    push_back(list, "Nice to meet you");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Nice to meet you");
    push_back(list, "Nice to meet you");
    push_back(list, "Nice to meet you");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Nice to meet you");
    push_back(list, "Nice to meet you");
    push_back(list, "Nice to meet you");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Nice to meet you");
    push_back(list, "Nice to meet you");
    push_back(list, "Nice to meet you");
    push_back(list, "Nice to meet you");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Hello");
    push_back(list, "Nice to meet you");
    push_back(list, "Long time no see");
    push_back(list, "Hi");
    EXPECT_NO_THROW(unique(list));
    EXPECT_EQ(size(list), 6);
    push_back(list, "Hola");
    EXPECT_EQ(size(list), 7);
    destroy(&list);
    init(&list);
    EXPECT_NO_THROW(unique(list));
    push_back(list, "Hello");
    EXPECT_NO_THROW(unique(list));
    EXPECT_EQ(size(list), 1);
    EXPECT_EQ(index_of(list, "Hello"), 0);
    destroy(&list);
}

TEST(StringList, replace) 
{
    using namespace string_list;

    List list{nullptr};
    init(&list);
    EXPECT_NO_THROW(replace(list, "Hi", "Hello"));
    push_back(list, "Hi");
    push_back(list, "Hello");
    push_back(list, "Good evening");
    push_back(list, "Good afternoon");
    push_back(list, "Nice to meet you");
    push_back(list, "Long time no see");
    EXPECT_NO_THROW(replace(list, "Hi", "Hola"));
    EXPECT_EQ(index_of(list, "Hola"), 0);
    EXPECT_EQ(size(list), 6);
    destroy(&list);
}

TEST(StringList, sort) 
{
    using namespace string_list;

    const char* data[]
    {
        "H",
        "J",
        "A",
        "A",
        "L",
        "L",
        "L",
        "L",
        "L",
        "A",
        "A",
        "L",
        "L",
        "L",
        "A",
        "A",
        "A",
        "A",
        "B",
        "B",
        "B",
        "B",
        "L",
        "L",
        "A",
        "A",
        "A",
        "A",
        "A",
        "A",
        "B",
        "B",
        "B",
        "B",
        "B",
        "B",
        "G",
        "B",
        "C",
        "I",
        "C",
        "C",
    };

    List list{nullptr};

    std::default_random_engine dre{std::random_device{}()};
    for(const auto i : std::views::iota(0, 1000))
    {
        init(&list);
        //std::shuffle(std::begin(data), std::end(data), dre);
        for(const auto& elem : data)
        {
            push_back(list, elem);
        }
        sort(list);
        std::sort(std::begin(data), std::end(data), &implementation::compare_less_than);
        auto j{implementation::head(list)};
        for(auto k{0}; j != nullptr; k += 1)
        {
            EXPECT_TRUE(std::strcmp(data[k], implementation::value(j)) == 0);
            j = implementation::next(j);
        }
        destroy(&list);
    } 

    for(const auto i : std::views::iota(0, 1000))
    {
        init(&list);
        std::shuffle(std::begin(data), std::end(data), dre);
        for(const auto& elem : data)
        {
            push_back(list, elem);
        }
        sort(list);
        pop_back(list);
        EXPECT_EQ(size(list), std::size(data) - 1);
        destroy(&list);
    }
}