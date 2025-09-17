#include "datastruct.hpp"
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <limits>

int main()
{
    std::vector<DataStruct> data;

    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(data)
    );

    if (std::cin.fail() && !std::cin.eof())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::sort(data.begin(), data.end(), compareDataStructs);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
