#include "shape_commands.hpp"
#include <functional>
#include <map>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <cmath>
#include "stream_guard.hpp"

namespace shapes
{
    double calcArea(const Polygon& poly)
    {
        if (poly.points.size() < 3) return 0.0;

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

    bool isEven(const Polygon& poly)
    {
        return poly.points.size() % 2 == 0;
    }

    bool isOdd(const Polygon& poly)
    {
        return !isEven(poly);
    }

    void doArea(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        std::string subcommand;
        in >> subcommand;

        if (subcommand.empty())
        {
            out << "<INVALID COMMAND>\n";
            return;
        }

        double result = 0.0;

        if (subcommand == "EVEN")
        {
            for (const auto& p : poly)
                if (isEven(p)) result += calcArea(p);
        }
        else if (subcommand == "ODD")
        {
            for (const auto& p : poly)
                if (isOdd(p)) result += calcArea(p);
        }
        else if (subcommand == "MEAN")
        {
            if (poly.empty())
            {
                out << "<INVALID COMMAND>\n";
                return;
            }
            for (const auto& p : poly) result += calcArea(p);
            result /= poly.size();
        }
        else
        {
            try
            {
                size_t vertexNum = std::stoul(subcommand);
                if (vertexNum < 3)
                {
                    out << "<INVALID COMMAND>\n";
                    return;
                }
                for (const auto& p : poly)
                    if (p.points.size() == vertexNum) result += calcArea(p);
            }
            catch (const std::exception&)
            {
                out << "<INVALID COMMAND>\n";
                return;
            }
        }

        StreamGuard guard(out);
        out << std::fixed << std::setprecision(1) << result << '\n';
    }

    void doMax(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        std::string subcommand;
        in >> subcommand;

        if (poly.empty() || subcommand.empty())
        {
            out << "<INVALID COMMAND>\n";
            return;
        }

        if (subcommand == "AREA")
        {
            double maxArea = calcArea(poly[0]);
            for (const auto& p : poly)
            {
                double area = calcArea(p);
                if (area > maxArea) maxArea = area;
            }
            StreamGuard guard(out);
            out << std::fixed << std::setprecision(1) << maxArea << '\n';
        }
        else if (subcommand == "VERTEXES")
        {
            size_t maxVertexes = poly[0].points.size();
            for (const auto& p : poly)
                if (p.points.size() > maxVertexes) maxVertexes = p.points.size();
            out << maxVertexes << '\n';
        }
        else
        {
            out << "<INVALID COMMAND>\n";
        }
    }

    void doMin(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        std::string subcommand;
        in >> subcommand;

        if (poly.empty() || subcommand.empty())
        {
            out << "<INVALID COMMAND>\n";
            return;
        }

        if (subcommand == "AREA")
        {
            double minArea = calcArea(poly[0]);
            for (const auto& p : poly)
            {
                double area = calcArea(p);
                if (area < minArea) minArea = area;
            }
            StreamGuard guard(out);
            out << std::fixed << std::setprecision(1) << minArea << '\n';
        }
        else if (subcommand == "VERTEXES")
        {
            size_t minVertexes = poly[0].points.size();
            for (const auto& p : poly)
                if (p.points.size() < minVertexes) minVertexes = p.points.size();
            out << minVertexes << '\n';
        }
        else
        {
            out << "<INVALID COMMAND>\n";
        }
    }

    void doCount(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        std::string subcommand;
        in >> subcommand;

        if (subcommand.empty())
        {
            out << "<INVALID COMMAND>\n";
            return;
        }

        size_t count = 0;

        if (subcommand == "EVEN")
        {
            count = std::count_if(poly.begin(), poly.end(), isEven);
        }
        else if (subcommand == "ODD")
        {
            count = std::count_if(poly.begin(), poly.end(), isOdd);
        }
        else
        {
            try
            {
                size_t vertexNum = std::stoul(subcommand);
                if (vertexNum < 3)
                {
                    out << "<INVALID COMMAND>\n";
                    return;
                }
                count = std::count_if(poly.begin(), poly.end(),
                    [vertexNum](const Polygon& p) { return p.points.size() == vertexNum; });
            }
            catch (const std::exception&)
            {
                out << "<INVALID COMMAND>\n";
                return;
            }
        }

        out << count << '\n';
    }

    void doRmecho(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        Polygon target;
        if (!(in >> target))
        {
            out << "<INVALID COMMAND>\n";
            return;
        }

        std::vector<Polygon> result;
        size_t removed = 0;
        bool prevWasTarget = false;

        for (const auto& p : poly)
        {
            if (p == target)
            {
                if (prevWasTarget)
                {
                    removed++;
                }
                else
                {
                    result.push_back(p);
                    prevWasTarget = true;
                }
            }
            else
            {
                result.push_back(p);
                prevWasTarget = false;
            }
        }

        out << removed << '\n';
        poly = std::move(result);
    }

    bool arePolygonsSame(const Polygon& p1, const Polygon& p2)
    {
        if (p1.points.size() != p2.points.size()) return false;

        std::vector<Point> sorted1 = p1.points;
        std::vector<Point> sorted2 = p2.points;

        std::sort(sorted1.begin(), sorted1.end(), [](const Point& a, const Point& b) {
            return a.x < b.x || (a.x == b.x && a.y < b.y);
        });
        std::sort(sorted2.begin(), sorted2.end(), [](const Point& a, const Point& b) {
            return a.x < b.x || (a.x == b.x && a.y < b.y);
        });

        int dx = sorted1[0].x - sorted2[0].x;
        int dy = sorted1[0].y - sorted2[0].y;

        for (size_t i = 0; i < sorted1.size(); ++i)
        {
            if (sorted1[i].x != sorted2[i].x + dx || sorted1[i].y != sorted2[i].y + dy)
            {
                return false;
            }
        }
        return true;
    }

    void doSame(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        Polygon target;
        if (!(in >> target))
        {
            out << "<INVALID COMMAND>\n";
            return;
        }

        size_t count = 0;
        for (const auto& p : poly)
        {
            if (arePolygonsSame(p, target))
            {
                count++;
            }
        }

        out << count << '\n';
    }
}
