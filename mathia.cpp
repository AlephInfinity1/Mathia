#include <iostream>
#include "mathia.hpp"

std::string input;
char c;

int main()
{
    session = true;
    info();
    while(session)
    {
        std::cout << "Mathia > ";
        std::getline(std::cin, input);
        loadCommand(input);
    }
}
