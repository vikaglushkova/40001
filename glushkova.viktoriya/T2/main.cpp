#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>
#include "datastruct.hpp"

int main() {
    using custom::DataStruct;
    std::vector<DataStruct> data;
    bool hasAnyInput = false;
    bool hasValidData = false;

    while (true) {
        DataStruct temp;
        if (std::cin >> temp) {
            data.push_back(temp);
            hasValidData = true;
            hasAnyInput = true;
        } else if (std::cin.eof()) {
            break;
        } else {
            hasAnyInput = true;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    if (!hasAnyInput) {
        std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped\n";
    }
    else if (!hasValidData) {
        std::cout << "Atleast one supported record type\n";
    }
    else {
        std::sort(data.begin(), data.end(), custom::compareDataStructs);

        std::copy(
            data.begin(),
            data.end(),
            std::ostream_iterator<DataStruct>(std::cout, "\n")
        );
    }

    return 0;
}
