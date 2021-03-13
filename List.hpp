#ifndef DEF_CMPT_INFO_LIST_HPP
#define DEF_CMPT_INFO_LIST_HPP
#include <cstdlib>
#include <stdexcept>
#include <ostream>

namespace cmpt_info
{
    template <typename ValueType>
    class Iterator;
    /**
     * @brief  Base linked list class. Compatible with <algorithm>
     */
    template <typename ValueType>
    class List
    {
        friend Iterator<ValueType>;

    protected:
        /**
         * @brief  A single node of the linked list
         */
        template <typename NodeValueType>
        struct ListNode
        {
            ListNode() {}
            NodeValueType _item;
            ListNode<NodeValueType> *_previous;
            ListNode<NodeValueType> *_next;
        };
        typedef ListNode<ValueType> node_t;

        ListNode<ValueType> *_first;
        ListNode<ValueType> *_last;
        size_t _size;

    public:
        /**
         * @brief  The iterator of this list
         */
        typedef Iterator<ValueType> iterator;
        /**
         * @brief  Constructor
         */
        List();
        /**
         * @brief  Constructor
         */
        List(const List<ValueType> &copy);
        /**
         * @brief  Desctructor
         */
        ~List();
        /**
         * @brief  Adds a new item to the front
         * @param  item: value to push 
         */
        ValueType& push_front(ValueType const &item);
        /**
         * @brief  Adds a new item to the back
         * @param  item: value to push
         */
        ValueType& push_back(ValueType const &item);
        /**
         * @brief  Returns the size of the list
         * @return the size of the list 
         */
        size_t size() const;
        /**
         * @brief  Returns true if the list is empty
         * @return true if the list is empty
         */
        bool empty() const;
        /**
         * @brief  Inserts an item inside the list
         * @param  it: iterator to where to insert the item
         * @param  item: value to insert
         */
        ValueType& insert(iterator const &it, ValueType const &item);
        /**
         * @brief  Removes an item from the list
         * @param  it: iterator of the item to remove
         */
        ValueType remove(iterator const &it);
        /**
         * @brief  Removes the first item of the list
         */
        ValueType pop_front();
        /**
         * @brief  Removes the last item of the list
         */
        ValueType pop_back();
        /**
         * @brief  Removes every item in the list
         */
        void clear();
        /**
         * @brief  Gets an iterator on the list
         * @return an iterator that points to the first item
         */
        iterator begin() const;
        /**
         * @brief  Gets a reverse iterator on the list
         * @return a reverse iterator that points to the last item
         */
        iterator rbegin() const;
        /**
         * @brief  Gets an iterator on the list pointing to the end
         * @return an iterator that points to the end of the list
         */
        iterator end() const;
        /**
         * @brief  Gets a reverse iterator on the list pointing to the end
         * @return a reverse iterator that points to the end of the list
         */
        iterator rend() const;
        /**
         * @brief  list = list
         */
        List<ValueType> &operator=(const List<ValueType> &right);
    };
    /**
     * @brief  Iterator class of the list
     */
    template <typename ValueType>
    class Iterator
    {
        friend List<ValueType>;

    protected:
        typename List<ValueType>::node_t *_ptr;
        typename List<ValueType>::node_t *_next;
        typename List<ValueType>::node_t *_prev;
        const List<ValueType> *_list;
        bool _reverse;

        Iterator(const List<ValueType> *list, typename List<ValueType>::node_t *ptr, bool reverse);

        void refresh();

    public:
        /**
         * @brief  Dereferencement
         */
        ValueType &value() const;
        /**
         * @brief  Dereferencement
         */
        ValueType *operator->() const;
        /**
         * @brief  Dereferencement
         */
        ValueType &operator*() const;
        /**
         * @brief  ++iterator
         */
        Iterator<ValueType> &operator++();
        /**
         * @brief  iterator++
         */
        Iterator<ValueType> operator++(int);
        /**
         * @brief  --iterator
         */
        Iterator<ValueType> &operator--();
        /**
         * @brief  iterator--
         */
        Iterator<ValueType> operator--(int);
        /**
         * @brief  it1 == it 2
         */
        bool operator==(const Iterator<ValueType> &right) const;
        /**
         * @brief  it1 != it2
         */
        bool operator!=(const Iterator<ValueType> &right) const;
    };
    template <typename ValueType>
    List<ValueType>::List() : _first(nullptr),
                              _last(nullptr),
                              _size(0)
    {
        //constructor
    }
    template <typename ValueType>
    List<ValueType>::List(const List<ValueType> &copy) : List()
    {
        for (auto it = copy.begin(); it != copy.end(); ++it)
            push_back(*it);
    }
    template <typename ValueType>
    List<ValueType>::~List()
    {
        //destructor
        node_t *it;
        node_t *next = _first;
        //we delete every dynamic allocation
        while ((it = next) != nullptr)
        {
            next = it->_next;
            delete it;
        }
    }
    template <typename ValueType>
    ValueType &List<ValueType>::push_front(ValueType const &item)
    {
        //we crate a new node dynamically and add it to the list
        node_t *node = new node_t();
        node->_item = item;
        node->_next = _first;
        node->_previous = nullptr;
        if (_first != nullptr)
            _first->_previous = node;
        if (_last == nullptr)
            _last = node;
        _first = node;
        ++_size;
        return node->_item;
    }
    template <typename ValueType>
    ValueType &List<ValueType>::push_back(ValueType const &item)
    {
        //we crate a new node dynamically and add it to the list
        node_t *node = new node_t();
        node->_item = item;
        node->_previous = _last;
        node->_next = nullptr;
        if (_last != nullptr)
            _last->_next = node;
        if (_first == nullptr)
            _first = node;
        _last = node;
        ++_size;
        return node->_item;
    }
    template <typename ValueType>
    ValueType &List<ValueType>::insert(iterator const &it, ValueType const &item)
    {
        if (it._list != this)
            throw std::invalid_argument("Iterator not pointing this list.");
        else if (it == end())
            return push_back(item);
        else if (it == begin())
            return push_front(item);
        else if (it == rend())
            return push_front(item);
        else if (it == rbegin())
            return push_back(item);
        else
        {
            if (it._reverse)
            {
                //we insert the value to where the iterator is, if reverse, we have to put it after the iterator instead of before
                node_t *node = new node_t();
                node->_item = item;
                if (it._ptr->_next != nullptr)
                    it._ptr->_next->_previous = node;
                node->_next = it.ptr->_next;
                it._ptr->_next = node;
                node->_previous = it._ptr;
                ++_size;
                return node->_item;
            }
            else
            {
                node_t *node = new node_t();
                node->_item = item;
                if (it._ptr->_previous != nullptr)
                    it._ptr->_previous->_next = node;
                node->_previous = it.ptr->_previous;
                it._ptr->_previous = node;
                node->_next = it._ptr;
                ++_size;
                return node->_item;
            }
        }
    }
    template <typename ValueType>
    ValueType List<ValueType>::remove(iterator const &it)
    {
        //we remove a node
        if (it._list != this)
            throw std::invalid_argument("Iterator not pointing this list.");
        if (_first == it._ptr)
            _first = it._ptr->_next;
        if (_last == it._ptr)
            _last = it._ptr->_previous;
        ValueType ret = it._ptr->_item;
        if (it._ptr->_previous != nullptr)
            it._ptr->_previous->_next = it._ptr->_next;
        if (it._ptr->_next != nullptr)
            it._ptr->_next->_previous = it._ptr->_previous;
        delete it._ptr; //don't forget
        --_size;
        return ret;
    }
    template <typename ValueType>
    size_t List<ValueType>::size() const
    {
        //foo
        return _size;
    }
    template <typename ValueType>
    bool List<ValueType>::empty() const
    {
        //bar
        return size() == 0;
    }
    template <typename ValueType>
    ValueType List<ValueType>::pop_front()
    {
        //wow
        auto it = begin();
        ValueType result = *it;
        remove(it);
        return result;
    }
    template <typename ValueType>
    ValueType List<ValueType>::pop_back()
    {
        //so sexy
        auto it = rbegin();
        ValueType result = *it;
        remove(it);
        return result;
    }
    template <typename ValueType>
    void List<ValueType>::clear()
    {
        //mmmmmh
        node_t* it = _first;
        while (it != nullptr)
        {
            node_t* next = it->_next;
            free(it);
            it = next;
        }
    }
    template <typename ValueType>
    typename List<ValueType>::iterator List<ValueType>::begin() const
    {
        //iterator pointing to the first item
        return iterator(this, _first, false);
    }
    template <typename ValueType>
    typename List<ValueType>::iterator List<ValueType>::rbegin() const
    {
        //reverse iterator pointing to the last item
        return iterator(this, _last, true);
    }
    template <typename ValueType>
    typename List<ValueType>::iterator List<ValueType>::end() const
    {
        //iterator pointing to nothing
        return iterator(this, nullptr, false);
    }
    template <typename ValueType>
    typename List<ValueType>::iterator List<ValueType>::rend() const
    {
        //reverse iterator pointing to nothing
        return iterator(this, nullptr, true);
    }
    template <typename ValueType>
    List<ValueType> &List<ValueType>::operator=(const List<ValueType> &right)
    {
        clear();
        for (auto it = right.begin(); it != right.end(); ++it)
            push_back(*it);
        return *this;
    }
    template <typename ValueType>
    std::ostream &operator<<(std::ostream &stream, List<ValueType> const &list)
    {
        //here we print the list in a stream, std::cout for example
        stream << "[ ";
        bool first = true;
        for (auto it = list.begin(); it != list.end(); ++it)
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
    template <typename ValueType>
    Iterator<ValueType>::Iterator(const List<ValueType> *list, typename List<ValueType>::node_t *ptr, bool reverse) : _ptr(ptr),
                                                                                                                      _next(nullptr),
                                                                                                                      _prev(nullptr),
                                                                                                                      _list(list),
                                                                                                                      _reverse(reverse)
    {
        //iterator constructor
        refresh();
    }
    template <typename ValueType>
    void Iterator<ValueType>::refresh()
    {
        //we refresh the values of the next and previous values of the iterator
        if (_ptr != nullptr)
        {
            if (_reverse)
            {
                _next = _ptr->_previous;
                _prev = _ptr->_next;
            }
            else
            {
                _next = _ptr->_next;
                _prev = _ptr->_previous;
            }
        }
        else
        {
            _next = nullptr;
            _prev = _reverse ? _list->_first : _list->_last;
        }
    }
    template <typename ValueType>
    ValueType &Iterator<ValueType>::value() const
    {
        //dereferencement
        return _ptr->_item;
    }
    template <typename ValueType>
    ValueType *Iterator<ValueType>::operator->() const
    {
        //dereferencement
        return &_ptr->_item;
    }
    template <typename ValueType>
    ValueType &Iterator<ValueType>::operator*() const
    {
        //dereferencement
        return _ptr->_item;
    }
    template <typename ValueType>
    Iterator<ValueType> &Iterator<ValueType>::operator++()
    {
        //++it
        _ptr = _next;
        refresh();
        return *this;
    }
    template <typename ValueType>
    Iterator<ValueType> Iterator<ValueType>::operator++(int)
    {
        //it++
        Iterator<ValueType> tmp(*this);
        operator++();
        return tmp;
    }
    template <typename ValueType>
    Iterator<ValueType> &Iterator<ValueType>::operator--()
    {
        //--it
        _ptr = _prev;
        refresh();
        return *this;
    }
    template <typename ValueType>
    Iterator<ValueType> Iterator<ValueType>::operator--(int)
    {
        //it--
        Iterator<ValueType> tmp(*this);
        operator--();
        return tmp;
    }
    template <typename ValueType>
    bool Iterator<ValueType>::operator==(const Iterator<ValueType> &right) const
    {
        //it1 == it2
        return _ptr == right._ptr && _reverse == right._reverse;
    }
    template <typename ValueType>
    bool Iterator<ValueType>::operator!=(const Iterator<ValueType> &right) const
    {
        //it1 != it2
        return !operator==(right);
    }
} // namespace cmpt_info

#endif
