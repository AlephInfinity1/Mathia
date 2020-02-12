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
        if(enable_debug_info)
        {
            std::cout << "(Debug) ";
        }
        if(calclock)
        {
            std::cout << "(Calc Lock) ";
        }
        std::cout << "> ";
        std::getline(std::cin, input);
        load_command(input);
    }
    return 0;
}
