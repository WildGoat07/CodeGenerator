#ifndef RANGE_H
#define RANGE_H

#include <string>

enum Range
{
    PUBLIC,
    PROTECTED,
    PRIVATE
};

std::string getRange(Range r);

#endif // RANGE_H
