#include "counter.hpp"

void Counter::process_lines(const std::vector<std::string>& data)
{
    bool multiLineComment{false};
    for(const auto& line : data)
    {
        process_line(line, multiLineComment);
    }
}

LineType<std::uint64_t> Counter::stats() const noexcept
{
    LineType<std::uint64_t> copy{};
    copy.any = countedLines_.any;
    copy.blank = countedLines_.blank;
    copy.code = countedLines_.code;
    copy.comment = countedLines_.comment;
    return copy;
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

bool Counter::is_comment(const std::string& line, bool& multiLineComment) const
{
    auto start{line.find("/*")};
    auto end{line.find("*/")};
    auto singleLine{line.find("//")};
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
        if(singleLine < start)
        {
            return true;
        }
        else if(end != std::string::npos && end > start + 1)
        {
            multiLineComment = false;
        }
        else
        {
            multiLineComment = true;
        }
        return true;
    }
    return singleLine != std::string::npos;
}

void Counter::reset() noexcept
{
    countedLines_.any = 0;
    countedLines_.blank = 0;
    countedLines_.code = 0;
    countedLines_.comment = 0;
}