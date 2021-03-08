#include "type.h"

Type::Type(bool Void):
    name(Void ? "void" : ""),
    constant(false),
    mode(COPY)
{

}
