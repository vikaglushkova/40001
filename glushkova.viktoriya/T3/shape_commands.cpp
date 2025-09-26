#include "shape_commands.hpp"
#include <functional>
#include <map>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <string>
#include <iterator>
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

    struct AreaCalculator
    {
        double operator()(double acc, const Polygon& poly) const
        {
            return acc + computeArea(poly);
        }
    };

    struct VertexCountPredicate
    {
        size_t count;
        VertexCountPredicate(size_t c) : count(c) {}
        bool operator()(const Polygon& poly) const
        {
            return poly.points.size() == count;
        }
    };

    struct EvenVertexPredicate
    {
        bool operator()(const Polygon& poly) const
        {
            return poly.points.size() % 2 == 0;
        }
    };

    struct OddVertexPredicate
    {
        bool operator()(const Polygon& poly) const
        {
            return poly.points.size() % 2 != 0;
        }
    };

    struct AreaCompare
    {
        bool operator()(const Polygon& a, const Polygon& b) const
        {
            return computeArea(a) < computeArea(b);
        }
    };

    struct VertexCountCompare
    {
        bool operator()(const Polygon& a, const Polygon& b) const
        {
            return a.points.size() < b.points.size();
        }
    };

    struct SameChecker
    {
        Polygon target;
        SameChecker(const Polygon& t) : target(t) {}

        bool operator()(const Polygon& poly) const
        {
            if (poly.points.size() != target.points.size()) return false;
            if (poly.points.empty()) return true;

            int dx = poly.points[0].x - target.points[0].x;
            int dy = poly.points[0].y - target.points[0].y;

            for (size_t i = 0; i < poly.points.size(); ++i)
            {
                if (poly.points[i].x != target.points[i].x + dx ||
                    poly.points[i].y != target.points[i].y + dy)
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
        EvenVertexPredicate pred;
        result = std::accumulate(shapes.begin(), shapes.end(), 0.0, 
            [&pred](double sum, const Polygon& poly) {
                return pred(poly) ? sum + computeArea(poly) : sum;
            });
    }
    else if (arg == "ODD")
    {
        OddVertexPredicate pred;
        result = std::accumulate(shapes.begin(), shapes.end(), 0.0,
            [&pred](double sum, const Polygon& poly) {
                return pred(poly) ? sum + computeArea(poly) : sum;
            });
    }
    else if (arg == "MEAN")
    {
        if (shapes.empty())
        {
            throw std::logic_error("<INVALID COMMAND>");
        }
        double total = std::accumulate(shapes.begin(), shapes.end(), 0.0, AreaCalculator{});
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

            VertexCountPredicate pred(num);
            result = std::accumulate(shapes.begin(), shapes.end(), 0.0,
                [&pred](double sum, const Polygon& poly) {
                    return pred(poly) ? sum + computeArea(poly) : sum;
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

    size_t result = 0;

    if (arg == "EVEN")
    {
        EvenVertexPredicate pred;
        result = std::count_if(shapes.begin(), shapes.end(), pred);
    }
    else if (arg == "ODD")
    {
        OddVertexPredicate pred;
        result = std::count_if(shapes.begin(), shapes.end(), pred);
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

            VertexCountPredicate pred(num);
            result = std::count_if(shapes.begin(), shapes.end(), pred);
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
    Polygon target;
    if (!(in >> target))
    {
        throw std::invalid_argument("<INVALID COMMAND>");
    }

    SameChecker checker(target);
    size_t count = std::count_if(shapes.begin(), shapes.end(), checker);
    out << count << '\n';
}
