#include <fstream>
#include <cassert>

#include "file_reader.hpp"

namespace file_reader
{
    std::vector<std::string> read_file_by_line(const std::filesystem::path& path)
    {
        std::vector<std::string> lines{};
        std::string str{};
        std::ifstream in{path};
        if(!in.is_open())
        {
            return lines;
        }
        while(std::getline(in, str))
        {
            lines.push_back(str);
        }
        if(str.empty())
        {
            lines.push_back(str);
        }
        return lines;
    }

    std::vector<std::filesystem::path> find_all_files_with_extensions(const std::filesystem::path& startDirectory, const std::vector<std::string_view>& extensions)
    {
        std::vector<std::filesystem::path> files{};
        for(const auto& entry : std::filesystem::recursive_directory_iterator{startDirectory})
        {
            if(entry.is_regular_file())
            {
                auto extension{entry.path().extension().string()};
                if(std::any_of(std::begin(extensions), std::end(extensions), 
                                [&](const auto& elem){
                                return elem == extension;}))
                {
                    files.push_back(entry.path());
                }
            }
        }
        return files;
    }
}