#include "attribute.h"

Attribute::Attribute():
    QListWidgetItem(),
    range(PRIVATE),
    variable(),
    staticAttribute(false),
    transientAttribute(false)
{

}

QVariant Attribute::data(int role) const
{
    return variable.data(role);
}
