//#include <functional>
//#include <iostream>
//#include <fstream>
//#include <limits>
//#include <stdexcept>
//
//#include "hash_table.hpp"
//#include "tables.hpp"
//
//#include "tests.hpp"
//
//namespace babinov
//{
//  void execCmdTables(const HashTable< std::string, Table >& tables, std::ostream& out);
//  void execCmdLoad(HashTable< std::string, Table >& tables, std::istream& in, std::ostream& out);
//  void execCmdSave(const HashTable< std::string, Table >& tables, std::istream& in, std::ostream& out);
//  void execCmdCreate(HashTable< std::string, Table >& tables, std::istream& in, std::ostream& out);
//  void execCmdInsert(HashTable< std::string, Table >& tables, std::istream& in, std::ostream& out);
//  void execCmdSelect(const HashTable< std::string, Table >& tables, std::istream& in, std::ostream& out);
//  void execCmdUpdate(HashTable< std::string, Table >& tables, std::istream& in, std::ostream& out);
//  void execCmdDelete(HashTable< std::string, Table >& tables, std::istream& in, std::ostream& out);
//  void execCmdClear(HashTable< std::string, Table >& tables, std::istream& in, std::ostream& out);
//  void execCmdClose(HashTable< std::string, Table >& tables, std::istream& in, std::ostream& out);
//}
//
//#ifdef FILE_TEST_MODE
//std::ifstream file("test_data.txt");
//#define in file
//#else
//#define in std::cin
//#endif
//
//int main()
//{
//  babinov::HashTable< std::string, babinov::Table > tables;
//  babinov::HashTable< std::string, std::function< void(std::istream&, std::ostream&) > > cmds;
//  {
//    using namespace std::placeholders;
//    cmds["tables"] = std::bind(babinov::execCmdTables, std::cref(tables), _2);
//    cmds["load"] = std::bind(babinov::execCmdLoad, std::ref(tables), _1, _2);
//    cmds["save"] = std::bind(babinov::execCmdSave, std::cref(tables), _1, _2);
//    cmds["create"] = std::bind(babinov::execCmdCreate, std::ref(tables), _1, _2);
//    cmds["insert"] = std::bind(babinov::execCmdInsert, std::ref(tables), _1, _2);
//    cmds["select"] = std::bind(babinov::execCmdSelect, std::cref(tables), _1, _2);
//    cmds["update"] = std::bind(babinov::execCmdUpdate, std::ref(tables), _1, _2);
//    cmds["delete"] = std::bind(babinov::execCmdDelete, std::ref(tables), _1, _2);
//    cmds["clear"] = std::bind(babinov::execCmdClear, std::ref(tables), _1, _2);
//    cmds["close"] = std::bind(babinov::execCmdClose, std::ref(tables), _1, _2);
//  }
//  std::string cmd;
//  std::cout << "==$ ";
//
//  while (in >> cmd)
//  {
//    try
//    {
//      cmds.at(cmd)(in, std::cout);
//    }
//    catch (const std::out_of_range&)
//    {
//      std::cout << "<INVALID COMMAND>" << '\n';
//    }
//    catch (const std::exception& e)
//    {
//      std::cout << e.what() << '\n';
//    }
//    in.clear();
//    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
//    std::cout << "==$ ";
//  }
//  return 0;
//}

#include <iostream>
#include "hash_table.hpp"
#include "tests.hpp"

int main()
{
  testHashTable();

  return 0;
}
