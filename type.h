#ifndef TYPE_H
#define TYPE_H
#include <string>
#include "variableMode.h"
#include "List.hpp"

struct Type
{
public:
    Type();
    Type(bool Void);
    std::string name;
    cmpt_info::List<Type> templateValues;
    bool constant;
    VariableMode mode;
};

#endif // TYPE_H
