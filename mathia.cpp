#include <iostream>
#include "mathia.hpp"

std::string input;
char c;

int main()
{
    info();
    while(true)
    {
        std::cout << "Mathia > ";
        std::getline(std::cin, input);
        loadCommand(input);
    }
}
