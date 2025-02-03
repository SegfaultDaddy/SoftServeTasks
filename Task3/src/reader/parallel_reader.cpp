#include <future>
#include "file_reader.hpp"

#include "parallel_reader.hpp"

LineCount<std::uint64_t> ParallelReader::process_files(const std::filesystem::path& startDirectory, const std::vector<std::string_view>& extensions)
{
    if(!std::filesystem::is_directory(startDirectory))
    {
        throw std::runtime_error{"Error provided wrong start directory name!"};
        return counter_.stats();
    }
    read_directory_and_process_files(startDirectory, extensions); 
    return counter_.stats();
}

void ParallelReader::reset() noexcept
{
    counter_.reset();
}

void ParallelReader::read_directory_and_process_files(const std::filesystem::path& startDirectory, const std::vector<std::string_view>& extensions)
{
    std::vector<std::future<void>> futures{};
    for(const auto& entry : std::filesystem::directory_iterator{startDirectory})
    {
        if(entry.is_regular_file())
        {
            const auto extension{entry.path().extension().string()};
            if(std::any_of(std::begin(extensions), std::end(extensions), 
                            [&](const auto& elem)
                            {
                                return elem == extension;
                            }))
            {
                futures.push_back(std::async(std::launch::async, [&, entry]{file_reader::read_file_by_line(entry, counter_);}));
            }
        }
        else if(entry.is_directory())
        {
            futures.push_back(std::async(std::launch::async, &ParallelReader::read_directory_and_process_files, this, entry, extensions));
        }
    }
    for(auto& future : futures)
    {
        future.wait();
    }
}