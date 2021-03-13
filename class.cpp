#include "class.h"

using namespace std;

Class::Class() :
    QListWidgetItem(),
    name(),
    interface(false),
    abstract(false),
    templateTypes(),
    attributes(),
    parents(),
    finalClass(false)
{
}
QVariant Class::data(int role) const
{
    if (role == Qt::DisplayRole)
        return QVariant(name);
    else
        return QListWidgetItem::data(role);
}
