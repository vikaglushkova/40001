#include "datastruct.hpp"
#include <sstream>
#include <cctype>
#include <iomanip>
#include <cmath>

bool parseDouble(std::istream& in, double& value) {
    std::string numStr;
    char c;
    while (in.get(c) && (isdigit(c) || c == '.' || c == '-' || c == '+' || c == 'e' || c == 'E')) {
        numStr += c;
    }

    if (numStr.empty()) return false;

    try {
        value = std::stod(numStr);
    } catch (...) {
        return false;
    }

    if (c == 'd' || c == 'D') {
        in.ignore();
    } else if (c != ':' && !isspace(c)) {
        in.putback(c);
        return false;
    }

    return true;
}

bool parseLongLong(std::istream& in, long long& value) {
    std::string numStr;
    char c;
    while (in.get(c) && (isdigit(c) || c == '-' || c == '+')) {
        numStr += c;
    }

    if (numStr.empty()) return false;

    try {
        value = std::stoll(numStr);
    } catch (...) {
        return false;
    }

    if (c == 'L' || c == 'l') {
        char next = in.peek();
        if (next == 'L' || next == 'l') {
            in.ignore();
        } else {
            return false;
        }
    } else if (c != ':' && !isspace(c)) {
        in.putback(c);
        return false;
    }

    return true;
}

std::istream& operator>>(std::istream& in, DataStruct& dest) {
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;

        if (line.front() != '(' || line.back() != ')') {
            continue;
        }

        std::istringstream iss(line.substr(1, line.size()-2));
        std::string part;
        bool hasKey1 = false, hasKey2 = false, hasKey3 = false;
        DataStruct temp;

        while (std::getline(iss, part, ':')) {
            if (part.empty()) continue;

            size_t space = part.find(' ');
            if (space == std::string::npos) continue;

            std::string key = part.substr(0, space);
            std::string value = part.substr(space+1);
            std::istringstream valStream(value);

            if (key == "key1") {
                if (!parseDouble(valStream, temp.key1)) continue;
                hasKey1 = true;
            }
            else if (key == "key2") {
                if (!parseLongLong(valStream, temp.key2)) continue;
                hasKey2 = true;
            }
            else if (key == "key3") {
                if (value.size() < 2 || value.front() != '"' || value.back() != '"') continue;
                temp.key3 = value.substr(1, value.size()-2);
                hasKey3 = true;
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
    out << "(:key1 " << std::fixed << std::setprecision(1) << data.key1 << "d"
        << ":key2 " << data.key2 << "ll"
        << ":key3 \"" << data.key3 << "\":)";
    return out;
}

bool compareDataStructs(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.length() < b.key3.length();
}
