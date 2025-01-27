#include <cstdlib>

#include <ranges>
#include <print>
#include <chrono>
#include <future>
#include <stack>
#include <fstream>

#include "file_reader.hpp"
#include "counter.hpp" 

template<typename T>
struct VectorChunk
{
    std::vector<T>::const_iterator begin;
    std::vector<T>::const_iterator end;
};

template<typename T>
std::vector<VectorChunk<T>> split(const std::vector<T>& data, std::size_t parts)
{
    std::vector<VectorChunk<T>> chunks{};
    if(std::size(data) < parts)
    {
        chunks.emplace_back(std::begin(data), std::end(data));
        return chunks;
    }

    auto step{std::size(data) / parts};
    for(std::size_t i{0}; i < std::size(data); i += step) 
    {
		auto begin{std::begin(data) + i};
		auto end{std::begin(data) + std::min(std::size(data), i + step)};
        chunks.emplace_back(begin, end);
	}

    return chunks;
}

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

void single_core_method(const std::vector<std::filesystem::path>& files, Counter& counter)
{
    for(const auto& file : files)
    {
        counter.count_line_types(file_reader::read_file_by_line(file));
    }
}

int main(int argc, char** argv)
{
    const std::filesystem::path doomPath{"D:/SoftServe/TestTasks/Doom"};

    Counter counter{};
    Counter concurrentCounter{};

    const auto start{std::chrono::steady_clock::now()};

    auto files{file_reader::find_all_files_with_extensions(doomPath, {".cpp", ".hpp", ".c", ".h"})};

    const auto allFilesFound{std::chrono::steady_clock::now()};

    first_method(files, concurrentCounter);

    const auto allConcurrentFilesRead{std::chrono::steady_clock::now()}; 

    single_core_method(files, counter);

    const auto allFilesRead{std::chrono::steady_clock::now()};
    const auto finish{std::chrono::steady_clock::now()};

    std::println("All files found: {}", std::chrono::duration_cast<std::chrono::milliseconds>(allFilesFound - start));
    std::println("All files read concurrently: {}", std::chrono::duration_cast<std::chrono::milliseconds>(allConcurrentFilesRead - allFilesFound));
    std::println("All files read single core: {}", std::chrono::duration_cast<std::chrono::milliseconds>(allFilesRead - allConcurrentFilesRead));
    std::println("All time: {}", std::chrono::duration_cast<std::chrono::milliseconds>(finish - start));

    std::println();
    std::println("Files counted concurrently any: {}", concurrentCounter.counted_lines().any.load());
    std::println("Files counted concurrently blank: {}", concurrentCounter.counted_lines().blank.load());
    std::println("Files counted concurrently comment: {}", concurrentCounter.counted_lines().comment.load());
    std::println("Files counted concurrently code: {}", concurrentCounter.counted_lines().code.load());

    std::println();
    std::println("Files counted single core any: {}", counter.counted_lines().any.load());
    std::println("Files counted single core blank: {}", counter.counted_lines().blank.load());
    std::println("Files counted single core comment: {}", counter.counted_lines().comment.load());
    std::println("Files counted single core code: {}", counter.counted_lines().code.load());
    return EXIT_SUCCESS;
}