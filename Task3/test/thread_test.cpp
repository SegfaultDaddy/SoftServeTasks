#include <future>

#include <gtest/gtest.h>

#include "thread_pool.hpp"
#include "threadsafe_queue.hpp"

class EmptyThreadsafeQueueFixture : public ::testing::Test
{
public:
    EmptyThreadsafeQueueFixture()
    {
    }

    ~EmptyThreadsafeQueueFixture()
    {
    }
protected:
    ThreadsafeQueue<int> queue;
};

class NonEmptyThreadsafeQueueFixture : public ::testing::Test
{
public:
    NonEmptyThreadsafeQueueFixture()
    {
        for(const auto& elem : data)
        {
            queue.push(elem);
        }
    }

    ~NonEmptyThreadsafeQueueFixture()
    {
    }
protected:
    ThreadsafeQueue<int> queue;
    const static std::vector<int> data;
};

const std::vector<int> NonEmptyThreadsafeQueueFixture::data
{
    1, 2, 3,
    4, 5, 6,
    7, 8, 9,
    10, 11, 12,
};

TEST_F(EmptyThreadsafeQueueFixture, default_constructor)
{
    EXPECT_TRUE(queue.empty());
}

TEST_F(EmptyThreadsafeQueueFixture, push)
{
    queue.push(10);
    EXPECT_EQ(10, *queue.try_pop());
}

TEST_F(NonEmptyThreadsafeQueueFixture, push)
{
    queue.push(10);
    EXPECT_NE(10, *queue.try_pop());
}

TEST_F(EmptyThreadsafeQueueFixture, threaded_push_gives_no_data_race)
{
    const std::size_t size{15};
    std::atomic_bool start{false};
    auto function{[&](const int value)
                  {
                      while(!start){}
                      for(std::size_t i{0}; i < size; i += 1)
                      {
                          queue.push(value);
                      }
                  }};
    std::vector<std::future<void>> futures{};
    futures.push_back(std::async(std::launch::async, function, 1));
    futures.push_back(std::async(std::launch::async, function, 2));
    futures.push_back(std::async(std::launch::async, function, 3));
    futures.push_back(std::async(std::launch::async, function, 4));
    start = true;
    for(auto& future : futures)
    {
        future.wait();
    }
    std::size_t actualSize{0};
    while(!queue.empty())
    {
        queue.try_pop();
        actualSize += 1;
    }
    EXPECT_EQ(std::size(futures) * size, actualSize);
}

TEST_F(NonEmptyThreadsafeQueueFixture, try_pop)
{
    std::atomic_bool start{false};
    auto function{[&]
                  {
                      while(!start){}
                      queue.try_pop();
                  }};
    std::vector<std::future<void>> futures{};
    for(const auto &elem : data)
    {
        futures.push_back(std::async(std::launch::async, function));
    }
    start = true;
    for(auto& future : futures)
    {
        future.wait();
    }
    EXPECT_TRUE(queue.empty());
}

TEST_F(NonEmptyThreadsafeQueueFixture, wait_and_pop)
{
    std::atomic_bool start{false};
    auto function{[&]
                  {
                      while(!start){}
                      queue.wait_and_pop();
                  }};
    std::vector<std::future<void>> futures{};
    for(const auto &elem : data)
    {
        futures.push_back(std::async(std::launch::async, function));
    }
    start = true;
    for(auto& future : futures)
    {
        future.wait();
    }
    EXPECT_TRUE(queue.empty());
}

TEST_F(EmptyThreadsafeQueueFixture, empty)
{
    EXPECT_TRUE(queue.empty());
}

TEST_F(NonEmptyThreadsafeQueueFixture, empty)
{
    EXPECT_FALSE(queue.empty());
}