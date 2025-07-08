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
    while (std::getline(in, line)) {
        if (line.empty() || line.front() != '(' || line.back() != ')') continue;

        size_t pos = 1;
        DataStruct temp;
        bool has_keys[3] = {false};

        while (pos < line.size() - 1) {
            size_t key_start = line.find(':', pos);
            if (key_start == std::string::npos) break;

            size_t space = line.find(' ', key_start);
            if (space == std::string::npos) break;

            std::string key = line.substr(key_start + 1, space - key_start - 1);
            size_t value_end = line.find(':', space + 1);
            if (value_end == std::string::npos) value_end = line.size() - 1;

            std::string value = line.substr(space + 1, value_end - space - 1);
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
                    temp.key3 = value.substr(1, value.size() - 2);
                    has_keys[2] = true;
                } else break;
            }
            pos = value_end;
        }

        if (has_keys[0] && has_keys[1] && has_keys[2]) {
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
    if (fabs(a.key1 - b.key1) > 1e-9) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.length() < b.key3.length();
}
