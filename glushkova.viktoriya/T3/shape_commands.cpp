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

        std::vector<size_t> indices(poly.points.size());
        std::iota(indices.begin(), indices.end(), 0);

        auto areaAccumulator = [&poly](double sum, size_t i) {
            size_t j = (i + 1) % poly.points.size();
            return sum + poly.points[i].x * poly.points[j].y - poly.points[j].x * poly.points[i].y;
        };

        double sum = std::accumulate(indices.begin(), indices.end(), 0.0, areaAccumulator);
        return 0.5 * std::abs(sum);
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

            return std::equal(
                poly.points.begin(), poly.points.end(),
                target.points.begin(),
                [dx, dy](const Point& p, const Point& t) {
                    return p.x == t.x + dx && p.y == t.y + dy;
                }
            );
        }
    };
}

void shapes::doArea(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
{
    std::string arg;
    in >> arg;

    std::map<std::string, std::function<double()>> handlers;
    double result = 0.0;

    handlers["EVEN"] = [&shapes]() {
        EvenVertexPredicate pred;
        std::vector<Polygon> filtered;
        std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(filtered), pred);
        return std::accumulate(filtered.begin(), filtered.end(), 0.0, AreaCalculator{});
    };

    handlers["ODD"] = [&shapes]() {
        OddVertexPredicate pred;
        std::vector<Polygon> filtered;
        std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(filtered), pred);
        return std::accumulate(filtered.begin(), filtered.end(), 0.0, AreaCalculator{});
    };

    handlers["MEAN"] = [&shapes]() {
        if (shapes.empty()) throw std::logic_error("<EMPTY POLYGONS>");
        return std::accumulate(shapes.begin(), shapes.end(), 0.0, AreaCalculator{}) / shapes.size();
    };

    auto it = handlers.find(arg);
    if (it != handlers.end()) {
        result = it->second();
    } else {
        try {
            size_t num = std::stoul(arg);
            if (num < 3) throw std::invalid_argument("<INVALID VERTEX COUNT>");

            VertexCountPredicate pred(num);
            std::vector<Polygon> filtered;
            std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(filtered), pred);
            result = std::accumulate(filtered.begin(), filtered.end(), 0.0, AreaCalculator{});
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("<INVALID ARGUMENT>");
        }
    }

    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1) << result << '\n';
}

void shapes::doMax(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
{
    std::string arg;
    in >> arg;

    if (shapes.empty()) throw std::logic_error("<EMPTY POLYGONS>");

    if (arg == "AREA") {
        AreaCompare comp;
        auto it = std::max_element(shapes.begin(), shapes.end(), comp);
        StreamGuard guard(out);
        out << std::fixed << std::setprecision(1) << computeArea(*it) << '\n';
    } else if (arg == "VERTEXES") {
        VertexCountCompare comp;
        auto it = std::max_element(shapes.begin(), shapes.end(), comp);
        out << it->points.size() << '\n';
    } else {
        throw std::invalid_argument("<INVALID ARGUMENT>");
    }
}

void shapes::doMin(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
{
    std::string arg;
    in >> arg;

    if (shapes.empty()) throw std::logic_error("<EMPTY POLYGONS>");

    if (arg == "AREA") {
        AreaCompare comp;
        auto it = std::min_element(shapes.begin(), shapes.end(), comp);
        StreamGuard guard(out);
        out << std::fixed << std::setprecision(1) << computeArea(*it) << '\n';
    } else if (arg == "VERTEXES") {
        VertexCountCompare comp;
        auto it = std::min_element(shapes.begin(), shapes.end(), comp);
        out << it->points.size() << '\n';
    } else {
        throw std::invalid_argument("<INVALID ARGUMENT>");
    }
}

void shapes::doCount(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
{
    std::string arg;
    in >> arg;

    size_t result = 0;

    if (arg == "EVEN") {
        EvenVertexPredicate pred;
        result = std::count_if(shapes.begin(), shapes.end(), pred);
    } else if (arg == "ODD") {
        OddVertexPredicate pred;
        result = std::count_if(shapes.begin(), shapes.end(), pred);
    } else {
        try {
            size_t num = std::stoul(arg);
            if (num < 3) throw std::invalid_argument("<INVALID VERTEX COUNT>");

            VertexCountPredicate pred(num);
            result = std::count_if(shapes.begin(), shapes.end(), pred);
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("<INVALID ARGUMENT>");
        }
    }

    out << result << '\n';
}

void shapes::doRmecho(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
{
    Polygon target;
    if (!(in >> target)) throw std::invalid_argument("<INVALID POLYGON>");

    struct IsTargetAndPrevious
    {
        const Polygon& target;
        mutable bool previousWasTarget = false;

        bool operator()(const Polygon& poly) const {
            if (poly == target) {
                if (previousWasTarget) return true;
                previousWasTarget = true;
                return false;
            }
            previousWasTarget = false;
            return false;
        }
    };

    auto pred = IsTargetAndPrevious{target};
    auto it = std::remove_if(shapes.begin(), shapes.end(), pred);
    size_t removedCount = std::distance(it, shapes.end());
    shapes.erase(it, shapes.end());
    out << removedCount << '\n';
}

void shapes::doSame(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
{
    Polygon target;
    if (!(in >> target)) throw std::invalid_argument("<INVALID POLYGON>");

    SameChecker checker(target);
    size_t count = std::count_if(shapes.begin(), shapes.end(), checker);
    out << count << '\n';
}

