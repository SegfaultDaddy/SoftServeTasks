#ifndef COUNTER_HPP
#define COUNTER_HPP 

#include <vector>
#include <string>
#include <atomic>

#include "line_type.hpp"

class Counter
{
public:
    void process_lines(const std::vector<std::string>& data);
    void process_line(const std::string& line, bool& multiLineCommentApproached);
    LineCount<std::uint64_t> stats() const noexcept;
    void reset() noexcept;
private:
    bool is_comment(const std::string& line, bool& multilineCommentApproached) const;

    LineCount<std::atomic_uint64_t> countedLines_;
};   

#endif