#include "tests.hpp"
#include <iostream>
#include <string>
#include "vector.hpp"

template< class T >
void print(const babinov::Vector< T >& vect)
{
  for (size_t i = 0; i < vect.size(); ++i)
  {
    std::cout << vect[i] << ' ';
  }
  std::cout << "| SIZE: " << vect.size() << " | CAPACITY: " << vect.capacity() << '\n';
}

void testVector()
{
  using namespace babinov;

  Vector< std::string > v({ "1", "2", "3", "4", "5", "6", "7", "8", "9" });
  print(v);

  Vector< std::string > vect;
  print(vect);
  std::cout << vect.isEmpty() << '\n';

  std::cout << "-------- INSERTION TEST: --------\n\n";

  vect.pushBack("a");
  vect.pushBack("b");
  vect.pushBack("c");
  print(vect);

  vect.pushBack("d");
  vect.pushBack("e");
  vect.pushBack("f");
  print(vect);

  vect.pushBack("g");
  vect.pushBack("h");
  vect.pushBack("i");
  print(vect);

  vect[0] = "A";
  vect[3] = "D";
  vect[8] = "I";
  print(vect);
  std::cout << '\n';

  std::cout << "-------- COPYING AND MOVING TEST: --------\n\n";

  Vector< std::string > vect2(vect);
  vect.pushBack("j");
  print(vect2);

  Vector< std::string > vect3(std::move(vect2));
  print(vect3);

  vect2 = vect3;
  vect3.pushBack("k");
  print(vect2);

  vect3 = std::move(vect2);
  print(vect3);
}
