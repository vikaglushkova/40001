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
