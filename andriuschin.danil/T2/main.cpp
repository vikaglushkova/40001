#include <iostream>

#include "data_struct.hpp"

int main()
{
  char literal = 'l';
  if (std::cin >> pgm::CharLiteral(literal)) {
    std::cout << literal << '\n';
  }
  else {
    std::cout << "uwu\n";
  }
}
