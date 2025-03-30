#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iterator>


struct DataStruct {
    double key1;
    char key2;
    std::string key3;
};


std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter))
        tokens.push_back(token);
    return tokens;
}


bool parseDataStruct(const std::string& input, DataStruct& data) {
    if (input.empty())
        return false;

    std::vector<std::string> tokens = splitString(input, ':');

    if (tokens.size() != 5)
        return false;

    for (size_t i = 1; i < tokens.size() - 1; i++) {
        std::vector<std::string> tmp = splitString(tokens[i], ' ');

        if (tmp.size() < 2) {
            return false;
        }


        if (tmp[0] == "key1") {
            try {
                data.key1 = std::stod(tmp[1]);
            } catch (...) {
                return false;
            }
        } else if (tmp[0] == "key2") {
            if (tmp[1].size() == 3 && tmp[1][0] == '\'' && tmp[1][2] == '\'') {
                data.key2 = static_cast<char>(tmp[1][1]);
            } else {
                return false;
            }
        } else if (tmp[0] == "key3") {
            if (tmp[1].size() >= 2 && tmp[1][0] == '\"' && tmp[1][tmp[1].size() - 1] == '\"') {
                data.key3 = tmp[1].substr(1, tmp[1].size() - 2);
            } else {
                return false;
            }
        } else
            return false;
    }

    return true;
}


bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) {
        return a.key1 < b.key1;
    }
    if (a.key2 != b.key2) {
        return a.key2 < b.key2;
    }
    return a.key3.length() < b.key3.length();
}


std::ostream& operator<<(std::ostream& os, const DataStruct& data) {
    os << "(:key1 " << std::scientific << std::setprecision(10) << data.key1 << ":"
       << "key2 '" << data.key2 << "':"
       << "key3 \"" << data.key3 << "\":)";
    return os;
}


#endif
