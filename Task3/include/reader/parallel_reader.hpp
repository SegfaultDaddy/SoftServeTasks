#ifndef PARALLEL_READER_HPP
#define PARALLEL_READER_HPP

#include <filesystem>

#include "line_count.hpp"
#include "counter.hpp"

class ParallelReader
{
public:
    LineCount<std::uint64_t> process_files(const std::filesystem::path& startDirectory, const std::vector<std::string_view>& extensions);
    void reset() noexcept;
private:
    void read_directory_and_process_files(const std::filesystem::path& startDirectory, const std::vector<std::string_view>& extensions);

    Counter counter_;   
};

#endif