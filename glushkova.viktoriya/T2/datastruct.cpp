#include "datastruct.hpp"
#include <sstream>
#include <cmath>
#include <iomanip>
#include <algorithm>

bool parseDouble(std::istream& in, double& value) {
    in >> value;
    if (!in) return false;
    if (in.peek() == 'd' || in.peek() == 'D') in.ignore();
    return true;
}

std::istream& operator>>(std::istream& in, DataStruct& dest) {
    std::string line;
    if (!std::getline(in, line)) return in;

    if (line.empty() || line.front() != '(' || line.back() != ')') {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::istringstream iss(line.substr(1, line.size()-2));
    DataStruct temp;
    bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

    for (std::string part; std::getline(iss, part, ':'); ) {
        if (part.empty()) continue;

        size_t space = part.find(' ');
        if (space == std::string::npos) continue;

        std::string key = part.substr(0, space);
        std::string value = part.substr(space+1);
        std::istringstream val_stream(value);

        if (key == "key1") {
            if (!parseDouble(val_stream, temp.key1)) break;
            hasKey1 = true;
        }
        else if (key == "key2") {
            if (!(val_stream >> temp.key2)) break;
            if (val_stream.peek() == 'L' || val_stream.peek() == 'l') {
                val_stream.ignore();
                if (val_stream.peek() == 'L' || val_stream.peek() == 'l') val_stream.ignore();
            }
            hasKey2 = true;
        }
        else if (key == "key3") {
            if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
                temp.key3 = value.substr(1, value.size()-2);
                hasKey3 = true;
            } else break;
        }
    }

    if (hasKey1 && hasKey2 && hasKey3) {
        dest = temp;
    } else {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
    out << "(:key1 " << std::fixed << std::setprecision(1)
        << data.key1 << "d:key2 " << data.key2 << "ll:key3 \""
        << data.key3 << "\":)";
    return out;
}

bool compareDataStructs(const DataStruct& a, const DataStruct& b) {
    if (fabs(a.key1 - b.key1) > 1e-9) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.length() < b.key3.length();
}
