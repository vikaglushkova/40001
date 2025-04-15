#include <iomanip>
#include "DataStruct.h"

namespace alymov 
{

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        char c = '0';
        in >> c;
        if (in && (c != dest.exp)) 
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, ULLHexIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        in >> std::hex >> dest.ref;
        return in;
    }

    std::istream& operator>>(std::istream& in, ComplexIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        double real = 0.0;
        double imag = 0.0;
        in >> DelimiterIO{ '#' } >> DelimiterIO{ 'c' } >> DelimiterIO{ '(' };
        in >> real >> imag;
        in >> DelimiterIO{ ')' };
        dest.ref = std::complex<double>(real, imag);
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& in, LabelIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        std::string data;
        if ((in >> StringIO{ data }) && (data != dest.exp)) 
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest) {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        DataStruct input;
        using sep = DelimiterIO;
        using label = LabelIO;
        using ullHex = ULLHexIO;
        using cmp = ComplexIO;
        using str = StringIO;
        std::string key = "";
        in >> sep{ '(' } >> sep{ ':' };
        for (int i = 0; i < 3; i++) 
        {
            in >> key;
            if (key == "key1") 
            {
                in >> std::hex >> ullHex{ input.key1 } >> std::dec;
            }
            else if (key == "key2") 
            {
                in >> cmp{ input.key2 };
            }
            else if (key == "key3") 
            {
                in >> str{ input.key3 };
            }
            else 
            {
                in.setstate(std::ios::failbit);
            }
            in >> sep{ ':' };
        }
        in >> sep{ ')' };
        if (in) 
        {
            dest = input;
        }
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
        std::ostream::sentry sentry(out);
        if (!sentry)
        {
            return out;
        }
        StreamGuard guard(out);
        out << "(:key1 0x" << std::uppercase << std::hex << src.key1
            << ":key2 " << std::fixed << std::setprecision(1)
            << "#c(" << src.key2.real() << " " << src.key2.imag() << ")"
            << ":key3 \"" << src.key3 << "\":)";
        return out;
    }

    bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) 
        {
            return a.key1 < b.key1;
        }
        if (a.key2 != b.key2) 
        {
            return a.key2.real() < b.key2.real() || (a.key2.real() == b.key2.real() && a.key2.imag() < b.key2.imag());
        }
        return a.key3.length() < b.key3.length();
    }
}
