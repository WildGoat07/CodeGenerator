#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H
#include "range.h"
#include "variable.h"

struct Attribute
{
public:
    Attribute();
    Range range;
    Variable variable;
};

#endif // ATTRIBUTE_H
