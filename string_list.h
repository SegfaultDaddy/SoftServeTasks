#ifndef STRING_LIST_H
#define STRING_LIST_H

#include <cstdlib>
#include <cstring>

namespace string_list
{
    using List = char**;
    using ListNode = char**;

    void init(List* list)
    {
        *list = static_cast<List>(std::malloc(sizeof(ListNode) * 2));
        (*list)[0] = nullptr;
        (*list)[1] = nullptr;
    }

    void destroy(List* list)
    {
    }

    ListNode create_node(const char* str)
    {
        const auto size{strlen(str) + 1};
        char* data{static_cast<char*>(std::malloc(sizeof(char) * (size)))};
        strcpy_s(data, size * sizeof(char), str);

        auto node{static_cast<ListNode>(std::malloc(sizeof(char*) + 2 * sizeof(ListNode)))};
        node[0] = data;
        node[1] = nullptr;
        node[2] = nullptr;

        return node;
    }

    void push_back(List list, const char* str)
    {
        auto next{create_node(str)};
        auto node{reinterpret_cast<char*>(next)};
        if(list[0] == nullptr && list[1] == nullptr) [[unlikely]]
        {
            list[0] = node;
            list[1] = node;
        }
        else
        {
            auto tail{reinterpret_cast<ListNode>(list[1])};
            tail[1] = node;
            next[2] = reinterpret_cast<char*>(tail);
            list[1] = node;
        }
    }

    void push_front(List list, const char* str)
    {
        auto next{create_node(str)};
        auto node{reinterpret_cast<char*>(next)};
        if(list[0] == nullptr && list[1] == nullptr) [[unlikely]]
        {
            list[0] = node;
            list[1] = node;
        }
        else
        {
            auto head{reinterpret_cast<ListNode>(list[0])};
            list[0] = node;
            next[1] = reinterpret_cast<char*>(head);
            head[2] = node;
        }
    }

    void pop_back(List list)
    {
        if(list[1] == nullptr)
        {
            return;
        }

        auto tail{reinterpret_cast<ListNode>(list[1])};
        auto prev{reinterpret_cast<ListNode>(tail[2])};

        if(prev == nullptr) [[unlikely]]
        {
            list[0] = nullptr;
            list[1] = nullptr;
        }
        else 
        {
            prev[1] = nullptr;
            list[1] = reinterpret_cast<char*>(prev);
        }

        free(tail[0]);
        free(tail);
    }

    void pop_front(List list)
    {
        if(list[0] == nullptr)
        {
            return;
        }

        auto head{reinterpret_cast<ListNode>(list[0])};
        auto next{reinterpret_cast<ListNode>(head[1])};

        if(next == nullptr) [[unlikely]]
        {
            list[0] = nullptr;
            list[1] = nullptr;
        }
        else 
        {
            next[1] = nullptr;
            list[0] = reinterpret_cast<char*>(next);
        }

        free(head[0]);
        free(head);
    }

    void remove(List list, const char* str)
    {

    }
}

#endif