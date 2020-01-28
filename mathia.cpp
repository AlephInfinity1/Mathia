#include <iostream>
#include "mathia.hpp"

std::string input;
char c;

int main()
{
    vars.clear();
    session = true;
    info();
    while(session)
    {
        std::cout << "Mathia ";
        if(enableDebugInfo)
        {
            std::cout << "(Debug Info Enabled) ";
        }
        if(calclock)
        {
            std::cout << "(Calc Lock Enabled) ";
        }
        std::cout << "> ";
        std::getline(std::cin, input);
        loadCommand(input);
    }
}
