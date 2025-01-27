#include "counter.hpp"

void Counter::count_line_types(const std::vector<std::string>& data)
{
    for(const auto& line : data)
    {
        process_line(line);
    }
}

const Counter::LineType& Counter::counted_lines() const noexcept
{
    return countedLines_;
}

void Counter::process_line(const std::string& line)
{
    ++countedLines_.any;
    if(line.empty())
    {
        ++countedLines_.blank;
    }
    else if(line.contains("//"))
    {
        ++countedLines_.comment;
    }
    else
    {
        ++countedLines_.code;
    }
}