#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "task.h"
#include "tests.h"


int main() {
    using namespace doomsday;
    std::vector<DataStruct> dataVec;
    std::string line;

    DataStruct data;
    while (!std::cin.eof()) {
        if (std::cin >> data)
            dataVec.push_back(data);
        else
            std::cin.clear();

    }


    std::sort(dataVec.begin(), dataVec.end(), compareDataStruct);

    std::copy(dataVec.begin(), dataVec.end(), std::ostream_iterator<DataStruct>(std::cout, "\n"));

    return 0;
}
