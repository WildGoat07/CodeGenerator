#ifndef TYPE_H
#define TYPE_H
#include <QString>
#include "variableMode.h"
#include "List.hpp"
#include <QListWidgetItem>
#include <QVariant>

struct Type : public QListWidgetItem
{
public:
    Type(char const* n = nullptr);
    QString name;
    cmpt_info::List<Type> templateValues;
    bool constant;
    VariableMode mode;
    bool array;
    virtual QVariant data(int role) const override;
};

#endif // TYPE_H
