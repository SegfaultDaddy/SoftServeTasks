#ifndef COUNTER_HPP
#define COUNTER_HPP 

#include <vector>
#include <string>

#include "line_type.hpp"

class Counter
{
public:
    void count_line_types(const std::vector<std::string>& data);
    const LineType& counted_lines() const noexcept;
    void reset() noexcept;
private:
    bool is_comment(const std::string& line, bool& multilineCommentApproached);
    void process_line(const std::string& line, bool& multiLineCommentApproached);

    LineType countedLines_;
};   

#endif