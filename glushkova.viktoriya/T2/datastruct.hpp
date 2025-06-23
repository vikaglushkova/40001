#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <iostream>
#include <string>

struct DataStruct {
    double key1 = 0.0;
    long long key2 = 0;
    std::string key3;

    bool operator<(const DataStruct& other) const {
        if (key1 != other.key1) return key1 < other.key1;
        if (key2 != other.key2) return key2 < other.key2;
        return key3.length() < other.key3.length();
    }
};

std::istream& operator>>(std::istream& is, DataStruct& ds);
std::ostream& operator<<(std::ostream& os, const DataStruct& ds);

#endif


