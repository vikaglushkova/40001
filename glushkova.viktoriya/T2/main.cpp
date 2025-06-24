#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "DataStruct.hpp"

int main() {
    using custom::DataStruct;
    std::vector<DataStruct> data;
    DataStruct temp;

    while (std::cin >> temp) {
        data.push_back(temp);
    }

    if (!std::cin.eof() && std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::sort(data.begin(), data.end(), custom::compareDataStructs);
    std::copy(data.begin(), data.end(), std::ostream_iterator<DataStruct>(std::cout, "\n"));

    return 0;
}
