#ifndef CONCURRENT_READER_HPP
#define CONCURRENT_READER_HPP

#include <filesystem>
#include <cstdint>

#include "vector_chunk.hpp"
#include "counter.hpp"

class ConcurrentReader
{
public:
    LineType<std::uint64_t> process_files_asynchronously(const std::vector<std::filesystem::path>& files);
    void reset() noexcept;
private:
    void read_and_process_file(const VectorChunk<std::filesystem::path>& chunk);

    Counter counter_;   
};

#endif