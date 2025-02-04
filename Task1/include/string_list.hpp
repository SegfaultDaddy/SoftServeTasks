#ifndef STRING_LIST_HPP
#define STRING_LIST_HPP

/**
 * @file string_list.hpp
 * @author Oleksandr(olexandrfedorych@gmail.com)
 * @brief Implementation of a doubly-linked list of C-strings.
 * @version 0.1
 * @date 2025-02-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <cstdlib>

namespace string_list
{
    /**
     * @brief Doubly-linked list of C-strings type. 
     * 
     * @note `List` consists of 2 fields:
     * @note 1. `head` of type `ListNode`. It points to the first node in the list.
     * @note 2. `tail` of type `ListNode`. It points to the last node in the list.
     */
    using List = char**;
    
    namespace implementation
    {
        /**
         * @brief Doubly-linked list of C-strings node type. Building block of `List`.
         * 
         * @note This type consist of 3 fields:
         * @note 1. `value` of type `char*`.
         * @note 2. `next` of type `ListNode`. Points to the next node in the list.
         * @note 3. `prev` of type `ListNode`. Points to the previous node in the list.
         */
        using ListNode = char**;

        /**
         * @brief Function that gets `head` field in `List`.
         * 
         * @param[in] list
         * @return `ListNode`
         */
        inline ListNode head(List list)
        {
            return reinterpret_cast<ListNode>(list[0]);
        }

        /**
         * @brief Function that gets `tail` field in `List`.
         * 
         * @param[in] list 
         * @return `ListNode`
         */
        inline ListNode tail(List list)
        {
            return reinterpret_cast<ListNode>(list[1]);
        }

        /**
         * @brief Function that sets `head` field in `List`.
         * 
         * @param list 
         * @param[in] head 
         */
        inline void set_head(List list, ListNode head)
        {
            list[0] = reinterpret_cast<char*>(head);
        }

        /**
         * @brief Function that sets `tail` field in `List`.
         * 
         * @param list 
         * @param[in] tail 
         */
        inline void set_tail(List list, ListNode tail)
        {
            list[1] = reinterpret_cast<char*>(tail);
        }

        /**
         * @brief Function that gets `value` field in `ListNode`.
         * 
         * @param[in] node 
         * @return `char*`.
         */
        inline char* value(ListNode node)
        {
            return node[0];
        }

        /**
         * @brief Function that gets `next` field in `ListNode`.
         * 
         * @param[in] node 
         * @return `ListNode`.
         */
        inline ListNode next(ListNode node)
        {
            return reinterpret_cast<ListNode>(node[1]);
        }

        /**
         * @brief Function that gets `prev` field in `ListNode`.
         * 
         * @param[in] node 
         * @return `ListNode`
         */
        inline ListNode prev(ListNode node)
        {
            return reinterpret_cast<ListNode>(node[2]);
        }

        /**
         * @brief Function that sets `value` field in `ListNode`.
         * 
         * @param node 
         * @param[in] value
         */
        inline void set_value(ListNode node, char* value)
        {
            node[0] = value;
        }

        /**
         * @brief Function that sets `next` field in `ListNode`.
         * 
         * @param node 
         * @param[in] next 
         */
        inline void set_next(ListNode node, ListNode next)
        {
            node[1] = reinterpret_cast<char*>(next);
        }

        /**
         * @brief Function that sets `prev` field in `ListNode`.
         * 
         * @param node 
         * @param[in] prev 
         */
        inline void set_prev(ListNode node, ListNode prev)
        {
            node[2] = reinterpret_cast<char*>(prev);
        }

        /**
         * @brief Function that compares two C-strings using `operator<`.
         * 
         * @param[in] first 
         * @param[in] second 
         * @return `true` if `first` is less than `second`.
         * @return `false` in every other case.
         */
        bool compare_less_than(const char* first, const char* second);

        /**
         * @brief Function that allocates a `ListNode` object and default initializes its fields expect provided `str`
         * 
         * @param[in] str this data will be copied into created `ListNode` field `value`.
         * @return created on heap and initialized with provided `str `ListNode`.
         */
        ListNode create_node(const char* str);

        /**
         * @brief Function that removes element(s) in `list` from position `start` if they is equal to `str`.
         * 
         * @param list initialized list
         * @param[in] start position from which remove will start.
         * @param[in] str nodes that contains value equal to this parameter will be removed from list.
         * @param[in] removeAll optional parameter is true removes all elements that match `str`, if false removes only first element.
         */
        void remove(List list, ListNode start, const char* str, const bool removeAll = true);

        /**
         * @brief Function that sorts `List` starting from `head`. Implemented using selection sort algorithm.
         * 
         * @param head position of a first node.
         * @param[in] compare predicate which compares `value` field in neighbour nodes.
         */
        void selection_sort(ListNode head, bool (*compare)(const char*, const char*) = &compare_less_than);
    }

    /** 
    * @brief Function that allocates memory using std::malloc and default initializes `List`. 
    * 
    * @param[out] list is a pointer to unitialized `List`. 
    * @warning If nullptr `list` provided then assertion will fail.
    */
    void init(List* list);

    /**
     * @brief Function that destroys and deallocates initialized `list`.
     * 
     * @param list is a pointer to initialized `List`.
     * @warning If nullptr `list` provided then assertion will fail.
     * @warning If `*list` is nullptr then assertion will fail.
     */
    void destroy(List* list); 

    /**
     * @brief Function that return size of `list`.
     * 
     * @param list 
     * @return `std::size_t` is a number of elements in `list`.
     * @warning If nullptr `list` provided then assertion will fail.
     */
    std::size_t size(const List list);

    /**
     * @brief Function predicate that checks if list contains elements.
     * 
     * @param[in] list 
     * @return true if `list` is empty.
     * @return false if `list` contains elements.
     * @warning If nullptr `list` provided then assertion will fail.
     */
    bool empty(const List list);
    
    /**
     * @brief Function that finds first element in the `list` and returns its index.
     * 
     * @param list 
     * @param str criterium using which we will find needed element in the list
     * @return if returns `std::size_t` index is less than size of `list` it means that element exists in list.
     * @return if returns `std::size_t` index is greater equal than size of `list` it means that element exists in list.
     * @warning If nullptr `list` provided then assertion will fail.
     */
    std::size_t index_of(const List list, const char* str);

    /**
     * @brief 
     * 
     * @param list 
     * @param str 
     * @warning If nullptr `list` provided then assertion will fail.
     */
    void push_front(List list, const char* str);

    /**
     * @brief 
     * 
     * @param list 
     * @param str 
     * @warning If nullptr `list` provided then assertion will fail.
     */
    void push_back(List list, const char* str);

    /**
     * @brief 
     * 
     * @param list 
     * @warning If nullptr `list` provided then assertion will fail.
     */
    void pop_front(List list);

    /**
     * @brief 
     * 
     * @param list 
     * @warning If nullptr `list` provided then assertion will fail.
     */
    void pop_back(List list);

    /**
     * @brief 
     * 
     * @param list 
     * @param str 
     * @param removeAll 
     * @warning If nullptr `list` provided then assertion will fail.
     */
    void remove(List list, const char* str, const bool removeAll = true);

    /**
     * @brief 
     * 
     * @param list 
     * @warning If nullptr `list` provided then assertion will fail.
     */
    void unique(List list);

    /**
     * @brief 
     * 
     * @param list 
     * @param src 
     * @param dest 
     * @param replaceAll 
     * @warning If nullptr `list` provided then assertion will fail.
     */
    void replace(List list, const char* src, const char* dest, const bool replaceAll = true);

    /**
     * @brief 
     * 
     * @param list 
     * @warning If nullptr `list` provided then assertion will fail.
     */
    void sort(List list);
}

#endif