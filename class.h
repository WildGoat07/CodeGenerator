#ifndef CLASS_H
#define CLASS_H
#include <tuple>
#include "variable.h"
#include "range.h"
#include "List.hpp"
#include "attribute.h"
#include "method.h"
#include "templatename.h"
#include "parent.h"
#include <QString>
#include <QListWidgetItem>
#include <QVariant>

struct Class : public QListWidgetItem
{
    Class();
    QString name;
    bool interface;
    bool abstract;
    cmpt_info::List<TemplateName> templateTypes;
    cmpt_info::List<Attribute> attributes;
    cmpt_info::List<Method> methods;
    cmpt_info::List<Parent> parents;
    bool finalClass;
    virtual QVariant data(int role) const override;
};

#endif // CLASS_H
