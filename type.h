#ifndef TYPE_H
#define TYPE_H
#include <QString>
#include "variableMode.h"
#include "List.hpp"

struct Type
{
public:
    Type();
    Type(bool Void);
    QString name;
    cmpt_info::List<Type> templateValues;
    bool constant;
    VariableMode mode;
    bool array;
};

#endif // TYPE_H
