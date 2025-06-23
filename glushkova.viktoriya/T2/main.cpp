#include "datastruct.hpp"
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>

int main() {
    std::vector<DataStruct> data;

    std::string test_input =
        "( :key1 50.0d :key2 -89LL :key3 \"Data\" :)\n"
        "( :key2 8911 :key1 100.0D :key3 \"Test\" :)\n"
        "( :key1 25.0d :key2 42ll :key3 \"ABC\" :)\n"
        "( :key3 \"Sample\" :key1 75.0d :key2 123ll :)\n";

    std::istringstream input(test_input);
    DataStruct ds;

    std::cout << "Trying to parse input...\n";
    while (input >> ds) {
        data.push_back(ds);
        std::cout << "Parsed record: " << ds << "\n";
    }

    if (data.empty()) {
        std::cerr << "\nERROR: No valid records found in input:\n" << test_input << "\n";
        return 1;
    }

    std::sort(data.begin(), data.end());

    std::cout << "\nSuccessfully parsed " << data.size() << " records. Sorted output:\n";
    for (const auto& item : data) {
        std::cout << item << "\n";
    }

    return 0;
}
