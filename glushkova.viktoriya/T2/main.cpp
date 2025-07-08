#include "datastruct.hpp"
#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>

int main() {
    std::vector<DataStruct> data;

    while (std::cin.good()) {
        DataStruct temp;
        if (std::cin >> temp) {
            data.push_back(temp);
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
