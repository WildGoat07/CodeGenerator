#ifndef VARIABLE_H
#define VARIABLE_H
#include <string>
#include "type.h"


struct Variable
{
public:
    Variable();
    std::string name;
    Type varType;
};

#endif // VARIABLE_H
