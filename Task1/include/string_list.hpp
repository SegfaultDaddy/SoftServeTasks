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
         * @brief `List` getter for field `head`.
         * 
         * @param[in] list
         * @return `ListNode`
         */
        inline ListNode head(List list)
        {
            return reinterpret_cast<ListNode>(list[0]);
        }

        /**
         * @brief `List` getter for field `tail`.
         * 
         * @param[in] list 
         * @return `ListNode`
         */
        inline ListNode tail(List list)
        {
            return reinterpret_cast<ListNode>(list[1]);
        }

        /**
         * @brief `List` setter for field `head`.
         * 
         * @param list 
         * @param[in] head 
         */
        inline void set_head(List list, ListNode head)
        {
            list[0] = reinterpret_cast<char*>(head);
        }

        /**
         * @brief `List` setter for field `tail`.
         * 
         * @param list 
         * @param[in] tail 
         */
        inline void set_tail(List list, ListNode tail)
        {
            list[1] = reinterpret_cast<char*>(tail);
        }

        /**
         * @brief `ListNode` getter for field `value`.
         * 
         * @param[in] node 
         * @return `char*`
         */
        inline char* value(ListNode node)
        {
            return node[0];
        }

        /**
         * @brief `ListNode` getter for field `next`.
         * 
         * @param[in] node 
         * @return `ListNode`
         */
        inline ListNode next(ListNode node)
        {
            return reinterpret_cast<ListNode>(node[1]);
        }

        /**
         * @brief `ListNode` getter for field `prev`.
         * 
         * @param[in] node 
         * @return `ListNode`
         */
        inline ListNode prev(ListNode node)
        {
            return reinterpret_cast<ListNode>(node[2]);
        }

        /**
         * @brief `ListNode` setter for field `value`.
         * 
         * @param node 
         * @param[in] value
         */
        inline void set_value(ListNode node, char* value)
        {
            node[0] = value;
        }

        /**
         * @brief `ListNode` setter for field `next`.
         * 
         * @param node 
         * @param[in] next 
         */
        inline void set_next(ListNode node, ListNode next)
        {
            node[1] = reinterpret_cast<char*>(next);
        }

        /**
         * @brief `ListNode` setter for field `prev`.
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
         * @brief Allocates a `ListNode` object and default initializes its fields expect provided `str`
         * 
         * @param[in] str this data will be copied into created `ListNode` field `value`.
         * @return created on heap `ListNode`.
         */
        ListNode create_node(const char* str);

        /**
         * @brief 
         * 
         * @param list 
         * @param[in] start 
         * @param[in] str 
         * @param[in] removeAll 
         */
        void remove(List list, ListNode start, const char* str, const bool removeAll = true);

        /**
         * @brief 
         * 
         * @param head 
         * @param compare 
         */
        void selection_sort(ListNode head, bool (*compare)(const char*, const char*) = &compare_less_than);
    }

    /** 
    * @brief Allocates memory using std::malloc and default initializes `List`. 
    * 
    * @param[out] list is a pointer to an unitialized doubly-linked list. 
    * @warning if nullptr provided then assertion will fail.
    */
    void init(List* list);

    /**
     * @brief 
     * 
     * @param list 
     */
    void destroy(List* list); 

    /**
     * @brief 
     * 
     * @param list 
     * @return std::size_t 
     */
    std::size_t size(const List list);

    /**
     * @brief 
     * 
     * @param list 
     * @return true 
     * @return false 
     */
    bool empty(const List list);
    
    /**
     * @brief 
     * 
     * @param list 
     * @param str 
     * @return std::size_t 
     */
    std::size_t index_of(const List list, const char* str);

    /**
     * @brief 
     * 
     * @param list 
     * @param str 
     */
    void push_front(List list, const char* str);

    /**
     * @brief 
     * 
     * @param list 
     * @param str 
     */
    void push_back(List list, const char* str);

    /**
     * @brief 
     * 
     * @param list 
     */
    void pop_front(List list);

    /**
     * @brief 
     * 
     * @param list 
     */
    void pop_back(List list);

    /**
     * @brief 
     * 
     * @param list 
     * @param str 
     * @param removeAll 
     */
    void remove(List list, const char* str, const bool removeAll = true);

    /**
     * @brief 
     * 
     * @param list 
     */
    void unique(List list);

    /**
     * @brief 
     * 
     * @param list 
     * @param src 
     * @param dest 
     * @param replaceAll 
     */
    void replace(List list, const char* src, const char* dest, const bool replaceAll = true);

    /**
     * @brief 
     * 
     * @param list 
     */
    void sort(List list);
}

#endif