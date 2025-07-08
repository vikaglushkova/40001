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
        std::istringstream iss(line);
        DataStruct temp;
        if (iss >> temp) {
            data.push_back(temp);
        }
    }

    std::sort(data.begin(), data.end(), compareDataStructs);

    for (const auto& item : data) {
        std::cout << "(:key1 " << std::fixed << std::setprecision(1)
                 << item.key1 << "d:key2 " << item.key2 << "ll:key3 \""
                 << item.key3 << "\":)\n";
    }

    return 0;
}
