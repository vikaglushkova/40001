#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>
#include "datastruct.hpp"

int main() {
    using custom::DataStruct;
    std::vector<DataStruct> data;

    while (true) {
        DataStruct temp;
        if (std::cin >> temp) {
            data.push_back(temp);
        } else if (std::cin.eof()) {
            break;
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (std::cin.eof()) break;
        }
    }

    if (data.empty()) {
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
