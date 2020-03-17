#include <iostream>
#include "mathutil.hpp"

int main()
{
    Polynomial P = input_polynomial("multiple");
    Polynomial Q = input_polynomial("multiplier");
    Polynomial product = P * Q;
    std::cout << "The product is " + product.to_string() << std::endl;
    return 0;
}
