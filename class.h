#ifndef CLASS_H
#define CLASS_H
#include <tuple>
#include "variable.h"
#include "range.h"
#include "List.hpp"
#include "attribute.h"
#include "method.h"
#include <QString>

struct Class
{
    Class();
    QString name;
    bool interface;
    bool abstract;
    cmpt_info::List<QString> templateTypes;
    cmpt_info::List<Attribute> attributes;
    cmpt_info::List<Method> methods;
    cmpt_info::List<std::tuple<Range, QString, bool, cmpt_info::List<Type> > > parents;
    bool finalClass;
};

#endif // CLASS_H
