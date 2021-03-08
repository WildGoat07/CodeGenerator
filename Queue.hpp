#ifndef DEF_CMPT_INFO_QUEUE_HPP
#define DEF_CMPT_INFO_QUEUE_HPP
#include <cstdlib>
#include <ostream>
#include "List.hpp"

namespace cmpt_info
{
    /**
     * @brief  queue class.
     */
    template <typename ValueType>
    class Queue : protected List<ValueType>
    {
        template <typename T>
        friend std::ostream &operator<<(std::ostream &stream, Queue<T> const &queue);

    public:
        /**
         * @brief  Constructor
         */
        Queue();
        /**
         * @brief  Adds a new item to the back
         * @param  item: value to push
         */
        void push(ValueType const &item);
        /**
         * @brief  Returns the size of the queue
         * @return the size of the queue 
         */
        size_t size() const;
        /**
         * @brief  Returns true if the queue is empty
         * @return true if the queue is empty
         */
        bool empty() const;
        /**
         * @brief  Removes the first item of the queue, and returns it
         */
        ValueType pop();
        /**
         * @brief  Returns the first item of the queue, without removing it
         */
        ValueType peek() const;
    };
    template <typename ValueType>
    Queue<ValueType>::Queue() : Queue::List()
    {
        //constructor
    }
    template <typename ValueType>
    void Queue<ValueType>::push(ValueType const &item)
    {
        List<ValueType>::push_back(item);
    }
    template <typename ValueType>
    size_t Queue<ValueType>::size() const
    {
        return List<ValueType>::size();
    }
    template <typename ValueType>
    bool Queue<ValueType>::empty() const
    {
        return List<ValueType>::empty();
    }
    template <typename ValueType>
    ValueType Queue<ValueType>::pop()
    {
        return List<ValueType>::pop_front();
    }
    template <typename ValueType>
    ValueType Queue<ValueType>::peek() const
    {
        return *List<ValueType>::begin();
    }
    template <typename ValueType>
    std::ostream &operator<<(std::ostream &stream, Queue<ValueType> const &queue)
    {
        //here we print the list in a stream, std::cout for example
        stream << "[ ";
        bool first = true;
        for (auto it = queue.begin(); it != queue.end(); ++it)
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
