#include "datastruct.hpp"
#include <iomanip>
#include <sstream>
#include <algorithm>

namespace custom {
    bool compareDataStructs(const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) return a.key1 < b.key1;
        if (a.key2 != b.key2) return a.key2 < b.key2;
        return a.key3.length() < b.key3.length();
    }

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        char c;
        if (in >> c && c != dest.exp) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, DoubleLitIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        double value;
        char suffix;
        if (in >> value >> suffix && (suffix == 'd' || suffix == 'D')) {
            dest.ref = value;
        } else {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, LongLongLitIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        long long value;
        std::string suffix;
        if (in >> value) {
            char next = in.peek();
            if (next == 'L' || next == 'l') {
                in >> suffix;
                if (suffix != "LL" && suffix != "ll") {
                    in.setstate(std::ios::failbit);
                }
            }
            dest.ref = value;
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        DataStruct temp;
        in >> DelimiterIO{'('} >> DelimiterIO{':'};

        std::string key;
        while (in >> key && key != ")") {
            if (key == "key1") {
                in >> DoubleLitIO{temp.key1} >> DelimiterIO{':'};
            } else if (key == "key2") {
                in >> LongLongLitIO{temp.key2} >> DelimiterIO{':'};
            } else if (key == "key3") {
                in >> StringIO{temp.key3} >> DelimiterIO{':'};
            } else {
                in.setstate(std::ios::failbit);
                break;
            }
        }

        if (in) dest = temp;
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
        std::ostream::sentry sentry(out);
        if (!sentry) return out;

        out << "(:key1 " << data.key1 << "d"
            << ":key2 " << data.key2 << "LL"
            << ":key3 \"" << data.key3 << "\":)";
        return out;
    }
}
