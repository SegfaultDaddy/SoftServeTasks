#include <future>

#include "pool_parallel_reader.hpp"
#include "file_reader.hpp"

LineCount<std::uint64_t> PoolParallelReader::stats() const noexcept
{
    return counter_.stats();
}

void PoolParallelReader::process_files(const std::filesystem::path& startDirectory, const std::vector<std::string_view>& extensions)
{
    if(!std::filesystem::is_directory(startDirectory))
    {
        throw std::runtime_error{"Error provided wrong start directory name!"};
    }
    read_directory_and_process_files(startDirectory, extensions); 
}

void PoolParallelReader::reset() noexcept
{
    counter_.reset();
}

void PoolParallelReader::read_directory_and_process_files(const std::filesystem::path& startDirectory, const std::vector<std::string_view>& extensions)
{
    std::vector<std::future<void>> futures{};
    std::vector<std::filesystem::path> files{};
    for(const auto& entry : std::filesystem::directory_iterator{startDirectory})
    {
        if(entry.is_regular_file())
        {
            const auto extension{entry.path().extension().string()};
            if(std::any_of(std::begin(extensions), std::end(extensions), 
                           [&](const auto& elem){return elem == extension;}))
            {
                files.push_back(entry);
            }
        }
        else if(entry.is_directory())
        {
            futures.push_back(pool_.submit([&, entry]{read_directory_and_process_files(entry, extensions);}));
        }
    }
    const auto size{std::thread::hardware_concurrency()};
    const auto chunks{split(files, size)};
    for(const auto& chunk : chunks)
    {
        futures.push_back(std::async(std::launch::async, &PoolParallelReader::read_and_process_file, this, chunk));
    }
    for(auto& future : futures)
    {
        future.wait();
    }
}

void PoolParallelReader::read_and_process_file(const VectorChunk<std::filesystem::path>& chunk)
{
    for(auto i{chunk.begin}; i != chunk.end; ++i)
    {
        file_reader::read_file_by_line(*i, counter_);
    }
}