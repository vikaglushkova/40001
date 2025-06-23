#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "datastruct.hpp"

int main() {
    using custom::DataStruct;

    std::vector<DataStruct> data;
    bool hasValidData = false;

    while (!std::cin.eof()) {
        DataStruct temp;
        if (std::cin >> temp) {
            data.push_back(temp);
            hasValidData = true;
        } else if (!std::cin.eof()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    if (!hasValidData) {
        std::cerr << "Looks like there is no supported record. Cannot determine input. Test skipped\n";
        return 0;
    }

    std::sort(data.begin(), data.end(), custom::compareDataStructs);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
