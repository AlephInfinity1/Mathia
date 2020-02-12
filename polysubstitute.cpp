#include <iostream>
#include "mathutil.hpp"

int main()
{
    Polynomial P = input_polynomial("");
    double n;
    std::cout << "Enter the number to substitute into the polynomial: " << std::endl;
    std::cin >> n;
    std::cout << "The resultant value is " << P.substitute(n) << "." << std::endl;
    return 0;
}
