#ifndef CONCURRENT_READER_HPP
#define CONCURRENT_READER_HPP

#include <filesystem>
#include <cstdint>

#include "vector_chunk.hpp"
#include "counter.hpp"

class AsyncConcurrentReader
{
public:
    void process_files(const std::vector<std::filesystem::path>& files);
    LineCount<std::uint64_t> stats() const noexcept;
    void reset() noexcept;
private:
    void read_and_process_file(const VectorChunk<std::filesystem::path>& chunk);

    Counter counter_;   
};

#endif