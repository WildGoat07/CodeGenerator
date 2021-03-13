#ifndef METHOD_H
#define METHOD_H
#include <QString>
#include <tuple>
#include "range.h"
#include "type.h"
#include "List.hpp"
#include "variable.h"
#include "templatename.h"
#include <QListWidgetItem>
#include <QVariant>

struct Method : public QListWidgetItem
{
public:
    enum Modifier
    {
        NONE,
        VIRTUAL,
        OVERRIDE,
        ABSTRACT,
        STATIC
    };
    enum Special
    {
        CONSTRUCTOR,
        DESTRUCTOR,
        BASIC
    };

    Method();
    QString name;
    cmpt_info::List<TemplateName> templateTypes;
    Range range;
    struct Type returnType;
    cmpt_info::List<Variable> parameters;
    Modifier modifier;
    bool constantMethod;
    bool finalMethod;
    Special specialMethod;
    virtual QVariant data(int role) const override;
};

#endif // METHOD_H
