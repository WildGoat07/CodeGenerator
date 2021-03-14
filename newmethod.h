#ifndef NEWMETHOD_H
#define NEWMETHOD_H

#include <QDialog>
#include "method.h"

namespace Ui {
class NewMethod;
}

class NewMethod : public QDialog
{
    Q_OBJECT

public:
    explicit NewMethod(QWidget *parent = nullptr, Method const *ref = nullptr);
    ~NewMethod();
    Method const &getResult();

private slots:
    void methodSpecialChanged();
    void validatePressed();
    void methodNameChanged();
    void methodRangeChanged();
    void methodModifierChanged();
    void editReturnTypePressed();
    void methodConstChanged();
    void methodFinalChanged();
    void addTemplatePressed();
    void methodTemplatesChanged();
    void upTemplatePressed();
    void downTemplatePressed();
    void editTemplatePressed();
    void deleteTemplatePressed();
    void addParameterPressed();
    void methodParametersChanged();
    void upParameterPressed();
    void downParameterPressed();
    void editParameterPressed();
    void deleteParameterPressed();

private:
    Ui::NewMethod *ui;
    Method generatedMethod;
};

#endif // NEWMETHOD_H
