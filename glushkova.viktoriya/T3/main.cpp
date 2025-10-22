#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <limits>
#include <map>
#include <functional>
#include <string>
#include <sstream>
#include "shapes.hpp"
#include "shape_commands.hpp"

int main(int argc, const char* argv[])
{
    using namespace shapes;

    const char* filename;
    if (argc == 2) {
        filename = argv[1];
    } else {
        filename = "input.txt";
    }

    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error: Cannot open file '" << filename << "'\n";
        return 1;
    }

    std::vector<Polygon> polygons;
    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty()) continue;

        std::istringstream iss(line);
        Polygon temp;

        if (iss >> temp && temp.points.size() >= 3)
        {
            polygons.push_back(temp);
        }
    }

    file.close();

    std::map<std::string, std::function<void(std::istream&, std::ostream&)>> cmds;
    using namespace std::placeholders;

    cmds["AREA"] = std::bind(doArea, std::ref(polygons), _1, _2);
    cmds["MAX"] = std::bind(doMax, std::ref(polygons), _1, _2);
    cmds["MIN"] = std::bind(doMin, std::ref(polygons), _1, _2);
    cmds["COUNT"] = std::bind(doCount, std::ref(polygons), _1, _2);
    cmds["RMECHO"] = std::bind(doRmecho, std::ref(polygons), _1, _2);
    cmds["SAME"] = std::bind(doSame, std::ref(polygons), _1, _2);
    cmds["ECHO"] = std::bind(doEcho, std::ref(polygons), _1, _2);
    cmds["INFRAME"] = std::bind(doInframe, std::ref(polygons), _1, _2);
    cmds["INTERSECTIONS"] = std::bind(doIntersections, std::ref(polygons), _1, _2);
    cmds["MAXSEQ"] = std::bind(doMaxseq, std::ref(polygons), _1, _2);
    cmds["PERMS"] = std::bind(doPerms, std::ref(polygons), _1, _2);
    cmds["RECTS"] = std::bind(doRects, std::ref(polygons), _1, _2);
    cmds["RIGHTSHAPES"] = std::bind(doRightshapes, std::ref(polygons), _1, _2);
    cmds["LESSAREA"] = std::bind(doLessarea, std::ref(polygons), _1, _2);

    std::string command;
    while (std::cin >> command)
    {
        try
        {
            auto it = cmds.find(command);
            if (it != cmds.end())
            {
                it->second(std::cin, std::cout);
            }
            else
            {
                std::cout << "<INVALID COMMAND>\n";
            }
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << "\n";
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return 0;
}
