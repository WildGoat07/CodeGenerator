#ifndef METHOD_H
#define METHOD_H
#include <string>
#include <tuple>
#include "range.h"
#include "type.h"
#include "List.hpp"
#include "variable.h"

struct Method
{
public:
    enum Modifier
    {
        NONE,
        VIRTUAL,
        OVERRIDE,
        ABSTRACT
    };
    Method();
    std::string name;
    cmpt_info::List<std::string> templateTypes;
    Range range;
    Type returnType;
    cmpt_info::List<Variable> parameters;
    Modifier modifier;
};

#endif // METHOD_H
