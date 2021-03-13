#include "type.h"

Type::Type():
    QListWidgetItem(),
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
QVariant Type::data(int role) const
{
    if (role == Qt::DisplayRole)
        return QVariant(name);
    else
        return QListWidgetItem::data(role);
}
