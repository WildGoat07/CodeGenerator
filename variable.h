#ifndef VARIABLE_H
#define VARIABLE_H
#include <QString>
#include "type.h"


struct Variable
{
public:
    Variable();
    QString name;
    Type varType;
};

#endif // VARIABLE_H
