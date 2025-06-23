#include "datastruct.hpp"
#include <sstream>
#include <cctype>
#include <stdexcept>

bool parseDouble(const std::string& str, double& value) {
    try {
        size_t pos;
        value = std::stod(str, &pos);
        if (pos != str.size() && str.back() != 'd' && str.back() != 'D') {
            return false;
        }
        return true;
    } catch (...) {
        return false;
    }
}

bool parseLongLong(const std::string& str, long long& value) {
    try {
        size_t pos;
        value = std::stoll(str, &pos);
        if (pos != str.size() && (str.find("LL") == std::string::npos && str.find("ll") == std::string::npos)) {
            return false;
        }
        return true;
    } catch (...) {
        return false;
    }
}

bool parseString(const std::string& str, std::string& value) {
    if (str.size() < 2 || str.front() != '"' || str.back() != '"') {
        return false;
    }
    value = str.substr(1, str.size() - 2);
    return true;
}

std::istream& operator>>(std::istream& in, DataStruct& data) {
    std::string line;
    if (!std::getline(in, line)) {
        return in;
    }

    if (line.empty() || line.front() != '(' || line.back() != ')') {
        in.setstate(std::ios::failbit);
        return in;
    }

    line = line.substr(1, line.size() - 2);
    DataStruct temp;
    bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

    size_t pos = 0;
    while (pos < line.size()) {
        while (pos < line.size() && (line[pos] == ' ' || line[pos] == ':')) {
            pos++;
        }
        if (pos >= line.size()) break;

        size_t key_end = line.find(' ', pos);
        if (key_end == std::string::npos) {
            in.setstate(std::ios::failbit);
            return in;
        }

        std::string key = line.substr(pos, key_end - pos);
        pos = key_end + 1;

        size_t value_end = line.find(':', pos);
        if (value_end == std::string::npos) {
            value_end = line.size();
        }

        std::string value_str = line.substr(pos, value_end - pos);
        value_str.erase(0, value_str.find_first_not_of(" \t"));
        value_str.erase(value_str.find_last_not_of(" \t") + 1);

        if (key == "key1") {
            if (!parseDouble(value_str, temp.key1)) {
                in.setstate(std::ios::failbit);
                return in;
            }
            hasKey1 = true;
        } else if (key == "key2") {
            if (!parseLongLong(value_str, temp.key2)) {
                in.setstate(std::ios::failbit);
                return in;
            }
            hasKey2 = true;
        } else if (key == "key3") {
            if (!parseString(value_str, temp.key3)) {
                in.setstate(std::ios::failbit);
                return in;
            }
            hasKey3 = true;
        } else {
            in.setstate(std::ios::failbit);
            return in;
        }

        pos = value_end;
    }

    if (!hasKey1 || !hasKey2 || !hasKey3) {
        in.setstate(std::ios::failbit);
        return in;
    }

    data = temp;
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
    out << "(:key1 " << data.key1 << "d:key2 " << data.key2 << "ll:key3 \"" << data.key3 << "\":)";
    return out;
}

bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.length() < b.key3.length();
}
