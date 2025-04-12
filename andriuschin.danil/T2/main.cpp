#include <iostream>
#include <vector>
#include <iterator>
#include <limits>
#include <algorithm>

#include "data_struct.hpp"
#include "comorator.hpp"

int main()
{
  std::vector< andriuschin::DataStruct > vector;
  while (!std::cin.eof())
  {
    std::copy(std::istream_iterator< andriuschin::DataStruct >(std::cin),
        std::istream_iterator< andriuschin::DataStruct >(),
        std::back_inserter(vector));
    if (std::cin.fail() && !std::cin.eof())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(vector.begin(), vector.end(), andriuschin::Comporator());
  for (andriuschin::DataStruct ds: vector)
  {
    std::cout << ds << '\n';
  }
}
