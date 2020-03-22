#include <iostream>
#include "mathutil.h"

int main()
{
    Polynomial P = inputPolynomial("multiple");
    Polynomial Q = inputPolynomial("multiplier");
    Polynomial product = P * Q;
    std::cout << "The product is " + product.to_string() << std::endl;
    return 0;
}
