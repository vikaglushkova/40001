#include "data_struct.hpp"

#include <iostream>

int main()
{
    double temp1 = 50.0;
    unsigned long long temp2 = 89;
    std::string temp3 = "***---***";
    trojan::DoubleLiteral dbl{temp1};
    std::cout << dbl << '\n';
    trojan::UllLiteral ull{temp2};
    std::cout << ull << '\n';
    trojan::String str{temp3};
    std::cout << str << '\n';
}
