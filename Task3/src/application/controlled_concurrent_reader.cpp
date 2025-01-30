#include <ranges>

#include "file_reader.hpp"
#include "controlled_concurrent_reader.hpp"

void ControlledConcurrentReader::process_files_asynchronously(const std::vector<std::filesystem::path>& files)
{
    const auto size{std::thread::hardware_concurrency()};
    auto chunks{split(files, size)};
    tasks_.clear();
    tasks_.resize(std::size(chunks));
    for(const auto& [i, chunk] : chunks | std::views::enumerate)
    {
        futures_.push_back(std::async(std::launch::async, &ControlledConcurrentReader::read_and_process_file, 
                                      this, chunk, std::ref(tasks_[i])));
    }
}

LineType<std::uint64_t> ControlledConcurrentReader::wait_and_get()
{
    for(const auto& future : futures_)
    {
        future.wait();
    }
    return counter_.counted_lines();
}

void ControlledConcurrentReader::read_and_process_file(const VectorChunk<std::filesystem::path>& chunk, std::reference_wrapper<Task> task)
{
    auto& ref{task.get()};
    ref.done = false;
    for(auto i{chunk.begin}; i != chunk.end; ++i)
    {
        ref.name = i->string();
        auto data{file_reader::read_file_by_line(*i)};
        ref.size = std::size(data);
        counter_.count_line_types(data, task);
    }
    ref.done = true;
}

std::vector<Task> ControlledConcurrentReader::tasks() const noexcept
{
    return tasks_;
}

bool ControlledConcurrentReader::done() const noexcept
{
    auto currentTasks{tasks()};
    return std::all_of(std::begin(currentTasks), std::end(currentTasks), 
                       [](const auto& task){return task.done;});
}

bool ControlledConcurrentReader::done(const std::vector<Task>& tasks) const noexcept
{
    const auto& currentTasks{tasks};
    return std::all_of(std::begin(currentTasks), std::end(currentTasks), 
                       [](const auto& task){return task.done;});
}


LineType<std::uint64_t> ControlledConcurrentReader::counted_lines() const noexcept
{
    return counter_.counted_lines();
}