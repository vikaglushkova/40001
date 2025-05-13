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
    std::vector< Point > temp;
    std::copy_n(std::istream_iterator< Point >(in), count, std::back_inserter(temp));
    if ((temp.size() == count))
    {
      poly.points = std::move(temp);
      return in;
    }
  }
  in.setstate(std::ios::failbit);
  return in;
}

double andriuschin::details::GetTriangulatedArea::operator()(const Point& point)
{
  using namespace std::placeholders;
  if (size < 2)
  {
    buf[size++] = point;
    return 0;
  }
  static auto getXDist = std::bind(std::minus<>{}, std::bind(getX, _1), std::bind(getX, _2));
  static auto getYDist = std::bind(std::minus<>{}, std::bind(getY, _1), std::bind(getY, _2));
  static auto sqr = std::bind(std::multiplies<>{}, _1, _1);
  static auto getDistSqr = std::bind(std::plus<>{}, std::bind(sqr, getXDist), std::bind(sqr, getYDist));
  static double(*sqrt)(double) = std::sqrt;
  static auto getDist = std::bind(sqrt, getDistSqr);
  static auto getPerimetr = std::bind(std::plus<>{}, std::bind(getDist, _1, _2),
      std::bind(std::plus<>{}, std::bind(getDist, _1, _3), std::bind(getDist, _2, _3)));
  static auto getSemiPerimetr = std::bind(std::divides<>{}, getPerimetr, 2);
  static auto getPMinusSide = std::bind(std::minus<>{}, getSemiPerimetr, std::bind(getDist, _1, _2));
  static auto getMultiply = std::bind(std::multiplies<>{}, std::bind(getPMinusSide, _1, _2, _3),
      std::bind(std::multiplies<>{}, std::bind(getPMinusSide, _2, _3, _1), std::bind(getPMinusSide, _3, _1, _2)));
  static auto getSqrArea = std::bind(std::multiplies<>{}, getSemiPerimetr, getMultiply);
  static auto getArea = std::bind(sqrt, getSqrArea);

  Point a = buf[1];
  buf[1] = point;
  return getArea(a, buf[0], point);
}
double andriuschin::GetArea::operator()(const Polygon& p)
{
  using namespace std::placeholders;
  return std::accumulate(p.points.begin(), p.points.end(), 0.0,
      std::bind(std::plus<>{}, _1, std::bind(details::GetTriangulatedArea{}, _2)));
}

size_t andriuschin::IsRightOnTheSameHeight::operator()(const Point& next)
{
  Point current = prev;
  prev = next;
  if (((current.y >= point.y) && (next.y <= point.y)) || ((current.y <= point.y) && (next.y >= point.y)))
  {
    double height = next.y - current.y;
    double width = next.x - current.x;
    double intersectX = point.x;
    if (height != 0)
    {
      intersectX = current.x + (point.y - current.y) / height * width;
    }
    if (point.x == intersectX)
    {
      return ON_SIDE;
    }
    return point.x > intersectX;
  }
  return 0;
}
bool andriuschin::IsInside::operator()(const Polygon& poly, const Point& point)
{
  size_t count = std::count_if(poly.points.begin(), poly.points.end(),
        IsRightOnTheSameHeight{poly.points.back(), point});
  return (count == 1) || (count >= IsRightOnTheSameHeight::ON_SIDE);
}
bool andriuschin::GetIntersections::operator()(const Polygon& lhs, const Polygon& rhs)
{
  return std::any_of(lhs.points.cbegin(), lhs.points.cend(), std::bind(IsInside{}, rhs, _1))
      || std::any_of(rhs.points.cbegin(), rhs.points.cend(), std::bind(IsInside{}, lhs, _1));
}
