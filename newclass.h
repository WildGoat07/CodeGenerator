#ifndef NEWCLASS_H
#define NEWCLASS_H
#include "class.h"

#include <QDialog>

namespace Ui {
class NewClass;
}

class NewClass : public QDialog
{
    Q_OBJECT

public:
    explicit NewClass(QWidget *parent = nullptr, Class const* ref = nullptr);
    ~NewClass();
    Class const &getResult();
private slots:
    void finalChanged();
    void typeChanged();
    void nameChanged();
    void addTemplatePressed();
    void classTemplatesChanged();
    void upTemplatePressed();
    void downTemplatePressed();
    void editTemplatePressed();
    void deleteTemplatePressed();
    void addParentPressed();
    void classParentChanged();
    void upParentPressed();
    void downParentPressed();
    void editParentPressed();
    void deleteParentPressed();

private:
    Ui::NewClass *ui;
    Class generatedClass;
};

#endif // NEWCLASS_H
