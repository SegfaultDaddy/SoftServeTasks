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

        ListNode create_node(const char* str)
        {
            const auto size{strlen(str) + 1};
            char* data{static_cast<char*>(std::malloc(sizeof(char) * (size)))};
            strcpy_s(data, size * sizeof(char), str);

            auto node{static_cast<ListNode>(std::malloc(sizeof(char*) + 2 * sizeof(ListNode)))};
            set_value(node, data);
            set_next(node, nullptr);
            set_prev(node, nullptr);

            return node;
        }

        void remove(List list, ListNode start, const char* str, bool firstOccurence = false)
        {
            auto current{start};
            while(current != nullptr)
            {
                auto next{implementation::next(current)};
                if(std::strcmp(value(current), str) == 0)
                {
                    auto prev{implementation::prev(current)};
                    auto conditionSatisfied{true};

                    if(next == nullptr) [[unlikely]]
                    {
                        set_tail(list, prev);
                        if(prev != nullptr)
                        {
                            set_next(prev, nullptr);
                        }
                        conditionSatisfied = false;
                    }
                    
                    if(prev == nullptr) [[unlikely]]
                    {
                        set_head(list, next);
                        if(next != nullptr)
                        {
                            set_prev(next, nullptr);
                        }
                        conditionSatisfied = false;
                    }

                    if(conditionSatisfied)
                    {
                        set_next(prev, next);
                        set_prev(next, prev);
                    }

                    std::free(value(current));
                    std::free(current);

                    if(firstOccurence)
                    {
                        return;
                    }
                }
                current = next; 
            }
        }

        ListNode split(ListNode head)
        {
            auto fast{head};
            auto slow{head};

            while(fast != nullptr && 
                  next(fast) != nullptr &&
                  next(next(fast)) != nullptr)
            {
                fast = next(next(fast));
                slow = next(slow);
            }

            auto remember{next(slow)};
            set_next(slow, nullptr); 

            if(remember != nullptr)
            {
                set_prev(remember, nullptr);
            }

            return remember; 
        }

        ListNode merge(List list, ListNode first, ListNode second)
        {
            auto compare{[](const char* first, const char* second){return std::strcmp(first, second) < 0;}}; 

            if(first == nullptr)
            { 
                return second;
            }

            if(second == nullptr)
            {
                return first;
            }

            if(compare(value(first), value(second)))
            {
                auto result{merge(list, next(first), second)};
                set_next(first, result);

                if(next(first) != nullptr)
                {
                    set_prev(next(first), first);
                } 

                set_prev(first, nullptr);
                return first;
            }
            else
            {
                auto result{merge(list, first, next(second))};
                set_next(second, result);
                
                if(next(second) != nullptr)
                {
                    set_prev(next(second), second);
                    if(next(next(second)) == nullptr)
                    {
                        set_tail(list, next(second));
                    }
                }  

                set_prev(second, nullptr);
                return second;
            }
        }

        ListNode merge_sort(List list, ListNode head)
        {
            if(head == nullptr ||
               next(head) == nullptr)
            {
                return head;
            }
            
            auto second{split(head)};
            head = merge_sort(list, head);
            second = merge_sort(list, second);

            return merge(list, head, second);
        }
    }

    bool empty(const List list)
    {
        using namespace implementation;

        return head(list) == nullptr && tail(list) == nullptr;
    }

    std::size_t size(const List list)
    {
        using namespace implementation;

        std::size_t counter{0};
        auto current{head(list)};
        while(current != nullptr)
        {
            counter += 1;
            current = next(current);
        }
        return counter;
    }

    std::size_t index_of(const List list, const char* str, const bool showError = false)
    {
        using namespace implementation;

        std::size_t counter{0};
        auto current{head(list)};
        while(current != nullptr)
        {
            if(std::strcmp(value(current), str) == 0)
            {
                return counter;
            }
            counter += 1;
            current = next(current);
        }

        assert(!showError && "Failed to find specified string in list");

        return counter;
    }

    void init(List* list)
    {
        using namespace implementation;

        *list = static_cast<List>(std::malloc(sizeof(ListNode) * 2));
        set_head(*list, nullptr);
        set_tail(*list, nullptr);
    }

    void destroy(List* list)
    {
        using namespace implementation;

        auto head{implementation::head(*list)};
        while(head != nullptr)
        {
            auto remember{next(head)};
            std::free(value(head));
            std::free(head);
            head = remember;
        }
        std::free(*list);
        *list = nullptr;
    } 

    void push_back(List list, const char* str)
    {
        using namespace implementation;

        auto next{create_node(str)};
        if(empty(list)) [[unlikely]]
        {
            set_head(list, next);
            set_tail(list, next);
        }
        else
        {
            auto tail{implementation::tail(list)};
            set_next(tail, next);
            set_prev(next, tail);
            set_tail(list, next);
        }
    }

    void push_front(List list, const char* str)
    {
        using namespace implementation;

        auto prev{create_node(str)};
        if(empty(list)) [[unlikely]]
        {
            set_head(list, prev);
            set_tail(list, prev);
        }
        else
        {
            auto head{implementation::head(list)};
            set_head(list, prev);
            set_next(prev, head);
            set_prev(head, prev);
        }
    }

    void pop_back(List list)
    {
        using namespace implementation;

        if(empty(list))
        {
            return;
        }

        auto tail{implementation::tail(list)};
        auto prev{implementation::prev(tail)};

        if(prev == nullptr) [[unlikely]]
        {
            set_head(list, nullptr);
            set_tail(list, nullptr);
        }
        else 
        {
            set_next(prev, nullptr);
            set_tail(list, prev);
        }

        std::free(value(tail));
        std::free(tail);
    }

    void pop_front(List list)
    {
        using namespace implementation;

        if(empty(list))
        {
            return;
        }

        auto head{implementation::head(list)};
        auto next{implementation::next(head)};

        if(next == nullptr) [[unlikely]]
        {
            set_head(list, nullptr);
            set_tail(list, nullptr);
        }
        else 
        {
            set_prev(next, nullptr);
            set_head(list, next);
        }

        std::free(value(head));
        std::free(head);
    }

    void remove(List list, const char* str, bool firstOccurence = false)
    {
        using namespace implementation;

        remove(list, head(list), str, false);
    }

    void unique(List list)
    {
        using namespace implementation;

        auto current{head(list)};
        while(current != nullptr)
        {
            auto next{implementation::next(current)};
            remove(list, next, value(current), false);
            current = implementation::next(current);
        }       
    }

    void replace(List list, const char* src, const char* dest)
    {
        using namespace implementation;

        auto current{head(list)};
        while(current != nullptr)
        {
            if(std::strcmp(value(current), src) == 0)
            {
                std::free(value(current));
                const auto size{std::strlen(dest) + 1};
                auto allocated{static_cast<char*>(std::malloc(sizeof(char) * size))};
                strcpy_s(allocated, size * sizeof(char), dest);
                set_value(current, allocated);
            }
            current = next(current);
        }   
    }

    void sort(List list)
    {
        using namespace implementation;

        set_head(list, merge_sort(list, head(list)));
    }
}

#endif