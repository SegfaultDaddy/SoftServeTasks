#include <print>

#include "counter.hpp"

void Counter::count_line_types(const std::vector<std::string>& data)
{
    bool multiLineComment{false};
    for(const auto& line : data)
    {
        process_line(line, multiLineComment);
    }
}

const LineType& Counter::counted_lines() const noexcept
{
    return countedLines_;
}

void Counter::process_line(const std::string& line, bool& multiLineComment)
{
    countedLines_.any += 1;
    if(line.empty())
    {
        countedLines_.blank += 1;
    }
    else if(is_comment(line, multiLineComment))
    {
        countedLines_.comment += 1;
    }
    else
    {
        countedLines_.code += 1;
    }
}

bool Counter::is_comment(const std::string& line, bool& multiLineComment)
{
    auto start{line.find_first_of("/*")};
    auto end{line.find_first_of("*/")};
    if(multiLineComment)
    {
        if(end != std::string::npos)
        {
            multiLineComment = false;
        }
        return true;
    }
    else if(start != std::string::npos)
    {
        if(end != std::string::npos && end > start + 1)
        {
            multiLineComment = false;
        }
        else
        {
            multiLineComment = true;
        }
        return true;
    }
    return line.contains("//");
}

void Counter::reset() noexcept
{
    countedLines_.any = 0;
    countedLines_.blank = 0;
    countedLines_.code = 0;
    countedLines_.comment = 0;
}