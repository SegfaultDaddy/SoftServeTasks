#ifndef STRING_LIST_H
#define STRING_LIST_H

#include <cstdlib>
#include <cstring>
#include <cassert>

namespace string_list
{
    using List = char**;
    
    namespace implementation
    {
        using ListNode = char**;

        inline ListNode head(List list)
        {
            return reinterpret_cast<ListNode>(list[0]);
        }

        inline ListNode tail(List list)
        {
            return reinterpret_cast<ListNode>(list[1]);
        }

        inline void set_head(List list, ListNode head)
        {
            list[0] = reinterpret_cast<char*>(head);
        }

        inline void set_tail(List list, ListNode tail)
        {
            list[1] = reinterpret_cast<char*>(tail);
        }

        inline char* value(ListNode node)
        {
            return node[0];
        }

        inline ListNode next(ListNode node)
        {
            return reinterpret_cast<ListNode>(node[1]);
        }

        inline ListNode prev(ListNode node)
        {
            return reinterpret_cast<ListNode>(node[2]);
        }

        inline void set_value(ListNode node, char* value)
        {
            node[0] = value;
        }

        inline void set_next(ListNode node, ListNode next)
        {
            node[1] = reinterpret_cast<char*>(next);
        }

        inline void set_prev(ListNode node, ListNode prev)
        {
            node[2] = reinterpret_cast<char*>(prev);
        }

        inline bool compare_less_than(const char* first, const char* second)
        {
            return std::strcmp(first, second) < 0;
        }

        ListNode create_node(const char* str);
        void remove(List list, ListNode start, const char* str, bool firstOccurence = false);
        ListNode split(ListNode head);
        ListNode merge(List list, ListNode first, ListNode second, bool (*compare)(const char*, const char*));
        ListNode merge_sort(List list, ListNode head, bool (*compare)(const char*, const char*) = &compare_less_than);
    }

    void init(List* list);
    void destroy(List* list); 
    bool empty(const List list);
    std::size_t size(const List list);
    std::size_t index_of(const List list, const char* str, const bool showError = false);
    void push_back(List list, const char* str);
    void push_front(List list, const char* str);
    void pop_back(List list);
    void pop_front(List list);
    void remove(List list, const char* str, bool firstOccurence = false);
    void unique(List list);
    void replace(List list, const char* src, const char* dest, const bool firstOccurence = false);
    void sort(List list);
}

#endif