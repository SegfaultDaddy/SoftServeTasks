#ifndef STRING_List_H
#define STRING_List_H

#include <cstdlib>
#include <cstring>

namespace string_list
{
    using ListNode = char**;
    
    void init(ListNode* node)
    {
        *node = nullptr;
    }

    void destroy(ListNode* node)
    {
        *node = nullptr;
    }

    void add(ListNode* node, const char* str)
    {
        const auto size{strlen(str) + 1};
        char* data{static_cast<char*>(std::malloc(sizeof(char) * (size)))};
        strcpy_s(data, size, str);

        auto next{static_cast<ListNode>(std::malloc(sizeof(char*) + sizeof(ListNode)))};
        next[0] = data;
        next[1] = nullptr;

        if(*node == nullptr) [[unlikely]]
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

    void remove(ListNode node, const char* str)
    {
        if(node == nullptr)
        {
            return;
        }

        auto current{node};
        while(current[1] != nullptr &&
              std::strcmp(current[0], str) == 0)
        {
            current = reinterpret_cast<ListNode>(current[1]);
        }

        
    }

    std::size_t size(const ListNode node)
    {
        std::size_t counter{0};
        auto current{node};

        while(current != nullptr)
        {
            counter += 1;
            current = reinterpret_cast<string_list::ListNode>(current[1]);
        }

        return counter;
    }

    std::size_t index_of(ListNode node, const char* str)
    {
        return 0;
    }

    void remove_duplicates(ListNode node)
    {
    }

    void replace(ListNode node, const char* before, const char* after)
    {
    }

    void sort(ListNode node)
    {
    }
}

#endif