#include "datastruct.hpp"
#include <sstream>
#include <cmath>
#include <iomanip>

bool parseDouble(std::istream& in, double& value) {
    in >> value;
    if (!in) return false;

    if (in.peek() == 'd' || in.peek() == 'D') in.ignore();
    return true;
}

std::istream& operator>>(std::istream& in, DataStruct& dest) {
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line.front() != '(' || line.back() != ')') continue;

        std::istringstream iss(line.substr(1, line.size()-2));
        DataStruct temp;
        bool has_keys[3] = {false};

        for (std::string part; std::getline(iss, part, ':'); ) {
            size_t space = part.find(' ');
            if (space == std::string::npos) continue;

            std::string key = part.substr(0, space);
            std::string value = part.substr(space+1);
            std::istringstream val_stream(value);

            if (key == "key1") {
                if (!parseDouble(val_stream, temp.key1)) break;
                has_keys[0] = true;
            }
            else if (key == "key2") {
                if (!(val_stream >> temp.key2)) break;
                if (val_stream.peek() == 'L' || val_stream.peek() == 'l') {
                    val_stream.ignore();
                    if (val_stream.peek() == 'L' || val_stream.peek() == 'l') val_stream.ignore();
                }
                has_keys[1] = true;
            }
            else if (key == "key3") {
                if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
                    temp.key3 = value.substr(1, value.size()-2);
                    has_keys[2] = true;
                } else break;
            }
        }

        if (has_keys[0] && has_keys[1] && has_keys[2]) {
            dest = temp;
            return in;
        }
    }
    in.setstate(std::ios::failbit);
    return in;
}

bool compareDataStructs(const DataStruct& a, const DataStruct& b) {
    if (fabs(a.key1 - b.key1) > 1e-9) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.length() < b.key3.length();
}
