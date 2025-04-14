#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <iosfwd>
#include <string>

namespace trojan
{
    struct DataStruct 
    {
        double key1;
        unsigned long long key2;
        std::string key3;
    };
    std::istream& operator>>(std::istream& stream, DataStruct& value);
    std::ostream& operator<<(std::ostream& stream, const DataStruct& value);
    
    struct DoubleLiteral
    {
        double& link;
    };
    std::istream& operator>>(std::istream& stream, DoubleLiteral& value);
    std::ostream& operator<<(std::ostream& stream, const DoubleLiteral& value);
    struct UllLiteral
    {
        unsigned long long& link;
    };
    std::istream& operator>>(std::istream& stream, UllLiteral& value);
    std::ostream& operator<<(std::ostream& stream, const UllLiteral& value);
    struct String
    {
        std::string& link;
    };
    std::istream& operator>>(std::istream& stream, String& value);
    std::ostream& operator<<(std::ostream& stream, const String& value);
}

#endif
