#include "type.h"

Type::Type(char const* n):
    QListWidgetItem(),
    name(n ? n : ""),
    templateValues(),
    constant(false),
    mode(COPY),
    array(false)
{
}
QVariant Type::data(int role) const
{
    if (role == Qt::DisplayRole)
        return QVariant(name);
    else
        return QListWidgetItem::data(role);
}
