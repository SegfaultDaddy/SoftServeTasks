#ifndef CONSOLE_UI_HPP
#define CONSOLE_UI_HPP

#include <string>

#include <ftxui/dom/elements.hpp>

#include "task.hpp"
#include "controlled_concurrent_reader.hpp"

class ConsoleUI
{
public:
    void run(const std::vector<std::filesystem::path>& files);
private:
    ftxui::Element render(const std::vector<Task>& tasks, const LineType<std::uint64_t> stats, 
                          const std::chrono::milliseconds time);
    ftxui::Element render_summary(const LineType<std::uint64_t> stats, const std::chrono::milliseconds time);
    ftxui::Element render_task(const Task& task);
    ftxui::Element to_text(const int number);
    ftxui::Element to_text(const std::uint64_t number);

    ControlledConcurrentReader reader_;
    std::string resetPosition_;
};

#endif