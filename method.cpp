#include "method.h"

Method::Method():
    QListWidgetItem(),
    name(),
    templateTypes(),
    range(PUBLIC),
    returnType(true),
    parameters(),
    modifier(NONE),
    constantMethod(false),
    finalMethod(false),
    specialMethod(BASIC)
{

}
QVariant Method::data(int role) const
{
    if (role == Qt::DisplayRole)
        return QVariant(name);
    else
        return QListWidgetItem::data(role);
}
