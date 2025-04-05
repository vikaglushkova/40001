#include <iostream>

#include "data_struct.hpp"

int main()
{
  pgm::RationalLiteral::value_type temp = {0, 0};
  if (std::cin >> pgm::RationalLiteral(temp))
  {
    std::cout << pgm::RationalLiteral(temp) << '\n';
  }
  else
  {
    std::cout << "UwU\n";
  }
}
