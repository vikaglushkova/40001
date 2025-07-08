#include "datastruct.hpp"
#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>

int main() {
    std::vector<DataStruct> data;
    bool hasAnyInput = false;
    bool hasValidRecords = false;

    while (true) {
        DataStruct temp;
        if (std::cin >> temp) {
            data.push_back(temp);
            hasValidRecords = true;
            hasAnyInput = true;
        }
        else if (std::cin.eof()) {
            break;
        }
        else {
            hasAnyInput = true;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    if (!hasAnyInput) {
        std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped\n";
    }
    else if (!hasValidRecords) {
        std::cout << "Atleast one supported record type\n";
    }
    else {
        std::sort(data.begin(), data.end(), compareDataStructs);
        std::copy(data.begin(), data.end(),
                std::ostream_iterator<DataStruct>(std::cout, "\n"));
    }

    return 0;
}
