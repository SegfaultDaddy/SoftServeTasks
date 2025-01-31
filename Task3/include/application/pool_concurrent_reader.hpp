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
    LineType<std::uint64_t> process_files_asynchronously(const std::vector<std::filesystem::path>& files);
    void reset() noexcept;
private:
    void read_and_process_file(const VectorChunk<std::filesystem::path>& chunk);

    Counter counter_;   
    ThreadPool pool;
};
#endif