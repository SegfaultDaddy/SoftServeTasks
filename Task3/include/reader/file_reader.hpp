#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include <filesystem>
#include <vector>
#include <string>
#include <string_view>

#include "counter.hpp"

namespace file_reader
{
    void read_file_by_line(const std::filesystem::path& path, Counter& counter);
    std::vector<std::filesystem::path> find_all_files_with_extensions(const std::filesystem::path& startDirectory, 
                                                                      const std::vector<std::string_view>& extensions);
}

#endif