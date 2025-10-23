#include "shapes.hpp"
#include "input_parser.hpp"

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

    if (in && in.peek() != ' ' && in.peek() != '\n' && in.peek() != '\r' && in.peek() != EOF)
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
