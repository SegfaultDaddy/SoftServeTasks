#ifndef POOL_PARALLEL_READER_HPP
#define POOL_PARALLEL_READER_HPP

#include <filesystem>

#include "line_count.hpp"
#include "vector_chunk.hpp"
#include "counter.hpp"
#include "thread_pool.hpp"

class PoolParallelReader
{
public:
    void process_files(const std::filesystem::path& startDirectory, const std::vector<std::string_view>& extensions);
    LineCount<std::uint64_t> stats() const noexcept;
    void reset() noexcept;
private:
    void read_and_process_file(const VectorChunk<std::filesystem::path>& chunk);
    void read_directory_and_process_files(const std::filesystem::path& startDirectory, const std::vector<std::string_view>& extensions);

    ThreadPool pool_;
    Counter counter_;
};

#endif