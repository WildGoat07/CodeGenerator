#ifndef PARENT_H
#define PARENT_H
#include "range.h"
#include <QString>
#include <QListWidgetItem>
#include <QVariant>
#include "List.hpp"
#include "type.h"


class Parent : public QListWidgetItem
{
public:
    Parent();
    virtual QVariant data(int role) const override;
    Range range;
    QString name;
    bool interface;
    cmpt_info::List<struct Type> templates;
};

#endif // PARENT_H
