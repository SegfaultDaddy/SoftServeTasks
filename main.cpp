#include <print>

// TEMPORARY
#include <cstdlib>

// MAIN FUNCTIONALITY
#include "string_list.h"
#include "forward_string_list.h"

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
    string_list::List list{nullptr};
    string_list::init(&list);
    string_list::push_back(list, "Hi");
    string_list::push_front(list, "Hello");
    string_list::push_back(list, "Hola");
    string_list::push_front(list, "Welcome");
    string_list::push_front(list, "Greetings");
    string_list::push_back(list, "Long time no see");

    string_list::pop_front(list);
    string_list::pop_front(list);
    string_list::pop_front(list);
    string_list::pop_back(list);
    string_list::pop_back(list);
    string_list::pop_front(list);
    string_list::pop_back(list);

    string_list::push_back(list, "Hi");
    string_list::push_front(list, "Hello");
    string_list::push_back(list, "Hola");
    string_list::push_front(list, "Welcome");
    string_list::push_front(list, "Greetings");
    string_list::push_back(list, "Long time no see");
    print(reinterpret_cast<string_list::ListNode>(list[0]));
    return EXIT_SUCCESS;
}

/*  
void print(const forward_string_list::ListNode list)
{
    auto current{list};
    while(current != nullptr)
    {
        std::println("{}", current[0]);
        current = reinterpret_cast<forward_string_list::ListNode>(current[1]);
    }
}

forward_string_list::ListNode head{};

forward_string_list::init(&head);

forward_string_list::add(&head, "Hi");
forward_string_list::add(&head, "Hi");
forward_string_list::add(&head, "Hello");
forward_string_list::add(&head, "Hi");
forward_string_list::add(&head, "What's up");
forward_string_list::add(&head, "Hi");
forward_string_list::add(&head, "Good evening");
forward_string_list::add(&head, "Hi");
forward_string_list::add(&head, "Nice to meet you");
forward_string_list::add(&head, "Nice to meet you");
forward_string_list::add(&head, "Hi");
forward_string_list::add(&head, "Hi");
forward_string_list::add(&head, "Hi");
forward_string_list::add(&head, "Hi");
forward_string_list::add(&head, "Hi");
forward_string_list::add(&head, "Nice to meet you");
forward_string_list::add(&head, "Hi");
forward_string_list::add(&head, "Hi");
forward_string_list::add(&head, "Hi");
forward_string_list::add(&head, "Hi");
forward_string_list::add(&head, "What's up");
forward_string_list::add(&head, "What's up");
forward_string_list::add(&head, "Nice to meet you");
forward_string_list::add(&head, "Long time no see");

forward_string_list::remove_duplicates(head);
print(head);

forward_string_list::destroy(&head);
*/