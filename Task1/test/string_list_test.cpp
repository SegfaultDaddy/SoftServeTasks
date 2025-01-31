#include <algorithm>
#include <ranges>
#include <unordered_set>

#include <gtest/gtest.h>

#include "string_list.hpp"

class EmptyStringListFixture : public ::testing::Test
{
public:
    EmptyStringListFixture()
    {
        string_list::init(&list);
    }

    ~EmptyStringListFixture()
    {
        string_list::destroy(&list);
    }
protected:
    string_list::List list;
};

class NonEmptyStringListFixture : public EmptyStringListFixture
{
public:
    NonEmptyStringListFixture()
    {
        using namespace string_list;

        for(const auto& elem : data)
        {
            push_back(list, elem);
        }
    }

protected:
    const static std::vector<const char*> data;
};

void hash_map_unique(std::vector<const char*>& data)
{
    std::unordered_set<const char*> seen{};
    
    data.erase(std::remove_if(std::begin(data), std::end(data),
                              [&seen](const char* x)
                              {
                                  return !seen.insert(x).second;
                              }),std::end(data));
}

const std::vector<const char*> NonEmptyStringListFixture::data
{
    "C",
    "D",
    "A",
    "B",
    "F",
    "Z",
    "C",
};

TEST(StringListTestLifetimeTest, init)
{
    using namespace string_list;

    List list{nullptr};
    init(&list);
    EXPECT_NE(nullptr, list);
}

TEST(StringListTestLifetimeTest, init_dies)
{
    using namespace string_list;

    EXPECT_DEATH(init(nullptr), "");
}

TEST(StringListTestLifetimeTest, destroy)
{
    using namespace string_list;

    List list{nullptr};
    init(&list);
    destroy(&list);
    EXPECT_EQ(nullptr, list);
}

TEST(StringListTestLifetime, destroy_dies_with_nullptr_provided)
{
    using namespace string_list;
    EXPECT_DEATH(destroy(nullptr), "");
}

TEST(StringListTestLifetime, destroy_dies_with_not_initialized_list)
{
    using namespace string_list;
    List list{};
    EXPECT_DEATH(destroy(&list), "");
}

TEST(StringListTestLifetime, size_dies)
{
    using namespace string_list;

    EXPECT_DEATH(size(nullptr), "");
}

TEST_F(EmptyStringListFixture, size)
{
    using namespace string_list;

    EXPECT_EQ(0, size(list));
}

TEST_F(NonEmptyStringListFixture, size)
{
    using namespace string_list;

    EXPECT_EQ(std::size(data), size(list));
}

TEST(StringListTestLifetime, empty_dies)
{
    using namespace string_list;

    EXPECT_DEATH(empty(nullptr), "");
}

TEST_F(EmptyStringListFixture, empty)
{
    using namespace string_list;

    EXPECT_TRUE(empty(list));
}

TEST_F(NonEmptyStringListFixture, empty)
{
    using namespace string_list;

    EXPECT_FALSE(empty(list));
}

TEST(StringListTestLifetime, index_of_dies)
{
    using namespace string_list;

    EXPECT_DEATH(index_of(nullptr, "A"), "");
}

TEST_F(EmptyStringListFixture, index_of)
{
    using namespace string_list;

    EXPECT_EQ(0, index_of(list, "A"));
}

TEST_F(NonEmptyStringListFixture, index_of)
{
    using namespace string_list;

    EXPECT_EQ(2, index_of(list, data[2]));
}

TEST(StringListTestLifetime, push_front_dies)
{
    using namespace string_list;

    EXPECT_DEATH(push_front(nullptr, "A"), "");
}

TEST_F(EmptyStringListFixture, push_front)
{
    using namespace string_list;
    using namespace string_list::implementation;

    push_front(list, "DATA");
    EXPECT_STREQ("DATA", value(head(list)));
}

TEST_F(NonEmptyStringListFixture, push_front)
{
    using namespace string_list;
    using namespace string_list::implementation;

    push_front(list, "DATA");
    EXPECT_STREQ("DATA", value(head(list)));
}

TEST(StringListTestLifetime, push_back_dies)
{
    using namespace string_list;

    EXPECT_DEATH(push_back(nullptr, "A"), "");
}

TEST_F(EmptyStringListFixture, push_back)
{
    using namespace string_list;
    using namespace string_list::implementation;

    push_back(list, "DATA");
    EXPECT_STREQ("DATA", value(tail(list)));
}

TEST_F(NonEmptyStringListFixture, push_back)
{
    using namespace string_list;
    using namespace string_list::implementation;

    push_back(list, "DATA");
    EXPECT_STREQ("DATA", value(tail(list)));
}

TEST(StringListTestLifetime, pop_front_dies)
{
    using namespace string_list;

    EXPECT_DEATH(pop_front(nullptr), "");
}

TEST_F(EmptyStringListFixture, pop_front_dies)
{
    using namespace string_list;
    using namespace string_list::implementation;

    EXPECT_DEATH(pop_front(list), "");
}

TEST_F(NonEmptyStringListFixture, pop_front)
{
    using namespace string_list;
    using namespace string_list::implementation;

    pop_front(list);
    EXPECT_STREQ(data.at(1), value(head(list)));
}

TEST(StringListTestLifetime, pop_back_dies)
{
    using namespace string_list;

    EXPECT_DEATH(pop_back(nullptr), "");
}

TEST_F(EmptyStringListFixture, pop_back_dies)
{
    using namespace string_list;
    using namespace string_list::implementation;

    EXPECT_DEATH(pop_back(list), "");
}

TEST_F(NonEmptyStringListFixture, pop_back)
{
    using namespace string_list;
    using namespace string_list::implementation;

    pop_back(list);
    EXPECT_STREQ(data.at(std::size(data) - 2), value(tail(list)));
}

TEST(StringListTestLifetime, remove_dies)
{
    using namespace string_list;

    EXPECT_DEATH(remove(nullptr, "A"), "");
}

TEST_F(EmptyStringListFixture, remove)
{
    using namespace string_list;

    EXPECT_NO_THROW(remove(list, "A"));
}

TEST_F(NonEmptyStringListFixture, remove_one)
{
    using namespace string_list;
    using namespace string_list::implementation;

    push_front(list, "REMOVE");
    push_back(list, "REMOVE");
    remove(list, "REMOVE", false);
    EXPECT_STRNE("REMOVE", value(head(list)));
    EXPECT_STREQ("REMOVE", value(tail(list)));
}

TEST_F(NonEmptyStringListFixture, remove_all)
{
    using namespace string_list;
    using namespace string_list::implementation;

    push_front(list, "REMOVE");
    push_back(list, "REMOVE");
    remove(list, "REMOVE", true);
    EXPECT_STRNE("REMOVE", value(head(list)));
    EXPECT_STRNE("REMOVE", value(tail(list)));
}

TEST(StringListTestLifetime, unique_dies)
{
    using namespace string_list;

    EXPECT_DEATH(unique(nullptr), "");
}

TEST_F(EmptyStringListFixture, unique)
{
    using namespace string_list;
    using namespace string_list::implementation;

    EXPECT_NO_THROW(unique(list));
}

TEST_F(NonEmptyStringListFixture, unique)
{
    using namespace string_list;
    using namespace string_list::implementation;

    EXPECT_EQ(std::size(data), size(list));
    auto copy{data};
    hash_map_unique(copy);
    unique(list);
    EXPECT_EQ(std::size(copy), size(list));
    for(const auto& [i, elem] : copy | std::views::enumerate)
    {
        EXPECT_EQ(i, index_of(list, elem));
    }
}

TEST(StringListTestLifetime, replace_dies)
{
    using namespace string_list;

    EXPECT_DEATH(replace(nullptr, "A", "B"), "");
}

TEST_F(EmptyStringListFixture, replace)
{
    using namespace string_list;
    using namespace string_list::implementation;

    EXPECT_NO_THROW(replace(list, "A", "C"));
}

TEST_F(NonEmptyStringListFixture, replace_one)
{
    using namespace string_list;
    using namespace string_list::implementation;

    push_front(list, "REPLACE");
    push_back(list, "REPLACE");
    replace(list, "REPLACE", "REPLACED", false);
    EXPECT_STREQ("REPLACED", value(head(list)));
    EXPECT_STRNE("REPLACED", value(tail(list)));
}

TEST_F(NonEmptyStringListFixture, replace_all)
{
    using namespace string_list;
    using namespace string_list::implementation;

    push_front(list, "REPLACE");
    push_back(list, "REPLACE");
    replace(list, "REPLACE", "REPLACED", true);
    EXPECT_STREQ("REPLACED", value(head(list)));
    EXPECT_STREQ("REPLACED", value(tail(list)));
}

TEST(StringListTestLifetime, sort_dies)
{
    using namespace string_list;

    EXPECT_DEATH(sort(nullptr), "");
}

TEST_F(EmptyStringListFixture, sort)
{
    using namespace string_list;
    using namespace string_list::implementation;

    EXPECT_NO_THROW(sort(list));
}

TEST_F(NonEmptyStringListFixture, sort)
{
    using namespace string_list;
    using namespace string_list::implementation;

    auto copy{data};
    hash_map_unique(copy);
    std::sort(std::begin(copy), std::end(copy), compare_less_than);
    unique(list);
    sort(list);
    for(const auto& [i, elem] : copy | std::views::enumerate)
    {
        EXPECT_EQ(i, index_of(list, elem));
    }
}