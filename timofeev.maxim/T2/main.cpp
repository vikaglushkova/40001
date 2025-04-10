#include <iostream>
#include "namespace.hpp"

int main()
{
  std::vector < timofeev::DataStruct > data;
  while(!std::cin.eof())
    if (std::cin)
    {
      std::copy(
        std::istream_iterator< timofeev::DataStruct >(std::cin),
        std::istream_iterator< timofeev::DataStruct >(),
        std::back_inserter(data)
      );
    }
    else
    {
      std::cin.clear();
    }

  std::sort(data.begin(), data.end(), timofeev::dataStructCompare);

  std::copy(
    std::begin(data),
    std::end(data),
    std::ostream_iterator< timofeev::DataStruct >(std::cout, "\n")
  );

  return 0;
}
