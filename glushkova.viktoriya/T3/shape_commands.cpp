#include "shape_commands.hpp"
#include <functional>
#include <map>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <string>
#include <iterator>
#include <sstream>
#include <cmath>
#include "stream_guard.hpp"

namespace
{
    using namespace shapes;

    double computeArea(const Polygon& poly)
    {
        if (poly.points.size() < 3) return 0.0;

        double area = 0.0;
        size_t n = poly.points.size();

        for (size_t i = 0; i < n; ++i)
        {
            size_t j = (i + 1) % n;
            area += poly.points[i].x * poly.points[j].y;
            area -= poly.points[j].x * poly.points[i].y;
        }

        return 0.5 * std::abs(area);
    }

    struct SameChecker
    {
        Polygon target;
        SameChecker(const Polygon& t) : target(t) {}

        bool operator()(const Polygon& poly) const
        {
            if (poly.points.size() != target.points.size()) return false;

            int dx = poly.points[0].x - target.points[0].x;
            int dy = poly.points[0].y - target.points[0].y;

            for (size_t i = 1; i < poly.points.size(); ++i)
            {
                int current_dx = poly.points[i].x - target.points[i].x;
                int current_dy = poly.points[i].y - target.points[i].y;

                if (current_dx != dx || current_dy != dy)
                {
                    return false;
                }
            }
            return true;
        }
    };

    bool isRectangle(const Polygon& poly)
    {
        if (poly.points.size() != 4) return false;

        std::vector<int> dots;
        for (size_t i = 0; i < 4; ++i) {
            size_t prev = (i + 3) % 4;
            size_t next = (i + 1) % 4;

            int dx1 = poly.points[i].x - poly.points[prev].x;
            int dy1 = poly.points[i].y - poly.points[prev].y;
            int dx2 = poly.points[next].x - poly.points[i].x;
            int dy2 = poly.points[next].y - poly.points[i].y;

            int dot = dx1 * dx2 + dy1 * dy2;
            dots.push_back(dot);
        }

        return std::all_of(dots.begin(), dots.end(), [](int dot) { return dot == 0; });
    }

    bool hasRightAngle(const Polygon& poly)
    {
        size_t n = poly.points.size();
        for (size_t i = 0; i < n; ++i) {
            size_t prev = (i + n - 1) % n;
            size_t next = (i + 1) % n;

            int dx1 = poly.points[i].x - poly.points[prev].x;
            int dy1 = poly.points[i].y - poly.points[prev].y;
            int dx2 = poly.points[next].x - poly.points[i].x;
            int dy2 = poly.points[next].y - poly.points[i].y;

            int dot = dx1 * dx2 + dy1 * dy2;
            if (dot == 0) return true;
        }
        return false;
    }

    bool isPermutation(const Polygon& a, const Polygon& b)
    {
        if (a.points.size() != b.points.size()) return false;

        std::vector<Point> a_sorted = a.points;
        std::vector<Point> b_sorted = b.points;

        auto comp = [](const Point& p1, const Point& p2) {
            return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
        };

        std::sort(a_sorted.begin(), a_sorted.end(), comp);
        std::sort(b_sorted.begin(), b_sorted.end(), comp);

        return std::equal(a_sorted.begin(), a_sorted.end(), b_sorted.begin());
    }
}

void shapes::doArea(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
{
    std::string arg;
    in >> arg;

    if (arg.empty())
    {
        throw std::invalid_argument("<INVALID COMMAND>");
    }

    double result = 0.0;

    if (arg == "EVEN")
    {
        result = std::accumulate(shapes.begin(), shapes.end(), 0.0,
            [](double sum, const Polygon& poly) {
                return (poly.points.size() % 2 == 0) ? sum + computeArea(poly) : sum;
            });
    }
    else if (arg == "ODD")
    {
        result = std::accumulate(shapes.begin(), shapes.end(), 0.0,
            [](double sum, const Polygon& poly) {
                return (poly.points.size() % 2 != 0) ? sum + computeArea(poly) : sum;
            });
    }
    else if (arg == "MEAN")
    {
        if (shapes.empty())
        {
            throw std::invalid_argument("<INVALID COMMAND>");
        }
        double total = std::accumulate(shapes.begin(), shapes.end(), 0.0,
            [](double sum, const Polygon& poly) {
                return sum + computeArea(poly);
            });
        result = total / static_cast<double>(shapes.size());
    }
    else
    {
        try
        {
            size_t num = std::stoul(arg);
            if (num < 3)
            {
                throw std::invalid_argument("<INVALID COMMAND>");
            }

            result = std::accumulate(shapes.begin(), shapes.end(), 0.0,
                [num](double sum, const Polygon& poly) {
                    return (poly.points.size() == num) ? sum + computeArea(poly) : sum;
                });
        }
        catch (const std::invalid_argument&)
        {
            throw std::invalid_argument("<INVALID COMMAND>");
        }
        catch (const std::out_of_range&)
        {
            throw std::invalid_argument("<INVALID COMMAND>");
        }
    }

    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
}

void shapes::doMax(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
{
    std::string arg;
    in >> arg;

    if (shapes.empty())
    {
        throw std::invalid_argument("<INVALID COMMAND>");
    }

    if (arg == "AREA")
    {
        auto it = std::max_element(shapes.begin(), shapes.end(),
            [](const Polygon& a, const Polygon& b) {
                return computeArea(a) < computeArea(b);
            });
        StreamGuard guard(out);
        out << std::fixed << std::setprecision(1) << computeArea(*it) << '\n';
    }
    else if (arg == "VERTEXES")
    {
        auto it = std::max_element(shapes.begin(), shapes.end(),
            [](const Polygon& a, const Polygon& b) {
                return a.points.size() < b.points.size();
            });
        out << it->points.size() << '\n';
    }
    else
    {
        throw std::invalid_argument("<INVALID COMMAND>");
    }
}

void shapes::doMin(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
{
    std::string arg;
    in >> arg;

    if (shapes.empty())
    {
        throw std::invalid_argument("<INVALID COMMAND>");
    }

    if (arg == "AREA")
    {
        auto it = std::min_element(shapes.begin(), shapes.end(),
            [](const Polygon& a, const Polygon& b) {
                return computeArea(a) < computeArea(b);
            });
        StreamGuard guard(out);
        out << std::fixed << std::setprecision(1) << computeArea(*it) << '\n';
    }
    else if (arg == "VERTEXES")
    {
        auto it = std::min_element(shapes.begin(), shapes.end(),
            [](const Polygon& a, const Polygon& b) {
                return a.points.size() < b.points.size();
            });
        out << it->points.size() << '\n';
    }
    else
    {
        throw std::invalid_argument("<INVALID COMMAND>");
    }
}

void shapes::doCount(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
{
    std::string arg;
    in >> arg;

    if (arg.empty())
    {
        throw std::invalid_argument("<INVALID COMMAND>");
    }

    size_t result = 0;

    if (arg == "EVEN")
    {
        result = std::count_if(shapes.begin(), shapes.end(),
            [](const Polygon& poly) {
                return poly.points.size() % 2 == 0;
            });
    }
    else if (arg == "ODD")
    {
        result = std::count_if(shapes.begin(), shapes.end(),
            [](const Polygon& poly) {
                return poly.points.size() % 2 != 0;
            });
    }
    else
    {
        try
        {
            size_t num = std::stoul(arg);
            if (num < 3)
            {
                throw std::invalid_argument("<INVALID COMMAND>");
            }

            result = std::count_if(shapes.begin(), shapes.end(),
                [num](const Polygon& poly) {
                    return poly.points.size() == num;
                });
        }
        catch (const std::invalid_argument&)
        {
            throw std::invalid_argument("<INVALID COMMAND>");
        }
        catch (const std::out_of_range&)
        {
            throw std::invalid_argument("<INVALID COMMAND>");
        }
    }

    out << result << '\n';
}

void shapes::doRmecho(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
{
    Polygon target;
    if (!(in >> target))
    {
        throw std::invalid_argument("<INVALID COMMAND>");
    }

    size_t removedCount = 0;
    auto it = shapes.begin();
    bool previousWasTarget = false;

    while (it != shapes.end())
    {
        if (*it == target)
        {
            if (previousWasTarget)
            {
                it = shapes.erase(it);
                ++removedCount;
            }
            else
            {
                previousWasTarget = true;
                ++it;
            }
        }
        else
        {
            previousWasTarget = false;
            ++it;
        }
    }

    out << removedCount << '\n';
}

void shapes::doSame(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
{
    in >> std::ws;

    Polygon target;
    if (!(in >> target) || target.points.size() < 3)
    {
        throw std::invalid_argument("<INVALID COMMAND>");
    }

    SameChecker checker(target);
    size_t count = std::count_if(shapes.begin(), shapes.end(), checker);
    out << count << '\n';
}

void shapes::doEcho(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
{
    Polygon target;
    if (!(in >> target))
    {
        throw std::invalid_argument("<INVALID COMMAND>");
    }

    size_t addedCount = 0;
    std::vector<Polygon> newShapes;

    for (const auto& poly : shapes)
    {
        newShapes.push_back(poly);
        if (poly == target)
        {
            newShapes.push_back(poly);
            ++addedCount;
        }
    }

    shapes = std::move(newShapes);
    out << addedCount << '\n';
}

void shapes::doInframe(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
{
    Polygon target;
    if (!(in >> target) || target.points.size() < 3)
    {
        throw std::invalid_argument("<INVALID COMMAND>");
    }

    if (shapes.empty())
    {
        out << "<TRUE>\n";
        return;
    }

    int min_x = shapes[0].points[0].x, max_x = shapes[0].points[0].x;
    int min_y = shapes[0].points[0].y, max_y = shapes[0].points[0].y;

    for (const auto& poly : shapes)
    {
        for (const auto& point : poly.points)
        {
            min_x = std::min(min_x, point.x);
            max_x = std::max(max_x, point.x);
            min_y = std::min(min_y, point.y);
            max_y = std::max(max_y, point.y);
        }
    }

    bool inside = true;
    for (const auto& point : target.points)
    {
        if (point.x < min_x || point.x > max_x || point.y < min_y || point.y > max_y)
        {
            inside = false;
            break;
        }
    }

    out << (inside ? "<TRUE>" : "<FALSE>") << '\n';
}

void shapes::doIntersections(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
{
    Polygon target;
    if (!(in >> target) || target.points.size() < 3)
    {
        throw std::invalid_argument("<INVALID COMMAND>");
    }

    size_t count = 0;
    for (const auto& poly : shapes)
    {
        bool intersects = false;

        for (size_t i = 0; i < target.points.size() && !intersects; ++i)
        {
            for (size_t j = 0; j < poly.points.size() && !intersects; ++j)
            {
                if (target.points[i].x == poly.points[j].x && target.points[i].y == poly.points[j].y)
                {
                    intersects = true;
                    break;
                }
            }
            if (intersects) break;
        }

        if (intersects) ++count;
    }

    out << count << '\n';
}

void shapes::doMaxseq(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
{
    Polygon target;
    if (!(in >> target) || target.points.size() < 3)
    {
        throw std::invalid_argument("<INVALID COMMAND>");
    }

    size_t max_seq = 0;
    size_t current_seq = 0;

    for (const auto& poly : shapes)
    {
        if (poly == target)
        {
            ++current_seq;
            max_seq = std::max(max_seq, current_seq);
        }
        else
        {
            current_seq = 0;
        }
    }

    out << max_seq << '\n';
}

void shapes::doPerms(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
{
    Polygon target;
    if (!(in >> target) || target.points.size() < 3)
    {
        throw std::invalid_argument("<INVALID COMMAND>");
    }

    size_t count = std::count_if(shapes.begin(), shapes.end(),
        [&target](const Polygon& poly) {
            return isPermutation(poly, target);
        });

    out << count << '\n';
}

void shapes::doRects(std::vector<Polygon>& shapes, std::istream&, std::ostream& out)
{
    size_t count = std::count_if(shapes.begin(), shapes.end(), isRectangle);
    out << count << '\n';
}

void shapes::doRightshapes(std::vector<Polygon>& shapes, std::istream&, std::ostream& out)
{
    size_t count = std::count_if(shapes.begin(), shapes.end(), hasRightAngle);
    out << count << '\n';
}

void shapes::doLessarea(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
{
    Polygon target;
    if (!(in >> target) || target.points.size() < 3)
    {
        throw std::invalid_argument("<INVALID COMMAND>");
    }

    double target_area = computeArea(target);
    size_t count = 0;

    for (const auto& poly : shapes) {
        if (computeArea(poly) < target_area) {
            ++count;
        }
    }

    out << count << '\n';
}
