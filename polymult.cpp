#include <iostream>
#include "mathutil.hpp"

int main()
{
    Polynomial P = inputPolynomial("multiple");
    Polynomial Q = inputPolynomial("multiplier");
    Polynomial product = P * Q;
    std::cout << "The product is " + product.toString() << std::endl;
    return 0;
}
