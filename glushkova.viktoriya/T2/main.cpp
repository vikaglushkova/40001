#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>
#include "datastruct.hpp"

int main() {
    using custom::DataStruct;
    std::vector<DataStruct> data;

    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(data)
    );

    if (!std::cin.eof() && std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    if (data.empty()) {
        std::cerr << "Error: no valid input data\n";
        return 1;
    }

    std::sort(data.begin(), data.end(), custom::compareDataStructs);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
