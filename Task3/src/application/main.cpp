#include <cstdlib>

#include <print>
#include <chrono>

#include "stopwatch.hpp"
#include "file_reader.hpp"
#include "concurrent_reader.hpp"

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
    Stopwatch timer{};
    ConcurrentReader reader{};

    timer.set_start();
    auto files{file_reader::find_all_files_with_extensions(argv[1], extensions)};
    timer.set_finish();
    const auto filesFoundTime{timer.time()};

    timer.set_start();
    const auto stats{reader.process_files_asynchronously(files)};
    timer.set_finish();
    const auto filesProcessedTime{timer.time()};

    timer.set_start();
    Counter counter{};
    for(const auto& file : files)
    {
        counter.count_line_types(file_reader::read_file_by_line(file));
    }
    timer.set_finish();
    const auto filesProcessedSingleCore{timer.time()};

    std::println("All files found: {}", filesFoundTime);
    std::println("All files processed concurrently: {}", filesProcessedTime);
    std::println("All files processed single core: {}", filesProcessedSingleCore);
    std::println("All time: {}", filesFoundTime + filesProcessedTime + filesProcessedSingleCore);

    std::println();
    std::println("Total lines: {}", stats.any);
    std::println("Blank lines: {}", stats.blank);
    std::println("Comment lines: {}", stats.comment);
    std::println("Code lines: {}", stats.code);

    std::println();
    const auto statsSingleCore{counter.counted_lines()};
    std::println("Total lines: {}", statsSingleCore.any);
    std::println("Blank lines: {}", statsSingleCore.blank);
    std::println("Comment lines: {}", statsSingleCore.comment);
    std::println("Code lines: {}", statsSingleCore.code);
    return EXIT_SUCCESS;
}