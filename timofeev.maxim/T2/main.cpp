#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <iterator>
#include <cassert>
#include <algorithm>
#include "DataStruct.hpp"

bool dataStructCompare(const DataStruct& a, const DataStruct& b)
{
  if (a.key1 != b.key1)
  {
    return a.key1 < b.key1;
  }

  if (a.key2 != b.key2)
  {
    return a.key2 < b.key2;
  }

  return (a.key3).length() < (b.key3).length();
}

bool has_e(std::string check)
{
  for (size_t i = 0; i < check.size(); i++)
  {
    if (check[i] == 'e' || check[i] == 'E')
    {
      return true;
    }
  }
  return false;
}
std::string utos(unsigned long long key)
{
  if (key == 0)
  {
    return "0b0";
  }
  else
  {
    std::string binaryPart = "0b";
    while (key != 0)
    {
      binaryPart.insert(2, (key % 2 == 1) ? "1" : "0");
      key /= 2;
    }
    return binaryPart;
  }
}

bool parsingString(std::istream& in, std::string& key3)
{
  std::string toReturn = "";
  getline(in, toReturn, ':');
  if (toReturn[1] == '"' && toReturn.back() == '"')
  {
    toReturn.erase(0, 2);
    toReturn.pop_back();
    key3 = toReturn;
    return true;
  }
  return false;
}

bool parsingDouble(std::istream& in, double& key1)
{
  std::string check;
  getline(in, check, ':');
  check.erase(0, 1);
  if (check[0] >= '1' && check[0] <= '9' && check[1] == '.' && has_e(check))
  {
    double toReturn = stod(check);
    key1 = toReturn;
    return true;
  }
  return false;
}

bool parsingUll(std::istream& in, unsigned long long& key2)
{
  std::string toCheck = "";
  getline(in, toCheck, ':');
  if (toCheck[1] != '0' || (toCheck[2] != 'b' && toCheck[2] != 'B'))
  {
    return false;
  }
  unsigned long long toWrite = stoull(toCheck.substr(3), nullptr, 2);
  key2 = 0;
  for (size_t i = 3; i < toCheck.length(); i++)
  {
    if (toCheck[i] != '0' && toCheck[i] != '1')
    {
      return false;
    }
  }
  key2 = toWrite;
  return true;
}
std::istream& operator >> (std::istream& in, DataStruct& data)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct input;
  char c = ' ';
  char b = ' ';
  in >> c;
  in >> b;
  if (c != '(' && b != ':')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  for (size_t i = 0; i < 3; i++)
  {
    std::string key = "";
    in >> key;
    if (key == "key1")
    {
      if (!parsingDouble(in, input.key1))
      {
        in.setstate(std::ios::failbit);
        return in;
      }
    }
    else if (key == "key2")
    {
      if (!parsingUll(in, input.key2))
      {
        in.setstate(std::ios::failbit);
        return in;
      }
    }
    else if (key == "key3")
    {
      if (!parsingString(in, input.key3))
      {
        in.setstate(std::ios::failbit);
        return in;
      }
    }
  }
  char end = ' ';
  in >> end;
  if (end != ')')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (in)
  {
    data = input;
  }
  return in;
}

std::ostream& operator<< (std::ostream& out, const DataStruct& data)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << "(";
  out << ":" << "key1" << " " << std::fixed << std::setprecision(2) << std::scientific << data.key1;
  out << ":" << "key2" << " " << utos(data.key2);
  out << ":" << "key3" << " " << data.key3;
  out << ":";
  out << ")";
  return out;
}

int main()
{
  std::vector <DataStruct> data;
  while(!std::cin.eof())
    if (std::cin)
    {
      std::copy(
        std::istream_iterator< DataStruct >(std::cin),
        std::istream_iterator< DataStruct >(),
        std::back_inserter(data)
      );
    }
    else
    {
      std::cin.clear();
    }

  std::sort(data.begin(), data.end(), dataStructCompare);

  std::copy(
    std::begin(data),
    std::end(data),
    std::ostream_iterator< DataStruct >(std::cout, "\n")
  );
  return 0;
}
