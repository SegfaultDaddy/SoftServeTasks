#ifndef PARALLEL_READER_HPP
#define PARALLEL_READER_HPP

#include <future>
#include <filesystem>

#include "line_count.hpp"
#include "counter.hpp"
#include "file_reader.hpp"

class ParallelReader
{
public:
    LineCount<std::uint64_t> process_files(const std::filesystem::path& startDirectory, const std::vector<std::string_view>& extensions)
    {
        if(!std::filesystem::is_directory(startDirectory))
        {
            return counter_.stats();
        }
        read_directory_and_process_files(startDirectory, extensions); 
        return counter_.stats();
    }
    void reset() noexcept
    {
        counter_.reset();
    }
private:
    void read_directory_and_process_files(const std::filesystem::path& startDirectory, const std::vector<std::string_view>& extensions)
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
                    file_reader::read_file_by_line(entry, counter_);
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

    Counter counter_;   
};

#endif