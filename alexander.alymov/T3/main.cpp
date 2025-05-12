#include <fstream>
#include "commands.h"

int main(int nArguments, char** file)
{
    using namespace alymov;
    if (nArguments < 2)
    {
        return 1;
    }

    std::ifstream inputF(file[1]);
    if (!inputF)
    {
        return 1;
    }

    std::vector< Polygon > polygon;
    while (inputF.good())
    {
        std::copy(
            std::istream_iterator<Polygon>(inputF),
            std::istream_iterator<Polygon>(),
            std::back_inserter(polygon)
        );
        if (inputF.fail() && !inputF.eof())
        {
            inputF.clear();
            inputF.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    std::cin.clear();

    std::string command;
    while (std::cin >> command)
    {
        if (command == "AREA")
        {
            std::string mode;
            std::cin >> mode;
            area(polygon, mode, std::cout);
        }
        else if (command == "MAX")
        {
            std::string mode;
            std::cin >> mode;
            max(polygon, mode, std::cout);
        }
        else if (command == "MIN")
        {
            std::string mode;
            std::cin >> mode;
            min(polygon, mode, std::cout);
        }
        else if (command == "COUNT")
        {
            std::string mode;
            std::cin >> mode;
            count(polygon, mode, std::cout);
        }
        else if (command == "LESSAREA")
        {
            lessarea(polygon, std::cin, std::cout);
        }
        else if (command == "MAXSEQ")
        {
            maxseq(polygon, std::cin, std::cout);
        }
        else
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "<INVALID COMMAND>" << std::endl;
        }
    }

    return 0;
}
