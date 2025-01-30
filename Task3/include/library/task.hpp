#ifndef TASK_HPP
#define TASK_HPP

#include <string>

struct Task
{
    std::string name;
    int processed;
    int size;
    bool done;
};

#endif