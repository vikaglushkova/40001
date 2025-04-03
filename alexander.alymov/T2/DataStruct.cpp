#include <iomanip>
#include "DataStruct.hpp"

iofmtguard::iofmtguard(std::basic_ios< char >& s) :
    s_(s),
    width_(s.width()),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
{}

iofmtguard::~iofmtguard()
{
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
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

std::istream& operator>>(std::istream& in, ULLHexIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    in >> std::hex >> dest.ref;
    return in;
}

std::istream& operator>>(std::istream& in, ComplexIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    double real = 0.0, imag = 0.0;
    in >> DelimiterIO{ '#' } >> DelimiterIO{ 'c' } >> DelimiterIO{ '(' };
    in >> real >> imag;
    in >> DelimiterIO{ ')' };
    dest.ref = std::complex<double>(real, imag);
    return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& operator>>(std::istream& in, LabelIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    std::string data;
    if ((in >> StringIO{ data }) && (data != dest.exp)) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, DataStruct& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    DataStruct input;
    using sep = DelimiterIO;
    using label = LabelIO;
    using ullHex = ULLHexIO;
    using cmp = ComplexIO;
    using str = StringIO;

    std::map<std::string, bool> keysFound = {
        {"key1", false},
        {"key2", false},
        {"key3", false}
    };

    in >> sep{ '{' };

    for (int i = 0; i < 3; ++i) {
        std::string key;
        char colon;
        in >> sep{ '"' };
        std::getline(in, key, '"');
        in >> sep{ ':' };

        if (key == "key1" && !keysFound["key1"]) {
            in >> ullHex{ input.key1 };
            keysFound["key1"] = true;
        }
        else if (key == "key2" && !keysFound["key2"]) {
            in >> cmp{ input.key2 };
            keysFound["key2"] = true;
        }
        else if (key == "key3" && !keysFound["key3"]) {
            in >> str{ input.key3 };
            keysFound["key3"] = true;
        }
        else {
            in.setstate(std::ios::failbit);
            return in;
        }

        if (i < 2) {
            in >> sep{ ',' };
        }
    }

    in >> sep{ '}' };

    if (in && keysFound["key1"] && keysFound["key2"] && keysFound["key3"]) {
        dest = input;
    }
    else {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
    std::ostream::sentry sentry(out);
    if (!sentry) return out;
    iofmtguard fmtguard(out);
    out << "{ ";
    out << "\"key1\": " << std::hex << std::showbase << src.key1 << ", ";
    out << "\"key2\": #c(" << std::fixed << std::setprecision(1) << src.key2.real() << " " << src.key2.imag() << "), ";
    out << "\"key3\": \"" << src.key3 << "\"";
    out << " }";
    return out;
}

bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) {
        return a.key1 < b.key1;
    }
    if (a.key2 != b.key2) {
        return a.key2.real() < b.key2.real() || (a.key2.real() == b.key2.real() && a.key2.imag() < b.key2.imag());
    }
    return a.key3.length() < b.key3.length();
}
