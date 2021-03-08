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
    bool staticAttribute;
    bool transientAttribute;
};

#endif // ATTRIBUTE_H
