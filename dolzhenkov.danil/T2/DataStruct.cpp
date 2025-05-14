#include "DataStruct.h"

namespace nspace {
    bool dataStructCompare(const DataStruct& a, const DataStruct& b)
    {
        if (a.key1 != b.key1)
        {
            return a.key1 < b.key1;
        }

        if (a.key2 != b.key2)
        {
            return a.key2 < b.key2;
        }

        return (a.key3).length() < (b.key3).length();
    }

    std::string beautyDouble(double check)
    {
        std::stringstream ss;
        ss << std::scientific << check;
        std::string str = ss.str();
        size_t i = str.find('e');
        while (str[i - 1] == '0' && str[i - 2] != '.')
        {
            str.erase(i - 1, 1);
            i = str.find('e');
        }
        while (str[i + 2] == '0')
        {
            str.erase(i + 2, 1);
            i = str.find('e');
        }
        return str;
    }

    std::string llToString(long long key)
    {
        return std::to_string(key) + "ll";
    }

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
    {
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

    std::istream& operator>>(std::istream& in, DoubleIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        std::string check;
        getline(in, check, ':');
        in.unget();
        if (check[0] >= '1' && check[0] <= '9' && check[1] == '.')
        {
            double toReturn = stod(check);
            dest.ref = toReturn;
        }
        else
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& in, SllLitIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        std::string toCheck = "";
        getline(in, toCheck, ':');
        in.unget();

        size_t llPos = toCheck.find("ll");
        size_t LLPos = toCheck.find("LL");

        if (llPos == std::string::npos && LLPos == std::string::npos)
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        try {
            long long value = std::stoll(toCheck.substr(0, toCheck.size() - 2));
            dest.ref = value;
        }
        catch (...) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        DataStruct input;
        {
            using sep = DelimiterIO;
            using dbl = DoubleIO;
            using str = StringIO;
            using sll = SllLitIO;
            in >> sep{ '(' } >> sep{ ':' };
            for (size_t i = 0; i < 3; i++)
            {
                std::string label = "";
                in >> label;
                if (label == "key1")
                {
                    in >> dbl{ input.key1 };
                    in >> sep{ ':' };
                }
                else if (label == "key2")
                {
                    in >> sll{ input.key2 };
                    in >> sep{ ':' };
                }
                else if (label == "key3")
                {
                    in >> str{ input.key3 };
                    in >> sep{ ':' };
                }
                else
                {
                    in.setstate(std::ios::failbit);
                    return in;
                }
            }
            in >> sep{ ')' };
        }
        if (in)
        {
            dest = input;
        }
        return in;
    }

    std::ostream& operator<< (std::ostream& out, const DataStruct& data)
    {
        std::ostream::sentry sentry(out);
        if (!sentry)
        {
            return out;
        }
        iofmtguard fmtguard(out);
        out << "(";
        out << ":" << "key1" << " " << beautyDouble(data.key1);
        out << ":" << "key2" << " " << llToString(data.key2);
        out << ":" << "key3" << " " << '"' << data.key3 << '"';
        out << ":";
        out << ")";
        return out;
    }

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
}