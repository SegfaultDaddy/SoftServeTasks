#ifndef COUNTER_HPP
#define COUNTER_HPP 

#include <vector>
#include <string>
#include <atomic>

#include "task.hpp"
#include "line_type.hpp"

class Counter
{
public:
    void count_line_types(const std::vector<std::string>& data);
    void count_line_types(const std::vector<std::string>& data, std::reference_wrapper<Task> task);
    LineType<std::uint64_t> counted_lines() const noexcept;
    void reset() noexcept;
private:
    bool is_comment(const std::string& line, bool& multilineCommentApproached) const;
    void process_line(const std::string& line, bool& multiLineCommentApproached);

    LineType<std::atomic_uint64_t> countedLines_;
};   

#endif