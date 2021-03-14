#include "parent.h"

Parent::Parent() :
    range(PUBLIC),
    name(),
    interface(false),
    templates()
{
}
QVariant Parent::data(int role) const
{
    if (role == Qt::DisplayRole)
        return QVariant(name);
    else
        return QListWidgetItem::data(role);
}
