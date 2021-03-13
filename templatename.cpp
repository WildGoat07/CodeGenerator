#include "templatename.h"

TemplateName::TemplateName() :
    QListWidgetItem(),
    name()
{

}
TemplateName::TemplateName(const QString& name) :
    TemplateName()
{
    operator=(name);
}

QVariant TemplateName::data(int role) const
{
    if (role == Qt::DisplayRole)
        return QVariant(name);
    else
        return QListWidgetItem::data(role);
}

TemplateName& TemplateName::operator=(const QString &name)
{
    this->name = name;
    return *this;
}
