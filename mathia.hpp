#include <iostream>
#include <cstdio>
#include "mathutil.hpp"

const char numLowerBound = '0'; //Value of the lower bound and upper bound values of numerals respectively ('0' and '9')
const char numUpperBound = '9';
bool enableDebugInfo = false; //Whether to display debug information after every command.
const int maxPriority = 2; //Specifies the maximum priority of the objects.
//char numerals[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
char operators[] = {'+', '-', '*', '/', '%', '^'};
bool session = true;
bool calclock = false;

void info() //Displays information message.
{
    std::cout << "Mathia Alpha v0.0.5 by zhang0313" << std::endl;
    std::cout << "Added help, forcedef, and rcl." << std::endl;
}

class Object
{
    private:
        std::string content; //Content of the object, as string.
        std::string type; //Possible types: null, number, operator, function (to be implemented)
        double value;
        int priority;
        Polynomial poly; //Polynomial value of the object.
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

class Variable
{
    private:
        std::string name; //Name of the variable, refered to in def.
        std::string type; //Type of the variable. Currently only "polynomial" is allowed
        Polynomial value; //The polynomial value of the variable.
    public:
        Variable(std::string n, std::string t, Polynomial v)
        {
            name = n;
            type = t;
            value = v;
        }
        std::string getName() //Getter functions
        {
            return name;
        }
        std::string getType()
        {
            return type;
        }
        Polynomial getValue()
        {
            return value;
        }
        void setName(std::string newName) //Setter functions
        {
            name = newName;
        }
        void setType(std::string newType)
        {
            type = newType;
        }
        void setValue(Polynomial newValue)
        {
            value = newValue;
        }
};

void addObject(std::string, std::string); //Adds the object into the list of objects.
void debug(); //Displays debug information.
int searchNum(int, std::string); //Searches the end of a value. begin: the beginning index to search from. str: the string to search from.
void compute(std::string); //Computes the equation
void logout(); //exits the program
void toggleDebug(std::string); //Toggles whether to display debug information
void toggleCalclock(); //Toggles calc lock (while under calc lock, the command is calc by default.)
void define(std::string, bool); //Defines a new variable.
void recall(std::string); //Recalls a variable by name.
void help(std::string); //Displays the information specified by the string.

std::vector<Object> objects; //Lists all the objects in the command line.
std::vector<Variable> vars; //Lists all the variable defined so far in the program.
int ptr; //An index used to divide the command into values.

void loadCommand(std::string command)
{
    if(calclock)
    {
        if(command.find("calclock") == 0)
        {
            toggleCalclock();
        }
        else if(command.find("logout") == 0 || command.find("exit") == 0 || command.find("quit") == 0)
        {
            logout();
        }
        else
        {
            compute(command);
        }

    }

    int i = command.find_first_of(' ', 0);
    std::string cmd;
    std::string para;
    if(i != std::string::npos)
    {
        cmd = command.substr(0, i);
        para = command.substr(i + 1);
    }
    else
    {
        cmd = command;
        para = "";
    }
    if(cmd == "compute" || cmd == "calc")
    {
        compute(para);
    }
    else if(cmd == "help")
    {
        help(para);
    }
    else if(cmd == "exit" || cmd == "logout" || cmd == "quit")
    {
        logout();
    }
    else if(cmd == "info")
    {
        info();
    }
    else if(cmd == "debug")
    {
        toggleDebug(para);
    }
    else if(cmd == "calclock")
    {
        toggleCalclock();
    }
    else if(cmd == "def")
    {
        define(para, false);
    }
    else if(cmd == "rcl" || cmd == "recall")
    {
        recall(para);
    }
    else if(cmd == "forcedef" || cmd == "fdef")
    {
        define(para, true);
    }
    else
    {
        std::cout << "Mathia: command \"" << cmd << "\" not found." << std::endl;
    }
    if(enableDebugInfo)
    {
        std::cout << cmd << std::endl;
        std::cout << para << std::endl;
    }
}

void compute(std::string command)
{
    if(command.size() <= 1)
    {
        return;
    }
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

void logout()
{
    std::cout << "\n[Session Complete]\n" << std::endl;
    session = false;
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

void toggleDebug(std::string para)
{
    if(para == "true" || para == "TRUE" || para == "1")
    {
        enableDebugInfo = true;
        std::cout << "Debug information enabled" << std::endl;
    }
    else if(para == "false" || para == "FALSE" || para == "0")
    {
        enableDebugInfo = false;
        std::cout << "Debug information disabled" << std::endl;
    }
    else
    {
        std::cout << "debug: invalid parameters (true/false accepted)" << std::endl;
    }

}

void toggleCalclock()
{
    if(calclock)
    {
        calclock = false;
        std::cout << "Calc Lock disabled" << std::endl;
        return;
    }
    else
    {
        calclock = true;
        std::cout << "Calc Lock enabled" << std::endl;
    }
}

void define(std::string para, bool force)
{
    int i = para.find_first_of(' ', 0);
    std::string type;
    std::string name;
    if(i != std::string::npos)
    {
        type = para.substr(0, i);
        name = para.substr(i + 1);
    }
    if(force == false) //If force def is not enabled, check if a variable by the same name already exists.
    {
        for(Variable x : vars)
        {
            if(x.getName() == para)
            {
                std::cout << "def: a variable by the name \"" << para << "\" already exists. Use forcedef if you want to redefine it." << std::endl;
                return;
            }
        }
    }
    if(type == "polynomial")
    {
        Polynomial P = inputPolynomial(name);
        Variable newVar (name, type, P);
        vars.push_back(newVar);
    }
    else
    {
        std::cout << "def: \"" << type << "\" is not a valid variable type." << std::endl;
        return;
    }

    std::cout << "Successfully created new " << type << " " << name << "." << std::endl;
}

void recall(std::string var)
{
    for(Variable x : vars)
    {
        if(x.getName() == var)
        {
            if(x.getType() == "polynomial")
            {
                std::cout << " > " << x.getValue().toString() << std::endl;
                return;
            }
        }
    }
    //If no variables match the name.
    std::cout << "recall: variable \"" << var << "\" does not exist. Use def to create one." << std::endl;
}

void help(std::string cmd)
{
    if(cmd == "compute" || cmd == "calc")
    {
        std::cout << "calc <expression>: returns the calculated value of the expression." << std::endl;
    }
    else if(cmd == "help")
    {
        std::cout << "help <cmd>: displays this help page. <cmd>: the command whose information is to be displayed." << std::endl;
    }
    else if(cmd == "exit" || cmd == "logout" || cmd == "quit")
    {
        std::cout << "logout: exits the session." << std::endl;
    }
    else if(cmd == "info")
    {
        std::cout << "info: shows the information of the newest update." << std::endl;
        std::cout << "For more detailed changelog, go to https://github.com/zhang0313/Mathia." << std::endl;
    }
    else if(cmd == "debug")
    {
        std::cout << "debug <option>: toggles debug information. <option>: true or false." << std::endl;
    }
    else if(cmd == "calclock")
    {
        std::cout << "calclock: toggles calc lock. While under calc lock, everything is run through calc." << std::endl;
    }
    else if(cmd == "def")
    {
        std::cout << "def <type> <name>: defines a variable of type <type> by the name <name>." << std::endl;
    }
    else if(cmd == "rcl" || cmd == "recall")
    {
        std::cout << "recall <name>: displays the value of the variable <name>." << std::endl;
    }
    else if(cmd == "forcedef" || cmd == "fdef")
    {
        std::cout << "forcedef <type> <name>: defines a variable of type <type> by the name <name>. Overwrites any previous data." << std::endl;
    }
    else
    {
        std::cout << "Use help <cmd> for more specific explanation of commands." << std::endl;
        std::cout << "A list of commands" << std::endl;
        std::cout << "calc\nhelp\nlogout\ninfo\ndebug\ncalclock\ndef\nrcl\nforcedef" << std::endl;
    }
}
