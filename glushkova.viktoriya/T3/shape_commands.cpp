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
    size_t countBefore = std::count(polygons.begin(), polygons.end(), newPoly);
    polygons.push_back(newPoly);
    out << countBefore << "\n";
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
        out << "<TRUE>\n";
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
    bool fits = true;
    for (const auto& point : target.points)
    {
        if (point.x < minX || point.x > maxX || point.y < minY || point.y > maxY)
        {
            fits = false;
            break;
        }
    }
    out << (fits ? "<TRUE>" : "<FALSE>") << "\n";
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
        int polyMinX = std::numeric_limits<int>::max();
        int polyMaxX = std::numeric_limits<int>::min();
        int polyMinY = std::numeric_limits<int>::max();
        int polyMaxY = std::numeric_limits<int>::min();
        for (const auto& p : poly.points)
        {
            polyMinX = std::min(polyMinX, p.x);
            polyMaxX = std::max(polyMaxX, p.x);
            polyMinY = std::min(polyMinY, p.y);
            polyMaxY = std::max(polyMaxY, p.y);
        }
        int targetMinX = std::numeric_limits<int>::max();
        int targetMaxX = std::numeric_limits<int>::min();
        int targetMinY = std::numeric_limits<int>::max();
        int targetMaxY = std::numeric_limits<int>::min();
        for (const auto& p : target.points)
        {
            targetMinX = std::min(targetMinX, p.x);
            targetMaxX = std::max(targetMaxX, p.x);
            targetMinY = std::min(targetMinY, p.y);
            targetMaxY = std::max(targetMaxY, p.y);
        }
        bool intersectsX = (polyMinX <= targetMaxX && polyMaxX >= targetMinX);
        bool intersectsY = (polyMinY <= targetMaxY && polyMaxY >= targetMinY);
        if (intersectsX && intersectsY)
        {
            count++;
        }
    }
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
            std::vector<Point> polySorted = poly.points;
            std::vector<Point> targetSorted = target.points;
            std::sort(polySorted.begin(), polySorted.end(),
                [](const Point& a, const Point& b) {
                    if (a.x == b.x) return a.y < b.y;
                    return a.x < b.x;
                });
            std::sort(targetSorted.begin(), targetSorted.end(),
                [](const Point& a, const Point& b) {
                    if (a.x == b.x) return a.y < b.y;
                    return a.x < b.x;
                });
            bool equal = true;
            for (size_t i = 0; i < polySorted.size(); ++i)
            {
                if (polySorted[i].x != targetSorted[i].x || polySorted[i].y != targetSorted[i].y)
                {
                    equal = false;
                    break;
                }
            }
            if (equal)
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
    for (size_t i = 0; i < polygons.size(); ++i)
    {
        if (polygons[i] == target)
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
            int dx1 = p[1].x - p[0].x;
            int dy1 = p[1].y - p[0].y;
            int dx2 = p[2].x - p[1].x;
            int dy2 = p[2].y - p[1].y;
            int dx3 = p[3].x - p[2].x;
            int dy3 = p[3].y - p[2].y;
            int dx4 = p[0].x - p[3].x;
            int dy4 = p[0].y - p[3].y;
            bool rightAngle1 = (dx1 * dx2 + dy1 * dy2) == 0;
            bool rightAngle2 = (dx2 * dx3 + dy2 * dy3) == 0;
            bool rightAngle3 = (dx3 * dx4 + dy3 * dy4) == 0;
            bool rightAngle4 = (dx4 * dx1 + dy4 * dy1) == 0;
            if (rightAngle1 && rightAngle2 && rightAngle3 && rightAngle4)
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
