#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <map>
#include <limits>
#include "polygon.hpp"
#include "parser.hpp"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "No file\n";
    return 1;
  }
  andriuschin::Parser parser(std::cin, std::cout);
  parser.init(argc, argv);
  std::map< std::string, void(andriuschin::Parser::*)() > comands = {
      {"AREA", &andriuschin::Parser::area},
      {"MAX", &andriuschin::Parser::max},
      {"MIN", &andriuschin::Parser::min},
      {"COUNT", &andriuschin::Parser::count},
      {"LESSAREA", &andriuschin::Parser::lessArea},
      {"INTERSECTIONS", &andriuschin::Parser::intersections}
  };
  std::string comand;
  while (!(std::cin >> comand).eof())
  {
    try
    {
      (parser.*(comands.at(comand)))();
    }
    catch(const std::logic_error&)
    {
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cerr << "<INVALID COMAND>\n";
    }
    catch (...)
    {
      std::cerr << "<INVALID COMAND>\n";
    }
  }
}
