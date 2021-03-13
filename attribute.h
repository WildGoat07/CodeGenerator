#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H
#include <QListWidgetItem>
#include <QVariant>
#include "range.h"
#include "variable.h"

struct Attribute : public QListWidgetItem
{
public:
    Attribute();
    Range range;
    Variable variable;
    bool staticAttribute;
    bool transientAttribute;
    virtual QVariant data(int role) const override;
};

#endif // ATTRIBUTE_H
