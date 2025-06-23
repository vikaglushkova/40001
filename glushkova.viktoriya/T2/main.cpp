#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>
#include "datastruct.hpp"

int main() {
    using custom::DataStruct;

    std::vector<DataStruct> data;
    DataStruct temp;

    while (std::cin >> temp) {
        data.push_back(temp);
    }

    if (std::cin.fail() && !std::cin.eof()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    if (data.empty()) {
        std::cerr << "Error: no valid input data\n";
        return 1;
    }

    std::sort(data.begin(), data.end(), custom::compareDataStructs);
    std::copy(data.begin(), data.end(), std::ostream_iterator<DataStruct>(std::cout, "\n"));

    return 0;
}
