#ifndef STRING_LIST_H
#define STRING_LIST_H

#include <cstdlib>
#include <cstring>

namespace string_list
{
    using List = char**;
    
    namespace implementation
    {
        using ListNode = char**;
    }

    std::size_t size(const List list)
    {
        using namespace implementation;

        std::size_t counter{0};
        auto current{reinterpret_cast<ListNode>(list[0])};
        while(current != nullptr)
        {
            counter += 1;
            current = reinterpret_cast<ListNode>(current[1]);
        }
        return counter;
    }

    std::size_t index_of(const List list, const char* str)
    {
        using namespace implementation;

        return 0;
    }

    namespace implementation
    {
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

        void remove(ListNode* head, const char* str, bool firstOccurence)
        {
            auto current{reinterpret_cast<ListNode>(*head)};
            while(current != nullptr)
            {
                auto next{reinterpret_cast<ListNode>(current[1])};
                if(std::strcmp(current[0], str) == 0)
                {
                    auto prev{reinterpret_cast<ListNode>(current[2])};

                    if(prev == nullptr) [[unlikely]]
                    {
                        *head = next;
                        if(next != nullptr)
                        {
                            next[2] = nullptr;
                        }
                    }
                    else 
                    {
                        prev[1] = reinterpret_cast<char*>(next);
                        next[2] = reinterpret_cast<char*>(prev);
                    }

                    std::free(current[0]);
                    std::free(current);

                    if(firstOccurence)
                    {
                        return;
                    }
                }
                current = next; 
            }
        }
    }

    void init(List* list)
    {
        using namespace implementation;

        *list = static_cast<List>(std::malloc(sizeof(ListNode) * 2));
        (*list)[0] = nullptr;
        (*list)[1] = nullptr;
    }

    void destroy(List* list)
    {
        using namespace implementation;

        auto head{reinterpret_cast<ListNode>((*list)[0])};
        while(head != nullptr)
        {
            auto remember{reinterpret_cast<ListNode>(head[1])};
            std::free(head[0]);
            std::free(head);
            head = remember;
        }
        std::free(*list);
        *list = nullptr;
    } 

    void push_back(List list, const char* str)
    {
        using namespace implementation;

        auto next{implementation::create_node(str)};
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
        using namespace implementation;

        auto next{implementation::create_node(str)};
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
        using namespace implementation;

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
        using namespace implementation;

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

    void remove(List list, const char* str, bool firstOccurence = false)
    {
        using namespace implementation;

        auto head{reinterpret_cast<ListNode>(list[0])};
        remove(&head, str, firstOccurence);
        list[0] = reinterpret_cast<char*>(head);
        if(list[0] == nullptr)
        {
            list[1] = nullptr;
        }
    }

    void unique(List list)
    {
        using namespace implementation;

        auto current{reinterpret_cast<ListNode>(list[0])};
        auto tail{reinterpret_cast<ListNode>(list[1])};
        while(current != tail)
        {
            const auto str{current[0]};
            const auto prev{current};
            current = reinterpret_cast<ListNode>(current[1]);
            remove(&current, str, false);
            prev[1] = reinterpret_cast<char*>(current);
        }
    }

    void replace(List list, const char* src, const char* dest)
    {
        using namespace implementation;

        auto current{reinterpret_cast<ListNode>(list[0])};
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

}

#endif