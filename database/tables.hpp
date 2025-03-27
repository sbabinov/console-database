#ifndef TABLES_HPP
#define TABLES_HPP
#include <iostream>
#include <string>

#include "vector.hpp"
#include "list.hpp"
#include "hash_table.hpp"

namespace babinov
{
  enum DataType
  {
    PK,
    INTEGER,
    REAL,
    TEXT
  };

  const HashTable< DataType, std::string > DATA_TYPES_AS_STR = {
    {PK, "PK"},
    {INTEGER, "INTEGER"},
    {REAL, "REAL"},
    {TEXT, "TEXT"}
  };

  const HashTable< std::string, DataType > DATA_TYPES_FROM_STR = {
    {"PK", PK},
    {"INTEGER", INTEGER},
    {"REAL", REAL},
    {"TEXT", TEXT}
  };

  class Table
  {
  public:
    using Row = Vector< std::string >;
    using Column = std::pair< std::string, DataType >;

    Table();
    explicit Table(const Vector< Column >& columns);
    Table(const Table& other);
    Table(Table&& other) noexcept;
    ~Table() = default;
    Table& operator=(const Table& other);
    Table& operator=(Table&& other) noexcept;

    bool isCorrectRow(const Row& row) const;
    const Vector< Column >& getColumns() const;
    const List< Row >& getRows() const;
    size_t getColumnIndex(const std::string& columnName) const;

    void readRow(std::istream& in);
    void printRow(std::ostream& out, List< Row >::Iterator iter) const;
    void printRow(std::ostream& out, const Row& row) const;

    void insert(const Row& row);
    Vector< Row > select(const std::string& columnName, const std::string& value) const;
    bool update(size_t rowId, const std::string& columnName, const std::string& value);
    bool del(const std::string& columnName, const std::string& value);
    void swap(Table& other) noexcept;
    void clear() noexcept;

  private:
    Vector< Column > columns_;
    List< Row > rows_;
    HashTable< size_t, List< Row >::Iterator > rowIters_;
    size_t lastId_;
  };
  std::istream& operator>>(std::istream& in, Table& table);
  std::istream& operator>>(std::istream& in, Table::Column& column);
  std::ostream& operator<<(std::ostream& out, const Table& table);
  std::ostream& operator<<(std::ostream& out, const Table::Column& column);
  bool isCorrectName(const std::string& name);
};

#endif
