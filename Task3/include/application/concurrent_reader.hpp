#ifndef CONCURRENT_READER_HPP
#define CONCURRENT_READER_HPP

#include <filesystem>
#include <functional>

#include "library/vector_chunk.hpp"
#include "library/counter.hpp"

namespace application
{
    class ConcurrentReader
    {
    public:
        const library::LineType& process_files_asynchronously(const std::vector<std::filesystem::path>& files);
        void reset() noexcept;
    private:
        void read_file(const library::VectorChunk<std::filesystem::path>& chunk);

        library::Counter counter_;   
    };
}

#endif