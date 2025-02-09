#include <cstdlib>

#include <print>
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>

#include "stopwatch.hpp"
#include "file_reader.hpp"
#include "async_concurrent_reader.hpp"
#include "pool_concurrent_reader.hpp"
#include "async_parallel_reader.hpp"

struct Context
{
    std::size_t filesCount;
    Stopwatch::TimeType filesFound;
    Stopwatch::TimeType filesProcessedParallel;
    Stopwatch::TimeType filesProcessedAsync;
    Stopwatch::TimeType filesProcessedPool;
    Stopwatch::TimeType filesProcessedSingleCore;
    LineCount<std::uint64_t> statsSingleCore;
    LineCount<std::uint64_t> statsParallel;
    LineCount<std::uint64_t> statsAsync; 
    LineCount<std::uint64_t> statsPool;
};

void print_data(const Context context, std::ostream& out)
{
    std::println(out, "All files found: {}", context.filesFound);
    std::println(out, "All files processed concurrently parallel implementation: {}", context.filesProcessedParallel);
    std::println(out, "All files processed concurrently async implementation: {}", context.filesProcessedAsync);
    std::println(out, "All files processed concurrently pool implementation: {}", context.filesProcessedPool);
    std::println(out, "All files processed single core: {}", context.filesProcessedSingleCore);
    const auto totalTime{context.filesFound + context.filesProcessedAsync + 
                         context.filesProcessedPool + context.filesProcessedSingleCore +
                         context.filesProcessedParallel};
    std::println(out, "All time: {}", totalTime);

    std::println(out);
    std::println(out, "Total files: {}", context.filesCount);

    std::println(out);
    std::println(out, "Parallel implementation:");
    std::println(out, "Total lines: {}", context.statsParallel.any);
    std::println(out, "Blank lines: {}", context.statsParallel.blank);
    std::println(out, "Comment lines: {}", context.statsParallel.comment);
    std::println(out, "Code lines: {}", context.statsParallel.code);

    std::println(out);
    std::println(out, "Async implementation:");
    std::println(out, "Total lines: {}", context.statsAsync.any);
    std::println(out, "Blank lines: {}", context.statsAsync.blank);
    std::println(out, "Comment lines: {}", context.statsAsync.comment);
    std::println(out, "Code lines: {}", context.statsAsync.code);

    std::println(out);
    std::println(out, "Pool implementation:");
    std::println(out, "Total lines: {}", context.statsPool.any);
    std::println(out, "Blank lines: {}", context.statsPool.blank);
    std::println(out, "Comment lines: {}", context.statsPool.comment);
    std::println(out, "Code lines: {}", context.statsPool.code);

    std::println(out);
    std::println(out, "Single core:");
    std::println(out, "Total lines: {}", context.statsSingleCore.any);
    std::println(out, "Blank lines: {}", context.statsSingleCore.blank);
    std::println(out, "Comment lines: {}", context.statsSingleCore.comment);
    std::println(out, "Code lines: {}", context.statsSingleCore.code);
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::println("Error 0 arguments provided! Provide project root directory.");
        return EXIT_FAILURE;
    }

    if(!std::filesystem::is_directory(argv[1]))
    {
        std::println("Error invalid argument! Provide project root directory.");
        return EXIT_FAILURE;
    }

    const std::vector<std::string_view> extensions{".cpp", ".hpp", ".c", ".h"};
    Context context{};
    Stopwatch stopwatch{};

    stopwatch.set_start();
    AsyncParallelReader parallelReader{extensions};
    parallelReader.process_files(argv[1]);
    stopwatch.set_finish();
    context.statsParallel = parallelReader.stats();
    context.filesProcessedParallel = stopwatch.time();

    stopwatch.set_start();
    auto files{file_reader::find_all_files_with_extensions(argv[1], extensions)};
    stopwatch.set_finish();
    context.filesCount = std::size(files);
    context.filesFound = stopwatch.time();

    stopwatch.set_start();
    Counter counter{};
    for(const auto& file : files)
    {
        file_reader::read_file_by_line(file, counter);
    }
    stopwatch.set_finish();
    context.filesProcessedSingleCore = stopwatch.time();
    context.statsSingleCore = counter.stats();
    
    stopwatch.set_start();
    AsyncConcurrentReader asyncReader{};
    asyncReader.process_files(files);
    stopwatch.set_finish();
    context.filesProcessedAsync = stopwatch.time();
    context.statsAsync = asyncReader.stats();

    stopwatch.set_start();
    PoolConcurrentReader poolReader{};
    poolReader.process_files(files);
    context.statsPool = poolReader.stats();
    stopwatch.set_finish();
    context.filesProcessedPool = stopwatch.time();

    print_data(context, std::cout);
    if(argc > 2)
    {
        std::ofstream dataStream{argv[2]};
        if(!dataStream.is_open())
        {
            throw std::runtime_error{"Error provided wrong filename!"};
        }
        print_data(context, dataStream);
    }
    else
    {
        std::ofstream dataStream{"default.txt"};
        print_data(context, dataStream);
    }
    return EXIT_SUCCESS;
}