#include <print>

#include <cstdlib>

#include "string_list.h"

void print(const string_list::implementation::ListNode head)
{
    using namespace string_list::implementation;

    auto current{head};
    while(current != nullptr)
    {
        std::println("{}", current[0]);
        current = reinterpret_cast<ListNode>(current[1]);
    }
}

int main()
{
    string_list::List list{nullptr};

    string_list::init(&list);

    string_list::push_back(list, "Hi");
    string_list::push_back(list, "Hola");
    string_list::push_back(list, "Hello");
    string_list::push_back(list, "Welcome");
    string_list::push_back(list, "Greetings");
    string_list::push_back(list, "Long time no see");

    string_list::sort(list);
    
    std::println("Size: {}", string_list::size(list));
    print(reinterpret_cast<string_list::implementation::ListNode>(list[0]));

    string_list::destroy(&list);
    return EXIT_SUCCESS;
}