#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include "ScopeGuard.h"


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

// (:key1 1.0000000000e+01:key2 'c':key3 "D a t a'":)
// (:key1 -1.23:key3 "doomsday":key2 ' ':)


bool parseDataStruct(const std::string& input, DataStruct& data) {
    bool flag1 = false;
    bool flag2 = false;
    bool flag3 = false;

    if (input.empty())
        return false;

    std::string tmp;
    auto it = input.begin();

    while (it != input.end()) {
        if (*it == '(' || *it == ')' || *it == ':') {
            ++it;
            continue;
        }

        tmp.clear();
        while (it != input.end() && *it != '(' && *it != ')' && *it != ':' && *it != '\'' && *it != '"' && *it != ' ') {
            tmp += *it++;
        }


        if (tmp == "key1") {
            if (flag1) return false;
            it += 1;
            tmp.clear();
            while (it != input.end() && *it != ':') {
                tmp += *it++;
            }
            try {
                data.key1 = std::stod(tmp);
            } catch (...) {
                return false;
            }
            flag1 = true;
        } else if (tmp == "key2") {
            if (flag2) return false;
            it += 2;
            tmp.clear();
            while (it != input.end() && *it != '\'') {
                tmp += *it++;
            }
            if (tmp.size() != 1) return false;
            data.key2 = static_cast<char>(tmp[0]);
            flag2 = true;
            ++it;
        } else if (tmp == "key3") {
            if (flag3) return false;
            it += 2;
            tmp.clear();

            while (it != input.end() && *it != '"') {
                tmp += *it++;
            }

            data.key3 = tmp;
            flag3 = true;
            ++it;
        }
    }

    return (flag1 && flag2 && flag3);
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


std::ostream & operator<<(std::ostream& os, const DataStruct& data) {
    std::ostream::sentry sentry(os);
    if (sentry) {
        StreamGuard guard(os);
        os << "(:key1 " << std::scientific << std::setprecision(10) << data.key1 << ":"
            << "key2 '" << data.key2 << "':"
            << "key3 \"" << data.key3 << "\":)";
    }
    return os;
}


#endif
