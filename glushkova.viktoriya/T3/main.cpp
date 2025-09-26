#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <limits>
#include <map>
#include <functional>
#include <string>
#include "shapes.hpp"
#include "shape_commands.hpp"

int main(int argc, const char* argv[])
{
    using namespace shapes;

    if (argc != 2) {
        std::cerr << "Error: Missing filename argument\n";
        std::cerr << "Usage: " << argv[0] << " filename\n";
        return 1;
    }

    const char* filename = argv[1];

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file '" << filename << "'\n";
        return 1;
    }

    std::vector<Polygon> polygons;
    Polygon temp;

    while (file >> temp) {
        polygons.push_back(temp);
    }

    if (!file.eof()) {
        std::cerr << "Warning: File contains errors, but some data was loaded\n";
    }

    file.close();

    if (polygons.empty()) {
        std::cerr << "Error: No valid polygons found in file\n";
        return 1;
    }

    std::cout << "Successfully loaded " << polygons.size() << " polygons\n";

    std::map<std::string, std::function<void(std::istream&, std::ostream&)>> cmds;
    using namespace std::placeholders;

    cmds["AREA"] = std::bind(doArea, std::ref(polygons), _1, _2);
    cmds["MAX"] = std::bind(doMax, std::ref(polygons), _1, _2);
    cmds["MIN"] = std::bind(doMin, std::ref(polygons), _1, _2);
    cmds["COUNT"] = std::bind(doCount, std::ref(polygons), _1, _2);
    cmds["RMECHO"] = std::bind(doRmecho, std::ref(polygons), _1, _2);
    cmds["SAME"] = std::bind(doSame, std::ref(polygons), _1, _2);

    std::cout << "Available commands: AREA, MAX, MIN, COUNT, RMECHO, SAME\n";
    std::cout << "Enter command or Ctrl+Z to exit:\n> ";

    std::string command;
    while (std::cin >> command) {
        try {
            auto it = cmds.find(command);
            if (it != cmds.end()) {
                it->second(std::cin, std::cout);
            } else {
                std::cout << "<INVALID COMMAND>\n";
            }
        } catch (const std::exception& e) {
            std::cout << "<ERROR: " << e.what() << ">\n";
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "> ";
    }

    return 0;
}

