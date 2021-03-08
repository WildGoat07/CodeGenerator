#include "method.h"

Method::Method():
    name(),
    templateTypes(),
    range(PUBLIC),
    returnType(true),
    parameters(),
    modifier(NONE),
    constantMethod(false),
    specialMethod(BASIC),
    finalMethod(false)
{

}
