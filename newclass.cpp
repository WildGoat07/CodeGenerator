#include "newclass.h"
#include "ui_newclass.h"
#include "newtemplatename.h"

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

    for (auto it = generatedClass.templateTypes.begin();it != generatedClass.templateTypes.end();++it)
        ui->classTemplates->addItem(&*it);

    connect(ui->finalClass, &QCheckBox::clicked, this, &NewClass::finalChanged);
    connect(ui->classType, &QComboBox::currentIndexChanged, this, &NewClass::typeChanged);
    connect(ui->className, &QLineEdit::textChanged, this, &NewClass::nameChanged);
    connect(ui->validate, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->addTemplate, &QPushButton::clicked, this, &NewClass::addTemplatePressed);
    connect(ui->classTemplates, &QListWidget::itemSelectionChanged, this, &NewClass::classTemplatesChanged);
    connect(ui->upTemplate, &QPushButton::clicked, this, &NewClass::upTemplatePressed);
    connect(ui->downTemplate, &QPushButton::clicked, this, &NewClass::downTemplatePressed);

    ui->className->selectAll();
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

void NewClass::addTemplatePressed()
{
    NewTemplateType dialog(this);
    if (dialog.exec())
        ui->classTemplates->addItem(&*generatedClass.templateTypes.push_back(dialog.getResult()));
}

void NewClass::classTemplatesChanged()
{
    if (ui->classTemplates->currentRow() != -1)
    {
        ui->editTemplate->setEnabled(true);
        ui->deleteTemplate->setEnabled(true);
        ui->upTemplate->setEnabled(true);
        ui->downTemplate->setEnabled(true);
    }
    else
    {
        ui->editTemplate->setEnabled(false);
        ui->deleteTemplate->setEnabled(false);
        ui->upTemplate->setEnabled(false);
        ui->downTemplate->setEnabled(false);
    }
}

#include <iostream>

void NewClass::upTemplatePressed()
{
    if (ui->classTemplates->currentRow() > 0)
    {
        auto it = generatedClass.templateTypes.find(dynamic_cast<TemplateName*>(ui->classTemplates->currentItem()));
        TemplateName moved = *it;
        generatedClass.templateTypes.remove(it);
        --it;
        it = generatedClass.templateTypes.insert(it, moved);
        while (ui->classTemplates->count())
            ui->classTemplates->takeItem(0);
        for (auto it2 = generatedClass.templateTypes.begin();it2 != generatedClass.templateTypes.end();++it2)
            ui->classTemplates->addItem(&*it2);
        ui->classTemplates->setCurrentItem(&*it);
    }
}

void NewClass::downTemplatePressed()
{
    if (ui->classTemplates->currentRow() < ui->classTemplates->count()-1)
    {
        auto it = generatedClass.templateTypes.find(dynamic_cast<TemplateName*>(ui->classTemplates->currentItem()));
        ++it;
        TemplateName moved = *it;
        generatedClass.templateTypes.remove(it);
        --it;
        it = generatedClass.templateTypes.insert(it, moved);
        ++it;
        while (ui->classTemplates->count())
            ui->classTemplates->takeItem(0);
        for (auto it2 = generatedClass.templateTypes.begin();it2 != generatedClass.templateTypes.end();++it2)
            ui->classTemplates->addItem(&*it2);
        ui->classTemplates->setCurrentItem(&*it);
    }
}

Class NewClass::getResult()
{
    return generatedClass;
}
