#include "tests.hpp"
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include "vector.hpp"
#include "hash_table.hpp"

void testHashTableTime()
{
  using namespace babinov;

  const size_t N_ROWS_TO_CONSIDER = 10000;
  const size_t N_ROWS_TO_INSERT = 100000;
  std::ifstream file("hash_table_test_data.txt");
  Vector< std::pair< size_t, std::string > > data;
  std::pair< size_t, std::string > row;
  for (size_t i = 0; i < N_ROWS_TO_CONSIDER + N_ROWS_TO_INSERT; ++i)
  {
    file >> row.first >> row.second;
    data.pushBack(row);
  }
  file.close();

  HashTable< size_t, std::string > table;
  for (size_t i = 0; i < N_ROWS_TO_INSERT; ++i)
  {
    table.insert(data[i].first, data[i].second);
  }

  auto timeBegin = std::chrono::steady_clock::now();
#ifdef INSERTION_MODE
  for (size_t i = 0; i < 2; ++i)
  {
    for (size_t j = N_ROWS_TO_INSERT; j < N_ROWS_TO_INSERT + N_ROWS_TO_CONSIDER / 2; ++j)
    {
      table.insert(data[j].first, data[j].second);
    }
  }
#endif

#ifdef SEARCH_MODE
  for (int i = N_ROWS_TO_CONSIDER / 2; i >= 0; --i)
  {
    table.find(data[i].first);
  }
  for (size_t i = N_ROWS_TO_INSERT; i < (N_ROWS_TO_INSERT + N_ROWS_TO_CONSIDER / 2); ++i)
  {
    table.find(data[i].first);
  }
#endif

#ifdef DELETION_MODE
  for (size_t i = 0; i < N_ROWS_TO_CONSIDER; ++i)
  {
    table.erase(data[i].first);
  }
#endif
  auto timeEnd = std::chrono::steady_clock::now();
  auto elapsedTime = std::chrono::duration_cast< std::chrono::milliseconds >(timeEnd - timeBegin);
  std::cout << "The time: " << elapsedTime.count() << " ms" << '\n';
}
