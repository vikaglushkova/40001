#include "polygon.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <functional>
#include <numeric>
#include <limits>

#include <demand.hpp>
#include <format_guard.hpp>

namespace andriuschin
{
  using namespace std::placeholders;
  static auto getX = std::bind(std::mem_fn(&Point::x), _1);
  static auto getY = std::bind(std::mem_fn(&Point::y), _1);
}

std::istream& andriuschin::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  FormatGuard guard(in);
  int x, y;
  if (in >> Demand{'('} >> x >> std::noskipws >> Demand{';'} >> y >> Demand{')'})
  {
    point.x = x;
    point.y = y;
    return in;
  }
  in.setstate(std::ios::failbit);
  return in;
}
std::istream& andriuschin::operator>>(std::istream& in, Polygon& poly)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  FormatGuard guard(in);
  size_t count;
  if (((in >> std::ws).peek() != '-') && (in >> count) && (count >= 3))
  {
    if (in >> std::ws)
    {
      std::vector< Point > temp;
      std::copy_n(std::istream_iterator< Point >(in), count, std::back_inserter(temp));
      if ((temp.size() == count))
      {
        poly.points = std::move(temp);
        return in;
      }
    }
  }
  in.setstate(std::ios::failbit);
  return in;
}

double andriuschin::GetTriangleArea::operator()(const Point& point)
{
  using namespace std::placeholders;
  if (size < 2)
  {
    buf[size++] = point;
    return 0;
  }
  static auto getXDist = std::bind(std::minus<>(), std::bind(getX, _1), std::bind(getX, _2));
  static auto getYDist = std::bind(std::minus<>(), std::bind(getY, _1), std::bind(getY, _2));
  static auto sqr = std::bind(std::multiplies<>(), _1, _1);
  static auto getDistSqr = std::bind(std::plus<>(), std::bind(sqr, getXDist), std::bind(sqr, getYDist));
  static double(*sqrt)(double) = std::sqrt;
  static auto getDist = std::bind(sqrt, getDistSqr);
  static auto getPerimetr = std::bind(std::plus<>(), std::bind(getDist, _1, _2), std::bind(std::plus<>(),
          std::bind(getDist, _1, _3),
          std::bind(getDist, _2, _3)));
  static auto getSemiPerimetr = std::bind(std::divides<>(), getPerimetr, 2);
  static auto getPMinusSide = std::bind(std::minus<>(), getSemiPerimetr, std::bind(getDist, _1, _2));
  static auto getMultiply = std::bind(std::multiplies<>(), std::bind(getPMinusSide, _1, _2, _3),
        std::bind(std::multiplies<>(), std::bind(getPMinusSide, _2, _3, _1), std::bind(getPMinusSide, _3, _1, _2)));
  static auto getSqrArea = std::bind(std::multiplies<>(), getSemiPerimetr, getMultiply);
  static auto getArea = std::bind(sqrt, getSqrArea);
  Point a = buf[1];
  buf[1] = point;
  return getArea(a, buf[0], point);
}
double andriuschin::GetArea::operator()(const Polygon& p)
{
  using namespace std::placeholders;
  return std::accumulate(p.points.begin(), p.points.end(), 0.0,
      std::bind(std::plus<>(), _1, std::bind(andriuschin::GetTriangleArea{}, _2)));
}
bool andriuschin::GetIntersections::operator()(const Polygon& lhs, const Polygon& rhs)
{
  const auto lessX = std::bind(std::less<>(), std::bind(getX, _1), std::bind(getX, _2));
  const auto lessY = std::bind(std::less<>(), std::bind(getY, _1), std::bind(getY, _2));
  const auto comp = std::bind(std::logical_and<>(), std::bind(lessX, _1, _2), std::bind(lessY, _1, _2));
  const auto minMaxLhs = std::minmax_element(lhs.points.cbegin(), lhs.points.cend(), comp);
  const auto minMaxRhs = std::minmax_element(rhs.points.cbegin(), rhs.points.cend(), comp);
  const auto& maxLhs = *(minMaxLhs.second);
  const auto& minLhs = *(minMaxLhs.first);
  const auto& maxRhs = *(minMaxRhs.second);
  const auto& minRhs = *(minMaxRhs.first);

  return !(comp(maxRhs, minLhs) || comp(maxLhs, minRhs));
}
