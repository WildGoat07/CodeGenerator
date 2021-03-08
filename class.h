#ifndef CLASS_H
#define CLASS_H
#include <string>
#include <tuple>
#include "variable.h"
#include "range.h"
#include "List.hpp"
#include "attribute.h"

struct Class
{
    Class();
    std::string name;
    bool interface;
    bool abstract;
    cmpt_info::List<std::string> templateTypes;
    cmpt_info::List<Attribute> attributes;
    cmpt_info::List<std::tuple<Range, Class*> > parents;

};

#endif // CLASS_H
