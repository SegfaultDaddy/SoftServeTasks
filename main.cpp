#include <print>

// TEMPORARY
#include <cstdlib>

// MAIN FUNCTIONALITY
#include "string_list.h"

void print(const string_list::ListNode list)
{
    auto current{list};
    while(current != nullptr)
    {
        std::println("{}", current[0]);
        current = reinterpret_cast<string_list::ListNode>(current[1]);
    }
}

int main()
{
    string_list::ListNode list{};

    string_list::init(&list);

    string_list::add(&list, "Hi");
    string_list::add(&list, "Hello");
    string_list::add(&list, "What's up");
    string_list::add(&list, "Good evening");
    string_list::add(&list, "Nice to meet you");
    string_list::add(&list, "Long time no see");

    print(list);
    std::println("{}", string_list::size(list));

    return EXIT_SUCCESS;
}