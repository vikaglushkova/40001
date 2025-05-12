#include "polygon.hpp"

#include <iostream>
#include <functional>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iterator>
#include <limits>

#include <demand.hpp>
#include <format_guard.hpp>

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
      if (temp.size() == count)
      {
        poly.points = std::move(temp);
        return in;
      }
    }
  }
  in.setstate(std::ios::failbit);
  return in;
}

std::ostream& andriuschin::operator<<(std::ostream& out, const andriuschin::Polygon& poly)
{
  out << poly.points.size() << ": ";
  for (auto i: poly.points)
  {
    out << '(' << i.x << ' ' << i.y << ") ";
  }
  return out;
}

double andriuschin::AreaCalc::operator()(const Point& point)
{
  using namespace std::placeholders;
  if (size < 2)
  {
    buf[size++] = point;
    return 0;
  }
  auto getX = std::bind(std::mem_fn(&Point::x), _1);
  auto getY = std::bind(std::mem_fn(&Point::y), _1);
  auto getXDist = std::bind(std::minus<>(), std::bind(getX, _1), std::bind(getX, _2));
  auto getYDist = std::bind(std::minus<>(), std::bind(getY, _1), std::bind(getY, _2));
  auto sqr = std::bind(std::multiplies<>(), _1, _1);
  auto getDistSqr = std::bind(std::plus<>(), std::bind(sqr, getXDist), std::bind(sqr, getYDist));
  double(*sqrt)(double) = std::sqrt;
  auto getDist = std::bind(sqrt, getDistSqr);
  auto getPerimetr = std::bind(std::plus<>(), std::bind(getDist, _1, _2), std::bind(std::plus<>(),
          std::bind(getDist, _1, _3),
          std::bind(getDist, _2, _3)));
  auto getSemiPerimetr = std::bind(std::divides<>(), getPerimetr, 2);
  auto getPMinusSide = std::bind(std::minus<>(), getSemiPerimetr, std::bind(getDist, _1, _2));
  auto getMultiply = std::bind(std::multiplies<>(), std::bind(getPMinusSide, _1, _2, _3),
        std::bind(std::multiplies<>(), std::bind(getPMinusSide, _2, _3, _1), std::bind(getPMinusSide, _3, _1, _2)));
  auto getSqrArea = std::bind(std::multiplies<>(), getSemiPerimetr, getMultiply);
  auto getArea = std::bind(sqrt, getSqrArea);
  Point a = buf[1];
  buf[1] = point;
  return getArea(a, buf[0], point);
}
double andriuschin::GetArea::operator()(const Polygon& p)
{
  using namespace std::placeholders;
  return std::accumulate(p.points.begin(), p.points.end(), 0.0,
      std::bind(std::plus<>(), _1, std::bind(andriuschin::AreaCalc{}, _2)));
}
