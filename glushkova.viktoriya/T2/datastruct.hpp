#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <iterator>
#include <cmath>
#include <algorithm>
#include <limits>

namespace custom {
    struct DataStruct {
        double key1;
        long long key2;
        std::string key3;
    };

    struct DelimiterIO {
        char expected;
    };

    struct DoubleLitIO {
        double& value;
    };

    struct LongLongLitIO {
        long long& value;
    };

    struct StringIO {
        std::string& value;
    };

    class IOFormatGuard {
    public:
        explicit IOFormatGuard(std::basic_ios<char>& stream);
        ~IOFormatGuard();
    private:
        std::basic_ios<char>& stream_;
        std::streamsize width_;
        char fill_;
        std::streamsize precision_;
        std::basic_ios<char>::fmtflags flags_;
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
