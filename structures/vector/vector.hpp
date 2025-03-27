#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <initializer_list>
#include <utility>

namespace babinov
{
  template< class T >
  class Vector
  {
  public:
    Vector();
    Vector(const std::initializer_list< T >& init);
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;
    ~Vector();

    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;
    T& operator[](size_t pos);
    const T& operator[](size_t pos) const;

    size_t size() const noexcept;
    size_t capacity() const noexcept;
    bool isEmpty() const noexcept;

    void swap(Vector& other) noexcept;
    void pushBack(const T& value);
    void clear() noexcept;

  private:
    static const size_t DEFAULT_CAPACITY_ = 3;
    T** elements_;
    size_t size_;
    size_t capacity_;
  };

  template< class T >
  Vector< T >::Vector():
    size_(0),
    capacity_(DEFAULT_CAPACITY_),
    elements_(new T*[DEFAULT_CAPACITY_] {nullptr})
  {}

  template< class T >
  Vector< T >::Vector(const std::initializer_list< T >& init):
    size_(0),
    capacity_(init.size()),
    elements_(new T* [init.size()]{nullptr})
  {
    for (auto it = init.begin(); it != init.end(); ++it)
    {
      pushBack(*it);
    }
  }

  template< class T >
  Vector< T >::Vector(const Vector& other):
    size_(other.size_),
    capacity_(other.size_ + 1),
    elements_(new T*[other.size_ + 1] {nullptr})
  {
    for (size_t i = 0; i < other.size_; ++i)
    {
      elements_[i] = new T(*other.elements_[i]);
    }
  }

  template< class T >
  Vector< T >::Vector(Vector&& other) noexcept:
    size_(other.size_),
    capacity_(other.size_ + 1),
    elements_(other.elements_)
  {
    other.size_ = 0;
    other.capacity_ = DEFAULT_CAPACITY_;
    other.elements_ = nullptr;
  }

  template< class T >
  Vector< T >::~Vector()
  {
    clear();
    delete[] elements_;
  }

  template< class T >
  Vector< T >& Vector< T >::operator=(const Vector& other)
  {
    if (this != &other)
    {
      Vector temp(other);
      swap(temp);
    }
    return *this;
  }

  template< class T >
  Vector< T >& Vector< T >::operator=(Vector&& other) noexcept
  {
    if (this != &other)
    {
      Vector temp(std::move(other));
      swap(temp);
    }
    return *this;
  }

  template< class T >
  T& Vector< T >::operator[](size_t pos)
  {
    return *elements_[pos];
  }

  template< class T >
  const T& Vector< T >::operator[](size_t pos) const
  {
    return *elements_[pos];
  }

  template< class T >
  size_t Vector< T >::size() const noexcept
  {
    return size_;
  }

  template< class T >
  size_t Vector< T >::capacity() const noexcept
  {
    return capacity_;
  }

  template< class T >
  bool Vector< T >::isEmpty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  void Vector< T >::swap(Vector& other) noexcept
  {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(elements_, other.elements_);
  }

  template< class T >
  void Vector< T >::pushBack(const T& value)
  {
    if (size_ == capacity_)
    {
      size_t size = size_;
      T** newElements = new T*[capacity_ * 2] {nullptr};
      for (size_t i = 0; i < size; ++i)
      {
        newElements[i] = new T(*elements_[i]);
      }
      clear();
      size_ = size;
      capacity_ *= 2;
      elements_ = newElements;
    }
    elements_[size_] = new T(value);
    ++size_;
  }

  template< class T >
  void Vector< T >::clear() noexcept
  {
    for (size_t i = 0; i < size_; ++i)
    {
      delete elements_[i];
    }
    size_ = 0;
  }
}

#endif
