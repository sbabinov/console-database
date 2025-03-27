#ifndef HASH_TABLE_ITERATOR_HPP
#define HASH_TABLE_ITERATOR_HPP
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
  class HashTableIterator: public std::iterator< std::forward_iterator_tag, std::pair< const TKey, TValue > >
  {
    friend class HashTable< TKey, TValue >;
  public:
    using ValueType = std::pair< const TKey, TValue >;
    using ListNode = detail::HashTableNode< ValueType >;
    using ListIterator = typename List< ListNode* >::Iterator;
    using ThisType = HashTableIterator< TKey, TValue >;

    explicit HashTableIterator(ListIterator current):
      listIter_(current)
    {}

    ~HashTableIterator() = default;
    HashTableIterator(const ThisType&) = default;
    ThisType& operator=(const ThisType&) = default;

    bool operator==(const ThisType&) const;
    bool operator!=(const ThisType&) const;
    ValueType& operator*() const;
    ValueType* operator->() const;
    ThisType& operator++();
    ThisType operator++(int);

  private:
    ListIterator listIter_;
  };

  template< class TKey, class TValue >
  bool HashTableIterator< TKey, TValue >::operator==(const ThisType& other) const
  {
    return listIter_ == other.listIter_;
  }

  template< class TKey, class TValue >
  bool HashTableIterator< TKey, TValue >::operator!=(const ThisType& other) const
  {
    return !(*this == other);
  }

  template< class TKey, class TValue >
  std::pair< const TKey, TValue >& HashTableIterator< TKey, TValue >::operator*() const
  {
    return (**listIter_).data;
  }

  template< class TKey, class TValue >
  std::pair< const TKey, TValue >* HashTableIterator< TKey, TValue >::operator->() const
  {
    return &(**this);
  }

  template< class TKey, class TValue >
  HashTableIterator< TKey, TValue >& HashTableIterator< TKey, TValue >::operator++()
  {
    ++listIter_;
    return *this;
  }

  template< class TKey, class TValue >
  HashTableIterator< TKey, TValue > HashTableIterator< TKey, TValue >::operator++(int)
  {
    ThisType temp(*this);
    operator++();
    return temp;
  }
}

#endif
