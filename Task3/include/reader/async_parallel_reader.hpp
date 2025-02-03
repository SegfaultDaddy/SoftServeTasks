#ifndef ASYNC_PARALLEL_READER_HPP
#define ASYNC_PARALLEL_READER_HPP

#include <filesystem>

#include "line_count.hpp"
#include "counter.hpp"

class AsyncParallelReader
{
public:
    void process_files(const std::filesystem::path& startDirectory, const std::vector<std::string_view>& extensions);
    LineCount<std::uint64_t> stats() const noexcept;
    void reset() noexcept;
private:
    void read_and_process_file(const VectorChunk<std::filesystem::path>& chunk);
    void read_directory_and_process_files(const std::filesystem::path& startDirectory, const std::vector<std::string_view>& extensions);

    Counter counter_;   
};

#endif