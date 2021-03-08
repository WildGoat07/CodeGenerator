#include "method.h"

Method::Method():
    name(),
    templateTypes(),
    range(PUBLIC),
    returnType(true),
    parameters(),
    modifier(NONE),
    staticMethod(false),
    constantMethod(false),
    specialMethod(BASIC)
{

}
