#include "newclass.h"
#include "ui_newclass.h"

NewClass::NewClass(QWidget *parent, Class const* ref) :
    QDialog(parent),
    ui(new Ui::NewClass)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    setFixedSize(size());

    if (ref != nullptr)
    {
        generatedClass = *ref;
        setWindowTitle("Éditer une classe");
    }
    ui->className->setText(QString(generatedClass.name));
    if (generatedClass.abstract)
        ui->classType->setCurrentIndex(2);
    else if (generatedClass.interface)
        ui->classType->setCurrentIndex(1);
    else
        ui->classType->setCurrentIndex(0);
    ui->finalClass->setChecked(generatedClass.finalClass);

    connect(ui->finalClass, &QCheckBox::released, this, &NewClass::finalChanged);
    connect(ui->classType, &QComboBox::currentIndexChanged, this, &NewClass::typeChanged);
    connect(ui->className, &QLineEdit::textChanged, this, &NewClass::nameChanged);
}

NewClass::~NewClass()
{
    delete ui;
}

void NewClass::finalChanged()
{
    generatedClass.finalClass = ui->finalClass->checkState() == Qt::CheckState::Checked;
}

void NewClass::nameChanged()
{
    generatedClass.name = ui->className->text();
}

void NewClass::typeChanged()
{
    switch (ui->classType->currentIndex())
    {
    case 0:
        generatedClass.abstract = false;
        generatedClass.interface = false;
        break;
    case 1:
        generatedClass.abstract = false;
        generatedClass.interface = true;
        break;
    case 2:
        generatedClass.abstract = true;
        generatedClass.interface = false;
        break;
    }
}

Class NewClass::getResult()
{
    return generatedClass;
}
