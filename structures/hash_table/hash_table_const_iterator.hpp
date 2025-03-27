#ifndef CONST_HASH_TABLE_ITERATOR_HPP
#define CONST_HASH_TABLE_ITERATOR_HPP
#include <iterator>
#include <list>
#include <utility>
#include "list.hpp"
#include "nodes.hpp"

namespace babinov
{
  template< class TKey, class TValue >
  class HashTable;

  template< class TKey, class TValue >
  class ConstHashTableIterator : public std::iterator< std::forward_iterator_tag, std::pair< const TKey, TValue > >
  {
    friend class HashTable< TKey, TValue >;
  public:
    using ValueType = std::pair< const TKey, TValue >;
    using ListNode = detail::HashTableNode< ValueType >;
    using ListIterator = typename List< ListNode* >::Iterator;
    using ConstListIterator = typename List< ListNode* >::ConstIterator;
    using ThisType = ConstHashTableIterator< TKey, TValue >;

    explicit ConstHashTableIterator(ConstListIterator current):
      listIter_(current)
    {}

    explicit ConstHashTableIterator(ListIterator current):
      listIter_(ConstListIterator(current))
    {}

    ~ConstHashTableIterator() = default;
    ConstHashTableIterator(const ThisType&) = default;
    ThisType& operator=(const ThisType&) = default;

    bool operator==(const ThisType&) const;
    bool operator!=(const ThisType&) const;
    const ValueType& operator*() const;
    const ValueType* operator->() const;
    ThisType& operator++();
    ThisType operator++(int);

  private:
    ConstListIterator listIter_;
  };

  template< class TKey, class TValue >
  bool ConstHashTableIterator< TKey, TValue >::operator==(const ThisType& other) const
  {
    return listIter_ == other.listIter_;
  }

  template< class TKey, class TValue >
  bool ConstHashTableIterator< TKey, TValue >::operator!=(const ThisType& other) const
  {
    return !(*this == other);
  }

  template< class TKey, class TValue >
  const std::pair< const TKey, TValue >& ConstHashTableIterator< TKey, TValue >::operator*() const
  {
    return (**listIter_).data;
  }

  template< class TKey, class TValue >
  const std::pair< const TKey, TValue >* ConstHashTableIterator< TKey, TValue >::operator->() const
  {
    return &(**this);
  }

  template< class TKey, class TValue >
  ConstHashTableIterator< TKey, TValue >& ConstHashTableIterator< TKey, TValue >::operator++()
  {
    ++listIter_;
    return *this;
  }

  template< class TKey, class TValue >
  ConstHashTableIterator< TKey, TValue > ConstHashTableIterator< TKey, TValue >::operator++(int)
  {
    ThisType temp(*this);
    operator++();
    return temp;
  }
}

#endif
