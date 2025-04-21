#include "task.h"
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include "ScopeGuard.h"

namespace doomsday
{
    bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) {
            return a.key1 < b.key1;
        }
        if (a.key2 != b.key2) {
            return a.key2 < b.key2;
        }
        return a.key3.length() < b.key3.length();
    }

    std::istream& operator>>(std::istream& is, DelimiterIO&& dest)
    {
        char c = '0';
        is >> c;
        if (is && (c != dest.delim))
        {
            is.setstate(std::ios::failbit);
        }
        return is;
    }

    std::istream& operator>>(std::istream& is, BBDelimIO& dest)
    {
        char c = '0';
        is >> std::noskipws >> c;
        if (is && (c == dest.delim))
        {
            is.setstate(std::ios::failbit);
        }
        dest.eye = c;
        return is;
    }

    std::istream& operator>>(std::istream& is, CharIO&& dest)
    {
        char c = '0';
        is >> c;
        if (is)
        {
            dest.ref = c;
            return is;
        }
        is.setstate(std::ios::failbit);
        return is;
    }

    std::istream& operator>>(std::istream& is, StringIO&& dest)
    {
        std::string res = "";
        BBDelimIO mainDel{'"'};
        while (is >> mainDel)
        {
            res += mainDel.eye;
        }
        dest.ref = res;
        is.clear();
        return is;
    }

    std::istream& operator>>(std::istream& is, DoubleIO&& dest)
    {
        std::streampos startPos = is.tellg();
        BBDelimIO mainDel{':'};
        std::string stmp = "";
        while (is >> mainDel)
        {
            stmp += mainDel.eye;
        }
        try
        {
            dest.ref = std::stod(stmp);
        }
        catch (...)
        {
            is.setstate(std::ios::failbit);
            return is;
        }
        is.clear();
        return is;
    }

    std::istream& operator>>(std::istream& is, keyLable& dest)
    {
        BBDelimIO mainDel{' '};
        while (is >> mainDel)
        {
            dest.key += mainDel.eye;
        }
        is.clear();
        // is.setstate(std::ios::failbit);
        return is;
    }

    std::istream& operator>>(std::istream& is, DataStruct& data)
    {
        DataStruct res;
        if (is.eof())
        {
            return is;
        }

        if (!(is >> DelimiterIO{'('}))
        {
            is.setstate(std::ios::failbit);
            return is;
        }

        if (!(is >> DelimiterIO{':'})) // заглушка
        {
            is.setstate(std::ios::failbit);
            return is;
        }

        for (int i = 0; i < 3; i++)
        {
            keyLable key;
            if (!(is >> key))
            {
                is.setstate(std::ios::failbit);
                return is;
            }

            if (key.key == "key1")
            {
                if (!(is >> DoubleIO{ res.key1 }))
                {
                    is.setstate(std::ios::failbit);
                    return is;
                }
            }
            else if (key.key == "key2")
            {
                if (!(is >> DelimiterIO{'\''} >> CharIO{ res.key2 } >> DelimiterIO{'\''}))
                {
                    is.setstate(std::ios::failbit);
                    return is;
                }
                if (!(is >> DelimiterIO{':'})) // заглушка
                {
                    is.setstate(std::ios::failbit);
                    return is;
                }
            }
            else if (key.key == "key3")
            {
                if (!(is >> DelimiterIO{'"'} >> StringIO{ res.key3 }))
                {
                    is.setstate(std::ios::failbit);
                    return is;
                }
                if (!(is >> DelimiterIO{':'})) // заглушка
                {
                    is.setstate(std::ios::failbit);
                    return is;
                }
            }
            else
            {
                is.setstate(std::ios::failbit);
                return is;
            }
        }
        if (!(is >> DelimiterIO{')'})) // заглушка
        {
            is.setstate(std::ios::failbit);
            return is;
        }
        data = res;
        return is;
    }

    std::string formatScientific(double number) {
        std::ostringstream oss;
        oss << std::scientific << std::setprecision(1) << number;

        std::string result = oss.str();
        size_t pos = result.find('e');
        if (pos != std::string::npos) {
            result.erase(pos + 2, 1);
        }
        return result;
    }

    std::ostream & operator<<(std::ostream& os, const DataStruct& data) {
        std::ostream::sentry sentry(os);
        if (sentry) {
            StreamGuard guard(os);
            os << "(:key1 " << formatScientific(data.key1) << ":"
                << "key2 '" << data.key2 << "':"
                << "key3 \"" << data.key3 << "\":)";
        }
        return os;
    }
}











