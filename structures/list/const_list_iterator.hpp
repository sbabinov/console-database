#ifndef CONST_LIST_ITERATOR_HPP
#define CONST_LIST_ITERATOR_HPP
#include <assert.h>
#include <iterator>
#include "nodes.hpp"

namespace babinov
{
  template< class T >
  class List;

  template< class T >
  class ConstListIterator: public std::iterator< std::bidirectional_iterator_tag, T >
  {
    friend class List< T >;
  public:
    using BaseNode = detail::BaseListNode;
    using ThisType = ConstListIterator< T >;

    explicit ConstListIterator(const BaseNode* ptr) :
      current_(ptr)
    {}

    explicit ConstListIterator(ListIterator< T > iter):
      current_(iter.current_)
    {}

    ConstListIterator(const ThisType&) = default;
    ThisType& operator=(const ThisType&) = default;

    bool operator==(const ThisType&) const;
    bool operator!=(const ThisType&) const;
    const T& operator*() const;
    const T* operator->() const;
    ThisType& operator++();
    ThisType operator++(int);
    ThisType& operator--();
    ThisType operator--(int);

  private:
    const BaseNode* current_;
  };

  template< class T >
  bool ConstListIterator< T >::operator==(const ThisType& other) const {
    return current_ == other.current_;
  }

  template< class T >
  bool ConstListIterator< T >::operator!=(const ThisType& other) const {
    return !(*this == other);
  }

  template< class T >
  const T& ConstListIterator< T >::operator*() const
  {
    assert(current_ != nullptr);
    return (*dynamic_cast< const detail::ListNode< T >* >(current_)).data;
  }

  template< class T >
  const T* ConstListIterator< T >::operator->() const
  {
    assert(current_ != nullptr);
    return &(**this);
  }

  template< class T >
  ConstListIterator< T >& ConstListIterator< T >::operator++()
  {
    assert(current_ != nullptr);
    current_ = current_->next;
    return *this;
  }

  template< class T >
  ConstListIterator< T > ConstListIterator< T >::operator++(int)
  {
    ListIterator<T> temp(*this);
    operator++();
    return temp;
  }

  template< class T >
  ConstListIterator< T >& ConstListIterator< T >::operator--()
  {
    assert(current_ != nullptr);
    current_ = current_->prev;
    return *this;
  }

  template< class T >
  ConstListIterator< T > ConstListIterator< T >::operator--(int)
  {
    ListIterator< T > temp(*this);
    operator--();
    return temp;
  }
}

#endif
