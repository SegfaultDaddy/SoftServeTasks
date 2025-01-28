#include <vector>
#include <thread>
#include <future>

#include "library/file_reader.hpp"

#include "application/concurrent_reader.hpp"

namespace application
{
    const library::LineType& ConcurrentReader::process_files_asynchronously(const std::vector<std::filesystem::path>& files)
    {
        const auto size{std::thread::hardware_concurrency()};
        std::vector<std::future<void>> futures{};
        auto chunks{library::split(files, size)};
        for(const auto& chunk : chunks)
        {
            futures.push_back(std::async(std::launch::async, &ConcurrentReader::read_file, this, chunk));
        }
        for(const auto& future : futures)
        {
            future.wait();
        }
        return counter_.counted_lines();
    }

    void ConcurrentReader::reset() noexcept
    {
        counter_.reset();
    }

    void ConcurrentReader::read_file(const library::VectorChunk<std::filesystem::path>& chunk)
    {
        for(auto i{chunk.begin}; i != chunk.end; ++i)
        {
            counter_.count_line_types(library::file_reader::read_file_by_line(*i));
        }
    }
}