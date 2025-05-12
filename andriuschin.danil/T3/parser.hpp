#ifndef PARSER_HPP
#define PARSER_HPP

#include <iosfwd>
#include "polygon.hpp"

namespace andriuschin
{
  class Parser
  {
  public:
    Parser(std::istream& in, std::ostream& out);
    void init(int argc, char** argv);
    void area();
    void max();
    void min();
    void count();
    void lessArea();
    void intersections();
private:
    std::istream& in_;
    std::ostream& out_;
    std::vector< Polygon > polygons_;

    bool eol();
  };
}

#endif
