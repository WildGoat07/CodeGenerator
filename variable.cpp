#include "variable.h"

Variable::Variable():
    QListWidgetItem(),
    name(),
    varType(false)
{

}
QVariant Variable::data(int role) const
{
    if (role == Qt::DisplayRole)
        return QVariant(name);
    else
        return QListWidgetItem::data(role);
}
