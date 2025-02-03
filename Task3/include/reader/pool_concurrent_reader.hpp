#ifndef POOL_CONCURRENT_READER_HPP
#define POOL_CONCURRENT_READER_HPP

#include <filesystem>
#include <cstdint>

#include "vector_chunk.hpp"
#include "counter.hpp"
#include "thread_pool.hpp"

class PoolConcurrentReader
{
public:
    void process_files(const std::vector<std::filesystem::path>& files);
    LineCount<std::uint64_t> stats() const noexcept;
    void reset() noexcept;
private:
    void read_and_process_file(const VectorChunk<std::filesystem::path>& chunk);

    Counter counter_;   
    ThreadPool pool;
};
#endif