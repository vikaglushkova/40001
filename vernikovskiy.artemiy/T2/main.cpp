#include <iostream>
#include "task.h"


int main() {
    std::vector<DataStruct> dataVec;
    std::string line;

    std::cout << "Ready" << std::endl;
    while (true) {
        std::getline(std::cin, line);
        if (line == "END") { 
            break;
        }
        DataStruct data;
        if (parseDataStruct(line, data)) {
            dataVec.push_back(data);
        }
        else
        {
            std::cout << "wrong format" << std::endl;
        }
    }


    std::sort(dataVec.begin(), dataVec.end(), compareDataStruct);

    std::copy(dataVec.begin(), dataVec.end(), std::ostream_iterator<DataStruct>(std::cout, "\n"));

    return 0;
}
