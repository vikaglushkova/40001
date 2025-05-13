#include "processor.hpp"

#include <fstream>
#include <iterator>
#include <functional>
#include <algorithm>
#include <numeric>
#include <limits>
#include "parser.hpp"

namespace andriuschin
{
  using namespace std::placeholders;
  static auto getData = std::bind(std::mem_fn(&Polygon::points), _1);
  static auto getDataSize = std::bind(&decltype(Polygon::points)::size, getData);
  static auto isEven = std::bind(std::equal_to<>(), std::bind(std::modulus<>(), getDataSize, 2), 0);
  static auto isOdd = std::bind(std::logical_not<>(), isEven);
  static auto getArea = std::bind(GetArea(), _1);
}

bool andriuschin::Context::eol()
{
  while (input.peek() != '\n')
  {
    char c = '\0';
    input.get(c);
    if (!std::isspace(c))
    {
      return false;
    }
  }
  return true;
}

bool andriuschin::MainProcessor::init(Context& context, int argc, char** argv)
{
  if (argc != 2)
  {
    context.error << "No file\n";
    return false;
  }
  std::ifstream file(argv[1]);
  if (!file)
  {
    context.error << "File wasn't found\n";
    return false;
  }
  using iterator = std::istream_iterator< Polygon >;
  while (!file.eof())
  {
    std::copy(iterator(file), iterator(), std::back_inserter(context.polygons));
    if (file.fail() && !file.eof())
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return true;
}

bool andriuschin::MainProcessor::area(Context& context)
{
  size_t num = 0;
  if (((context.input >> std::ws).peek() != '-') && (context.input >> num) && (num >= 3))
  {
    return AreaProccessor().count(context, num);
  }
  context.input.clear(context.input.rdstate() & ~std::ios::failbit);
  Parser< AreaProccessor >::map_type comands = {
    {"EVEN", &AreaProccessor::even},
    {"ODD", &AreaProccessor::odd},
    {"MEAN", &AreaProccessor::mean}
  };
  Parser< AreaProccessor > parser({}, context, std::move(comands));
  return parser.run();
}
bool andriuschin::AreaProccessor::count(Context& context, size_t num)
{
  if (context.polygons.empty() || !context.eol())
  {
    return false;
  }
  using namespace std::placeholders;

  auto isExpectedSize = std::bind(std::equal_to<>(), getDataSize, num);
  auto workspaceEnd = std::partition(context.polygons.begin(), context.polygons.end(), isExpectedSize);
  auto area = std::accumulate(context.polygons.begin(), workspaceEnd, 0.0, std::bind(std::plus<>{}, _1,
        std::bind(GetArea{}, _2)));
  context.output << area << '\n';
  return true;
}
bool andriuschin::AreaProccessor::even(Context& context)
{
  if (!context.eol())
  {
    return false;
  }
  using namespace std::placeholders;
  const auto workspaceEnd = std::partition(context.polygons.begin(), context.polygons.end(), isEven);
  const auto area = std::accumulate(context.polygons.begin(), workspaceEnd, 0.0, std::bind(std::plus<>{}, _1,
        std::bind(GetArea{}, _2)));
  context.output << area << '\n';
  return true;
}
bool andriuschin::AreaProccessor::odd(Context& context)
{
  if (!context.eol())
  {
    return false;
  }
  using namespace std::placeholders;
  const auto workspaceEnd = std::partition(context.polygons.begin(), context.polygons.end(), isOdd);
  const auto area = std::accumulate(context.polygons.begin(), workspaceEnd, 0.0, std::bind(std::plus<>{}, _1,
        std::bind(GetArea{}, _2)));
  context.output << area << '\n';
  return true;
}
bool andriuschin::AreaProccessor::mean(Context& context)
{
  if (context.polygons.empty() || !context.eol())
  {
    return false;
  }
  using namespace std::placeholders;
  const auto area = std::accumulate(context.polygons.begin(), context.polygons.end(), 0.0, std::bind(std::plus<>{}, _1,
        std::bind(GetArea{}, _2)));
  context.output << area / context.polygons.size() << '\n';
  return true;
}

bool andriuschin::MainProcessor::min(Context& context)
{
  static Parser< MinMaxProcessor< false > >::map_type comands = {
    {"AREA", &MinMaxProcessor< false >::area},
    {"VERTEXES", &MinMaxProcessor< false >::vertexes}
  };
  static Parser< MinMaxProcessor< false > > parser({}, context, std::move(comands));
  return parser.run();
}
bool andriuschin::MainProcessor::max(Context& context)
{
  static Parser< MinMaxProcessor< true > >::map_type comands = {
    {"AREA", &MinMaxProcessor< true >::area},
    {"VERTEXES", &MinMaxProcessor< true >::vertexes}
  };
  static Parser< MinMaxProcessor< true > > parser({}, context, std::move(comands));
  return parser.run();
}

bool andriuschin::MainProcessor::count(Context& context)
{
  size_t num = 0;
  if (((context.input >> std::ws).peek() != '-') && (context.input >> num))
  {
    if ((num >= 3))
    {
      return CountProcessor().vertexes(context, num);
    }
    return false;
  }
  context.input.clear(context.input.rdstate() & ~std::ios::failbit);
  Parser< CountProcessor >::map_type comands = {
    {"EVEN", &CountProcessor::even},
    {"ODD", &CountProcessor::odd}
  };
  static Parser< CountProcessor > parser({}, context, std::move(comands));
  return parser.run();
}
bool andriuschin::CountProcessor::vertexes(Context& context, size_t num)
{
  if (!context.eol())
  {
    return false;
  }
  const auto isExpectedSize = std::bind(std::equal_to<>(), getDataSize, num);
  const auto count = std::count_if(context.polygons.begin(), context.polygons.end(), isExpectedSize);
  context.output << count << '\n';
  return true;
}
bool andriuschin::CountProcessor::even(Context& context)
{
  if (!context.eol())
  {
    return false;
  }
  const auto count = std::count_if(context.polygons.begin(), context.polygons.end(), isEven);
  context.output << count << '\n';
  return true;
}
bool andriuschin::CountProcessor::odd(Context& context)
{
  if (!context.eol())
  {
    return false;
  }
  const auto count = std::count_if(context.polygons.begin(), context.polygons.end(), isOdd);
  context.output << count << '\n';
  return true;
}
bool andriuschin::MainProcessor::lessArea(Context& context)
{
  Polygon poly;
  if (!(context.input >> poly) || !context.eol())
  {
    return false;
  }
  const auto count = std::count_if(context.polygons.begin(), context.polygons.end(), std::bind(
        std::less<>(), getArea, GetArea()(poly)));
  context.output << count << '\n';
  return true;
}
bool andriuschin::MainProcessor::intersections(Context& context)
{
  Polygon poly;
  if (!(context.input >> poly) || !context.eol())
  {
    return false;
  }
  using namespace std::placeholders;
  const auto intersect = std::bind(GetIntersections(), std::cref(poly), _1);

  context.output << std::count_if(context.polygons.begin(), context.polygons.end(), intersect) << '\n';
  return true;
}
