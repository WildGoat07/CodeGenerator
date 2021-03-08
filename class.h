#ifndef CLASS_H
#define CLASS_H
#include <string>
#include <tuple>
#include "variable.h"
#include "range.h"
#include "List.hpp"

struct Class
{
    Class();
    std::string name;
    bool abstract;
    cmpt_info::List<std::string> templateTypes;
    cmpt_info::List<std::tuple<Range, Variable> > attributes;
};

#endif // CLASS_H
