#include "DataStruct.h"
#include <vector>
#include <iostream>
#include <algorithm>

int main() {
    std::vector<DataStruct> data;
    //std::istringstream iss(
    //    "{ \"key1\": 0xFFFA, \"key2\": #c(1.0 -1.0), \"key3\": \"Hello\" }\n"
    //    "{ \"key1\": 0X0100f, \"key2\": #c(-1.0 1.0), \"key3\": \"World\" }\n"
    //    "{ \"key1\": 0xFFFA, \"key2\": #c(1.0 -1.0), \"key3\": \"Hi\" }"
    //    "{ \"key2\": #c(1.0 -1.0), \"key3\": \"Hi\", \"key1\": 0xFFFA }"
    //);

    //std::copy(
    //    std::istream_iterator<DataStruct>(iss),
    //    std::istream_iterator<DataStruct>(),
    //    std::back_inserter(data)
    //);

    while (std::cin.good()) {
        std::copy(
            std::istream_iterator<DataStruct>(std::cin),
            std::istream_iterator<DataStruct>(),
            std::back_inserter(data)
        );
        if (std::cin.fail() && !std::cin.eof())
        {
            std::cin.clear();
            std::cin.ignore();
        }
    }

    std::sort(data.begin(), data.end(), compareDataStruct);

    std::copy(
        std::begin(data),
        std::end(data),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
