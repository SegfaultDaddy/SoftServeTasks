#include <print>

#include "./library/counter.hpp"

namespace library
{
    void Counter::count_line_types(const std::vector<std::string>& data)
    {
        bool multiLineCommentApproached{false};
        for(const auto& line : data)
        {
            process_line(line, multiLineCommentApproached);
        }
    }

    const LineType& Counter::counted_lines() const noexcept
    {
        return countedLines_;
    }

    void Counter::process_line(const std::string& line, bool& multiLineCommentApproached)
    {
        countedLines_.any += 1;
        if(line.empty())
        {
            countedLines_.blank += 1;
        }
        else if(is_comment(line, multiLineCommentApproached))
        {
            countedLines_.comment += 1;
        }
        else
        {
            countedLines_.code += 1;
        }
    }

    bool Counter::is_comment(const std::string& line, bool& multiLineCommentApproached)
    {
        auto start{line.find_first_of("/*")};
        auto end{line.find_first_of("*/")};
        if(multiLineCommentApproached)
        {
            if(end != std::string::npos)
            {
                multiLineCommentApproached = false;
            }
            return true;
        }
        else if(start != std::string::npos)
        {
            if(end != std::string::npos && end > start + 1)
            {
                multiLineCommentApproached = false;
            }
            else
            {
                multiLineCommentApproached = true;
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
}