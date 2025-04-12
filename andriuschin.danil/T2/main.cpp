#include <iostream>

#include "data_struct.hpp"

int main()
{
  andriuschin::DataStruct dataStruct;
  if (std::cin >> dataStruct)
  {
    std::cout << dataStruct << '\n';
  }
  else
  {
    std::cout << "UwU\n";
  }
}
