#ifndef ASYNC_PARALLEL_READER_HPP
#define ASYNC_PARALLEL_READER_HPP

#include <filesystem>

#include "line_count.hpp"
#include "counter.hpp"

class AsyncParallelReader
{
public:
    AsyncParallelReader(const std::vector<std::string_view>& extensions);
    void process_files(const std::filesystem::path& startDirectory);
    LineCount<std::uint64_t> stats() const noexcept;
    void reset() noexcept;
private:
    void read_directory_and_process_files(const std::filesystem::path& startDirectory);

    Counter counter_;   
    const std::vector<std::string_view> extensions_;
};

#endif