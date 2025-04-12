#include <iostream>

#include "data_struct.hpp"

int main()
{
  std::string temp = "\"hello\"";
  if (std::cin >> andriuschin::StringLiteral(temp))
  {
    std::cout << andriuschin::StringLiteral(temp) << '\n';
  }
  else
  {
    std::cout << "UwU\n";
  }
}
