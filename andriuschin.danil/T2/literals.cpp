#include "data_struct.hpp"

#include <iostream>
#include "format_guard.hpp"

andriuschin::CharLiteral::CharLiteral(const char& value) noexcept:
  data_(value),
  link_(data_)
{}
andriuschin::CharLiteral::CharLiteral(char& value) noexcept:
  data_(value),
  link_(value)
{}
std::ostream& andriuschin::operator<<(std::ostream& out, const CharLiteral& value)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  return out << '\'' << value.link_ << '\'';
}
std::istream& andriuschin::operator>>(std::istream& in, CharLiteral&& value)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  FormatGuard guard(in);
  char c = '\0';
  if (in >> Demand{'\''} >> std::noskipws >> c >> Demand{'\''})
  {
    value.link_ = c;
  }
  return in;
}

andriuschin::RationalLiteral::RationalLiteral(const Rational& value) noexcept:
  data_(value),
  link_(data_)
{}
andriuschin::RationalLiteral::RationalLiteral(Rational& value) noexcept:
  data_(value),
  link_(value)
{}
std::ostream& andriuschin::operator<<(std::ostream& out, const RationalLiteral& value)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  return out << "(:N " << value.link_.first << ":D " << value.link_.second << ":)";
}
std::istream& andriuschin::operator>>(std::istream& in, RationalLiteral&& value)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  FormatGuard guard(in);
  unsigned long long denominator = 0;
  long long numerator = 0;

  in >> Demand{'('} >> std::noskipws >> Demand{':'};
  in >> Demand{'N'} >> Demand{' '} >> numerator;
  in >> Demand{':'} >> Demand{'D'} >> Demand{' '};
  if (!in || in.peek() == '-')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  in >> denominator >> Demand{':'};
  in >> std::noskipws >> Demand{')'};
  if (!in)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  value.link_ = {numerator, denominator};
  return in;
}

andriuschin::StringLiteral::StringLiteral(const std::string& value):
  data_(value),
  link_(data_)
{}
andriuschin::StringLiteral::StringLiteral(std::string& value):
  data_(value),
  link_(value)
{}
std::ostream& andriuschin::operator<<(std::ostream& out, const StringLiteral& value)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  return out << '"' << value.link_ << '"';
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
    value.link_ = std::move(temp);
  }
  return in;
}
