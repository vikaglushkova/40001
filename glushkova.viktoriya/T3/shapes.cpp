#include "shapes.hpp"
#include "input_parser.hpp"
#include <algorithm>
#include <cmath>
#include <tuple>

std::istream& shapes::operator>>(std::istream& in, Point& point)
{
    std::istream::sentry guard(in);
    if (!guard) return in;
    using del = Delimiter;
    Point temp{0, 0};
    in >> del{'('} >> temp.x >> del{';'} >> temp.y >> del{')'};
    if (in) point = temp;
    return in;
}

std::istream& shapes::operator>>(std::istream& in, Polygon& poly)
{
    std::istream::sentry guard(in);
    if (!guard) return in;

    size_t vertexes = 0;
    if (!(in >> vertexes)) return in;
    if (vertexes < 3)
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::vector<Point> temp;
    for (size_t i = 0; i < vertexes; ++i)
    {
        Point p;
        in >> p;
        if (!in)
        {
            in.setstate(std::ios::failbit);
            return in;
        }
        temp.push_back(p);
    }

    if (in && temp.size() == vertexes)
    {
        poly.points = std::move(temp);
    }
    else
    {
        in.setstate(std::ios::failbit);
    }

    return in;
}

bool shapes::Point::operator==(const Point& other) const
{
    return (x == other.x) && (y == other.y);
}

bool shapes::Point::operator!=(const Point& other) const
{
    return !(*this == other);
}

bool shapes::Point::operator<(const Point& other) const
{
    return std::tie(x, y) < std::tie(other.x, other.y);
}

bool shapes::Polygon::operator==(const Polygon& other) const
{
    if (points.size() != other.points.size()) return false;
    for (size_t i = 0; i < points.size(); ++i)
    {
        if (points[i] != other.points[i]) return false;
    }
    return true;
}

bool shapes::Polygon::operator!=(const Polygon& other) const
{
    return !(*this == other);
}

double shapes::calcArea(const Polygon& poly)
{
    double area = 0.0;
    size_t n = poly.points.size();
    for (size_t i = 0; i < n; ++i)
    {
        const Point& p1 = poly.points[i];
        const Point& p2 = poly.points[(i + 1) % n];
        area += (p1.x * p2.y - p2.x * p1.y);
    }
    return std::abs(area) / 2.0;
}

bool shapes::arePolygonsSame(const Polygon& poly1, const Polygon& poly2)
{
    if (poly1.points.size() != poly2.points.size()) return false;

    if (poly1 == poly2) return true;

    size_t n = poly1.points.size();
    for (size_t shift = 0; shift < n; ++shift) {
        bool match = true;
        for (size_t i = 0; i < n; ++i) {
            if (poly1.points[i] != poly2.points[(i + shift) % n]) {
                match = false;
                break;
            }
        }
        if (match) return true;
    }

    for (size_t shift = 0; shift < n; ++shift) {
        bool match = true;
        for (size_t i = 0; i < n; ++i) {
            if (poly1.points[i] != poly2.points[(n - 1 - i + shift) % n]) {
                match = false;
                break;
            }
        }
        if (match) return true;
    }

    return false;
}
