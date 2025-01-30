#include <thread>
#include <print>

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>

#include "stopwatch.hpp"
#include "console_ui.hpp"

void ConsoleUI::run(const std::vector<std::filesystem::path>& files)
{
    using namespace ftxui;
    using namespace std::chrono_literals;

    Stopwatch stopwatch{};
    stopwatch.set_start();
    reader_.process_files_asynchronously(files);
    filesFound_ = std::size(files);
    while(true)
    {
        auto tasks{reader_.tasks()};
        stopwatch.set_finish();
        auto document = render(tasks, reader_.counted_lines(), stopwatch.time());
        auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
        Render(screen, document);
        std::print("{}", resetPosition_);
        std::this_thread::sleep_for(0.01s);
        screen.Print();
        if(reader_.done(tasks))
        {
            break;
        }
        resetPosition_ = screen.ResetPosition();

    }
    reader_.wait_and_get();
    std::println();
}

ftxui::Element ConsoleUI::render(const std::vector<Task>& tasks, const LineType<std::uint64_t> stats, 
                                 const std::chrono::milliseconds time)
{
    using namespace ftxui;

    std::vector<Element> entries{};
    const auto max{std::max_element(std::begin(tasks), std::end(tasks), 
                                    [](const auto& first, const auto& second)
                                    {
                                        return std::size(first.name) < std::size(second.name);
                                    })};
    for(const auto& task : tasks)
    {
        entries.push_back(render_task(task, std::size(max->name)));
    }
    return vbox({window(text(" File "), vbox(std::move(entries))),
                 hbox({render_summary(stats, time),
                       filler(),
                      }),
    });
}

ftxui::Element ConsoleUI::render_summary(const LineType<std::uint64_t> stats, const std::chrono::milliseconds time)
{
    using namespace ftxui;

    auto summary = vbox({hbox({text("- Total files: "),
                               to_text(filesFound_) | bold,
                              }) | color(Color::Green),
                         hbox({text("- Any:         "),
                               to_text(stats.any) | bold,
                              }) | color(Color::Green),
                         hbox({text("- Blank:       "),
                               to_text(stats.blank) | bold,
                              }) | color(Color::Green),
                         hbox({text("- Comment:     "),
                               to_text(stats.comment) | bold,
                              }) | color(Color::Green),
                         hbox({text("- Code:        "),
                               to_text(stats.code) | bold,
                              }) | color(Color::Green),
                         hbox({text("- Time:        "),
                               text(std::format("{}", time)) | bold,
                              }) | color(Color::Green),
                        });
    return window(text(" Summary "), summary);
}

ftxui::Element ConsoleUI::render_task(const Task& task, const std::size_t paddings)
{
    using namespace ftxui;

    auto style = (task.processed == task.size) ? dim : bold;
    return hbox({text(task.name) | style | size(WIDTH, EQUAL, paddings),
                 separator(),
                 to_text(task.processed),
                 text("/"),
                 to_text(task.size),
                 separator(),
                 gauge(task.processed / static_cast<float>(task.size)),
                });
}

ftxui::Element ConsoleUI::to_text(const int number)
{
    using namespace ftxui;

    return text(std::to_string(number)) | size(WIDTH, EQUAL, 5);
}

ftxui::Element ConsoleUI::to_text(const std::uint64_t number)
{
    using namespace ftxui;

    return text(std::to_string(number)) | size(WIDTH, EQUAL, 12);
} 