#include "shape_commands.hpp"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <limits>
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
    if (polygons.empty())
    {
        out << "0\n";
        return;
    }

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
        out << sum << "\n";
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
        out << sum << "\n";
    }
    else if (type == "MEAN")
    {
        double sum = 0.0;
        for (const auto& poly : polygons)
        {
            sum += calcArea(poly);
        }
        out << (sum / polygons.size()) << "\n";
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
            out << sum << "\n";
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

    if (type == "AREA")
    {
        auto maxIt = std::max_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) {
                return calcArea(a) < calcArea(b);
            });
        StreamGuard guard(out);
        out << std::fixed << std::setprecision(1) << calcArea(*maxIt) << "\n";
    }
    else if (type == "VERTEXES")
    {
        auto maxIt = std::max_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) {
                return a.points.size() < b.points.size();
            });
        out << maxIt->points.size() << "\n";
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

    if (type == "AREA")
    {
        auto minIt = std::min_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) {
                return calcArea(a) < calcArea(b);
            });
        StreamGuard guard(out);
        out << std::fixed << std::setprecision(1) << calcArea(*minIt) << "\n";
    }
    else if (type == "VERTEXES")
    {
        auto minIt = std::min_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) {
                return a.points.size() < b.points.size();
            });
        out << minIt->points.size() << "\n";
    }
    else
    {
        out << "<INVALID COMMAND>\n";
    }
}

void shapes::doCount(std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
    if (polygons.empty())
    {
        out << "0\n";
        return;
    }

    std::string type;
    in >> type;

    if (type == "EVEN")
    {
        out << std::count_if(polygons.begin(), polygons.end(), isEven) << "\n";
    }
    else if (type == "ODD")
    {
        out << std::count_if(polygons.begin(), polygons.end(), isOdd) << "\n";
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

            out << std::count_if(polygons.begin(), polygons.end(),
                [vertexCount](const Polygon& poly) {
                    return poly.points.size() == vertexCount;
                }) << "\n";
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

    polygons.push_back(newPoly);

    size_t count = std::count(polygons.begin(), polygons.end(), newPoly);
    out << count << "\n";
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

    size_t initialSize = polygons.size();
    size_t removedCount = 0;

    auto it = std::find(polygons.begin(), polygons.end(), target);
    if (it != polygons.end())
    {
        polygons.erase(it);
        removedCount = 1;
    }

    out << removedCount << "\n";
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

    size_t count = std::count(polygons.begin(), polygons.end(), target);
    out << count << "\n";
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
    size_t count = std::count_if(polygons.begin(), polygons.end(),
        [targetArea](const Polygon& poly) {
            return calcArea(poly) < targetArea;
        });

    out << count << "\n";
}

shapes::BoundingBox shapes::getOverallBoundingBox(const std::vector<Polygon>& polygons)
{
    if (polygons.empty())
    {
        return {0, 0, 0, 0};
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

    return {minX, maxX, minY, maxY};
}

bool shapes::isPointInBoundingBox(const Point& p, const BoundingBox& box)
{
    return p.x >= box.minX && p.x <= box.maxX && p.y >= box.minY && p.y <= box.maxY;
}

bool shapes::isPolygonInFrame(const Polygon& poly, const BoundingBox& frame)
{
    for (const auto& point : poly.points)
    {
        if (!isPointInBoundingBox(point, frame))
        {
            return false;
        }
    }
    return true;
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

    BoundingBox overallBox = getOverallBoundingBox(polygons);

    bool fits = isPolygonInFrame(target, overallBox);
    out << (fits ? "<TRUE>" : "<FALSE>") << "\n";
}

bool shapes::doPolygonsIntersect(const Polygon& poly1, const Polygon& poly2)
{
    BoundingBox box1 = {std::numeric_limits<int>::max(), std::numeric_limits<int>::min(),
                        std::numeric_limits<int>::max(), std::numeric_limits<int>::min()};
    BoundingBox box2 = {std::numeric_limits<int>::max(), std::numeric_limits<int>::min(),
                        std::numeric_limits<int>::max(), std::numeric_limits<int>::min()};

    for (const auto& p : poly1.points)
    {
        box1.minX = std::min(box1.minX, p.x);
        box1.maxX = std::max(box1.maxX, p.x);
        box1.minY = std::min(box1.minY, p.y);
        box1.maxY = std::max(box1.maxY, p.y);
    }

    for (const auto& p : poly2.points)
    {
        box2.minX = std::min(box2.minX, p.x);
        box2.maxX = std::max(box2.maxX, p.x);
        box2.minY = std::min(box2.minY, p.y);
        box2.maxY = std::max(box2.maxY, p.y);
    }

    return !(box1.maxX < box2.minX || box1.minX > box2.maxX ||
             box1.maxY < box2.minY || box1.minY > box2.maxY);
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

    size_t count = std::count_if(polygons.begin(), polygons.end(),
        [&target](const Polygon& poly) {
            return doPolygonsIntersect(poly, target);
        });

    out << count << "\n";
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
            bool samePoints = true;
            std::vector<Point> sortedPoly = poly.points;
            std::vector<Point> sortedTarget = target.points;
            std::sort(sortedPoly.begin(), sortedPoly.end(),
                [](const Point& a, const Point& b) {
                    return a.x == b.x ? a.y < b.y : a.x < b.x;
                });
            std::sort(sortedTarget.begin(), sortedTarget.end(),
                [](const Point& a, const Point& b) {
                    return a.x == b.x ? a.y < b.y : a.x < b.x;
                });

            if (sortedPoly == sortedTarget)
            {
                count++;
            }
        }
    }

    out << count << "\n";
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

    out << maxSeq << "\n";
}

void shapes::doRects(std::vector<Polygon>& polygons, std::istream&, std::ostream& out)
{
    size_t rectCount = 0;

    for (const auto& poly : polygons)
    {
        if (poly.points.size() == 4)
        {
            const auto& p = poly.points;
            bool isRect = true;

            if ((p[0].x == p[1].x && p[1].y == p[2].y && p[2].x == p[3].x && p[3].y == p[0].y) ||
                (p[0].y == p[1].y && p[1].x == p[2].x && p[2].y == p[3].y && p[3].x == p[0].x))
            {
                rectCount++;
            }
        }
    }

    out << rectCount << "\n";
}

void shapes::doRightShapes(std::vector<Polygon>& polygons, std::istream&, std::ostream& out)
{
    size_t rightAngleCount = 0;

    for (const auto& poly : polygons)
    {
        size_t n = poly.points.size();
        bool hasRightAngle = false;

        for (size_t i = 0; i < n && !hasRightAngle; ++i)
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
            }
        }

        if (hasRightAngle)
        {
            rightAngleCount++;
        }
    }

    out << rightAngleCount << "\n";
}
