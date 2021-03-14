#ifndef NEWATTRIBUTE_H
#define NEWATTRIBUTE_H

#include <QDialog>
#include "attribute.h"

namespace Ui {
class NewAttribute;
}

class NewAttribute : public QDialog
{
    Q_OBJECT

public:
    explicit NewAttribute(QWidget *parent = nullptr, Attribute const *ref = nullptr);
    ~NewAttribute();
    Attribute const &getResult();

private slots:
    void attributeNameChanged();
    void editTypePressed();
    void attributeRangeChanged();
    void attributeStaticChanged();
    void attributeTransientChanged();

private:
    Ui::NewAttribute *ui;
    Attribute generatedAttribute;
};

#endif // NEWATTRIBUTE_H
