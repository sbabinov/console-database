#ifndef LIST_HPP
#define LIST_HPP
#include <utility>
#include "list_iterator.hpp"
#include "const_list_iterator.hpp"
#include "nodes.hpp"

namespace babinov
{
  template< class T >
  class List
  {
  public:
    using BaseNode = detail::BaseListNode;
    using Node = detail::ListNode< T >;
    using Iterator = ListIterator< T >;
    using ConstIterator = ConstListIterator< T >;

    List();
    List(const List& other);
    List(List&& other) noexcept;
    ~List();

    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;

    bool isEmpty() const noexcept;
    size_t size() const noexcept;

    T& front();
    T& back();

    void pushFront(const T& value);
    void pushBack(const T& value);
    void popFront();
    void popBack();
    Iterator insert(Iterator pos, const T& value);
    Iterator erase(Iterator pos);
    void clear() noexcept;
    void swap(List& other) noexcept;

    Iterator begin() noexcept;
    Iterator begin() const noexcept;
    ConstIterator cbegin() const noexcept;
    Iterator end() noexcept;
    Iterator end() const noexcept;
    ConstIterator cend() const noexcept;

  private:
    BaseNode* fake_;
    size_t size_;
  };

  template< class T >
  List< T >::List():
    size_(0),
    fake_(new BaseNode())
  {}

  template< class T >
  List< T >::List(const List< T >& other):
    size_(0)
  {
    fake_ = new BaseNode();
    for (auto it = other.cbegin(); it != other.cend(); ++it)
    {
      pushBack(*it);
    }
  }

  template< class T >
  List< T >::List(List< T >&& other) noexcept:
    size_(other.size_),
    fake_(other.fake_)
  {
    other.size_ = 0;
    other.fake_ = nullptr;
  }

  template< class T >
  List< T >::~List()
  {
    clear();
    delete fake_;
  }

  template< class T >
  List< T >& List< T >::operator=(const List& other)
  {
    if (this != &other)
    {
      List< T > temp(other);
      swap(temp);
    }
    return *this;
  }

  template< class T >
  List< T >& List< T >::operator=(List&& other) noexcept
  {
    if (this != &other)
    {
      List< T > temp(std::move(other));
      swap(temp);
    }
    return *this;
  }

  template< class T >
  bool List< T >::isEmpty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  T& List< T >::front()
  {
    return *begin();
  }

  template< class T >
  T& List< T >::back()
  {
    return *std::prev(end());
  }

  template< class T >
  void List< T >::pushFront(const T& value)
  {
    insert(begin(), value);
  }

  template< class T >
  void List< T >::pushBack(const T& value)
  {
    insert(end(), value);
  }

  template< class T >
  void List< T >::popFront()
  {
    erase(begin());
  }

  template< class T >
  void List< T >::popBack()
  {
    erase(std::prev(end()));
  }

  template< class T >
  ListIterator< T > List< T >::insert(Iterator pos, const T& value)
  {
    Node* newNode = new Node(value);
    BaseNode* currentNode = pos.current_;
    newNode->next = currentNode;
    if (size_)
    {
      currentNode->prev->next = newNode;
      newNode->prev = currentNode->prev;
    }
    else
    {
      newNode->prev = currentNode;
      currentNode->next = newNode;
    }
    currentNode->prev = newNode;
    ++size_;
    return Iterator(newNode);
  }

  template< class T >
  ListIterator< T > List< T >::erase(Iterator pos)
  {
    assert(size_ != 0);
    BaseNode* node = pos.current_;
    node->prev->next = node->next;
    node->next->prev = node->prev;
    ++pos;
    delete node;
    --size_;
    return pos;
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    if (!size_)
    {
      return;
    }
    BaseNode* current = fake_->next;
    while (current != fake_)
    {
      current = current->next;
      delete current->prev;
    }
    size_ = 0;
  }

  template< class T >
  void List< T >::swap(List& other) noexcept
  {
    std::swap(size_, other.size_);
    std::swap(fake_, other.fake_);
  }

  template< class T >
  ListIterator< T > List< T >::begin() noexcept
  {
    if (!size_)
    {
      return Iterator(fake_);
    }
    return Iterator(fake_->next);
  }

  template< class T >
  ListIterator< T > List< T >::begin() const noexcept
  {
    if (!size_)
    {
      return Iterator(fake_);
    }
    return Iterator(fake_->next);
  }

  template< class T >
  ConstListIterator< T > List< T >::cbegin() const noexcept
  {
    if (!size_)
    {
      return ConstIterator(fake_);
    }
    return ConstIterator(fake_->next);
  }

  template< class T >
  ListIterator< T > List< T >::end() noexcept
  {
    return Iterator(fake_);
  }

  template< class T >
  ListIterator< T > List< T >::end() const noexcept
  {
    return Iterator(fake_);
  }

  template< class T >
  ConstListIterator< T > List< T >::cend() const noexcept
  {
    return ConstIterator(fake_);
  }
}

#endif
