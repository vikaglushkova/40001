#include "data_struct.hpp"
#include "stream_guard.hpp"

#include <iostream>
#include <iomanip>
#include <cstring>

std::ostream& trojan::operator<<(std::ostream& stream, const DoubleLiteral& value)
{
    std::ostream::sentry sentry(stream);
    if (!sentry) {
        return stream;
    }
    return stream << std::setprecision(1) << std::fixed << value.link << 'd';
}
std::ostream& trojan::operator<<(std::ostream& stream, const UllLiteral& value)
{
    std::ostream::sentry sentry(stream);
    if (!sentry) {
        return stream;
    }
    return stream << value.link << "ull";
}
std::ostream& trojan::operator<<(std::ostream& stream, const String& value)
{
    std::ostream::sentry sentry(stream);
    if (!sentry) {
        return stream;
    }
    return stream << '"' << value.link << '"';
}
std::istream& trojan::operator>>(std::istream& stream, Delimiter&& value)
{
    std::istream::sentry sentry(stream);
    if (!sentry) {
        return stream;
    }
    char c = '\0';
    stream >> c;
    if (c != value.exp) {
        stream.setstate(std::ios::failbit);
    }
    return stream;
}
std::istream& trojan::operator>>(std::istream& stream, DoubleLiteral&& value)
{
    std::istream::sentry sentry(stream);
    if (!sentry) {
        return stream;
    }
    StreamGuard guard(stream);
    double temp = 0.0;
    char c = '\0';
    if (!(stream >> temp >> std::noskipws >> c) && ((c != 'd') && (c != 'D'))) {
        stream.setstate(std::ios::failbit);
        return stream;
    }
    value.link = temp;
    return stream;
}
std::istream& trojan::operator>>(std::istream& stream, UllLiteral&& value)
{
    std::istream::sentry sentry(stream);
    if (!sentry) {
        return stream;
    }
    StreamGuard guard(stream);
    unsigned long long temp = 1;
    char c[4] = "\0";
    if ((stream.peek() == '-')
            && (!(stream >> temp >> std::noskipws >> c))
            && ((std::strcmp(c, "ull") != 0)
                && (std::strcmp(c, "ULL") != 0))) {
        stream.setstate(std::ios::failbit);
        return stream; 
    }
    value.link = temp;
    return stream;
}
std::istream& trojan::operator>>(std::istream& stream, String&& value)
{
    std::istream::sentry sentry(stream);
    if (!sentry) {
        return stream;
    }
    std::string temp;
    if (std::getline(stream >> Delimiter{ '"' }, temp, '"')) {
        value.link = temp;
    }
    return stream;
}
