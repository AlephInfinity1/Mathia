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

class Object;
class Variable;

std::vector<Object> objects; //Lists all the objects in the command line.
std::vector<Variable> vars; //Lists all the variable defined so far in the program.

class Object
{
    private:
        std::string content; //Content of the object, as string.
        std::string type; //Possible types: null, number, operator, function (to be implemented)
        double value;
        int priority;
        Polynomial poly; //Polynomial value of the object.
        int bracketLvl; //The number of nested brackets of the operator.
        int loc; //Index in the objects vector.
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
        /*Object(std::string c, std::string t, int v)
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
        }*/
        Object(std::string c, std::string t, int l)
        {
            content = c;
            type = t;
            loc = l;
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
        int getBracketLvl()
        {
            return bracketLvl;
        }
        int getLoc()
        {
            return loc;
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
        void setBracketLvl(int newLvl)
        {
            bracketLvl = newLvl;
        }
        void setLoc(int newLoc)
        {
            loc = newLoc;
        }
        void update() //Updates the object information based on the object type and content.
        {
            for(int i = 0; i < objects.size(); i++)
            {
                if(&objects[i] == this)
                {
                    loc = i;
                }
            }
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
                        priority = -1; //Invalid operator, will not be computed.
                        break;
                }
                bracketLvl = 0;
                for(int i = loc; i >= 0; i--)
                {
                    if(objects[i].getType() == "openingBracket" || objects[i].getContent() == "(")
                    {
                        bracketLvl = bracketLvl + 1;
                        if(enableDebugInfo)
                        {
                            std::cout << "Object: " << this -> getContent() << std::endl;
                            std::cout << "Update: found opening bracket." << std::endl;
                            std::cout << "bracketLvl is now " << bracketLvl << std::endl;
                            std::cout << "getBracketLvl returns " << this -> getBracketLvl() << std::endl;
                        }
                    }
                    if(objects[i].getType() == "closingBracket" || objects[i].getContent() == ")")
                    {
                        bracketLvl = bracketLvl - 1;
                        if(enableDebugInfo)
                        {
                            std::cout << "Object: " << this -> getContent() << std::endl;
                            std::cout << "Update: found closing bracket." << std::endl;
                            std::cout << "bracketLvl is now " << bracketLvl << std::endl;
                            std::cout << "getBracketLvl returns " << this -> getBracketLvl() << std::endl;
                        }
                    }
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

void addObject(std::string, std::string, int); //Adds the object into the list of objects.
void debug(int); //Displays debug information.
int searchNum(int, std::string); //Searches the end of a value. begin: the beginning index to search from. str: the string to search from.
void compute(std::string); //Computes the equation
void logout(); //exits the program
void toggleDebug(std::string); //Toggles whether to display debug information
void toggleCalclock(); //Toggles calc lock (while under calc lock, the command is calc by default.)
void define(std::string, bool); //Defines a new variable.
void recall(std::string); //Recalls a variable by name.
void help(std::string); //Displays the information specified by the string.
int getLeftNum(int); //Gets the nearest number to the left of the index provided.
int getRightNum(int);

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
        return;
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
    if(command.size() <= 0)
    {
        return;
    }
    int i, j, k, l = 0;
    for(i = 0; i < command.size(); i++)
    {
        if(command[i] >= numLowerBound && command[i] <= numUpperBound)
        {
            ptr = searchNum(i, command);
            addObject(command.substr(i, ptr - i), "number", -1);
            i = ptr;
        }
        for(j = 0; j < 6; j++)
        {
            if(command[i] == operators[j])
            {
                addObject(command.substr(i, 1), "operator", -1);
                break;
            }
        }
        if(command[i] == '(')
        {
            addObject(command.substr(i, 1), "openingBracket", -1);
        }
        if(command[i] == ')')
        {
            addObject(command.substr(i, 1), "closingBracket", -1);
        }
    }
    for(Object x : objects)
    {
        x.update();
    }
    if(enableDebugInfo)
    {
        debug(0);
    }
    for(k = 5; k >= 0; k--)
    {
        for(i = maxPriority; i > 0; i--) //Processes operators in descending priority.
        {
            for(j = 0; j < objects.size(); j++)
            {
                if(objects[j].getType() == "operator" && objects[j].getPriority() == i && objects[j].getBracketLvl() == k)
                {
                    double val1 = objects[getLeftNum(j)].getNumericValue();
                    double val2 = objects[getRightNum(j)].getNumericValue();
                    if(enableDebugInfo)
                    {
                        std::cout << "val1: " << val1 << std::endl;
                        std::cout << "val2: " << val2 << std::endl;
                    }
                    switch(objects[j].getContent()[0])
                    {
                        case '+':
                            addObject(std::to_string(val1 + val2), "number", j + 2);
                            break;
                        case '-':
                            addObject(std::to_string(val1 - val2), "number", j + 2);
                            break;
                        case '*':
                            addObject(std::to_string(val1 * val2), "number", j + 2);
                            break;
                        case '/':
                            addObject(std::to_string(val1 / val2), "number", j + 2);
                            break;
                        case '^':
                            addObject(std::to_string(pow(val1, val2)), "number", j + 2);
                            break;
                    }
                    objects.erase(objects.begin() + j - 1, objects.begin() + j + 2);
                    j = 0;
                    if(enableDebugInfo)
                    {
                        l++;
                        debug(l);
                    }
                }
            }
        }
    }
    for(i = 0; i < objects.size(); i++)
    {
        if(objects[i].getType() == "number") //Returns the first number in objects.
        {
            break;
        }
    }
    std::cout << " > " << objects[i].getContent() << std::endl;
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

void addObject(std::string content, std::string type, int pos) //Adds an object to objects.
{
    Object newObject (content, type, pos);
    newObject.update();
    if(pos == -1)
    {
        newObject.setLoc(objects.size());
        objects.push_back(newObject);
    }
    else
    {
        objects.insert(objects.begin() + pos, newObject);
    }
}

void debug(int step) //Displays debug information.
{
    int i;
    std::cout << "Step: " << step << std::endl;
    for(i = 0; i < objects.size(); i++)
    {
        std::cout << "Content: " << objects[i].getContent() << std::endl;
        std::cout << "Type: " << objects[i].getType() << std::endl;
        std::cout << "Loc: " << objects[i].getLoc() << std::endl;
        std::cout << "Bracket level: " << objects[i].getBracketLvl() << std::endl;
        std::cout << "\n";
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

int getLeftNum(int begin)
{
    for(; begin >= 0; begin--)
    {
        if(objects[begin].getType() == "number")
        {
            return begin;
        }
    }
    return -1;
}

int getRightNum(int begin)
{
    for(; begin < objects.size(); begin++)
    {
        if(objects[begin].getType() == "number")
        {
            return begin;
        }
    }
    return objects.size();
}
