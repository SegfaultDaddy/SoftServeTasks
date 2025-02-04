#include <future>

#include "file_reader.hpp"
#include "vector_chunk.hpp"
#include "async_parallel_reader.hpp"

AsyncParallelReader::AsyncParallelReader(const std::vector<std::string_view>& extensions)
    : extensions_{extensions}
{
}

LineCount<std::uint64_t> AsyncParallelReader::stats() const noexcept
{
    return counter_.stats();
}

void AsyncParallelReader::process_files(const std::filesystem::path& startDirectory)
{
    if(!std::filesystem::is_directory(startDirectory))
    {
        throw std::runtime_error{"Error provided wrong start directory name!"};
    }
    read_directory_and_process_files(startDirectory); 
}

void AsyncParallelReader::reset() noexcept
{
    counter_.reset();
}

void AsyncParallelReader::read_directory_and_process_files(const std::filesystem::path& startDirectory)
{
    std::vector<std::future<void>> futures{};
    for(const auto& entry : std::filesystem::directory_iterator{startDirectory})
    {
        if(entry.is_regular_file())
        {
            const auto extension{entry.path().extension().string()};
            if(std::any_of(std::begin(extensions_), std::end(extensions_), 
                           [&](const auto& elem){return elem == extension;}))
            {
                file_reader::read_file_by_line(entry, counter_);
            }
        }
        else if(entry.is_directory())
        {
            futures.push_back(std::async(std::launch::async, &AsyncParallelReader::read_directory_and_process_files, this, entry));
        }
    }
    for(auto& future : futures)
    {
        future.wait();
    }
}