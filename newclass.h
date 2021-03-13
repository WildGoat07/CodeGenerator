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
    Class getResult();
private slots:
    void finalChanged();
    void typeChanged();
    void nameChanged();

private:
    Ui::NewClass *ui;
    Class generatedClass;
};

#endif // NEWCLASS_H
