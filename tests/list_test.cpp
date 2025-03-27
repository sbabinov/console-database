#include "tests.hpp"
#include <iostream>
#include <utility>
#include "list.hpp"

template< class T >
void print(const babinov::List< T >& list)
{
  for (auto it = list.cbegin(); it != list.cend(); ++it)
  {
    std::cout << *it << ' ';
  }
  std::cout << "| SIZE: " << list.size() << '\n';
}

void testList()
{
  using namespace babinov;
  List< double > list;
  
  std::cout << list.isEmpty() << '\n';
  std::cout << (list.begin() == list.end()) << '\n';
  std::cout << (list.cbegin() == list.cend()) << '\n';

  std::cout << "-------- INSERTION TEST: --------\n\n";

  list.pushBack(10);
  list.pushBack(11);
  list.pushBack(12);
  list.pushBack(13);
  list.pushBack(14);
  print(list);

  list.pushFront(9);
  list.pushFront(8);
  list.pushFront(7);
  list.pushFront(6);
  list.pushFront(5);
  print(list);

  std::cout << list.front() << ' ' << list.back() << '\n';

  auto it = list.insert(list.begin(), 4);
  ++it;
  ++it;
  it = list.insert(it, 6.5);
  ++it;
  ++it;
  ++it;
  it = list.insert(it, 8.5);
  --it;
  list.insert(it, 7.5);
  list.insert(list.end(), 15);
  print(list);
  std::cout << '\n';

  std::cout << "-------- COPYING AND MOVING TEST: --------\n\n";

  List< double > list2(list);
  list.pushBack(100);
  list2.pushBack(200);
  print(list2);
  list.popBack();

  List< double > list3(std::move(list2));
  list3.popBack();
  print(list3);

  list2 = list3;
  list2.pushBack(500);
  print(list2);

  list3 = std::move(list2);
  list3.popBack();
  print(list3);
  std::cout << '\n';

  std::cout << "-------- DELETING TEST: --------\n\n";

  list.popBack();
  list.popBack();
  print(list);

  list.popFront();
  list.popFront();
  print(list);

  it = list.begin();
  ++it;
  ++it;
  it = list.erase(it);
  ++it;
  list.erase(it);
  print(list);

  list.erase(list.begin());
  list.erase(--list.end());
  print(list);

  list3.clear();
  print(list3);
}
