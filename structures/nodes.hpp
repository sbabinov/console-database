#ifndef NODES_HPP
#define NODES_HPP

namespace babinov
{
  namespace detail
  {
    struct BaseListNode
    {
      BaseListNode* next;
      BaseListNode* prev;
      BaseListNode():
        next(nullptr),
        prev(nullptr)
      {}
      virtual ~BaseListNode() = default;
    };

    template< class T >
    struct ListNode: BaseListNode
    {
      T data;
      ListNode(const T& newData):
        BaseListNode(),
        data(newData)
      {}
    };

    template< class T >
    struct HashTableNode
    {
      T data;
      size_t hash;
      HashTableNode(const T& newData, size_t h):
        data(newData),
        hash(h)
      {}
    };
  }
}

#endif
