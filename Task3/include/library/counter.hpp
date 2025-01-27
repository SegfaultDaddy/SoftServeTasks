#ifndef COUNTER_HPP
#define COUNTER_HPP 

#include <vector>
#include <string>
#include <atomic>
#include <unordered_map>

class Counter
{
public:
    struct LineType
    {
        std::atomic_int64_t any;
        std::atomic_int64_t blank;
        std::atomic_int64_t comment;
        std::atomic_int64_t code;
    };

    void count_line_types(const std::vector<std::string>& data);
    const LineType& counted_lines() const noexcept;
private:
    void process_line(const std::string& line);

    LineType countedLines_;
};

#endif