#include <print>
#include "string_list.hpp"

int main()
{
    string_list::List list{};
    string_list::init(&list);
    string_list::destroy(&list);
    return 0;
}