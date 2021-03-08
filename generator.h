#ifndef GENERATOR_H
#define GENERATOR_H
#include "class.h"
#include "List.hpp"
#include <ostream>


class Generator
{
public:
    Generator();
    cmpt_info::List<Class> classes;
    void GenerateCpp(std::ostream& stream);
    void GenerateJava(std::ostream& stream);
};

#endif // GENERATOR_H
