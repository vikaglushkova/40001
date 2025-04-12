#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <utility>
#include <string>

namespace andriuschin
{
  struct DataStruct
  {
    char key1;
    std::pair< long long, unsigned long long > key2;
    std::string key3;
  };
  std::ostream& operator<<(std::ostream& out, const DataStruct& value);
  std::istream& operator>>(std::istream& in, DataStruct& value);

  struct Demand
  {
    char expected = '\0';
  };
  std::istream& operator>>(std::istream& in, Demand&& value);

  struct CharLiteral;
  std::ostream& operator<<(std::ostream& out, const CharLiteral& value);
  std::istream& operator>>(std::istream& in, CharLiteral&& value);

  struct RationalLiteral;
  std::ostream& operator<<(std::ostream& out, const RationalLiteral& value);
  std::istream& operator>>(std::istream& in, RationalLiteral&& value);

  struct StringLiteral;
  std::ostream& operator<<(std::ostream& out, const StringLiteral& value);
  std::istream& operator>>(std::istream& in, StringLiteral&& value);

  struct CharLiteral
  {
    CharLiteral(const char& value) noexcept;
    CharLiteral(char& value) noexcept;

    friend std::ostream& andriuschin::operator<<(std::ostream& out, const CharLiteral& value);
    friend std::istream& andriuschin::operator>>(std::istream& in, CharLiteral&& value);
  private:
    char data;
    char& link;
  };

  struct RationalLiteral
  {
    using value_type = std::pair< long long, unsigned long long >;
    RationalLiteral(const value_type& value) noexcept;
    RationalLiteral(value_type& value) noexcept;

    friend std::ostream& andriuschin::operator<<(std::ostream& out, const RationalLiteral& value);
    friend std::istream& andriuschin::operator>>(std::istream& in, RationalLiteral&& value);
  private:
    value_type data;
    value_type& link;
  };

  struct StringLiteral
  {
    StringLiteral(const std::string& value);
    StringLiteral(std::string& value);

    friend std::ostream& andriuschin::operator<<(std::ostream& out, const StringLiteral& value);
    friend std::istream& andriuschin::operator>>(std::istream& in, StringLiteral&& value);
  private:
    std::string data;
    std::string& link;
  };
}

#endif
