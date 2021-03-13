#ifndef VARIABLE_H
#define VARIABLE_H
#include <QString>
#include "type.h"
#include <QListWidgetItem>
#include <QVariant>


struct Variable : public QListWidgetItem
{
public:
    Variable();
    QString name;
    struct Type varType;
    virtual QVariant data(int role) const override;
};

#endif // VARIABLE_H
