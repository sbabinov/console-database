#include <fstream>
#include <functional>
#include <stdexcept>

#include "hash_table.hpp"
#include "delimiters.hpp"
#include "tables.hpp"

babinov::DataType getColumnType(const babinov::Table& table, const std::string& columnName)
{
  return table.getColumns()[table.getColumnIndex(columnName)].second;
}

void readTableName(std::istream& in, const babinov::HashTable< std::string, babinov::Table >& tables, std::string& dest)
{
  in >> dest;
  if (tables.find(dest) == tables.cend())
  {
    throw std::invalid_argument("<ERROR: TABLE DOESN'T EXIST>");
  }
}

void readValue(std::istream& in, std::string& value, babinov::DataType dataType)
{
  using del = babinov::CharDelimiterI;
  if (dataType == babinov::TEXT)
  {
    in >> del::sensitive('\"');
    if (in)
    {
      std::getline(in, value, '\"');
    }
  }
  else
  {
    in >> value;
  }
  if (!in)
  {
    throw std::invalid_argument("<ERROR: INVALID VALUE>");
  }
}

void readCondition(std::istream& in, const babinov::Table& table, std::string& columnName, std::string& value)
{
  std::getline(in, columnName, '=');
  try
  {
    babinov::DataType dataType = getColumnType(table, columnName);
    readValue(in, value, dataType);
  }
  catch (const std::out_of_range&)
  {
    throw std::invalid_argument("<ERROR: INVALID COLUMN>");
  }
}

namespace babinov
{
  void execCmdTables(const HashTable< std::string, Table >& tables, std::ostream& out)
  {
    for (auto it = tables.cbegin(); it != tables.cend(); ++it)
    {
      const Vector< Table::Column >& columns = (*it).second.getColumns();
      out << "- " << (*it).first << "  [ ";
      for (size_t i = 0; i < columns.size(); ++i)
      {
        out << columns[i] << ' ';
      }
      out << ']' << '\n';
    }
  }

  void execCmdLoad(HashTable< std::string, Table >& tables, std::istream& in, std::ostream& out)
  {
    std::string fileName;
    std::string tableName;
    in >> fileName >> tableName;
    if (!isCorrectName(tableName))
    {
      throw std::invalid_argument("<ERROR: INVALID TABLE NAME>");
    }
    std::ifstream file(fileName);
    if (!file.is_open())
    {
      throw std::invalid_argument("<ERROR: FILE DOESN'T EXIST>");
    }
    if (tables.find(tableName) != tables.end())
    {
      throw std::invalid_argument("<ERROR: TABLE ALREADY EXISTS>");
    }
    Table newTable;
    file >> newTable;
    if ((!file.eof()) || (!newTable.getColumns().size()))
    {
      throw std::invalid_argument("<ERROR: INVALID TABLE>");
    }
    tables[tableName] = std::move(newTable);
    out << "<SUCCESSFULLY LOADED>" << '\n';
  }

  void execCmdSave(const HashTable< std::string, Table >& tables, std::istream& in, std::ostream& out)
  {
    std::string tableName;
    readTableName(in, tables, tableName);
    std::string fileName;
    in >> fileName;
    std::ofstream file(fileName);
    file << tables.at(tableName);
    out << "<SUCCESSFULLY SAVED>" << '\n';
  }

  void execCmdCreate(HashTable< std::string, Table >& tables, std::istream& in, std::ostream& out)
  {
    std::string tableName;
    in >> tableName;
    if (!isCorrectName(tableName))
    {
      throw std::invalid_argument("<ERORR: INVALID TABLE NAME>");
    }
    if (tables.find(tableName) != tables.end())
    {
      throw std::invalid_argument("<ERROR: TABLE ALREADY EXISTS>");
    }
    Vector< Table::Column > columns;
    while (in && (in.peek() != '\n'))
    {
      Table::Column column;
      in >> column;
      columns.pushBack(column);
    }
    if ((!in) || (!columns.size()))
    {
      throw std::invalid_argument("<ERROR: INVALID COLUMNS>");
    }
    try
    {
      tables[tableName] = Table(std::move(columns));
      out << "<SUCCESSFULLY CREATED>" << '\n';
    }
    catch (const std::invalid_argument&)
    {
      throw std::invalid_argument("<ERROR: INVALID COLUMNS>");
    }
  }

  void execCmdInsert(HashTable< std::string, Table >& tables, std::istream& in, std::ostream& out)
  {
    std::string tableName;
    readTableName(in, tables, tableName);
    Table& table = tables[tableName];
    Table::Row row;
    std::string data;
    const Vector< Table::Column >& columns = table.getColumns();
    for (size_t i = 1; i < columns.size(); ++i)
    {
      readValue(in, data, columns[i].second);
      row.pushBack(data);
    }
    try
    {
      table.insert(std::move(row));
      out << "<SUCCESSFULLY INSERTED>" << '\n';
    }
    catch (const std::invalid_argument&)
    {
      throw std::invalid_argument("<ERROR: INVALID VALUE>");
    }
  }

  void execCmdSelect(const HashTable< std::string, Table >& tables, std::istream& in, std::ostream& out)
  {
    std::string tableName;
    readTableName(in, tables, tableName);
    const Table& table = tables.at(tableName);
    std::string columnName;
    std::string value;
    in.get();
    readCondition(in, table, columnName, value);
    try
    {
      Vector< Table::Row > selection = table.select(columnName, value);
      for (size_t i = 0; i < selection.size(); ++i)
      {
        table.printRow(out, selection[i]);
        out << '\n';
      }
    }
    catch (const std::out_of_range&)
    {
      throw std::invalid_argument("<ERROR: INVALID COLUMN>");
    }
    catch (const std::invalid_argument&)
    {
      throw std::invalid_argument("<ERROR: INVALID VALUE>");
    }
  }

  void execCmdClose(HashTable< std::string, Table >& tables, std::istream& in, std::ostream& out)
  {
    std::string tableName;
    readTableName(in, tables, tableName);
    std::string answer;
    out << "Are you sure you want to close this table (Y/N)?" << '\n' << "> ";
    in >> answer;
    if ((answer == "Y") || (answer == "y"))
    {
      tables.erase(tableName);
      out << "<TABLE SUCCESFULLY CLOSED>" << '\n';
    }
  }

  void execCmdUpdate(HashTable< std::string, Table >& tables, std::istream& in, std::ostream& out)
  {
    std::string tableName;
    readTableName(in, tables, tableName);
    Table& table = tables[tableName];
    size_t id = 0;
    std::string columnName;
    std::string value;
    in >> id;
    if (!in)
    {
      throw std::invalid_argument("<ERROR: INVALID ID>");
    }
    in >> columnName;
    bool isUpdated = false;
    try
    {
      readValue(in, value, getColumnType(table, columnName));
      isUpdated = table.update(id, columnName, std::move(value));
    }
    catch (const std::out_of_range&)
    {
      throw std::invalid_argument("<ERROR: INVALID COLUMN>");
    }
    catch (const std::invalid_argument&)
    {
      throw std::invalid_argument("<ERROR: INVALID VALUE>");
    }
    catch (const std::logic_error&)
    {
      throw std::invalid_argument("<ERROR: CANNOT UPDATE ID FIELD>");
    }
    if (!isUpdated)
    {
      throw std::invalid_argument("<ERROR: INVALID ID>");
    }
    out << "<SUCCESSFULLY UPDATED>" << '\n';
  }

  void execCmdDelete(HashTable< std::string, Table >& tables, std::istream& in, std::ostream& out)
  {
    std::string tableName;
    readTableName(in, tables, tableName);
    Table& table = tables[tableName];
    std::string columnName;
    std::string value;
    in.get();
    readCondition(in, table, columnName, value);
    try
    {
      bool isDeleted = table.del(columnName, value);
      if (isDeleted)
      {
        out << "<SUCCESSFULLY DELETED>" << '\n';
      }
      else
      {
        out << "<THERE ARE NOT ROWS WITH SPECIFIED CONDITION>" << '\n';
      }
    }
    catch (const std::out_of_range&)
    {
      throw std::invalid_argument("<ERROR: INVALID COLUMN>");
    }
    catch (const std::invalid_argument&)
    {
      throw std::invalid_argument("<ERROR: INVALID VALUE>");
    }
  }

  void execCmdClear(HashTable< std::string, Table >& tables, std::istream& in, std::ostream& out)
  {
    std::string tableName;
    readTableName(in, tables, tableName);
    tables[tableName].clear();
    out << "<SUCCESSFULLY CLEARED>" << '\n';
  }
}
