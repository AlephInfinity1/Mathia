#include <iostream>
#include "mathutil.h"

int main()
{
    Polynomial P = inputPolynomial();
    double n;
    std::cout << "Enter the number to substitute into the polynomial: " << std::endl;
    std::cin >> n;
    std::cout << "The resultant value is " << P.substitute(n) << "." << std::endl;
    return 0;
}
