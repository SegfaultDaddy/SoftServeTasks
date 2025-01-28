#include <cstdlib>

#include <ranges>
#include <print>
#include <chrono>
#include <future>
#include <stack>
#include <fstream>

#include "library/timer.hpp"
#include "library/file_reader.hpp"

#include "application/concurrent_reader.hpp"

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::println("Error 0 arguments provided! Provide root directory path of your project");
        return EXIT_FAILURE;
    }

    if(!std::filesystem::is_directory(argv[1]))
    {
        std::println("Error invalid argument! Provide root directory path of your project");
        return EXIT_FAILURE;
    }

    const std::vector<std::string_view> extensions{".cpp", ".hpp", ".c", ".h"};
    library::Timer timer{};
    application::ConcurrentReader reader{};

    timer.set_start();
    auto files{library::file_reader::find_all_files_with_extensions(argv[1], extensions)};
    timer.set_finish();
    const auto filesFoundTime{timer.time()};

    timer.set_start();
    const auto& stats{reader.process_files_asynchronously(files)};
    timer.set_finish();
    const auto filesProcessedTime{timer.time()};

    timer.set_start();
    library::Counter counter{};
    for(const auto& file : files)
    {
        counter.count_line_types(library::file_reader::read_file_by_line(file));
    }
    timer.set_finish();
    const auto filesProcessedSingleCore{timer.time()};

    std::println("All files found: {}", filesFoundTime);
    std::println("All files processed concurrently: {}", filesProcessedTime);
    std::println("All files processed single core: {}", filesProcessedSingleCore);
    std::println("All time: {}", filesFoundTime + filesProcessedTime + filesProcessedSingleCore);

    std::println();
    std::println("Total lines: {}", stats.any.load());
    std::println("Blank lines: {}", stats.blank.load());
    std::println("Comment lines: {}", stats.comment.load());
    std::println("Code lines: {}", stats.code.load());

    std::println();
    const auto& statsSingleCore{counter.counted_lines()};
    std::println("Total lines: {}", statsSingleCore.any.load());
    std::println("Blank lines: {}", statsSingleCore.blank.load());
    std::println("Comment lines: {}", statsSingleCore.comment.load());
    std::println("Code lines: {}", statsSingleCore.code.load());

    return EXIT_SUCCESS;
}