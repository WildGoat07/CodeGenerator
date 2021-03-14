#ifndef NEWPARAMETER_H
#define NEWPARAMETER_H

#include <QDialog>
#include "variable.h"

namespace Ui {
class NewParameter;
}

class NewParameter : public QDialog
{
    Q_OBJECT

public:
    explicit NewParameter(QWidget *parent = nullptr, Variable const *ref = nullptr);
    ~NewParameter();
    Variable const &getResult();

private slots:
    void parameterNameChanged();
    void editTypePressed();

private:
    Ui::NewParameter *ui;
    Variable generatedVariable;
};

#endif // NEWPARAMETER_H
