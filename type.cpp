#include "type.h"

Type::Type():
    name(),
    templateValues(),
    constant(false),
    mode(COPY)
{

}
Type::Type(bool Void):
    name(Void ? "void" : ""),
    templateValues(),
    constant(false),
    mode(COPY)
{

}
