#include "shape_commands.hpp"
#include <functional>
#include <map>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <cmath>
#include <vector>
#include <iterator>
#include <set>
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

    bool isEven(const Polygon& poly) { return poly.points.size() % 2 == 0; }
    bool isOdd(const Polygon& poly) { return !isEven(poly); }

    Point getMinPoint(const Polygon& poly)
    {
        int min_x = std::numeric_limits<int>::max();
        int min_y = std::numeric_limits<int>::max();
        for (const auto& p : poly.points)
        {
            if (p.x < min_x) min_x = p.x;
            if (p.y < min_y) min_y = p.y;
        }
        return {min_x, min_y};
    }

    Point getMaxPoint(const Polygon& poly)
    {
        int max_x = std::numeric_limits<int>::min();
        int max_y = std::numeric_limits<int>::min();
        for (const auto& p : poly.points)
        {
            if (p.x > max_x) max_x = p.x;
            if (p.y > max_y) max_y = p.y;
        }
        return {max_x, max_y};
    }

    Point getTotalMinPoint(const std::vector<Polygon>& polygons)
    {
        if (polygons.empty()) return {0, 0};

        int min_x = std::numeric_limits<int>::max();
        int min_y = std::numeric_limits<int>::max();

        for (const auto& poly : polygons)
        {
            Point poly_min = getMinPoint(poly);
            if (poly_min.x < min_x) min_x = poly_min.x;
            if (poly_min.y < min_y) min_y = poly_min.y;
        }
        return {min_x, min_y};
    }

    Point getTotalMaxPoint(const std::vector<Polygon>& polygons)
    {
        if (polygons.empty()) return {0, 0};

        int max_x = std::numeric_limits<int>::min();
        int max_y = std::numeric_limits<int>::min();

        for (const auto& poly : polygons)
        {
            Point poly_max = getMaxPoint(poly);
            if (poly_max.x > max_x) max_x = poly_max.x;
            if (poly_max.y > max_y) max_y = poly_max.y;
        }
        return {max_x, max_y};
    }

    bool isPointInFrame(const Point& p, const Point& min_pt, const Point& max_pt)
    {
        return p.x >= min_pt.x && p.x <= max_pt.x && p.y >= min_pt.y && p.y <= max_pt.y;
    }

    bool isPolygonInFrame(const Polygon& poly, const Point& min_pt, const Point& max_pt)
    {
        for (const auto& p : poly.points)
        {
            if (!isPointInFrame(p, min_pt, max_pt)) return false;
        }
        return true;
    }

    void doArea(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        std::string subcommand;
        in >> subcommand;

        if (subcommand.empty()) { out << "<INVALID COMMAND>\n"; return; }

        double result = 0.0;

        if (subcommand == "EVEN")
        {
            result = std::accumulate(poly.begin(), poly.end(), 0.0,
                [](double sum, const Polygon& p) { return isEven(p) ? sum + calcArea(p) : sum; });
        }
        else if (subcommand == "ODD")
        {
            result = std::accumulate(poly.begin(), poly.end(), 0.0,
                [](double sum, const Polygon& p) { return isOdd(p) ? sum + calcArea(p) : sum; });
        }
        else if (subcommand == "MEAN")
        {
            if (poly.empty()) { out << "<INVALID COMMAND>\n"; return; }
            result = std::accumulate(poly.begin(), poly.end(), 0.0,
                [](double sum, const Polygon& p) { return sum + calcArea(p); }) / poly.size();
        }
        else
        {
            try
            {
                size_t vertexNum = std::stoul(subcommand);
                if (vertexNum < 3) { out << "<INVALID COMMAND>\n"; return; }
                result = std::accumulate(poly.begin(), poly.end(), 0.0,
                    [vertexNum](double sum, const Polygon& p) {
                        return p.points.size() == vertexNum ? sum + calcArea(p) : sum;
                    });
            }
            catch (const std::exception&) { out << "<INVALID COMMAND>\n"; return; }
        }

        StreamGuard guard(out);
        out << std::fixed << std::setprecision(1) << result << "\n";
    }

    void doMax(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        std::string subcommand;
        in >> subcommand;

        if (poly.empty() || subcommand.empty()) { out << "<INVALID COMMAND>\n"; return; }

        if (subcommand == "AREA")
        {
            auto maxElem = std::max_element(poly.begin(), poly.end(),
                [](const Polygon& a, const Polygon& b) { return calcArea(a) < calcArea(b); });
            StreamGuard guard(out);
            out << std::fixed << std::setprecision(1) << calcArea(*maxElem) << "\n";
        }
        else if (subcommand == "VERTEXES")
        {
            auto maxElem = std::max_element(poly.begin(), poly.end(),
                [](const Polygon& a, const Polygon& b) { return a.points.size() < b.points.size(); });
            out << maxElem->points.size() << "\n";
        }
        else { out << "<INVALID COMMAND>\n"; }
    }

    void doMin(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        std::string subcommand;
        in >> subcommand;

        if (poly.empty() || subcommand.empty()) { out << "<INVALID COMMAND>\n"; return; }

        if (subcommand == "AREA")
        {
            auto minElem = std::min_element(poly.begin(), poly.end(),
                [](const Polygon& a, const Polygon& b) { return calcArea(a) < calcArea(b); });
            StreamGuard guard(out);
            out << std::fixed << std::setprecision(1) << calcArea(*minElem) << "\n";
        }
        else if (subcommand == "VERTEXES")
        {
            auto minElem = std::min_element(poly.begin(), poly.end(),
                [](const Polygon& a, const Polygon& b) { return a.points.size() < b.points.size(); });
            out << minElem->points.size() << "\n";
        }
        else { out << "<INVALID COMMAND>\n"; }
    }

    void doCount(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        std::string subcommand;
        in >> subcommand;

        if (subcommand.empty()) { out << "<INVALID COMMAND>\n"; return; }

        size_t count = 0;

        if (subcommand == "EVEN") { count = std::count_if(poly.begin(), poly.end(), isEven); }
        else if (subcommand == "ODD") { count = std::count_if(poly.begin(), poly.end(), isOdd); }
        else
        {
            try
            {
                size_t vertexNum = std::stoul(subcommand);
                if (vertexNum < 3) { out << "<INVALID COMMAND>\n"; return; }
                count = std::count_if(poly.begin(), poly.end(),
                    [vertexNum](const Polygon& p) { return p.points.size() == vertexNum; });
            }
            catch (const std::exception&) { out << "<INVALID COMMAND>\n"; return; }
        }

        out << count << "\n";
    }

    void doRmecho(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        Polygon target;
        if (!(in >> target)) { out << "<INVALID COMMAND>\n"; return; }
        in >> std::ws;
        if (in.peek() != '\n' && in.peek() != EOF) { out << "<INVALID COMMAND>\n"; return; }
        if (target.points.size() < 3) { out << "<INVALID COMMAND>\n"; return; }

        std::vector<Polygon> result;
        size_t removed = 0;

        auto it = poly.begin();
        while (it != poly.end())
        {
            auto range_start = it;
            auto range_end = std::find_if_not(it, poly.end(),
                [&target](const Polygon& p) { return p == target; });

            if (range_start != range_end)
            {
                result.push_back(*range_start);
                size_t seq_size = std::distance(range_start, range_end);
                if (seq_size > 1) removed += (seq_size - 1);
                it = range_end;
            }
            else
            {
                if (it != poly.end()) result.push_back(*it);
                ++it;
            }
        }

        out << removed << "\n";
        poly = std::move(result);
    }

    bool arePolygonsSame(const Polygon& p1, const Polygon& p2)
    {
        if (p1.points.size() != p2.points.size()) return false;
        int dx = p1.points[0].x - p2.points[0].x;
        int dy = p1.points[0].y - p2.points[0].y;

        for (size_t i = 0; i < p1.points.size(); ++i)
        {
            Point expected = {p2.points[i].x + dx, p2.points[i].y + dy};
            if (p1.points[i] != expected) return false;
        }
        return true;
    }

    void doSame(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        Polygon target;
        if (!(in >> target)) { out << "<INVALID COMMAND>\n"; return; }
        in >> std::ws;
        if (in.peek() != '\n' && in.peek() != EOF) { out << "<INVALID COMMAND>\n"; return; }
        if (target.points.size() < 3) { out << "<INVALID COMMAND>\n"; return; }

        size_t count = std::count_if(poly.begin(), poly.end(),
            [&target](const Polygon& p) { return arePolygonsSame(p, target); });
        out << count << "\n";
    }

    void doEcho(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        Polygon target;
        if (!(in >> target)) { out << "<INVALID COMMAND>\n"; return; }
        in >> std::ws;
        if (in.peek() != '\n' && in.peek() != EOF) { out << "<INVALID COMMAND>\n"; return; }
        if (target.points.size() < 3) { out << "<INVALID COMMAND>\n"; return; }

        std::vector<Polygon> result;
        size_t added = 0;

        for (const auto& p : poly)
        {
            result.push_back(p);
            if (p == target)
            {
                result.push_back(p);
                added++;
            }
        }

        out << added << "\n";
        poly = std::move(result);
    }

    void doLessArea(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        Polygon target;
        if (!(in >> target)) { out << "<INVALID COMMAND>\n"; return; }
        in >> std::ws;
        if (in.peek() != '\n' && in.peek() != EOF) { out << "<INVALID COMMAND>\n"; return; }
        if (target.points.size() < 3) { out << "<INVALID COMMAND>\n"; return; }

        double target_area = calcArea(target);
        size_t count = std::count_if(poly.begin(), poly.end(),
            [target_area](const Polygon& p) { return calcArea(p) < target_area; });
        out << count << "\n";
    }

    void doInframe(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        Polygon target;
        if (!(in >> target)) { out << "<INVALID COMMAND>\n"; return; }
        in >> std::ws;
        if (in.peek() != '\n' && in.peek() != EOF) { out << "<INVALID COMMAND>\n"; return; }
        if (target.points.size() < 3) { out << "<INVALID COMMAND>\n"; return; }

        if (poly.empty()) { out << "<FALSE>\n"; return; }

        Point min_pt = getTotalMinPoint(poly);
        Point max_pt = getTotalMaxPoint(poly);

        bool is_in_frame = isPolygonInFrame(target, min_pt, max_pt);
        out << (is_in_frame ? "<TRUE>\n" : "<FALSE>\n");
    }

    void doIntersections(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        Polygon target;
        if (!(in >> target)) { out << "<INVALID COMMAND>\n"; return; }
        in >> std::ws;
        if (in.peek() != '\n' && in.peek() != EOF) { out << "<INVALID COMMAND>\n"; return; }
        if (target.points.size() < 3) { out << "<INVALID COMMAND>\n"; return; }

        size_t count = 0;
        Point target_min = getMinPoint(target);
        Point target_max = getMaxPoint(target);

        for (const auto& p : poly)
        {
            Point p_min = getMinPoint(p);
            Point p_max = getMaxPoint(p);

            bool intersects = !(p_max.x < target_min.x || p_min.x > target_max.x ||
                               p_max.y < target_min.y || p_min.y > target_max.y);
            if (intersects) count++;
        }

        out << count << "\n";
    }

    bool arePolygonsPermutation(const Polygon& p1, const Polygon& p2)
    {
        if (p1.points.size() != p2.points.size()) return false;

        std::vector<Point> p1_sorted = p1.points;
        std::vector<Point> p2_sorted = p2.points;

        std::sort(p1_sorted.begin(), p1_sorted.end(),
            [](const Point& a, const Point& b) { return a.x == b.x ? a.y < b.y : a.x < b.x; });
        std::sort(p2_sorted.begin(), p2_sorted.end(),
            [](const Point& a, const Point& b) { return a.x == b.x ? a.y < b.y : a.x < b.x; });

        return p1_sorted == p2_sorted;
    }

    void doPerms(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        Polygon target;
        if (!(in >> target)) { out << "<INVALID COMMAND>\n"; return; }
        in >> std::ws;
        if (in.peek() != '\n' && in.peek() != EOF) { out << "<INVALID COMMAND>\n"; return; }
        if (target.points.size() < 3) { out << "<INVALID COMMAND>\n"; return; }

        size_t count = std::count_if(poly.begin(), poly.end(),
            [&target](const Polygon& p) { return arePolygonsPermutation(p, target); });
        out << count << "\n";
    }

    void doMaxseq(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        Polygon target;
        if (!(in >> target)) { out << "<INVALID COMMAND>\n"; return; }
        in >> std::ws;
        if (in.peek() != '\n' && in.peek() != EOF) { out << "<INVALID COMMAND>\n"; return; }
        if (target.points.size() < 3) { out << "<INVALID COMMAND>\n"; return; }

        size_t max_seq = 0;
        size_t current_seq = 0;

        for (const auto& p : poly)
        {
            if (p == target)
            {
                current_seq++;
                max_seq = std::max(max_seq, current_seq);
            }
            else
            {
                current_seq = 0;
            }
        }

        out << max_seq << "\n";
    }

    bool isRectangle(const Polygon& poly)
    {
        if (poly.points.size() != 4) return false;

        std::vector<int> dx, dy;
        for (size_t i = 0; i < 4; ++i)
        {
            size_t next = (i + 1) % 4;
            dx.push_back(poly.points[next].x - poly.points[i].x);
            dy.push_back(poly.points[next].y - poly.points[i].y);
        }

        int dot1 = dx[0] * dx[1] + dy[0] * dy[1];
        int dot2 = dx[1] * dx[2] + dy[1] * dy[2];
        int dot3 = dx[2] * dx[3] + dy[2] * dy[3];
        int dot4 = dx[3] * dx[0] + dy[3] * dy[0];

        return (dot1 == 0 && dot2 == 0 && dot3 == 0 && dot4 == 0);
    }

    void doRects(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        in >> std::ws;
        if (in.peek() != '\n' && in.peek() != EOF) { out << "<INVALID COMMAND>\n"; return; }

        size_t count = std::count_if(poly.begin(), poly.end(), isRectangle);
        out << count << "\n";
    }

    bool hasRightAngle(const Polygon& poly)
    {
        size_t n = poly.points.size();
        for (size_t i = 0; i < n; ++i)
        {
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

    void doRightShapes(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        in >> std::ws;
        if (in.peek() != '\n' && in.peek() != EOF) { out << "<INVALID COMMAND>\n"; return; }

        size_t count = std::count_if(poly.begin(), poly.end(), hasRightAngle);
        out << count << "\n";
    }
}
