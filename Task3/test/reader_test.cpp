#include <gtest/gtest.h>

#include <thread>

#include "counter.hpp"
#include "stopwatch.hpp"
#include "vector_chunk.hpp"

class CounterTest : public ::testing::Test
{
public:
    void SetUp() override
    {
        counter.process_lines(data);
    }

    Counter counter{}; 
    const static std::vector<std::string> data;
};

const std::vector<std::string> CounterTest::data
{
    "/*/",
    "kdgkjdfkfka",
    "/*/",
    "dfsdsfds",
    "/**/",
    "///**/",
    "",
    "//fdgfdg df",
    "dsfjkdskfdks",
};

TEST_F(CounterTest, count_line_types_works_properly)
{
    const auto stats{counter.stats()};
    EXPECT_EQ(9, stats.any);
    EXPECT_EQ(1, stats.blank);
    EXPECT_EQ(2, stats.code);
    EXPECT_EQ(6, stats.comment);
}

TEST_F(CounterTest, reset)
{
    counter.reset();
    const auto& stats{counter.stats()};
    EXPECT_EQ(0, stats.any);
    EXPECT_EQ(0, stats.blank);
    EXPECT_EQ(0, stats.code);
    EXPECT_EQ(0, stats.comment);
}

TEST(StopwatchTest, time_works)
{
    using namespace std::chrono_literals;

    Stopwatch stopwatch{};
    stopwatch.set_start();
    std::this_thread::sleep_for(1s);
    stopwatch.set_finish();
    EXPECT_LE(std::chrono::microseconds{1s}, stopwatch.time());
}

TEST(VectorChunkTest, split_with_enough_data_returns_enough_parts)
{
    std::vector<int> data{2, 3, 3, 4, 2, 3, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5};
    const auto chunks{split(data, 4)};
    EXPECT_EQ(4, std::size(chunks));
}

TEST(VectorChunkTest, split_with_enough_data_divides_properly)
{
    std::vector<int> data{2, 3, 3, 4, 2, 3, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5};
    const auto chunks{split(data, 4)};
    std::size_t size{0};
    for(const auto& chunk : chunks)
    {
        size += std::distance(chunk.begin, chunk.end);
    }
    EXPECT_EQ(std::size(data), size);
}

TEST(VectorChunkTest, split_with_not_enough_data_returns_only_one_chunk)
{
    std::vector<int> data{2, 3, 3};
    const auto chunks{split(data, 5)};
    EXPECT_EQ(1, std::size(chunks));
}

TEST(VectorChunkTest, split_with_not_enough_data_divides_properly)
{
    std::vector<int> data{2, 3, 3};
    const auto chunks{split(data, 5)};
    std::size_t size{0};
    for(const auto& chunk : chunks)
    {
        size += std::distance(chunk.begin, chunk.end);
    }
    EXPECT_EQ(std::size(data), size);
}