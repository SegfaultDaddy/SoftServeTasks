#include <thread>
#include <future>

#include "file_reader.hpp"

#include "async_concurrent_reader.hpp"

LineCount<std::uint64_t> AsyncConcurrentReader::process_files(const std::vector<std::filesystem::path>& files)
{
    const auto size{std::thread::hardware_concurrency()};
    std::vector<std::future<void>> futures{};
    auto chunks{split(files, size)};
    for(const auto& chunk : chunks)
    {
        futures.push_back(std::async(std::launch::async, &AsyncConcurrentReader::read_and_process_file, this, chunk));
    }
    for(const auto& future : futures)
    {
        future.wait();
    }
    return counter_.stats();
}

void AsyncConcurrentReader::reset() noexcept
{
    counter_.reset();
}

void AsyncConcurrentReader::read_and_process_file(const VectorChunk<std::filesystem::path>& chunk)
{
    for(auto i{chunk.begin}; i != chunk.end; ++i)
    {
        file_reader::read_file_by_line(*i, counter_);
    }
}