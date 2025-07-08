#include "datastruct.hpp"
#include <sstream>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <cctype>

bool parseDouble(std::istream& in, double& value) {
    in >> value;
    if (!in) return false;
    if (std::tolower(in.peek()) == 'd') in.ignore();
    return true;
}

std::istream& operator>>(std::istream& in, DataStruct& dest) {
    std::string line;
    while (std::getline(in, line)) {
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty()) continue;

        if (line.front() != '(' || line.back() != ')') {
            continue;
        }

        std::istringstream iss(line.substr(1, line.size() - 2));
        DataStruct temp;
        bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

        while (iss) {
            char next;
            while (iss.get(next) && (next == ':' || std::isspace(next))) {}
            if (!iss) break;
            iss.unget();

            std::string key;
            if (!(iss >> key)) break;

            if (key == "key1") {
                if (!parseDouble(iss, temp.key1)) break;
                hasKey1 = true;
            }
            else if (key == "key2") {
                if (!(iss >> temp.key2)) break;
                if (std::tolower(iss.peek()) == 'l') {
                    iss.ignore();
                    if (std::tolower(iss.peek()) == 'l') iss.ignore();
                }
                hasKey2 = true;
            }
            else if (key == "key3") {
                char quote;
                while (iss.get(quote) && quote != '"') {}
                if (!iss) break;

                std::string value;
                std::getline(iss, value, '"');
                temp.key3 = value;
                hasKey3 = true;
            }
            else {
                break;
            }
        }

        if (hasKey1 && hasKey2 && hasKey3) {
            dest = temp;
            return in;
        }
    }
    in.setstate(std::ios::failbit);
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
    out << "(:key1 " << std::fixed << std::setprecision(1) 
        << data.key1 << "d:key2 " << data.key2 << "ll:key3 \"" 
        << data.key3 << "\":)";
    return out;
}

bool compareDataStructs(const DataStruct& a, const DataStruct& b) {
    const double eps = 1e-9;
    if (std::abs(a.key1 - b.key1) > eps) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.length() < b.key3.length();
}
