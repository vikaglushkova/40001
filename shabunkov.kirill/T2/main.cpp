#include <iostream>
#include <algorithm>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include "DataStruct.h"

int main()
{
  using nspace::DataStruct;
  using nspace::compareDataStruct;

  std::vector< DataStruct >  data;

  while (!std::cin.eof() && !std::cin.bad())
  {
    std::copy(
      std::istream_iterator< DataStruct >(std::cin),
      std::istream_iterator< DataStruct >(),
      std::back_inserter(data)
    );

    if (std::cin.fail() && !std::cin.eof() && !std::cin.bad())
    {
      std::cin.clear();
      std::cin.ignore();
    }
  }

  std::sort(data.begin(), data.end(), compareDataStruct);

  std::copy(
    std::begin(data),
    std::end(data),
    std::ostream_iterator< DataStruct >(std::cout, "\n")
  );

  return 0;
}

