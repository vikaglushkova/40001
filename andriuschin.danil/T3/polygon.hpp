#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include <iosfwd>

namespace andriuschin
{
  struct Point
  {
    int x, y;
  };
  std::istream& operator>>(std::istream& in, Point& poly);
  struct Polygon
  {
    std::vector< Point > points;
  };
  std::istream& operator>>(std::istream& in, Polygon& poly);
  std::ostream& operator<<(std::ostream& out, const Polygon& poly);
  struct AreaCalc
  {
    double operator()(const Point& point);
    Point buf[2];
    size_t size = 0;
  };
  struct GetArea
  {
    double operator()(const andriuschin::Polygon& p);
  };
}

#endif
