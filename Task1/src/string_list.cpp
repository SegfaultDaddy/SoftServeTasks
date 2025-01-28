#include <cstring>
#include <cassert>

#include "string_list.hpp"

namespace string_list
{
    namespace implementation
    {
        bool compare_less_than(const char* first, const char* second)
        {
            return std::strcmp(first, second) < 0;
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

        void remove(List list, ListNode start, const char* str, const bool removeAll)
        {
            auto current{start};
            while(current != nullptr)
            {
                auto next{implementation::next(current)};
                if(std::strcmp(value(current), str) == 0)
                {
                    auto prev{implementation::prev(current)};
                    auto innerNode{true};

                    if(prev == nullptr)
                    {
                        set_head(list, next);
                        if(next != nullptr)
                        {
                            set_prev(next, nullptr);
                        }
                        innerNode = false;
                    }

                    if(next == nullptr)
                    {
                        set_tail(list, prev);
                        if(prev != nullptr)
                        {
                            set_next(prev, nullptr);
                        }
                        innerNode = false;
                    }

                    if(innerNode)
                    {
                        set_next(prev, next);
                        set_prev(next, prev);
                    }

                    std::free(value(current));
                    std::free(current);

                    if(!removeAll)
                    {
                        return;
                    }
                }
                current = next; 
            }
        }

        void selection_sort(ListNode head, bool (*compare)(const char*, const char*))
        {
            auto start{head};
            while(start != nullptr) 
            {
                auto min{start};

                auto current{next(start)};
                while(current != nullptr) 
                {
                    if(compare(value(current), value(min))) 
                    {
                        min = current;
                    }
                    current = next(current);
                }

                if(min != start) 
                {
                    auto value{implementation::value(start)};
                    set_value(start, implementation::value(min));
                    set_value(min, value);
                }

                start = next(start);
            }
        }
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

    bool empty(const List list) 
    {
        using namespace implementation;

        return head(list) == nullptr && tail(list) == nullptr;
    }

    std::size_t index_of(const List list, const char* str, const bool showError)
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

        assert((list != nullptr) && "Provide viable address not nullptr");
        *list = static_cast<List>(std::malloc(sizeof(ListNode) * 2));
        set_head(*list, nullptr);
        set_tail(*list, nullptr);
    }

    void destroy(List* list)
    {
        using namespace implementation;

        assert((list != nullptr) && "Provide viable address not nullptr");
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

        auto node{create_node(str)};
        if(empty(list)) [[unlikely]]
        {
            set_head(list, node);
            set_tail(list, node);
        }
        else
        {
            auto tail{implementation::tail(list)};
            set_next(tail, node);
            set_prev(node, tail);
            set_tail(list, node);
        }
    }

    void push_front(List list, const char* str)
    {
        using namespace implementation;

        auto node{create_node(str)};
        if(empty(list)) [[unlikely]]
        {
            set_head(list, node);
            set_tail(list, node);
        }
        else
        {
            auto head{implementation::head(list)};
            set_head(list, node);
            set_next(node, head);
            set_prev(head, node);
        }
    }

    void pop_back(List list)
    {
        using namespace implementation;

        assert(!empty(list) && "Using pop_back on empty list");

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

        assert(!empty(list) && "Using pop_front on empty list");

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

    void remove(List list, const char* str, const bool removeAll)
    {
        using namespace implementation;

        remove(list, head(list), str, removeAll);
    }

    void unique(List list)
    {
        using namespace implementation;

        auto current{head(list)};
        while(current != nullptr)
        {
            auto next{implementation::next(current)};
            remove(list, next, value(current), true);
            current = implementation::next(current);
        }       
    }

    void replace(List list, const char* src, const char* dest, const bool removeAll)
    {
        using namespace implementation;

        auto current{head(list)};
        while(current != nullptr)
        {
            if(std::strcmp(value(current), src) == 0)
            {
                const auto size{std::strlen(dest) + 1};
                auto allocated{static_cast<char*>(std::realloc(value(current), sizeof(char) * size))};
                strcpy_s(allocated, size * sizeof(char), dest);
                set_value(current, allocated);

                if(!removeAll)
                {
                    return;
                }
            }
            current = next(current);
        }   
    }

    void sort(List list)
    {
        using namespace implementation;

        auto head{implementation::head(list)};
        selection_sort(head);
    }
}