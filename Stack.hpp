#ifndef DEF_CMPT_INFO_STACK_HPP
#define DEF_CMPT_INFO_STACK_HPP
#include <cstdlib>
#include <ostream>
#include "List.hpp"

namespace cmpt_info
{
    /**
     * @brief  stack class.
     */
    template <typename ValueType>
    class Stack : protected List<ValueType>
    {
        template <typename T>
        friend std::ostream &operator<<(std::ostream &stream, Stack<T> const &queue);

    public:
        /**
         * @brief  Constructor
         */
        Stack();
        /**
         * @brief  Adds a new item to the back
         * @param  item: value to push
         */
        void push(ValueType const &item);
        /**
         * @brief  Returns the size of the stack
         * @return the size of the stack 
         */
        size_t size() const;
        /**
         * @brief  Returns true if the stack is empty
         * @return true if the stack is empty
         */
        bool empty() const;
        /**
         * @brief  Removes the first item of the stack, and returns it
         */
        ValueType pop();
        /**
         * @brief  Returns the first item of the stack, without removing it
         */
        ValueType peek() const;
    };
    template <typename ValueType>
    Stack<ValueType>::Stack() : Stack::List()
    {
        //constructor
    }
    template <typename ValueType>
    void Stack<ValueType>::push(ValueType const &item)
    {
        List<ValueType>::push_back(item);
    }
    template <typename ValueType>
    size_t Stack<ValueType>::size() const
    {
        return List<ValueType>::size();
    }
    template <typename ValueType>
    bool Stack<ValueType>::empty() const
    {
        return List<ValueType>::empty();
    }
    template <typename ValueType>
    ValueType Stack<ValueType>::pop()
    {
        return List<ValueType>::pop_back();
    }
    template <typename ValueType>
    ValueType Stack<ValueType>::peek() const
    {
        return *List<ValueType>::end();
    }
    template <typename ValueType>
    std::ostream &operator<<(std::ostream &stream, Stack<ValueType> const &stack)
    {
        //here we print the list in a stream, std::cout for example
        stream << "[ ";
        bool first = true;
        for (auto it = stack.begin(); it != stack.end(); ++it)
        {
            if (!first)
                stream << ", ";
            else
                first = false;
            stream << *it;
        }
        stream << " ]";
        return stream;
    }
} // namespace cmpt_info

#endif
