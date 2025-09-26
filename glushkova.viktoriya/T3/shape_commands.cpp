#include "shape_commands.hpp"
#include <functional>
#include <map>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <string>
#include <iterator>
#include <sstream>
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
            if (poly.points.size() < 3) return false;

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
            throw std::logic_error("<INVALID COMMAND>");
        }
        double total = std::accumulate(shapes.begin(), shapes.end(), 0.0,
            [](double sum, const Polygon& poly) {
                return sum + computeArea(poly);
            });
        result = total / shapes.size();
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
        throw std::logic_error("<INVALID COMMAND>");
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
        throw std::logic_error("<INVALID COMMAND>");
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

    if (shapes.empty())
    {
        out << "0\n";
        return;
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
