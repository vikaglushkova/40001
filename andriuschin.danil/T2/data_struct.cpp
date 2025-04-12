#include "data_struct.hpp"

#include <iostream>
#include <iomanip>
#include <cstring>

#include "stream_guard.hpp"

std::ostream& andriuschin::operator<<(std::ostream& out, const andriuschin::DataStruct& value)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  return out << "(:key1 " << CharLiteral(value.key1) << ":key2 " << RationalLiteral(value.key2)
      << ":key3 " << StringLiteral(value.key3) << ":)";
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
  if (!(in >> Demand{'('}))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  while ((in >> std::setw(5) >> prefix) && (std::memcmp(prefix, ":key", 4) == 0))
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
    if (wasRecived[0] && wasRecived[1] && wasRecived[2])
    {
      break;
    }
  }
  return in >> Demand{':'} >> Demand{')'};
}

std::istream& andriuschin::operator>>(std::istream& in, andriuschin::Demand&& value)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c = '\0';
  if (!(in >> c) || (c != value.expected))
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
  if (!sentry)
  {
    return out;
  }
  return out << '\'' << value.link << '\'';
}
std::istream& andriuschin::operator>>(std::istream& in, CharLiteral&& value)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  StreamGuard guard(in);
  char c = '\0';
  if (in >> Demand{'\''} >> std::noskipws >> c >> Demand{'\''})
  {
    value.link = c;
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
  if (!sentry)
  {
    return out;
  }
  return out << "(:N " << value.link.first << ":D " << value.link.second << ":)";
}
std::istream& andriuschin::operator>>(std::istream& in, RationalLiteral&& value)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  StreamGuard guard(in);;
  long long denominator = 0;
  long long numerator = 0;

  in >> Demand{'('} >> std::noskipws >> Demand{':'};
  in >> Demand{'N'} >> std::skipws >> numerator;
  in >> Demand{':'} >> std::noskipws >> Demand{'D'};
  in >> std::skipws >> denominator >> Demand{':'};
  in >> std::noskipws >> Demand{')'};
  if (!in || denominator <= 0)
  {
    in.setstate(std::ios::failbit);
  }

  value.link = {numerator, denominator};
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
  if (!sentry)
  {
    return out;
  }
  return out << '"' << value.link << '"';
}
std::istream& andriuschin::operator>>(std::istream& in, StringLiteral&& value)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string temp;

  if (std::getline(in >> Demand{'"'}, temp, '"'))
  {
    value.link = std::move(temp);
  }
  return in;
}
