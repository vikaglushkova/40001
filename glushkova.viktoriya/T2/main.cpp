#include "datastruct.hpp"
#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>
#include <iomanip>

int main() {
    std::vector<DataStruct> data;
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        DataStruct temp;
        if (iss >> temp) {
            data.push_back(temp);
        }
    }

    std::sort(data.begin(), data.end(), compareDataStructs);

    for (const auto& item : data) {
        std::cout << item << '\n';
    }

    return 0;
}
