#ifndef NEWTEMPLATENAME_H
#define NEWTEMPLATENAME_H

#include <QDialog>
#include "templatename.h"

namespace Ui {
class NewTemplateType;
}

class NewTemplateType : public QDialog
{
    Q_OBJECT

public:
    explicit NewTemplateType(QWidget *parent = nullptr, TemplateName const* ref = nullptr);
    ~NewTemplateType();
    TemplateName getResult();

private slots:
    void nameChanged();

private:
    Ui::NewTemplateType *ui;
    TemplateName generatedTemplate;
};

#endif // NEWTEMPLATENAME_H
