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
  StreamGuard guard(in);
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
