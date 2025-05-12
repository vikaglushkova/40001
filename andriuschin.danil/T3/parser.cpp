#include "parser.hpp"

#include <iostream>
#include <utility>
#include <algorithm>
#include <functional>
#include <numeric>
#include <vector>
#include <stdexcept>
#include <iterator>
#include <fstream>

andriuschin::Parser::Parser(std::istream& in, std::ostream& out):
  in_(in),
  out_(out),
  polygons_()
{}

void andriuschin::Parser::init(int argc, char** argv)
{
  std::ifstream file(argv[1]);
  if (!file)
  {
    throw std::logic_error("");
  }
  std::copy(std::istream_iterator< Polygon >(file), std::istream_iterator< Polygon >(), std::back_inserter(polygons_));
}

void andriuschin::Parser::area()
{
  size_t num = 0;
  if (((in_ >> std::ws).peek() != '-') && (in_ >> num) && (num >= 3))
  {
    if (!eol())
    {
      throw std::logic_error("");
    }
    using namespace std::placeholders;
    auto getData = std::bind(std::mem_fn(&Polygon::points), _1);
    auto getDataSize = std::bind(&decltype(Polygon::points)::size, getData);
    auto isExpectedSize = std::bind(std::equal_to<>(), getDataSize, num);

    auto workspaceEnd = std::partition(polygons_.begin(), polygons_.end(), isExpectedSize);

    auto area = std::accumulate(polygons_.begin(), workspaceEnd, 0.0, std::bind(std::plus<>{}, _1,
          std::bind(GetArea{}, _2)));
    out_ << area << '\n';
    return;
  }

  in_.clear(in_.rdstate() & ~std::ios::failbit);
  std::string subcomand;
  if (in_ >> subcomand)
  {
    using namespace std::placeholders;
    if (!eol())
    {
      throw std::logic_error("");
    }
    if (subcomand == "EVEN")
    {
      auto getData = std::bind(std::mem_fn(&Polygon::points), _1);
      auto getDataSize = std::bind(&decltype(Polygon::points)::size, getData);
      auto isEven = std::bind(std::equal_to<>(), std::bind(std::modulus<>(), getDataSize, 2), 0);

      auto workspaceEnd = std::partition(polygons_.begin(), polygons_.end(), isEven);
      auto area = std::accumulate(polygons_.begin(), workspaceEnd, 0.0, std::bind(std::plus<>{}, _1,
            std::bind(GetArea{}, _2)));
      out_ << area << '\n';
    }
    else if (subcomand == "ODD")
    {
      auto getData = std::bind(std::mem_fn(&Polygon::points), _1);
      auto getDataSize = std::bind(&decltype(Polygon::points)::size, getData);
      auto isEven = std::bind(std::equal_to<>(), std::bind(std::modulus<>(), getDataSize, 2), 1);

      auto workspaceEnd = std::partition(polygons_.begin(), polygons_.end(), isEven);
      auto area = std::accumulate(polygons_.begin(), workspaceEnd, 0.0, std::bind(std::plus<>{}, _1,
            std::bind(GetArea{}, _2)));
      out_ << area << '\n';
    }
    else if (subcomand == "MEAN")
    {
      auto area = std::accumulate(polygons_.begin(), polygons_.end(), 0.0, std::bind(std::plus<>{}, _1,
            std::bind(GetArea{}, _2)));
      out_ << area / polygons_.size() << '\n';
    }
    else
    {
      throw std::logic_error("");
    }
    return;
  }
  throw std::logic_error("");
}

void andriuschin::Parser::max()
{
  using namespace std::placeholders;
  std::string subcomand;
  if (in_ >> subcomand)
  {
    if (!eol())
    {

    }
    if (subcomand == "AREA")
    {
      auto max = std::max_element(polygons_.begin(), polygons_.end(), std::bind(std::less<>(),
            std::bind(GetArea(), _1), std::bind(GetArea(), _2)));
      out_ << GetArea()(*max) << '\n';
      return;
    }
    else if (subcomand == "VERTEXES")
    {
      auto getData = std::bind(std::mem_fn(&Polygon::points), _1);
      auto getDataSize = std::bind(&decltype(Polygon::points)::size, getData);
      auto max = std::max_element(polygons_.begin(), polygons_.end(), std::bind(std::less<>(),
            std::bind(getDataSize, _1), std::bind(getDataSize, _2)));
      out_ << max->points.size() << '\n';
      return;
    }
  }
  throw std::logic_error("");
}
void andriuschin::Parser::min()
{
  using namespace std::placeholders;
  std::string subcomand;
  if (in_ >> subcomand)
  {
    if (!eol())
    {

    }
    if (subcomand == "AREA")
    {
      auto min = std::min_element(polygons_.begin(), polygons_.end(), std::bind(std::less<>(),
            std::bind(GetArea(), _1), std::bind(GetArea(), _2)));
      out_ << GetArea()(*min) << '\n';
      return;
    }
    else if (subcomand == "VERTEXES")
    {
      auto getData = std::bind(std::mem_fn(&Polygon::points), _1);
      auto getDataSize = std::bind(&decltype(Polygon::points)::size, getData);
      auto min = std::min_element(polygons_.begin(), polygons_.end(), std::bind(std::less<>(),
            std::bind(getDataSize, _1), std::bind(getDataSize, _2)));
      out_ << min->points.size() << '\n';
      return;
    }
  }
  throw std::logic_error("");
}
void andriuschin::Parser::count()
{
  out_ << "COUNT\n";
}
void andriuschin::Parser::lessArea()
{
out_ << "LESSAREA\n";
}
void andriuschin::Parser::intersections()
{
  out_ << "INTERSECTIONS\n";
}

bool andriuschin::Parser::eol()
{
  while (in_.peek() != '\n')
  {
    char c = '\0';
    in_.get(c);
    if (!std::isspace(c))
    {
      return false;
    }
  }
  return true;
}
