#include "data_struct.hpp"

#include <iostream>
#include "stream_guard.hpp"

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
std::istream& andriuschin::operator>>(std::istream& in, CharLiteral& value)
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
std::istream& andriuschin::operator>>(std::istream& in, CharLiteral&& value)
{
  return in >> value;
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
std::istream& andriuschin::operator>>(std::istream& in, RationalLiteral& value)
{
  std::istream::sentry sentry(in);
  if (sentry)
  {
    StreamGuard guard(in);
    char c = '\0';
    long long denumenator = 0;
    long long numerator = 0;

    if (!(in >> c) || (c != '(') || !(in >> std::noskipws >> c) || (c != ':'))
    {}
    else if (!(in >> c) || (c != 'N') || !(in >> std::skipws >> numerator))
    {}
    else if (!(in >> c) || (c != ':') || !(in >> std::noskipws >> c) || (c != 'D'))
    {}
    else if (!(in >> std::skipws >> denumenator >> c) || (denumenator <= 0) || (c != ':'))
    {}
    else if ((in >> std::noskipws >> c) && (c == ')'))
    {
      value.link = {numerator, denumenator};
      return in;
    }
    in.setstate(std::ios::failbit);
  }
  return in;
}
std::istream& andriuschin::operator>>(std::istream& in, RationalLiteral&& value)
{
  return in >> value;
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
std::istream& andriuschin::operator>>(std::istream& in, StringLiteral& value)
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
std::istream& andriuschin::operator>>(std::istream& in, StringLiteral&& value)
{
  return in >> value;
}
