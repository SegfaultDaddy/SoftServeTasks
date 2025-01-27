#include <cstdlib>

#include <ranges>
#include <print>
#include <chrono>
#include <future>
#include <stack>
#include <fstream>

#include "file_reader.hpp"
#include "vector_chunk.hpp"
#include "counter.hpp" 

void read_file(const VectorChunk<std::filesystem::path>& chunk, Counter* counter)
{
    for(auto i{chunk.begin}; i != chunk.end; ++i)
    {
        counter->count_line_types(file_reader::read_file_by_line(*i));
    }
}

void first_method(const std::vector<std::filesystem::path>& files, Counter& counter)
{
    const auto size{std::thread::hardware_concurrency()};
    std::vector<std::future<void>> futures{};

    auto chunks{split(files, size)};
    for(const auto& chunk : chunks)
    {
        futures.push_back(std::async(std::launch::async, &read_file, chunk, &counter));
    }

    for(const auto& future : futures)
    {
        future.wait();
    }
}

int main(int argc, char** argv)
{
    const std::filesystem::path doomPath{"D:/SoftServe/Doom"};
    Counter concurrentCounter{};
    const auto start{std::chrono::steady_clock::now()};

    auto files{file_reader::find_all_files_with_extensions(doomPath, {".cpp", ".hpp", ".c", ".h"})};

    const auto allFilesFound{std::chrono::steady_clock::now()};

    first_method(files, concurrentCounter);

    const auto finish{std::chrono::steady_clock::now()};

    std::println("All files found: {}", std::chrono::duration_cast<std::chrono::milliseconds>(allFilesFound - start));
    std::println("All files processed: {}", std::chrono::duration_cast<std::chrono::milliseconds>(finish - allFilesFound));
    std::println("All time: {}", std::chrono::duration_cast<std::chrono::milliseconds>(finish - start));

    std::println();
    std::println("Files counted concurrently any: {}", concurrentCounter.counted_lines().any.load());
    std::println("Files counted concurrently blank: {}", concurrentCounter.counted_lines().blank.load());
    std::println("Files counted concurrently comment: {}", concurrentCounter.counted_lines().comment.load());
    std::println("Files counted concurrently code: {}", concurrentCounter.counted_lines().code.load());
    return EXIT_SUCCESS;
}