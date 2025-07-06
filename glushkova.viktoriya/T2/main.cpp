#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>
#include "datastruct.hpp"

int main() {
    using custom::DataStruct;
    std::vector<DataStruct> data;
    bool hasInput = false;
    bool hasValidRecords = false;

    while (true) {
        DataStruct temp;
        if (std::cin >> temp) {
            data.push_back(temp);
            hasValidRecords = true;
            hasInput = true;
        } else if (std::cin.eof()) {
            break;
        } else {
            hasInput = true;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (std::cin.eof()) break;
        }
    }

    if (!hasInput) {
        std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped\n";
        return 0;
    }
    else if (!hasValidRecords) {
        std::cout << "Atleast one supported record type\n";
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
