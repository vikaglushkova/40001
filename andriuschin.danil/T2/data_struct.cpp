#include "data_struct.hpp"

#include <iostream>
#include "stream_guard.hpp"

pgm::CharLiteral::CharLiteral(const char& value) noexcept:
  data(value),
  link(data)
{}
pgm::CharLiteral::CharLiteral(char& value) noexcept:
  data(value),
  link(value)
{}

std::ostream& pgm::operator<<(std::ostream& out, const CharLiteral& value)
{
  std::ostream::sentry sentry(out);
  if (sentry) {
    return out << '\'' << value.link << '\'';
  }
  return out;
}

std::istream& pgm::operator>>(std::istream& in, CharLiteral& value)
{
  std::istream::sentry sentry(in);
  if (sentry) {
    StreamGuard guard(in);
    char input[3] = {};
    if ((in >> input[0]) && (input[0] == '\'')) {
      if ((in >> input[1] >> input[2]) && (input[2] == '\'')) {
        value.link = input[1];
        return in;
      }
    }
    in.setstate(std::ios::failbit);
  }
  return in;
}
std::istream& pgm::operator>>(std::istream& in, CharLiteral&& value)
{
  return in >> value;
}
