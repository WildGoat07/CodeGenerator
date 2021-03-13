#ifndef METHOD_H
#define METHOD_H
#include <QString>
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
        ABSTRACT,
        STATIC
    };
    enum Special
    {
        CONSTRUCTOR,
        DESTRUCTOR,
        BASIC
    };

    Method();
    QString name;
    cmpt_info::List<QString> templateTypes;
    Range range;
    Type returnType;
    cmpt_info::List<Variable> parameters;
    Modifier modifier;
    bool constantMethod;
    bool finalMethod;
    Special specialMethod;
};

#endif // METHOD_H
