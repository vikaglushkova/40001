#include "datastruct.hpp"
#include <sstream>
#include <cctype>
#include <vector>
#include <algorithm>

namespace {
    void trim(std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
    }

    bool parseDouble(const std::string& str, double& value) {
        try {
            size_t pos = 0;
            value = std::stod(str, &pos);
            if (pos < str.size() && (str[pos] == 'd' || str[pos] == 'D')) pos++;
            return pos == str.size();
        }
        catch (...) {
            return false;
        }
    }

    bool parseLongLong(const std::string& str, long long& value) {
        try {
            size_t pos = 0;
            value = std::stoll(str, &pos);
            if (pos < str.size() && (str[pos] == 'l' || str[pos] == 'L')) pos += 2;
            return pos == str.size();
        }
        catch (...) {
            return false;
        }
    }

    bool parseString(std::string str, std::string& value) {
        trim(str);
        if (str.size() < 2 || str.front() != '"' || str.back() != '"') return false;
        value = str.substr(1, str.size() - 2);
        return true;
    }
}

std::istream& operator>>(std::istream& is, DataStruct& ds) {
    std::string line;
    while (std::getline(is, line)) {
        trim(line);
        if (line.empty()) continue;

        if (line.front() != '(' || line.back() != ')') continue;

        line = line.substr(1, line.size() - 2);
        std::vector<std::string> parts;
        std::istringstream iss(line);
        std::string part;

        while (std::getline(iss, part, ':')) {
            trim(part);
            if (!part.empty()) parts.push_back(part);
        }

        DataStruct temp;
        bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

        for (const auto& p : parts) {
            if (p.find("key1") == 0) {
                std::string val = p.substr(4);
                trim(val);
                if (!parseDouble(val, temp.key1)) break;
                hasKey1 = true;
            }
            else if (p.find("key2") == 0) {
                std::string val = p.substr(4);
                trim(val);
                if (!parseLongLong(val, temp.key2)) break;
                hasKey2 = true;
            }
            else if (p.find("key3") == 0) {
                std::string val = p.substr(4);
                if (!parseString(val, temp.key3)) break;
                hasKey3 = true;
            }
        }

        if (hasKey1 && hasKey2 && hasKey3) {
            ds = temp;
            return is;
        }
    }

    is.setstate(std::ios::failbit);
    return is;
}

std::ostream& operator<<(std::ostream& os, const DataStruct& ds) {
    os << "(:key1 " << ds.key1 << "d :key2 " << ds.key2 << "ll :key3 \"" << ds.key3 << "\" :)";
    return os;
}
