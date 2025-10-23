#ifndef SHAPE_COMMANDS_HPP
#define SHAPE_COMMANDS_HPP
#include "shapes.hpp"
#include <vector>

namespace shapes
{
    double calcArea(const Polygon& poly);
    bool isEven(const Polygon& poly);
    bool isOdd(const Polygon& poly);

    void doArea(std::vector<Polygon>& poly, std::istream& in, std::ostream& out);
    void doMax(std::vector<Polygon>& poly, std::istream& in, std::ostream& out);
    void doMin(std::vector<Polygon>& poly, std::istream& in, std::ostream& out);
    void doCount(std::vector<Polygon>& poly, std::istream& in, std::ostream& out);

    void doRmecho(std::vector<Polygon>& poly, std::istream& in, std::ostream& out);
    void doSame(std::vector<Polygon>& poly, std::istream& in, std::ostream& out);
    void doEcho(std::vector<Polygon>& poly, std::istream& in, std::ostream& out);
    void doLessArea(std::vector<Polygon>& poly, std::istream& in, std::ostream& out);
    void doInframe(std::vector<Polygon>& poly, std::istream& in, std::ostream& out);
    void doIntersections(std::vector<Polygon>& poly, std::istream& in, std::ostream& out);
    void doPerms(std::vector<Polygon>& poly, std::istream& in, std::ostream& out);
    void doMaxseq(std::vector<Polygon>& poly, std::istream& in, std::ostream& out);
    void doRects(std::vector<Polygon>& poly, std::istream& in, std::ostream& out);
    void doRightShapes(std::vector<Polygon>& poly, std::istream& in, std::ostream& out);
}
#endif
