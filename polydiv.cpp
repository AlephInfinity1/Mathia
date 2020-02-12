#include <iostream>
#include "mathutil.hpp"

int main()
{
    Polynomial P = input_polynomial("dividend");
    Polynomial Q = input_polynomial("divisor");
    Polynomial quotient = P / Q;
    Polynomial remainder = P % Q;
    std::cout << "The quotient is " + quotient.to_string() << std::endl;
    std::cout << "The remainder is " + remainder.to_string() << std::endl;
    return 0;
}
