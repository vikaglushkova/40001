#include "data_struct.hpp"

#include <iostream>
#include <iomanip>
#include <cstring>

#include "stream_guard.hpp"

std::ostream& andriuschin::operator<<(std::ostream& out, const andriuschin::DataStruct& value)
{
  std::ostream::sentry sentry(out);
  if (sentry)
  {
    return out << "(:key1 " << CharLiteral(value.key1)
        << ":key2 " << RationalLiteral(value.key2)
        << ":key3 " << StringLiteral(value.key3)
        << ":)";
  }
  return out;
}
std::istream& andriuschin::operator>>(std::istream& in, DataStruct& value)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  StreamGuard guard(in);
  constexpr size_t nKeys = 3;
  size_t key = 0;
  char prefix[5] = "";
  bool wasRecived[nKeys] = {};
  if (!(in >> prefix[0]) || (prefix[0] != '('))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  for (size_t i = 0; (i < nKeys) && (in >> std::setw(5) >> prefix) && (std::memcmp(prefix, ":key", 4) == 0); i++)
  {
    if (!(in >> key) || (key == 0) || (key > 3) || (wasRecived[key - 1]))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    wasRecived[key - 1] = true;
    switch (key)
    {
    case 1:
      in >> CharLiteral(value.key1);
      break;
    case 2:
      in >> RationalLiteral(value.key2);
      break;
    case 3:
      in >> StringLiteral(value.key3);
      break;
    }
  }
  if (!(in >> prefix[0] >> prefix[1]) || (std::memcmp(prefix, ":)", 2)
      || !(wasRecived[0] && wasRecived[1] && wasRecived[2])))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

andriuschin::CharLiteral::CharLiteral(const char& value) noexcept:
  data(value),
  link(data)
{}
andriuschin::CharLiteral::CharLiteral(char& value) noexcept:
  data(value),
  link(value)
{}
std::ostream& andriuschin::operator<<(std::ostream& out, const CharLiteral& value)
{
  std::ostream::sentry sentry(out);
  if (sentry)
  {
    return out << '\'' << value.link << '\'';
  }
  return out;
}
std::istream& andriuschin::operator>>(std::istream& in, CharLiteral&& value)
{
  std::istream::sentry sentry(in);
  if (sentry)
  {
    StreamGuard guard(in);
    char input[3] = {};
    if ((in >> input[0]) && (input[0] == '\''))
    {
      if ((in >> input[1] >> input[2]) && (input[2] == '\''))
      {
        value.link = input[1];
        return in;
      }
    }
    in.setstate(std::ios::failbit);
  }
  return in;
}

andriuschin::RationalLiteral::RationalLiteral(const value_type& value) noexcept:
  data(value),
  link(data)
{}
andriuschin::RationalLiteral::RationalLiteral(value_type& value) noexcept:
  data(value),
  link(value)
{}
std::ostream& andriuschin::operator<<(std::ostream& out, const RationalLiteral& value)
{
  std::ostream::sentry sentry(out);
  if (sentry)
  {
    return out << "(:N " << value.link.first << ":D " << value.link.second << ":)";
  }
  return out;
}
std::istream& andriuschin::operator>>(std::istream& in, RationalLiteral&& value)
{
  std::istream::sentry sentry(in);
  if (sentry)
  {
    StreamGuard guard(in);
    char c = '\0';
    long long denominator = 0;
    long long numerator = 0;

    if (!(in >> c) || (c != '(') || !(in >> std::noskipws >> c) || (c != ':'))
    {}
    else if (!(in >> c) || (c != 'N') || !(in >> std::skipws >> numerator))
    {}
    else if (!(in >> c) || (c != ':') || !(in >> std::noskipws >> c) || (c != 'D'))
    {}
    else if (!(in >> std::skipws >> denominator >> c) || (denominator <= 0) || (c != ':'))
    {}
    else if ((in >> std::noskipws >> c) && (c == ')'))
    {
      value.link = {numerator, denominator};
      return in;
    }
    in.setstate(std::ios::failbit);
  }
  return in;
}

andriuschin::StringLiteral::StringLiteral(const std::string& value):
  data(value),
  link(data)
{}
andriuschin::StringLiteral::StringLiteral(std::string& value):
  data(value),
  link(value)
{}
std::ostream& andriuschin::operator<<(std::ostream& out, const StringLiteral& value)
{
  std::ostream::sentry sentry(out);
  if (sentry)
  {
    return out << '"' << value.link << '"';
  }
  return out;
}
std::istream& andriuschin::operator>>(std::istream& in, StringLiteral&& value)
{
  std::istream::sentry sentry(in);
  if (sentry)
  {
    StreamGuard guard(in);
    std::string temp;
    char c = '\0';

    if ((in >> c) && (c == '"') &&
      (std::getline(in, temp, '"')))
    {
      value.link = std::move(temp);
      return in;
    }
    in.setstate(std::ios::failbit);
  }
  return in;
}
