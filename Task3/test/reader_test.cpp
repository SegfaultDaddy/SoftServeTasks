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