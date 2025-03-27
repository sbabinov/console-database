#ifndef LIST_ITERATOR_HPP
#define LIST_ITERATOR_HPP
#include <assert.h>
#include <iterator>
#include "nodes.hpp"

namespace babinov
{
  template< class T >
  class List;

  template< class T >
  class ConstListIterator;

  template< class T >
  class ListIterator: public std::iterator< std::bidirectional_iterator_tag, T >
  {
    friend class List< T >;
    friend class ConstListIterator< T >;
  public:
    using BaseNode = detail::BaseListNode;
    using ThisType = ListIterator< T >;

    ListIterator():
      current_(nullptr)
    {}

    explicit ListIterator(BaseNode* ptr):
      current_(ptr)
    {}

    ListIterator(const ThisType&) = default;
    ThisType& operator=(const ThisType&) = default;

    bool operator==(const ThisType&) const;
    bool operator!=(const ThisType&) const;
    T& operator*();
    const T& operator*() const;
    T* operator->();
    const T* operator->() const;
    ThisType& operator++();
    ThisType operator++(int);
    ThisType& operator--();
    ThisType operator--(int);

  private:
    BaseNode* current_;
  };

  template< class T >
  bool ListIterator< T >::operator==(const ThisType& other) const {
    return current_ == other.current_;
  }

  template< class T >
  bool ListIterator< T >::operator!=(const ThisType& other) const {
    return !(*this == other);
  }

  template< class T >
  T& ListIterator< T >::operator*()
  {
    assert(current_ != nullptr);
    return (*dynamic_cast< detail::ListNode< T >* >(current_)).data;
  }

  template< class T >
  const T& ListIterator< T >::operator*() const
  {
    assert(current_ != nullptr);
    return (*dynamic_cast< detail::ListNode< T >* >(current_)).data;
  }

  template< class T >
  T* ListIterator< T >::operator->()
  {
    assert(current_ != nullptr);
    return &(**this);
  }

  template< class T >
  const T* ListIterator< T >::operator->() const
  {
    assert(current_ != nullptr);
    return &(**this);
  }

  template< class T >
  ListIterator< T >& ListIterator< T >::operator++()
  {
    assert(current_ != nullptr);
    current_ = current_->next;
    return *this;
  }

  template< class T >
  ListIterator< T > ListIterator< T >::operator++(int)
  {
    ListIterator<T> temp(*this);
    operator++();
    return temp;
  }

  template< class T >
  ListIterator< T >& ListIterator< T >::operator--()
  {
    assert(current_ != nullptr);
    current_ = current_->prev;
    return *this;
  }

  template< class T >
  ListIterator< T > ListIterator< T >::operator--(int)
  {
    ListIterator< T > temp(*this);
    operator--();
    return temp;
  }
}

#endif
