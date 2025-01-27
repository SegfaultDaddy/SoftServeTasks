#include "counter.hpp"

void Counter::count_line_types(const std::vector<std::string>& data)
{
    for(const auto& line : data)
    {
        process_line(line);
    }
}

const LineType& Counter::counted_lines() const noexcept
{
    return countedLines_;
}

void Counter::process_line(const std::string& line)
{
    countedLines_.any += 1;
    if(line.empty())
    {
        countedLines_.blank += 1;
    }
    else if(is_comment(line))
    {
        countedLines_.comment += 1;
    }
    else
    {
        countedLines_.code += 1;
    }
}

bool Counter::is_comment(const std::string& line)
{
    return line.contains("//");
}