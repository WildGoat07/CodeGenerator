#ifndef CLASS_H
#define CLASS_H
#include <string>
#include <tuple>
#include "variable.h"
#include "range.h"
#include "List.hpp"
#include "attribute.h"
#include "method.h"

struct Class
{
    Class();
    std::string name;
    bool interface;
    bool abstract;
    cmpt_info::List<std::string> templateTypes;
    cmpt_info::List<Attribute> attributes;
    cmpt_info::List<Method> methods;
    cmpt_info::List<std::tuple<Range, std::string, bool, cmpt_info::List<Type> > > parents;
    bool finalClass;
};

#endif // CLASS_H
