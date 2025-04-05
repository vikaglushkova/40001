#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <utility>
#include <string>

namespace pgm
{
  struct DataStruct
  {
    char key1;
    std::pair< long long, unsigned long long > key2;
    std::string key3;
  };

  struct CharLiteral
  {
    char data;
    char& link;

    CharLiteral(const char& value) noexcept;
    CharLiteral(char& value) noexcept;
  };

  std::ostream& operator<<(std::ostream& out, const CharLiteral& value);
  std::istream& operator>>(std::istream& in, CharLiteral& value);
  std::istream& operator>>(std::istream& in, CharLiteral&& value);

  std::ostream& operator<<(std::ostream& out, const DataStruct& value);
  std::istream& operator>>(std::istream& in, DataStruct& value);
}

#endif
