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
#include <limits>
#include "stream_guard.hpp"

namespace shapes
{
    struct HasNumOfVertexes
    {
        size_t vertexNum;
        explicit HasNumOfVertexes(size_t num) : vertexNum(num) {}
        bool operator()(const Polygon& poly) const { return poly.points.size() == vertexNum; }
    };

    struct AreaComparator
    {
        bool operator()(const Polygon& a, const Polygon& b) const { return calcArea(a) < calcArea(b); }
    };

    struct VertexCountComparator
    {
        bool operator()(const Polygon& a, const Polygon& b) const { return a.points.size() < b.points.size(); }
    };

    struct IsSamePredicate
    {
        const Polygon& target;
        explicit IsSamePredicate(const Polygon& t) : target(t) {}
        bool operator()(const Polygon& p) const { return arePolygonsSame(p, target); }
    };

    struct IsPermutationPredicate
    {
        const Polygon& target;
        explicit IsPermutationPredicate(const Polygon& t) : target(t) {}
        bool operator()(const Polygon& p) const { return arePolygonsPermutation(p, target); }
    };

    struct LessAreaPredicate
    {
        double targetArea;
        explicit LessAreaPredicate(double area) : targetArea(area) {}
        bool operator()(const Polygon& p) const { return calcArea(p) < targetArea; }
    };

    std::pair<Point, Point> makePointPair(const Point& p1, const Point& p2)
    {
        return std::make_pair(p1, p2);
    }

    double shoelaceFormula(double acc, const std::pair<Point, Point>& points)
    {
        return acc + (points.first.x * points.second.y - points.second.x * points.first.y);
    }

    double sumArea(double acc, const Polygon& poly)
    {
        return acc + calcArea(poly);
    }

    double calcArea(const Polygon& poly)
    {
        if (poly.points.size() < 3) return 0.0;

        std::vector<std::pair<Point, Point>> pointPairs;
        std::transform(poly.points.begin(), poly.points.end() - 1,
                      poly.points.begin() + 1,
                      std::back_inserter(pointPairs), makePointPair);
        pointPairs.emplace_back(poly.points.back(), poly.points.front());

        double areaSum = std::accumulate(pointPairs.begin(), pointPairs.end(), 0.0, shoelaceFormula);
        return std::abs(areaSum) / 2.0;
    }

    bool isEven(const Polygon& poly) { return poly.points.size() % 2 == 0; }
    bool isOdd(const Polygon& poly) { return !isEven(poly); }

    Point getMinPoint(const Polygon& poly)
    {
        auto min_x = std::min_element(poly.points.begin(), poly.points.end(),
            [](const Point& a, const Point& b) { return a.x < b.x; });
        auto min_y = std::min_element(poly.points.begin(), poly.points.end(),
            [](const Point& a, const Point& b) { return a.y < b.y; });
        return {min_x->x, min_y->y};
    }

    Point getMaxPoint(const Polygon& poly)
    {
        auto max_x = std::max_element(poly.points.begin(), poly.points.end(),
            [](const Point& a, const Point& b) { return a.x < b.x; });
        auto max_y = std::max_element(poly.points.begin(), poly.points.end(),
            [](const Point& a, const Point& b) { return a.y < b.y; });
        return {max_x->x, max_y->y};
    }

    Point getTotalMinPoint(const std::vector<Polygon>& polygons)
    {
        if (polygons.empty()) return {0, 0};

        auto min_x_poly = std::min_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) { return getMinPoint(a).x < getMinPoint(b).x; });
        auto min_y_poly = std::min_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) { return getMinPoint(a).y < getMinPoint(b).y; });

        return {getMinPoint(*min_x_poly).x, getMinPoint(*min_y_poly).y};
    }

    Point getTotalMaxPoint(const std::vector<Polygon>& polygons)
    {
        if (polygons.empty()) return {0, 0};

        auto max_x_poly = std::max_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) { return getMaxPoint(a).x < getMaxPoint(b).x; });
        auto max_y_poly = std::max_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) { return getMaxPoint(a).y < getMaxPoint(b).y; });

        return {getMaxPoint(*max_x_poly).x, getMaxPoint(*max_y_poly).y};
    }

    bool isPointInFrame(const Point& p, const Point& min_pt, const Point& max_pt)
    {
        return p.x >= min_pt.x && p.x <= max_pt.x && p.y >= min_pt.y && p.y <= max_pt.y;
    }

    bool isPolygonInFrame(const Polygon& poly, const Point& min_pt, const Point& max_pt)
    {
        return std::all_of(poly.points.begin(), poly.points.end(),
            [&](const Point& p) { return isPointInFrame(p, min_pt, max_pt); });
    }

    bool arePolygonsSame(const Polygon& p1, const Polygon& p2)
    {
        if (p1.points.size() != p2.points.size()) return false;
        int dx = p1.points[0].x - p2.points[0].x;
        int dy = p1.points[0].y - p2.points[0].y;

        return std::equal(p1.points.begin(), p1.points.end(), p2.points.begin(),
            [dx, dy](const Point& a, const Point& b) {
                Point expected = {b.x + dx, b.y + dy};
                return a == expected;
            });
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

    bool isRectangle(const Polygon& poly)
    {
        if (poly.points.size() != 4) return false;

        std::vector<int> dx(4), dy(4);
        std::transform(poly.points.begin(), poly.points.end() - 1,
                      poly.points.begin() + 1,
                      dx.begin(),
                      [](const Point& a, const Point& b) { return b.x - a.x; });

        std::transform(poly.points.begin(), poly.points.end() - 1,
                      poly.points.begin() + 1,
                      dy.begin(),
                      [](const Point& a, const Point& b) { return b.y - a.y; });

        dx[3] = poly.points[0].x - poly.points[3].x;
        dy[3] = poly.points[0].y - poly.points[3].y;

        auto dot_product = [](const std::vector<int>& v1, const std::vector<int>& v2, size_t i) {
            return v1[i] * v2[(i + 1) % 4] + v1[(i + 1) % 4] * v2[i];
        };

        return std::all_of(dx.begin(), dx.end(),
            [&, index = 0](const int&) mutable {
                size_t i = index++;
                return dot_product(dx, dy, i) == 0;
            });
    }

    bool hasRightAngle(const Polygon& poly)
    {
        size_t n = poly.points.size();

        std::vector<int> dx(n), dy(n);
        std::transform(poly.points.begin(), poly.points.end() - 1,
                      poly.points.begin() + 1,
                      dx.begin(),
                      [](const Point& a, const Point& b) { return b.x - a.x; });

        std::transform(poly.points.begin(), poly.points.end() - 1,
                      poly.points.begin() + 1,
                      dy.begin(),
                      [](const Point& a, const Point& b) { return b.y - a.y; });

        dx[n-1] = poly.points[0].x - poly.points[n-1].x;
        dy[n-1] = poly.points[0].y - poly.points[n-1].y;

        return std::any_of(dx.begin(), dx.end(),
            [&, index = 0](const int&) mutable {
                size_t i = index++;
                int dot = dx[i] * dx[(i + 1) % n] + dy[i] * dy[(i + 1) % n];
                return dot == 0;
            });
    }

    void doArea(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        std::string subcommand;
        in >> subcommand;

        if (subcommand.empty()) { out << "<INVALID COMMAND>\n"; return; }

        std::map<std::string, std::function<double()>> cmds;
        cmds["EVEN"] = [&]() {
            std::vector<Polygon> evenPolygons;
            std::copy_if(poly.begin(), poly.end(), std::back_inserter(evenPolygons), isEven);
            return std::accumulate(evenPolygons.begin(), evenPolygons.end(), 0.0, sumArea);
        };

        cmds["ODD"] = [&]() {
            std::vector<Polygon> oddPolygons;
            std::copy_if(poly.begin(), poly.end(), std::back_inserter(oddPolygons), isOdd);
            return std::accumulate(oddPolygons.begin(), oddPolygons.end(), 0.0, sumArea);
        };

        cmds["MEAN"] = [&]() {
            if (poly.empty()) throw std::logic_error("");
            return std::accumulate(poly.begin(), poly.end(), 0.0, sumArea) / poly.size();
        };

        double result = 0.0;
        try
        {
            if (cmds.find(subcommand) != cmds.end())
            {
                result = cmds[subcommand]();
            }
            else
            {
                size_t vertexNum = std::stoul(subcommand);
                if (vertexNum < 3) throw std::logic_error("");

                std::vector<Polygon> filtered;
                std::copy_if(poly.begin(), poly.end(), std::back_inserter(filtered), HasNumOfVertexes(vertexNum));
                result = std::accumulate(filtered.begin(), filtered.end(), 0.0, sumArea);
            }
        }
        catch (...)
        {
            out << "<INVALID COMMAND>\n";
            return;
        }

        StreamGuard guard(out);
        out << std::fixed << std::setprecision(1) << result << "\n";
    }

    void doMax(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        std::string subcommand;
        in >> subcommand;

        if (poly.empty() || subcommand.empty()) { out << "<INVALID COMMAND>\n"; return; }

        try
        {
            if (subcommand == "AREA")
            {
                auto maxElem = std::max_element(poly.begin(), poly.end(), AreaComparator());
                StreamGuard guard(out);
                out << std::fixed << std::setprecision(1) << calcArea(*maxElem) << "\n";
            }
            else if (subcommand == "VERTEXES")
            {
                auto maxElem = std::max_element(poly.begin(), poly.end(), VertexCountComparator());
                out << maxElem->points.size() << "\n";
            }
            else
            {
                out << "<INVALID COMMAND>\n";
            }
        }
        catch (...)
        {
            out << "<INVALID COMMAND>\n";
        }
    }

    void doMin(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        std::string subcommand;
        in >> subcommand;

        if (poly.empty() || subcommand.empty()) { out << "<INVALID COMMAND>\n"; return; }

        try
        {
            if (subcommand == "AREA")
            {
                auto minElem = std::min_element(poly.begin(), poly.end(), AreaComparator());
                StreamGuard guard(out);
                out << std::fixed << std::setprecision(1) << calcArea(*minElem) << "\n";
            }
            else if (subcommand == "VERTEXES")
            {
                auto minElem = std::min_element(poly.begin(), poly.end(), VertexCountComparator());
                out << minElem->points.size() << "\n";
            }
            else
            {
                out << "<INVALID COMMAND>\n";
            }
        }
        catch (...)
        {
            out << "<INVALID COMMAND>\n";
        }
    }

    void doCount(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        std::string subcommand;
        in >> subcommand;

        if (subcommand.empty()) { out << "<INVALID COMMAND>\n"; return; }

        size_t count = 0;
        try
        {
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
                size_t vertexNum = std::stoul(subcommand);
                if (vertexNum < 3) throw std::logic_error("");
                count = std::count_if(poly.begin(), poly.end(), HasNumOfVertexes(vertexNum));
            }
            out << count << "\n";
        }
        catch (...)
        {
            out << "<INVALID COMMAND>\n";
        }
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

    void doSame(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        Polygon target;
        if (!(in >> target)) { out << "<INVALID COMMAND>\n"; return; }
        in >> std::ws;
        if (in.peek() != '\n' && in.peek() != EOF) { out << "<INVALID COMMAND>\n"; return; }
        if (target.points.size() < 3) { out << "<INVALID COMMAND>\n"; return; }

        size_t count = std::count_if(poly.begin(), poly.end(), IsSamePredicate(target));
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

        size_t added = std::accumulate(poly.begin(), poly.end(), 0,
            [&result, &target](size_t sum, const Polygon& p) {
                result.push_back(p);
                if (p == target) {
                    result.push_back(p);
                    return sum + 1;
                }
                return sum;
            });

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
        size_t count = std::count_if(poly.begin(), poly.end(), LessAreaPredicate(target_area));
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

        Point target_min = getMinPoint(target);
        Point target_max = getMaxPoint(target);

        size_t count = std::count_if(poly.begin(), poly.end(),
            [&](const Polygon& p) {
                Point p_min = getMinPoint(p);
                Point p_max = getMaxPoint(p);
                return !(p_max.x < target_min.x || p_min.x > target_max.x ||
                        p_max.y < target_min.y || p_min.y > target_max.y);
            });

        out << count << "\n";
    }

    void doPerms(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        Polygon target;
        if (!(in >> target)) { out << "<INVALID COMMAND>\n"; return; }
        in >> std::ws;
        if (in.peek() != '\n' && in.peek() != EOF) { out << "<INVALID COMMAND>\n"; return; }
        if (target.points.size() < 3) { out << "<INVALID COMMAND>\n"; return; }

        size_t count = std::count_if(poly.begin(), poly.end(), IsPermutationPredicate(target));
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
        auto it = poly.begin();

        while (it != poly.end())
        {
            auto seq_start = std::find(it, poly.end(), target);
            if (seq_start == poly.end()) break;

            auto seq_end = std::find_if_not(seq_start, poly.end(),
                [&target](const Polygon& p) { return p == target; });

            size_t seq_len = std::distance(seq_start, seq_end);
            max_seq = std::max(max_seq, seq_len);
            it = seq_end;
        }

        out << max_seq << "\n";
    }

    void doRects(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        in >> std::ws;
        if (in.peek() != '\n' && in.peek() != EOF) { out << "<INVALID COMMAND>\n"; return; }

        size_t count = std::count_if(poly.begin(), poly.end(), isRectangle);
        out << count << "\n";
    }

    void doRightShapes(std::vector<Polygon>& poly, std::istream& in, std::ostream& out)
    {
        in >> std::ws;
        if (in.peek() != '\n' && in.peek() != EOF) { out << "<INVALID COMMAND>\n"; return; }

        size_t count = std::count_if(poly.begin(), poly.end(), hasRightAngle);
        out << count << "\n";
    }
}
