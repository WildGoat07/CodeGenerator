#ifndef VARIABLE_H
#define VARIABLE_H
#include <string>
#include "VariableMode.h"


struct Variable
{
public:
    Variable();
    std::string name;
    bool constant;
    std::string type;
    VariableMode mode;
};

#endif // VARIABLE_H
