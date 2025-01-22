#ifndef FORWARD_STRING_LIST_H
#define FORWARD_STRING_LIST_H 

#include <cstdlib>
#include <cstring>

namespace forward_string_list
{
    using ListNode = char**;
    
    void init(ListNode* head)
    {
        *head = nullptr;
    }

    void destroy(ListNode* head)
    {
        while(*head != nullptr)
        {
            auto remember{reinterpret_cast<ListNode>((*head)[1])};
            free(*head[0]);
            free(*head);
            *head = remember;
        }
        *head = nullptr;
    }

    void push_front(ListNode* node, const char* str)
    {
        const auto size{strlen(str) + 1};
        char* data{static_cast<char*>(std::malloc(sizeof(char) * (size)))};
        strcpy_s(data, size * sizeof(char), str);

        auto next{static_cast<ListNode>(std::malloc(sizeof(char*) + sizeof(ListNode)))};
        next[0] = data;
        next[1] = nullptr;

        auto current{*node};
        *node = next;

        if(current != nullptr)
        {
            next[1] = reinterpret_cast<char*>(current);
        }
    }

    void add(ListNode* node, const char* str)
    {
        const auto size{strlen(str) + 1};
        char* data{static_cast<char*>(std::malloc(sizeof(char) * (size)))};
        strcpy_s(data, size * sizeof(char), str);

        auto next{static_cast<ListNode>(std::malloc(sizeof(char*) + sizeof(ListNode)))};
        next[0] = data;
        next[1] = nullptr;

        if(*node == nullptr)
        {
            *node = next;
        }
        else
        {
            auto current{*node};
            while(current[1] != nullptr)
            {
                current = reinterpret_cast<ListNode>(current[1]);
            }
            current[1] = reinterpret_cast<char*>(next);
        }
    }

    bool remove(ListNode* head, const char* str)
    {
        ListNode prev{nullptr};
        auto current{*head};
        while(current != nullptr)
        {
            if(std::strcmp(current[0], str) == 0)
            {
                auto next{reinterpret_cast<ListNode>(current[1])};
                if(prev == nullptr) [[unlikely]]
                {
                    *head = next;
                }
                else 
                {
                    prev[1] = reinterpret_cast<char*>(next);
                }
                free(current[0]);
                free(current);
                return true;
            }
            prev = current;
            current = reinterpret_cast<ListNode>(current[1]);
        }
        return false;
    }

    std::size_t size(const ListNode head)
    {
        std::size_t counter{0};
        auto current{head};
        while(current != nullptr)
        {
            counter += 1;
            current = reinterpret_cast<ListNode>(current[1]);
        }
        return counter;
    }

    std::size_t index_of(const ListNode head, const char* str)
    {
        std::size_t counter{0};
        auto current{head};
        while(current != nullptr)
        {
            if(std::strcmp(current[0], str) == 0)
            {
                return counter;
            }
            counter += 1;
            current = reinterpret_cast<ListNode>(current[1]);
        }
        return counter;
    }

    void remove_duplicates(ListNode node)
    {
        auto current{node};
        while(current != nullptr)
        {
            auto next{reinterpret_cast<ListNode>(current[1])};
            while(remove(&next, current[0]))
            {
                current[1] = reinterpret_cast<char*>(next);
            }           
            current = next;
        }
    }

    void replace(ListNode head, const char* src, const char* dest)
    {
        auto current{head};
        while(current != nullptr)
        {
            if(std::strcmp(current[0], src) == 0)
            {
                std::free(current[0]);
                const auto size{std::strlen(dest) + 1};
                auto allocated{static_cast<char*>(std::malloc(sizeof(char) * size))};
                strcpy_s(allocated, size * sizeof(char), dest);
                current[0] = allocated;
            }
            current = reinterpret_cast<ListNode>(current[1]);
        }   
    }

    void sort(ListNode* node)
    {
    }
}

#endif