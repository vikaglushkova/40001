#include "datastruct.hpp"

namespace custom {
    bool compareDataStructs(const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) return a.key1 < b.key1;
        if (a.key2 != b.key2) return a.key2 < b.key2;
        return a.key3.length() < b.key3.length();
    }

    bool isValidDataStruct(const DataStruct& ds) {
        return !(std::isnan(ds.key1) || std::isinf(ds.key1) || ds.key3.empty());
    }

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        char c;
        if (in >> c && c != dest.expected) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, DoubleLitIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        double value;
        if (in >> value) {
            dest.value = value;
        } else {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, LongLongLitIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        long long value;
        if (in >> value) {
            dest.value = value;
        } else {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        in >> DelimiterIO{'"'};
        std::getline(in, dest.value, '"');
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        DataStruct temp;
        in >> DelimiterIO{'('} >> DelimiterIO{':'};

        std::string field;
        while (in >> field && field != ")") {
            if (field == "key1") {
                in >> DoubleLitIO{temp.key1} >> DelimiterIO{':'};
            } else if (field == "key2") {
                in >> LongLongLitIO{temp.key2} >> DelimiterIO{':'};
            } else if (field == "key3") {
                in >> StringIO{temp.key3} >> DelimiterIO{':'};
            } else {
                in.setstate(std::ios::failbit);
                break;
            }
        }

        if (in && isValidDataStruct(temp)) {
            dest = temp;
        } else {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
        std::ostream::sentry sentry(out);
        if (!sentry) return out;

        IOFormatGuard guard(out);
        out << "(:key1 " << data.key1
            << ":key2 " << data.key2
            << ":key3 \"" << data.key3 << "\":)";
        return out;
    }

    IOFormatGuard::IOFormatGuard(std::basic_ios<char>& stream) :
        stream_(stream),
        width_(stream.width()),
        fill_(stream.fill()),
        precision_(stream.precision()),
        flags_(stream.flags()) {}

    IOFormatGuard::~IOFormatGuard() {
        stream_.width(width_);
        stream_.fill(fill_);
        stream_.precision(precision_);
        stream_.flags(flags_);
    }
}
