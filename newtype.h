#ifndef NEWTYPE_H
#define NEWTYPE_H

#include <QDialog>
#include "type.h"

namespace Ui {
class NewType;
}

class NewType : public QDialog
{
    Q_OBJECT

public:
    explicit NewType(QWidget *parent = nullptr, Type const *ref = nullptr);
    ~NewType();
    Type const &getResult();

private slots:
    void typeNameChanged();
    void typeModeChanged();
    void typeConstChanged();
    void typeListChanged();
    void addTemplatePressed();
    void typeTemplatesChanged();
    void upTemplatePressed();
    void downTemplatePressed();
    void editTemplatePressed();
    void deleteTemplatePressed();

private:
    Ui::NewType *ui;
    Type generatedType;
};

#endif // NEWTYPE_H
