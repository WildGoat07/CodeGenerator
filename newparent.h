#ifndef NEWPARENT_H
#define NEWPARENT_H

#include <QDialog>
#include "parent.h"

namespace Ui {
class NewParent;
}

class NewParent : public QDialog
{
    Q_OBJECT

public:
    explicit NewParent(QWidget *parent = nullptr, Parent const* ref = nullptr);
    ~NewParent();
    Parent const &getResult();

private slots:
    void nameChanged();
    void interfaceChanged();
    void rangeChanged();
    void addTemplatePressed();
    void parentTemplatesChanged();
    void upTemplatePressed();
    void downTemplatePressed();
    void editTemplatePressed();
    void deleteTemplatePressed();

private:
    Ui::NewParent *ui;
    Parent generatedParent;
};

#endif // NEWPARENT_H
