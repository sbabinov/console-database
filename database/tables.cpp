#include "tables.hpp"
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <string>
#include <utility>

#include "hash_table.hpp"
#include "delimiters.hpp"

bool isCorrectValue(const std::string& value, babinov::DataType dataType)
{
  size_t size = value.size();
  size_t* pos = &size;
  try
  {
    if (dataType == babinov::PK)
    {
      if (value.size() && value[0] == '-')
      {
        return false;
      }
      std::stoull(value, pos);
    }
    else if (dataType == babinov::INTEGER)
    {
      std::stoi(value, pos);
    }
    else if (dataType == babinov::REAL)
    {
      std::stod(value, pos);
    }
    return *pos == value.size();
  }
  catch (const std::invalid_argument&)
  {
    return false;
  }
}

bool isEqual(const std::string& first, const std::string& second, babinov::DataType dataType)
{
  try
  {
    if (dataType == babinov::PK)
    {
      return std::stoull(first) == std::stoull(first);
    }
    else if (dataType == babinov::INTEGER)
    {
      return std::stoi(first) == std::stoi(second);
    }
    else if (dataType == babinov::REAL)
    {
      return std::stod(first) == std::stod(second);
    }
    return first == second;
  }
  catch (const std::invalid_argument&)
  {
    return false;
  }
}

namespace babinov
{
  bool isCorrectName(const std::string& name)
  {
    auto pred = [&](const char ch) -> bool
    {
      return !((std::isalnum(ch)) || (ch == '_'));
    };
    return (std::find_if(name.cbegin(), name.cend(), pred)) == (name.cend());
  }

  Table::Table():
    columns_(),
    rows_(),
    rowIters_(),
    lastId_(0)
  {}

  Table::Table(const Vector< Column >& columns):
    rows_(),
    rowIters_(),
    lastId_(0)
  {
    for (size_t i = 0; i < columns.size(); ++i)
    {
      Column column = columns[i];
      if ((!isCorrectName(column.first)) || (column.second == PK))
      {
        throw std::invalid_argument("Invalid columns");
      }
    }
    Vector< Column > tempColumns;
    tempColumns.pushBack({"id", PK});
    for (size_t i = 0; i < columns.size(); ++i)
    {
      tempColumns.pushBack(columns[i]);
    }
    columns_ = std::move(tempColumns);
  }
   
  Table::Table(const Table& other):
    columns_(other.columns_),
    rows_(other.rows_),
    lastId_(other.lastId_)
  {
    for (auto it = rows_.begin(); it != rows_.end(); ++it)
    {
      rowIters_.insert(std::stoull((*it)[0]), it);
    }
  }

  Table::Table(Table&& other) noexcept:
    columns_(std::move(other.columns_)),
    rows_(std::move(other.rows_)),
    rowIters_(std::move(other.rowIters_)),
    lastId_(other.lastId_)
  {
    other.lastId_ = 0;
  }

  Table& Table::operator=(const Table& other)
  {
    if (this != &other)
    {
      Table temp(other);
      swap(temp);
    }
    return *this;
  }

  Table& Table::operator=(Table&& other) noexcept
  {
    if (this != &other)
    {
      Table temp(std::move(other));
      swap(temp);
    }
    return *this;
  }

  bool Table::isCorrectRow(const Row& row) const
  {
    if (row.size() != (columns_.size() - 1))
    {
      return false;
    }
    for (size_t i = 0; i < row.size(); ++i)
    {
      if (!isCorrectValue(row[i], columns_[i + 1].second))
      {
        return false;
      }
    }
    return true;
  }

  const Vector< Table::Column >& Table::getColumns() const
  {
    return columns_;
  }

  const List< Table::Row >& Table::getRows() const
  {
    return rows_;
  }

  size_t Table::getColumnIndex(const std::string& columnName) const
  {
    size_t index = 0;
    for (; (index < columns_.size()) && (columns_[index].first != columnName); ++index) {}
    if (index == columns_.size())
    {
      throw std::out_of_range("Column doesn't exist");
    }
    return index;
  }

  void Table::insert(const Row& row)
  {
    if (!isCorrectRow(row))
    {
      throw std::invalid_argument("Invalid row");
    }
    size_t pk = lastId_ + 1;
    Row processed;
    processed.pushBack(std::to_string(pk));
    for (size_t i = 0; i < row.size(); ++i)
    {
      processed.pushBack(row[i]);
    }
    rows_.pushBack(std::move(processed));
    auto iter = std::prev(rows_.end());
    rowIters_[pk] = iter;
    ++lastId_;
  }

  Vector< Table::Row > Table::select(const std::string& columnName, const std::string& value) const
  {
    size_t index = getColumnIndex(columnName);
    DataType dataType = columns_[index].second;
    if (!isCorrectValue(value, dataType))
    {
      throw std::invalid_argument("Invalid value");
    }
    Vector< Row > result;
    if (columnName == "id")
    {
      size_t pk = std::stoull(value);
      if (rowIters_.find(pk) != rowIters_.cend())
      {
        result.pushBack(*rowIters_.at(pk));
      }
      return result;
    }
    for (auto it = rows_.begin(); it != rows_.end(); ++it)
    {
      if (isEqual((*it)[index], value, dataType))
      {
        result.pushBack(*it);
      }
    }
    return result;
  }

  bool Table::update(size_t rowId, const std::string& columnName, const std::string& value)
  {
    if (columnName == "id")
    {
      throw std::logic_error("Cannot update id field");
    }
    size_t index = getColumnIndex(columnName);
    if (!isCorrectValue(value, columns_[index].second))
    {
      throw std::invalid_argument("Invalid value");
    }
    try
    {
      (*rowIters_.at(rowId))[index] = value;
      return true;
    }
    catch (const std::out_of_range&)
    {
      return false;
    }
  }

  bool Table::del(const std::string& columnName, const std::string& value)
  {
    size_t index = getColumnIndex(columnName);
    DataType dataType = columns_[index].second;
    if (!isCorrectValue(value, dataType))
    {
      throw std::invalid_argument("Invalid value");
    }
    if (columnName == "id")
    {
      size_t pk = std::stoull(value);
      if (rowIters_.find(pk) != rowIters_.end())
      {
        rows_.erase(rowIters_[pk]);
        rowIters_.erase(pk);
        return true;
      }
      return false;
    }
    bool isDeleted = false;
    auto it = rows_.begin();
    while (it != rows_.end())
    {
      auto temp = it;
      ++it;
      if (isEqual((*temp)[index], value, dataType))
      {
        size_t pk = std::stoull((*temp)[0]);
        rows_.erase(temp);
        rowIters_.erase(pk);
        isDeleted = true;
      }
    }
    return isDeleted;
  }

  void Table::swap(Table& other) noexcept
  {
    std::swap(columns_, other.columns_);
    std::swap(rows_, other.rows_);
    std::swap(rowIters_, other.rowIters_);
    std::swap(lastId_, other.lastId_);
  }

  void Table::clear() noexcept
  {
    rows_.clear();
    rowIters_.clear();
    lastId_ = 0;
  }

  std::istream& operator>>(std::istream& in, Table::Column& column)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    using del = CharDelimiterI;
    std::string dataType;
    std::getline(in, column.first, ':');
    in >> dataType;
    if (DATA_TYPES_FROM_STR.find(dataType) != DATA_TYPES_FROM_STR.cend())
    {
      column.second = DATA_TYPES_FROM_STR.at(dataType);
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  void Table::readRow(std::istream& in)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return;
    }
    using del = CharDelimiterI;
    std::string data;
    in >> del::insensitive('[') >> data;
    size_t pk = std::stoull(data);

    Table::Row row;
    row.pushBack(data);

    for (size_t i = 1; i < columns_.size(); ++i)
    {
      if (columns_[i].second == TEXT)
      {
        in >> del::sensitive('\"');
        std::getline(in, data, '\"');
      }
      else
      {
        in >> data;
      }
      row.pushBack(data);
    }
    in >> del::insensitive(']');

    if (in)
    {
      rows_.pushBack(std::move(row));
      auto iter = std::prev(rows_.end());
      rowIters_[pk] = iter;
      lastId_ = std::max(lastId_, pk);
    }
  }

  std::istream& operator>>(std::istream& in, Table& table)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    using del = StringDelimiterI;
    size_t nColumns = 0;
    in >> nColumns >> del::sensitive("COLUMNS:");
    Vector< Table::Column > columns;
    Table::Column column;
    in >> column;
    for (size_t i = 1; in && (i < nColumns); ++i)
    {
      in >> column;
      columns.pushBack(column);
    }
    if ((!nColumns) || (columns.size() != nColumns - 1))
    {
      in.setstate(std::ios::failbit);
    }
    if (in.fail())
    {
      return in;
    }
    table = Table(std::move(columns));
    while (in)
    {
      table.readRow(in);
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const Table::Column& column)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    out << column.first << ':' << DATA_TYPES_AS_STR.at(column.second);
    return out;
  }

  void Table::printRow(std::ostream& out, const Table::Row& row) const
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return;
    }
    out << "[ ";
    for (size_t i = 0; i < columns_.size(); ++i)
    {
      if (columns_[i].second == TEXT)
      {
        out << '\"' << row[i] << '\"';
      }
      else
      {
        out << row[i];
      }
      out << ' ';
    }
    out << ']';
  }

  void Table::printRow(std::ostream& out, List< Row >::Iterator iter) const
  {
    printRow(out, *iter);
  }

  std::ostream& operator<<(std::ostream& out, const Table& table)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    const Vector< Table::Column >& columns = table.getColumns();
    out << columns.size() << ' ' << "COLUMNS: ";
    for (size_t i = 0; i < columns.size(); ++i)
    {
      out << columns[i] << ' ';
    }
    const List< Table::Row >& rows = table.getRows();
    for (auto it = rows.begin(); it != rows.end(); ++it)
    {
      out << '\n';
      table.printRow(out, it);
    }
    return out;
  }
}
