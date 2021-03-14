#include "range.h"

std::string getRange(Range r)
{
    switch (r) {
    case PUBLIC:
        return "public";
    case PROTECTED:
        return "protected";
    case PRIVATE:
        return "private";
    default:
        return "";
    }
}
