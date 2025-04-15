#include <iostream>
#include <vector>
#include <iterator>
#include <limits>
#include <algorithm>

#include "data_struct.hpp"
#include "comporator.hpp"

int main()
{
  using IstremIter = std::istream_iterator< andriuschin::DataStruct >;
  using OstreamIter = std::ostream_iterator< andriuschin::DataStruct >;
  std::vector< andriuschin::DataStruct > vector;

  while (!std::cin.eof())
  {
    std::copy(IstremIter(std::cin), IstremIter(), std::back_inserter(vector));
    if (std::cin.fail() && !std::cin.eof())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::sort(vector.begin(), vector.end(), andriuschin::Comporator());
  std::copy(vector.begin(), vector.end(), OstreamIter(std::cout, "\n"));
}
