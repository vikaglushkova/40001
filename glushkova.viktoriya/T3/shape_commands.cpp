#include "shape_commands.hpp"
#include <algorithm>
#include <numeric>
#include <limits>
#include <iterator>
#include <cmath>
#include <iomanip>
#include "stream_guard.hpp"

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

bool shapes::isEven(const Polygon& poly)
{
    return poly.points.size() % 2 == 0;
}

bool shapes::isOdd(const Polygon& poly)
{
    return poly.points.size() % 2 != 0;
}

void shapes::doArea(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    std::string type;
    in >> type;

    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1);

    if (type == "EVEN")
    {
        double sum = 0.0;
        for (const auto& poly : polygons)
        {
            if (isEven(poly))
            {
                sum += calcArea(poly);
            }
        }
        out << sum << '\n';
    }
    else if (type == "ODD")
    {
        double sum = 0.0;
        for (const auto& poly : polygons)
        {
            if (isOdd(poly))
            {
                sum += calcArea(poly);
            }
        }
        out << sum << '\n';
    }
    else if (type == "MEAN")
    {
        if (polygons.empty())
        {
            out << "<INVALID COMMAND>\n";
            return;
        }
        double sum = 0.0;
        for (const auto& poly : polygons)
        {
            sum += calcArea(poly);
        }
        out << (sum / polygons.size()) << '\n';
    }
    else
    {
        try
        {
            size_t vertexCount = std::stoul(type);
            if (vertexCount < 3)
            {
                out << "<INVALID COMMAND>\n";
                return;
            }
            double sum = 0.0;
            for (const auto& poly : polygons)
            {
                if (poly.points.size() == vertexCount)
                {
                    sum += calcArea(poly);
                }
            }
            out << sum << '\n';
        }
        catch (...)
        {
            out << "<INVALID COMMAND>\n";
        }
    }
}

void shapes::doMax(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    if (polygons.empty())
    {
        out << "<INVALID COMMAND>\n";
        return;
    }

    std::string type;
    in >> type;

    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1);

    if (type == "AREA")
    {
        double maxArea = calcArea(polygons[0]);
        for (const auto& poly : polygons)
        {
            maxArea = std::max(maxArea, calcArea(poly));
        }
        out << maxArea << '\n';
    }
    else if (type == "VERTEXES")
    {
        size_t maxVertexes = polygons[0].points.size();
        for (const auto& poly : polygons)
        {
            maxVertexes = std::max(maxVertexes, poly.points.size());
        }
        out << maxVertexes << '\n';
    }
    else
    {
        out << "<INVALID COMMAND>\n";
    }
}

void shapes::doMin(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    if (polygons.empty())
    {
        out << "<INVALID COMMAND>\n";
        return;
    }

    std::string type;
    in >> type;

    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1);

    if (type == "AREA")
    {
        double minArea = calcArea(polygons[0]);
        for (const auto& poly : polygons)
        {
            minArea = std::min(minArea, calcArea(poly));
        }
        out << minArea << '\n';
    }
    else if (type == "VERTEXES")
    {
        size_t minVertexes = polygons[0].points.size();
        for (const auto& poly : polygons)
        {
            minVertexes = std::min(minVertexes, poly.points.size());
        }
        out << minVertexes << '\n';
    }
    else
    {
        out << "<INVALID COMMAND>\n";
    }
}

void shapes::doCount(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    std::string type;
    in >> type;

    if (type == "EVEN")
    {
        size_t count = 0;
        for (const auto& poly : polygons)
        {
            if (isEven(poly))
            {
                count++;
            }
        }
        out << count << '\n';
    }
    else if (type == "ODD")
    {
        size_t count = 0;
        for (const auto& poly : polygons)
        {
            if (isOdd(poly))
            {
                count++;
            }
        }
        out << count << '\n';
    }
    else
    {
        try
        {
            size_t vertexCount = std::stoul(type);
            if (vertexCount < 3)
            {
                out << "<INVALID COMMAND>\n";
                return;
            }
            size_t count = 0;
            for (const auto& poly : polygons)
            {
                if (poly.points.size() == vertexCount)
                {
                    count++;
                }
            }
            out << count << '\n';
        }
        catch (...)
        {
            out << "<INVALID COMMAND>\n";
        }
    }
}

void shapes::doEcho(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    Polygon newPoly;
    in >> newPoly;

    if (!in || newPoly.points.size() < 3)
    {
        out << "<INVALID COMMAND>\n";
        return;
    }

    size_t count = 0;
    std::vector<Polygon> result;

    for (const auto& poly : polygons)
    {
        result.push_back(poly);
        if (poly == newPoly)
        {
            result.push_back(newPoly);
            count++;
        }
    }

    polygons = std::move(result);
    out << count << '\n';
}

void shapes::doRmecho(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    Polygon target;
    in >> target;

    if (!in || target.points.size() < 3)
    {
        out << "<INVALID COMMAND>\n";
        return;
    }

    size_t removed = 0;
    std::vector<Polygon> result;

    for (size_t i = 0; i < polygons.size(); ++i)
    {
        if (i > 0 && polygons[i] == target && polygons[i-1] == target)
        {
            removed++;
        }
        else
        {
            result.push_back(polygons[i]);
        }
    }

    polygons = std::move(result);
    out << removed << '\n';
}

void shapes::doSame(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    Polygon target;
    in >> target;

    if (!in || target.points.size() < 3)
    {
        out << "<INVALID COMMAND>\n";
        return;
    }

    size_t count = 0;
    for (const auto& poly : polygons)
    {
        if (poly.points.size() == target.points.size())
        {
            count++;
        }
    }

    out << count << '\n';
}

void shapes::doLessArea(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    Polygon target;
    in >> target;

    if (!in || target.points.size() < 3)
    {
        out << "<INVALID COMMAND>\n";
        return;
    }

    double targetArea = calcArea(target);
    size_t count = 0;

    for (const auto& poly : polygons)
    {
        if (calcArea(poly) < targetArea)
        {
            count++;
        }
    }

    out << count << '\n';
}

void shapes::doInframe(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    Polygon target;
    in >> target;

    if (!in || target.points.size() < 3)
    {
        out << "<INVALID COMMAND>\n";
        return;
    }

    if (polygons.empty())
    {
        out << "<FALSE>\n";
        return;
    }

    int minX = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    int minY = std::numeric_limits<int>::max();
    int maxY = std::numeric_limits<int>::min();

    for (const auto& poly : polygons)
    {
        for (const auto& point : poly.points)
        {
            minX = std::min(minX, point.x);
            maxX = std::max(maxX, point.x);
            minY = std::min(minY, point.y);
            maxY = std::max(maxY, point.y);
        }
    }

    bool inside = true;
    for (const auto& point : target.points)
    {
        if (point.x < minX || point.x > maxX || point.y < minY || point.y > maxY)
        {
            inside = false;
            break;
        }
    }

    out << (inside ? "<TRUE>" : "<FALSE>") << '\n';
}

void shapes::doIntersections(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    Polygon target;
    in >> target;

    if (!in || target.points.size() < 3)
    {
        out << "<INVALID COMMAND>\n";
        return;
    }

    size_t count = 0;
    for (const auto& poly : polygons)
    {
        bool intersects = false;
        for (const auto& p1 : target.points)
        {
            for (const auto& p2 : poly.points)
            {
                if (p1.x == p2.x && p1.y == p2.y)
                {
                    intersects = true;
                    break;
                }
            }
            if (intersects) break;
        }
        if (poly == target)
        {
            intersects = true;
        }
        if (intersects)
        {
            count++;
        }
    }
    out << count << '\n';
}

void shapes::doPerms(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    Polygon target;
    in >> target;

    if (!in || target.points.size() < 3)
    {
        out << "<INVALID COMMAND>\n";
        return;
    }

    size_t count = 0;
    for (const auto& poly : polygons)
    {
        if (poly.points.size() == target.points.size())
        {
            std::vector<Point> sortedPoly = poly.points;
            std::vector<Point> sortedTarget = target.points;
            std::sort(sortedPoly.begin(), sortedPoly.end(), [](const Point& a, const Point& b) {
                return a.x == b.x ? a.y < b.y : a.x < b.x;
            });
            std::sort(sortedTarget.begin(), sortedTarget.end(), [](const Point& a, const Point& b) {
                return a.x == b.x ? a.y < b.y : a.x < b.x;
            });
            if (sortedPoly == sortedTarget)
            {
                count++;
            }
        }
    }
    out << count << '\n';
}

void shapes::doMaxseq(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    Polygon target;
    in >> target;

    if (!in || target.points.size() < 3)
    {
        out << "<INVALID COMMAND>\n";
        return;
    }

    size_t maxSeq = 0;
    size_t currentSeq = 0;

    for (const auto& poly : polygons)
    {
        if (poly == target)
        {
            currentSeq++;
            maxSeq = std::max(maxSeq, currentSeq);
        }
        else
        {
            currentSeq = 0;
        }
    }
    out << maxSeq << '\n';
}

void shapes::doRects(std::vector<Polygon>& polygons, std::istream&, std::ostream& out)
{
    size_t rectCount = 0;

    for (const auto& poly : polygons)
    {
        if (poly.points.size() == 4)
        {
            const auto& p1 = poly.points[0];
            const auto& p2 = poly.points[1];
            const auto& p3 = poly.points[2];
            const auto& p4 = poly.points[3];
            int v1x = p2.x - p1.x;
            int v1y = p2.y - p1.y;
            int v2x = p3.x - p2.x;
            int v2y = p3.y - p2.y;
            int v3x = p4.x - p3.x;
            int v3y = p4.y - p3.y;
            int v4x = p1.x - p4.x;
            int v4y = p1.y - p4.y;
            bool isRect = (v1x * v2x + v1y * v2y == 0) &&
                         (v2x * v3x + v2y * v3y == 0) &&
                         (v3x * v4x + v3y * v4y == 0) &&
                         (v4x * v1x + v4y * v1y == 0);
            if (isRect)
            {
                rectCount++;
            }
        }
    }
    out << rectCount << '\n';
}

void shapes::doRightShapes(std::vector<Polygon>& polygons, std::istream&, std::ostream& out)
{
    size_t rightCount = 0;

    for (const auto& poly : polygons)
    {
        size_t n = poly.points.size();
        bool hasRightAngle = false;
        for (size_t i = 0; i < n; ++i)
        {
            const Point& a = poly.points[i];
            const Point& b = poly.points[(i + 1) % n];
            const Point& c = poly.points[(i + 2) % n];
            int abx = b.x - a.x;
            int aby = b.y - a.y;
            int bcx = c.x - b.x;
            int bcy = c.y - b.y;
            int dot = abx * bcx + aby * bcy;
            if (dot == 0)
            {
                hasRightAngle = true;
                break;
            }
        }
        if (hasRightAngle)
        {
            rightCount++;
        }
    }
    out << rightCount << '\n';
}
