#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <iosfwd>
#include <vector>

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
  struct GetArea
  {
    double operator()(const andriuschin::Polygon& p);
  };
  struct GetIntersections
  {
    bool operator()(const Polygon& poly, const Point& point);
  };
  namespace details
  {
    struct GetTriangleArea
    {
      double operator()(const Point& point);
      Point buf[2];
      size_t size = 0;
    };
  }
}

#endif
