#include "tests.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include "tables.hpp"
#include "vector.hpp"

void printRows(const babinov::Vector< babinov::Table::Row >& rows_)
{
  for (size_t i = 0; i < rows_.size(); ++i)
  {
    std::cout << "[ ";
    for (size_t j = 0; j < rows_[i].size(); ++j)
    {
      std::cout << rows_[i][j] << ' ';
    }
    std::cout << ']' << '\n';
  }
}

void testTable()
{
  using namespace babinov;

  std::cout << "-------- CREATION TEST: --------\n\n";
  Vector< Table::Column > columns_;
  columns_.pushBack({ "first_name", TEXT });
  columns_.pushBack({ "last_name", TEXT });
  columns_.pushBack({ "balance", REAL });
  Table t(columns_);
  std::cout << t << '\n';
  columns_.pushBack({ "id", PK });
  try
  {
    Table t2(columns_);
  }
  catch (const std::invalid_argument& e)
  {
    columns_.clear();
    std::cerr << e.what() << '\n';
  }

  columns_.pushBack({ "first_name", TEXT });
  columns_.pushBack({ "last_name", TEXT });
  columns_.pushBack({ "first_name", TEXT });
  try
  {
    Table t3(columns_);
  }
  catch (const std::invalid_argument& e)
  {
    columns_.clear();
    std::cerr << e.what() << '\n';
  }

  columns_.pushBack({ "first name", TEXT });
  columns_.pushBack({ "last_name", TEXT });
  try
  {
    Table t4(columns_);
  }
  catch (const std::invalid_argument& e)
  {
    columns_.clear();
    std::cerr << e.what() << '\n';
  }

  columns_.pushBack({ "first_name", TEXT });
  columns_.pushBack({ "last_name", TEXT });
  columns_.pushBack({ "age", INTEGER });
  columns_.pushBack({ "balance", REAL });
  Table table(columns_);
  std::cout << table << '\n' << '\n';

  std::cout << "-------- INSERTION TEST: --------\n\n";

  table.insert({ "Jonh", "Smith", "32", "100.2" });
  table.insert({ "Steve", "Johnson", "28", "1000" });
  table.insert({ "Carl", "Walker", "25", "2000.54" });
  table.insert({ "Charlie", "Jackson", "21", "12462.1" });
  table.insert({ "Oliver", "Evans", "42", "0" });
  table.insert({ "Jack", "Roberts", "39", "3254.24" });
  table.insert({ "Harry", "Thomson", "25", "243422" });
  table.insert({ "William", "Taylor", "54", "4234.03" });
  table.insert({ "George", "Brown", "19", "6342.13" });
  table.insert({ "Tomas", "Willson", "30", "98023.10" });
  std::cout << table << '\n';

  try
  {
    table.insert({ "Jonh", "32", "100.2" });
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }

  try
  {
    table.insert({ "Jonh", "Smith", "-", "0" });
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }

  std::cout << "-------- SELECTION TEST: --------\n\n";

  printRows(table.select("first_name", "George"));
  printRows(table.select("id", "3"));
  printRows(table.select("balance", "1000"));
  printRows(table.select("age", "30"));
  printRows(table.select("age", "18"));

  try
  {
    table.select("name", "George Thomson");
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }

  try
  {
    table.select("balance", "-");
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  std::cout << '\n';

  std::cout << "-------- UPDATION TEST: --------\n\n";

  bool isUpdated = false;
  isUpdated = table.update(1, "balance", "0");
  std::cout << isUpdated << '\n' << table << '\n';

  isUpdated = table.update(100, "balance", "0");
  std::cout << isUpdated << '\n';

  try
  {
    isUpdated = table.update(1, "balance_", "0");
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }

  try
  {
    isUpdated = table.update(1, "id", "0");
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }

  try
  {
    isUpdated = table.update(1, "age", "-");
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }

  std::cout << table << '\n' << '\n';

  std::cout << "-------- COPYING AND MOVING TEST: --------\n\n";

  Table table2(table);
  table.insert({ "Mary", "Brown", "26", "7562.7" });
  std::cout << table2 << '\n' << '\n';

  table2 = table;
  table.update(10, "age", "27");
  std::cout << table2 << '\n' << '\n';

  Table table3(std::move(table));
  table3.update(2, "balance", "0");
  std::cout << table3 << '\n' << '\n';

  table = std::move(table3);
  std::cout << table << '\n' << '\n';

  std::cout << "-------- DELETION TEST: --------\n\n";

  bool isDeleted = false;
  isDeleted = table.del("id", "2");
  std::cout << isDeleted << '\n';
  isDeleted = table.del("id", "2");
  std::cout << isDeleted << '\n';
  isDeleted = table.del("first_name", "Jack");
  std::cout << isDeleted << '\n';
  isDeleted = table.del("age", "28");
  std::cout << isDeleted << '\n';
  isDeleted = table.del("balance", "2000.54");
  std::cout << isDeleted << '\n';
  std::cout << table << '\n';

  try
  {
    table.del("balance_", "0");
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }

  try
  {
    table.del("age", "-");
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }

  table.clear();
  std::cout << table << '\n';
  table.insert({ "Mary", "Brown", "26", "7562.7" });
  std::cout << table << '\n';

  std::cout << "-------- OTHER TESTS: --------\n\n";

  Table notes({ { "name", TEXT }, { "note", TEXT } });
  notes.insert({ "note1", "row1\nrow2\nrow3\n\n\n   row4   \n row5" });
  std::cout << notes;
}
