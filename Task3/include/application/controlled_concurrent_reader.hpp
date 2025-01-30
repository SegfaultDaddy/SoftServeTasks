#ifndef CONTROLLED_CONCURRENT_READER_HPP
#define CONTROLLED_CONCURRENT_READER_HPP

#include <vector>
#include <filesystem>
#include <future>

#include "line_type.hpp"
#include "vector_chunk.hpp"
#include "counter.hpp"
#include "task.hpp"

class ControlledConcurrentReader
{
public:
    void process_files_asynchronously(const std::vector<std::filesystem::path>& files);
    LineType<std::uint64_t> wait_and_get();
    std::vector<Task> tasks() const noexcept;
    bool done() const noexcept;
    bool done(const std::vector<Task>& tasks) const noexcept;
    LineType<std::uint64_t> counted_lines() const noexcept;
private:
    void read_and_process_file(const VectorChunk<std::filesystem::path>& chunk, std::reference_wrapper<Task> task);

    Counter counter_;
    std::vector<Task> tasks_;
    std::vector<std::future<void>> futures_;
};

#endif