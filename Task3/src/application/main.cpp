#include <cstdlib>

#include <print>
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>

#include "stopwatch.hpp"
#include "file_reader.hpp"
#include "concurrent_reader.hpp"
#include "thread_pool.hpp"
#include "pool_concurrent_reader.hpp"

struct Contex
{
    std::size_t filesCount;
    Stopwatch::TimeType filesFound;
    Stopwatch::TimeType filesProcessed;
    Stopwatch::TimeType filesProcessedSingleCore;
    LineType<std::uint64_t> concurrent; 
    LineType<std::uint64_t> singleCore;
};

void print_data(const Contex data, std::ostream& out)
{
    std::println(out, "All files found: {}", data.filesFound);
    std::println(out, "All files processed concurrently: {}", data.filesProcessed);
    std::println(out, "All files processed single core: {}", data.filesProcessedSingleCore);
    std::println(out, "All time: {}", data.filesFound + data.filesProcessed + data.filesProcessedSingleCore);

    std::println(out, "Total files: {}", data.filesCount);
    std::println(out);
    std::println(out, "Multithreaded:");
    std::println(out, "Total lines: {}", data.concurrent.any);
    std::println(out, "Blank lines: {}", data.concurrent.blank);
    std::println(out, "Comment lines: {}", data.concurrent.comment);
    std::println(out, "Code lines: {}", data.concurrent.code);

    std::println(out);
    std::println(out, "Single core:");
    std::println(out, "Total lines: {}", data.singleCore.any);
    std::println(out, "Blank lines: {}", data.singleCore.blank);
    std::println(out, "Comment lines: {}", data.singleCore.comment);
    std::println(out, "Code lines: {}", data.singleCore.code);
}

int main(int argc, char** argv)
{
    /*
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
    */
    const std::vector<std::string_view> extensions{".cpp", ".hpp", ".c", ".h"};
    Stopwatch stopwatch{};
    stopwatch.set_start();
    auto files{file_reader::find_all_files_with_extensions("D:/SoftServe", extensions)};
    stopwatch.set_finish();
    const auto filesFoundTime{stopwatch.time()};

    stopwatch.set_start();
    PCR reader{};
    const auto stats{reader.process_files_asynchronously(files)};
    stopwatch.set_finish();
    const auto filesProcessedTime{stopwatch.time()};

    stopwatch.set_start();
    Counter counter{};
    for(const auto& file : files)
    {
        file_reader::read_file_by_line(file, counter);
    }
    stopwatch.set_finish();
    const auto filesProcessedSingleCore{stopwatch.time()};

    Contex data{};
    data.concurrent = stats;
    data.singleCore = counter.stats();
    data.filesCount = std::size(files);
    data.filesFound = filesFoundTime;
    data.filesProcessedSingleCore = filesProcessedSingleCore;
    data.filesProcessed = filesProcessedTime; 

    print_data(data, std::cout);
    if(argc > 2)
    {
        std::ofstream dataStream{argv[2]};
        print_data(data, dataStream);
    }
    else
    {
        std::ofstream dataStream{"default.txt"};
        print_data(data, dataStream);
    }
    return EXIT_SUCCESS;
}