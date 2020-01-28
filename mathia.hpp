#include <iostream>
#include <cstdio>
#include "mathutil.hpp"

const char numLowerBound = '0'; //Value of the lower bound and upper bound values of numerals respectively ('0' and '9')
const char numUpperBound = '9';
const bool enableDebugInfo = false; //Whether to display debug information after every command.
const int maxPriority = 2; //Specifies the maximum priority of the objects.
//char numerals[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
char operators[] = {'+', '-', '*', '/', '%', '^'};

void info() //Displays information message.
{
    std::cout << "Mathia Alpha v0.2 by zhang0313" << std::endl;
    std::cout << "Basic arithmetic functions are now available, though they are still a bit buggy." << std::endl;
}

class Object
{
    private:
        std::string content; //Content of the object, as string.
        std::string type; //Possible types: null, number, operator, function (to be implemented)
        double value; //The numerical value of the NUMBER.
        int priority; //Priority of the operator.
    public:
        Object()
        {
            content = "";
            type = "null";
            value = 0;
            priority = 0;
        }
        Object(std::string c, std::string t)
        {
            content = c;
            type = t;
        }
        Object(std::string c, std::string t, int v)
        {
            if(t.compare("operator") == 0) //Checks the operator type.
            {
                content = c;
                type = t;
                priority = v;
            }
            else if(t.compare("number") == 0)
            {
                content = c;
                type = t;
                value = v;
            }

        }
        Object(std::string c, std::string t, double v)
        {
            content = c;
            type = t;
            value = v;
        }
        std::string getContent()
        {
            return content;
        }
        std::string getType()
        {
            return type;
        }
        double getNumericValue()
        {
            return value;
        }
        int getPriority()
        {
            return priority;
        }
        void setContent(std::string newContent)
        {
            content = newContent;
        }
        void setType(std::string newType)
        {
            type = newType;
        }
        void setNumericValue(double newValue)
        {
            value = newValue;
        }
        void setPriority(int newPriority)
        {
            priority = newPriority;
        }
        void update() //Updates the object information based on the object type and content.
        {
            if(type == "number")
            {
                value = std::stod(content);
            }
            if(type == "operator")
            {
                switch(content[0])
                {
                    case '+':
                    case '-':
                        priority = 1;
                        break;
                    case '*':
                    case '/':
                    case '%':
                        priority = 2;
                        break;
                    case '^':
                        priority = 3;
                        break;
                    default:
                        priority = -1;
                        break;
                }
            }
        }
};

void addObject(std::string, std::string); //Adds the object into the list of objects.
void debug(); //Displays debug information.
int searchNum(int, std::string); //Searches the end of a value. begin: the beginning index to search from. str: the string to search from.

std::vector<Object> objects; //Lists all the objects in the command line.
int ptr; //An index used to divide the command into values.

void loadCommand(std::string command)
{
    int i, j;
    for(i = 0; i < command.size(); i++)
    {
        if(command[i] >= numLowerBound && command[i] <= numUpperBound)
        {
            ptr = searchNum(i, command);
            addObject(command.substr(i, ptr - i), "number");
            i = ptr;
        }
        for(j = 0; j < 6; j++)
        {
            if(command[i] == operators[j])
            {
                addObject(command.substr(i, 1), "operator");
                break;
            }
        }
    }
    if(enableDebugInfo)
    {
        debug();
    }
    for(i = maxPriority; i > 0; i--)
    {
        for(j = 0; j < objects.size(); j++)
        {
            Object* objPtr = &objects[j];
            if(objects[j].getType() == "operator" && objects[j].getPriority() == i)
            {
                double val1 = (objPtr - 1) -> getNumericValue();
                double val2 = (objPtr + 1) -> getNumericValue();
                switch(objects[j].getContent()[0])
                {
                    case '+':
                        addObject(std::to_string(val1 + val2), "number");
                        break;
                    case '-':
                        addObject(std::to_string(val1 - val2), "number");
                        break;
                    case '*':
                        addObject(std::to_string(val1 * val2), "number");
                        break;
                    case '/':
                        addObject(std::to_string(val1 / val2), "number");
                        break;
                    case '^':
                        addObject(std::to_string(pow(val1, val2)), "number");
                        break;
                }
                objects.erase(objects.begin() + j - 1, objects.begin() + j + 2);
                j = 0;
            }
        }
    }
    std::cout << " > " << objects[0].getContent() << std::endl;
    objects.clear();
}

int searchNum(int begin, std::string str) //A function to search for the end of the value, and returns the index.
{
    int i, j;
    bool decimalPlace = false;
    for(i = begin; i < str.size(); i++)
    {
        if(str[i] >= numLowerBound && str[i] <= numUpperBound)
        {
            goto CHECK;
        }
        if(str[i] == '.' && !decimalPlace)
        {
            decimalPlace = true;
            goto CHECK;
        }
        return i;
        CHECK:
        ;
    }
    return (int) str.size();
}

void addObject(std::string content, std::string type) //Adds an object to objects.
{
    Object newObject (content, type);
    newObject.update();
    objects.push_back(newObject);
}

void debug() //Displays debug information.
{
    int i;
    for(i = 0; i < objects.size(); i++)
    {
        std::cout << objects[i].getContent() << std::endl;
        std::cout << objects[i].getType() << std::endl;
    }
}
