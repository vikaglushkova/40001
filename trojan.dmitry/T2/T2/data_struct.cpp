#include "data_struct.hpp"

#include <iostream>
#include <iomanip>

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
