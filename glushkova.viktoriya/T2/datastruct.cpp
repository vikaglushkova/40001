#include "datastruct.hpp"
#include <iomanip>
#include <sstream>
#include <limits>

namespace custom {
    bool compareDataStructs(const DataStruct& a, const DataStruct& b) {
        if (std::abs(a.key1) != std::abs(b.key1))
            return std::abs(a.key1) < std::abs(b.key1);

        double a_ratio = static_cast<double>(a.key2.first) / a.key2.second;
        double b_ratio = static_cast<double>(b.key2.first) / b.key2.second;
        if (a_ratio != b_ratio)
            return a_ratio < b_ratio;

        return a.key3.length() < b.key3.length();
    }

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        char c = '0';
        in >> c;
        if (in && (c != dest.exp)) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, ComplexIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        double real = 0.0, imag = 0.0;
        in >> DelimiterIO{'#'} >> DelimiterIO{'c'} >> DelimiterIO{'('}
           >> real >> imag >> DelimiterIO{')'};

        if (in) dest.ref = std::complex<double>(real, imag);
        return in;
    }

    std::istream& operator>>(std::istream& in, RationalIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        long long n = 0;
        unsigned long long d = 1;
        in >> DelimiterIO{'('} >> DelimiterIO{':'} >> DelimiterIO{'N'} >> n
           >> DelimiterIO{':'} >> DelimiterIO{'D'} >> d >> DelimiterIO{':'} >> DelimiterIO{')'};

        if (in && d != 0) {
            dest.ref = {n, d};
        } else {
            in.setstate(std::ios::failbit);
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

        DataStruct input;
        in >> DelimiterIO{'('} >> DelimiterIO{':'};

        std::string key;
        while (true) {
            in >> key;
            if (key == "key1") {
                in >> ComplexIO{input.key1} >> DelimiterIO{':'};
            } else if (key == "key2") {
                in >> RationalIO{input.key2} >> DelimiterIO{':'};
            } else if (key == "key3") {
                in >> StringIO{input.key3} >> DelimiterIO{':'};
            } else if (key == ")") {
                break;
            } else {
                in.setstate(std::ios::failbit);
                break;
            }
        }

        if (in) dest = input;
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
        std::ostream::sentry sentry(out);
        if (!sentry) return out;

        out << "(:key1 #c(" << data.key1.real() << " " << data.key1.imag() << ")"
            << ":key2 (:N " << data.key2.first << ":D " << data.key2.second << ":)"
            << ":key3 \"" << data.key3 << "\":)";
        return out;
    }
}
