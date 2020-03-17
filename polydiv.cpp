#include <iostream>
#include "mathutil.h"

int main()
{
    Polynomial P = inputPolynomial("dividend");
    Polynomial Q = inputPolynomial("divisor");
    Polynomial quotient = P / Q;
    Polynomial remainder = P % Q;
    std::cout << "The quotient is " + quotient.toString() << std::endl;
    std::cout << "The remainder is " + remainder.toString() << std::endl;
    return 0;
}
