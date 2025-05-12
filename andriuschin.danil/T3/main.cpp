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
#include "processor.hpp"
#include "parser.hpp"

int main(int argc, char** argv)
{
  std::vector< andriuschin::Polygon > polygons;
  andriuschin::MainProcessor processor;
  andriuschin::Parser< andriuschin::MainProcessor >::map_type comands = {
    {"AREA", &andriuschin::MainProcessor::area},
    {"MAX", &andriuschin::MainProcessor::max},
    {"MIN", &andriuschin::MainProcessor::min},
    {"COUNT", &andriuschin::MainProcessor::count},
    {"LESSAREA", &andriuschin::MainProcessor::lessArea},
    {"INTERSECTIONS", &andriuschin::MainProcessor::intersections}
  };
  andriuschin::Parser< andriuschin::MainProcessor > parser({}, {std::cin, std::cout, std::cerr, polygons},
      std::move(comands));
  if (!processor.init(parser.context, argc, argv))
  {
    return 1;
  }
  while (parser.run())
  {}
}
