#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <iostream>
#include <string>
#include <limits>

namespace custom {
    struct DataStruct {
        double key1;
        long long key2;
        std::string key3;
    };

    struct DelimiterIO {
        char exp;
    };

    struct DoubleLitIO {
        double& ref;
    };

    struct LongLongLitIO {
        long long& ref;
    };

    struct StringIO {
        std::string& ref;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, DoubleLitIO&& dest);
    std::istream& operator>>(std::istream& in, LongLongLitIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, DataStruct& dest);
    std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

    bool compareDataStructs(const DataStruct& a, const DataStruct& b);
}

#endif
