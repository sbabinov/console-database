#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <cmath>
#include <utility>
#include <initializer_list>
#include <stdexcept>
#include "nodes.hpp"
#include "list.hpp"
#include "hash_table_iterator.hpp"
#include "hash_table_const_iterator.hpp"

namespace babinov
{
  template< class TKey, class TValue >
  class HashTable
  {
  public:
    using ValueType = std::pair< const TKey, TValue >;
    using ListNode = detail::HashTableNode< ValueType >;
    using ListIterator = typename List< ListNode* >::Iterator;
    using Iterator = HashTableIterator< TKey, TValue >;
    using ConstIterator = ConstHashTableIterator< TKey, TValue >;

    HashTable();
    HashTable(const std::initializer_list< ValueType >& init);
    HashTable(const HashTable< TKey, TValue >& other);
    HashTable(HashTable< TKey, TValue >&& other) noexcept;
    ~HashTable();

    HashTable< TKey, TValue >& operator=(const HashTable< TKey, TValue >& other);
    HashTable< TKey, TValue >& operator=(HashTable< TKey, TValue >&& other) noexcept;
    TValue& operator[](const TKey& key);
    TValue& operator[](TKey&& key);
    TValue& at(const TKey& key);
    const TValue& at(const TKey& key) const;

    bool isEmpty() const noexcept;
    size_t size() const noexcept;
    size_t bucketCount() const noexcept;

    void rehash(size_t count);
    std::pair< Iterator, bool > insert(const TKey& key, const TValue& value);
    std::pair< Iterator, bool > insert(const TKey& key, TValue&& value);
    Iterator erase(Iterator pos);
    bool erase(const TKey& key);
    void swap(HashTable< TKey, TValue >& other) noexcept;
    void clear() noexcept;

    Iterator find(const TKey& key);
    ConstIterator find(const TKey& key) const;

    Iterator begin() noexcept;
    Iterator end() noexcept;
    ConstIterator cbegin() const noexcept;
    ConstIterator cend() const noexcept;

  private:
    static const size_t DEFAULT_CAPACITY_ = 5;
    static constexpr float MAX_LOAD_FACTOR_ = 0.75;
    size_t count_;
    size_t capacity_;
    List< ListNode* > elements_;
    ListIterator* buckets_;

    size_t calculateNextCapacity(size_t current) const;
    Iterator find(const TKey& key, size_t hash);
    ConstIterator find(const TKey& key, size_t hash) const;
    std::pair< Iterator, bool > insert(const TKey& key, const TValue& value, size_t hash);
  };

  template< class TKey, class TValue >
  HashTable< TKey, TValue >::HashTable():
    capacity_(DEFAULT_CAPACITY_),
    count_(0),
    buckets_(new ListIterator[DEFAULT_CAPACITY_]),
    elements_()
  {
    for (size_t i = 0; i < DEFAULT_CAPACITY_; ++i)
    {
      buckets_[i] = elements_.end();
    }
  }

  template< class TKey, class TValue >
  HashTable< TKey, TValue >::HashTable(const std::initializer_list< ValueType >& init):
    capacity_(init.size() / MAX_LOAD_FACTOR_ + 1),
    count_(0),
    buckets_(new ListIterator[init.size() / MAX_LOAD_FACTOR_ + 1]),
    elements_()
  {
    for (size_t i = 0; i < capacity_; ++i)
    {
      buckets_[i] = elements_.end();
    }
    for (auto it = init.begin(); it != init.end(); ++it)
    {
      insert((*it).first, (*it).second);
    }
  }

  template< class TKey, class TValue >
  HashTable< TKey, TValue >::HashTable(const HashTable< TKey, TValue >& other):
    capacity_(other.capacity_),
    count_(0),
    buckets_(new ListIterator[other.capacity_]),
    elements_()
  {
    for (size_t i = 0; i < capacity_; ++i)
    {
      buckets_[i] = elements_.end();
      if (other.buckets_[i] != other.elements_.end())
      {
        ListIterator iter = other.buckets_[i];
        insert(TKey((**iter).data.first), TValue((**iter).data.second), (**iter).hash);
      }
    }
    for (auto it = other.elements_.cbegin(); it != other.elements_.cend(); ++it)
    {
      insert(TKey((**it).data.first), TValue((**it).data.second), (**it).hash);
    }
  }

  template< class TKey, class TValue >
  HashTable< TKey, TValue >::HashTable(HashTable< TKey, TValue >&& other) noexcept:
    capacity_(other.capacity_),
    count_(other.count_),
    elements_(std::move(other.elements_)),
    buckets_(other.buckets_)
  {
    other.capacity_ = DEFAULT_CAPACITY_;
    other.count_ = 0;
    other.buckets_ = nullptr;
  }

  template< class TKey, class TValue >
  HashTable< TKey, TValue >::~HashTable()
  {
    clear();
    delete[] buckets_;
  }

  template< class TKey, class TValue >
  HashTable< TKey, TValue >& HashTable< TKey, TValue >::operator=(const HashTable< TKey, TValue >& other)
  {
    if (this != &other)
    {
      HashTable< TKey, TValue > temp(other);
      swap(temp);
    }
    return *this;
  }

  template< class TKey, class TValue >
  HashTable< TKey, TValue >& HashTable< TKey, TValue >::operator=(HashTable< TKey, TValue >&& other) noexcept
  {
    if (this != &other)
    {
      HashTable< TKey, TValue > temp(std::move(other));
      swap(temp);
    }
    return *this;
  }

  template< class TKey, class TValue >
  TValue& HashTable< TKey, TValue >::operator[](const TKey& key)
  {
    auto desired = insert(key, TValue());
    return (*(desired.first)).second;
  }

  template< class TKey, class TValue >
  TValue& HashTable< TKey, TValue >::operator[](TKey&& key)
  {
    return operator[](key);
  }

  template< class TKey, class TValue >
  TValue& HashTable< TKey, TValue >::at(const TKey& key)
  {
    auto desired = find(key);
    if (desired == end())
    {
      throw std::out_of_range("There are not value with specific key");
    }
    return (*desired).second;
  }

  template< class TKey, class TValue >
  const TValue& HashTable< TKey, TValue >::at(const TKey& key) const
  {
    auto desired = find(key);
    if (desired == cend())
    {
      throw std::out_of_range("There are not value with specific key");
    }
    return (*desired).second;
  }

  template< class TKey, class TValue >
  bool HashTable< TKey, TValue >::isEmpty() const noexcept
  {
    return count_ == 0;
  }

  template< class TKey, class TValue >
  size_t HashTable< TKey, TValue >::size() const noexcept
  {
    return count_;
  }

  template< class TKey, class TValue >
  size_t HashTable< TKey, TValue >::bucketCount() const noexcept
  {
    return capacity_;
  }

  template< class TKey, class TValue >
  void HashTable< TKey, TValue >::clear() noexcept
  {
    auto it = elements_.begin();
    auto temp = it;
    while (it != elements_.end())
    {
      buckets_[(**it).hash % capacity_] = elements_.end();
      ++it;
      delete *temp;
      elements_.erase(temp);
      temp = it;
    }
    count_ = 0;
  }

  template< class TKey, class TValue >
  void HashTable< TKey, TValue >::rehash(size_t count)
  {
    size_t newCapacity = DEFAULT_CAPACITY_;
    while ((newCapacity < count) || (newCapacity < (count_ / MAX_LOAD_FACTOR_)))
    {
      newCapacity = calculateNextCapacity(newCapacity);
    }

    if (newCapacity == capacity_)
    {
      return;
    }

    List< ListNode* > newElements;
    ListIterator* newTable = new ListIterator[newCapacity] {};
    for (size_t i = 0; i < newCapacity; ++i)
    {
      newTable[i] = newElements.end();
    }

    auto it = elements_.begin();
    auto temp = it;
    while (it != elements_.end())
    {
      size_t index = (**it).hash % newCapacity;
      if (newTable[index] == newElements.end())
      {
        newElements.pushFront(*it);
        newTable[index] = newElements.begin();
      }
      else
      {
        newElements.insert(newTable[index], *it);
        --(newTable[index]);
      }
      ++it;
      elements_.erase(temp);
      temp = it;
    }
    elements_ = std::move(newElements);
    delete[] buckets_;
    buckets_ = newTable;
    capacity_ = newCapacity;
  }

  template< class TKey, class TValue >
  std::pair< HashTableIterator< TKey, TValue >, bool > HashTable< TKey, TValue >::insert(
    const TKey& key, const TValue& value
  )
  {
    return insert(key, value, std::hash< TKey >()(key));
  }

  template< class TKey, class TValue >
  std::pair< HashTableIterator< TKey, TValue >, bool > HashTable< TKey, TValue >::insert(
    const TKey& key, TValue&& value
  )
  {
    return insert(key, value, std::hash< TKey >()(key));
  }

  template< class TKey, class TValue >
  HashTableIterator< TKey, TValue > HashTable< TKey, TValue >::erase(Iterator pos)
  {
    ListIterator iter = pos.listIter_;
    size_t hash = (*iter)->hash;
    if ((pos.listIter_ == elements_.begin()) || (((*(--iter))->hash % capacity_) != (hash % capacity_)))
    {
      size_t index = hash % capacity_;
      delete *(pos.listIter_);
      ListIterator next = elements_.erase(pos.listIter_);
      if ((next != elements_.end()) && (((*next)->hash % capacity_) == (hash % capacity_)))
      {
        buckets_[index] = next;
      }
      else
      {
        buckets_[index] = elements_.end();
      }
      --count_;
      return Iterator(next);
    }
    else
    {
      delete *(pos.listIter_);
      --count_;
      return Iterator(elements_.erase(pos.listIter_));
    }
  }

  template< class TKey, class TValue >
  bool HashTable< TKey, TValue >::erase(const TKey& key)
  {
    auto it = find(key);
    if (it != end())
    {
      erase(it);
      return true;
    }
    return false;
  }

  template< class TKey, class TValue >
  void HashTable< TKey, TValue >::swap(HashTable< TKey, TValue >& other) noexcept
  {
    std::swap(capacity_, other.capacity_);
    std::swap(count_, other.count_);
    std::swap(elements_, other.elements_);
    std::swap(buckets_, other.buckets_);
  }

  template< class TKey, class TValue >
  HashTableIterator< TKey, TValue > HashTable< TKey, TValue >::find(const TKey& key)
  {
    return find(key, std::hash< TKey >()(key));
  }

  template< class TKey, class TValue >
  ConstHashTableIterator< TKey, TValue > HashTable< TKey, TValue >::find(const TKey& key) const
  {
    return find(key, std::hash< TKey >()(key));
  }

  template< class TKey, class TValue >
  HashTableIterator< TKey, TValue > HashTable< TKey, TValue >::begin() noexcept
  {
    return Iterator(elements_.begin());
  }

  template< class TKey, class TValue >
  HashTableIterator< TKey, TValue > HashTable< TKey, TValue >::end() noexcept
  {
    return Iterator(elements_.end());
  }

  template< class TKey, class TValue >
  ConstHashTableIterator< TKey, TValue > HashTable< TKey, TValue >::cbegin() const noexcept
  {
    return ConstIterator(elements_.cbegin());
  }

  template< class TKey, class TValue >
  ConstHashTableIterator< TKey, TValue > HashTable< TKey, TValue >::cend() const noexcept
  {
    return ConstIterator(elements_.cend());
  }

  template< class TKey, class TValue >
  size_t HashTable< TKey, TValue >::calculateNextCapacity(size_t current) const
  {
    size_t degree = 0;
    for (; current > std::pow(2, degree); ++degree) {};
    ++degree;
    return std::pow(2, degree) - (std::pow(2, degree) - std::pow(2, degree - 1)) / 2 - 1;
  }

  template< class TKey, class TValue >
  HashTableIterator< TKey, TValue > HashTable< TKey, TValue >::find(const TKey& key, size_t hash)
  {
    size_t index = hash % capacity_;
    auto bucketIter = buckets_[index];
    if (bucketIter == elements_.end())
    {
      return end();
    }
    while ((bucketIter != elements_.end())
      && (((**bucketIter).hash % capacity_) == index)
      && ((**bucketIter).data.first != key))
    {
      ++bucketIter;
    }
    if ((bucketIter != elements_.end()) && ((**bucketIter).data.first == key))
    {
      return Iterator(bucketIter);
    }
    return end();
  }

  template< class TKey, class TValue >
  ConstHashTableIterator< TKey, TValue > HashTable< TKey, TValue >::find(const TKey& key, size_t hash) const
  {
    size_t index = hash % capacity_;
    auto bucketIter = buckets_[index];
    if (bucketIter == elements_.end())
    {
      return cend();
    }
    while ((bucketIter != elements_.end())
      && (((**bucketIter).hash % capacity_) == index)
      && ((**bucketIter).data.first != key))
    {
      ++bucketIter;
    }
    if ((bucketIter != elements_.end()) && ((**bucketIter).data.first == key))
    {
      return ConstIterator(bucketIter);
    }
    return cend();
  }

  template< class TKey, class TValue >
  std::pair< HashTableIterator< TKey, TValue >, bool > HashTable< TKey, TValue >::insert(
    const TKey& key, const TValue& value, size_t hash)
  {
    Iterator desired = find(key, hash);
    if (desired != end())
    {
      return std::pair< Iterator, bool >(desired, false);
    }

    size_t index = hash % capacity_;
    if (capacity_ < ((count_ + 1) / MAX_LOAD_FACTOR_))
    {
      rehash(calculateNextCapacity(capacity_));
      index = hash % capacity_;
    }

    ListNode* node = new ListNode(ValueType(key, value), hash);
    std::pair < Iterator, bool > result(end(), true);
    try
    {
      if (buckets_[index] == elements_.end())
      {
        elements_.pushFront(node);
        buckets_[index] = elements_.begin();
        result.first = begin();
      }
      else
      {
        elements_.insert(buckets_[index], node);
        --(buckets_[index]);
        result.first = Iterator(buckets_[index]);
      }
    }
    catch (...)
    {
      delete node;
      throw;
    }
    ++count_;
    return result;
  }
}

#endif
