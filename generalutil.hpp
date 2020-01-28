#include <vector>
#include <sstream>
#include <iomanip>
#include <limits>

double* doubleVectorToArray(std::vector<double> vector)
{
    double* array = new double[vector.size()];
    for(int i = 0; i < vector.size(); i++)
    {
        array[i] = vector[i];
    }
    return array;
}

namespace my
{
    std::string to_string(double d)
    {
        std::ostringstream stm;
        stm << std::setprecision(std::numeric_limits<double>::digits10) << d ;
        return stm.str() ;
    }
}
//Credits to JLBorges from http://www.cplusplus.com/forum/general/229142/.