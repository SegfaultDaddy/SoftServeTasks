#include <future>
#include <thread>

#include "file_reader.hpp"
#include "pool_concurrent_reader.hpp"

LineType<std::uint64_t> PCR::process_files_asynchronously(const std::vector<std::filesystem::path>& files)
{
    const auto size{std::thread::hardware_concurrency()};
    std::vector<std::future<void>> futures{};
    auto chunks{split(files, size)};
    for(const auto& chunk : chunks)
    {
        futures.push_back(pool.submit([&]{this->read_and_process_file(chunk);}));
    }
    for(const auto& future : futures)
    {
        future.wait();
    }
    return counter_.stats();
}

void PCR::reset() noexcept
{
    counter_.reset();
}

void PCR::read_and_process_file(const VectorChunk<std::filesystem::path>& chunk)
{
    for(auto i{chunk.begin}; i != chunk.end; ++i)
    {
        file_reader::read_file_by_line(*i, counter_);
    }
}