#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <iostream>
#include <functional>
#include <algorithm>
#include "polygon.hpp"

namespace andriuschin
{
  struct Context
  {
    std::istream& input;
    std::ostream& output;
    std::ostream& error;
    std::vector< Polygon >& polygons;
    bool eol();
  };

  struct MainProcessor
  {
    bool init(Context& context, int argc, char** argv);
    bool area(Context& context);
    bool max(Context& context);
    bool min(Context& context);
    bool count(Context& context);
    bool lessArea(Context& context);
    bool intersections(Context& context);
  };

  struct AreaProccessor
  {
    bool count(Context& context, size_t num);
    bool odd(Context& context);
    bool even(Context& context);
    bool mean(Context& context);
  };

  template< bool isMax >
  struct MinMaxProcessor
  {
    bool area(Context& context);
    bool vertexes(Context& context);
  };

  struct CountProcessor
  {
    bool odd(Context& context);
    bool even(Context& context);
    bool vertexes(Context& context, size_t num);
  };
}

template< bool isMax >
bool andriuschin::MinMaxProcessor< isMax >::area(Context& context)
{
  if (!context.eol())
  {
    return false;
  }
  using namespace std::placeholders;
  auto minMax = std::max_element(context.polygons.begin(), context.polygons.end(),
      std::bind(std::conditional_t< isMax, std::less<>, std::greater<> >(), std::bind(GetArea(), _1),
        std::bind(GetArea(), _2)));
  context.output << GetArea()(*minMax) << '\n';
  return true;
}

template< bool isMax >
bool andriuschin::MinMaxProcessor< isMax >::vertexes(Context& context)
{
  if (!context.eol())
  {
    return false;
  }
  using namespace std::placeholders;
  auto getData = std::bind(std::mem_fn(&Polygon::points), _1);
  auto getDataSize = std::bind(&decltype(Polygon::points)::size, getData);
  auto minMax = std::max_element(context.polygons.begin(), context.polygons.end(),
      std::bind(std::conditional_t< isMax, std::less<>, std::greater<> >(), std::bind(getDataSize, _1),
        std::bind(getDataSize, _2)));
  context.output << minMax->points.size() << '\n';
  return true;
}

#endif
