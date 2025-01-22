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
    string_list::ListNode head{};

    string_list::init(&head);

    string_list::add(&head, "Hi");
    string_list::add(&head, "Hi");
    string_list::add(&head, "Hello");
    string_list::add(&head, "Hi");
    string_list::add(&head, "What's up");
    string_list::add(&head, "Hi");
    string_list::add(&head, "Good evening");
    string_list::add(&head, "Hi");
    string_list::add(&head, "Nice to meet you");
    string_list::add(&head, "Nice to meet you");
    string_list::add(&head, "Hi");
    string_list::add(&head, "Hi");
    string_list::add(&head, "Hi");
    string_list::add(&head, "Hi");
    string_list::add(&head, "Hi");
    string_list::add(&head, "Nice to meet you");
    string_list::add(&head, "Hi");
    string_list::add(&head, "Hi");
    string_list::add(&head, "Hi");
    string_list::add(&head, "Hi");
    string_list::add(&head, "What's up");
    string_list::add(&head, "What's up");
    string_list::add(&head, "Nice to meet you");
    string_list::add(&head, "Long time no see");

    string_list::remove_duplicates(head);
    print(head);

    string_list::destroy(&head);
    return EXIT_SUCCESS;
}