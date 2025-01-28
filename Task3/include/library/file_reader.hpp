#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include <filesystem>
#include <vector>
#include <string>
#include <string_view>

namespace file_reader
{
    std::vector<std::string> read_file_by_line(const std::filesystem::path& path);
    std::vector<std::filesystem::path> find_all_files_with_extensions(const std::filesystem::path& startDirectory, 
                                                                    const std::vector<std::string_view>& extensions);
}

#endif