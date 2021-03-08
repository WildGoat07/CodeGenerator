#ifndef TYPE_H
#define TYPE_H
#include <string>
#include "variableMode.h"

struct Type
{
public:
    Type(bool Void);
    std::string name;
    bool constant;
    VariableMode mode;
};

#endif // TYPE_H
