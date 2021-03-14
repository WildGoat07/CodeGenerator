#include "newclass.h"
#include "ui_newclass.h"
#include "newtemplatename.h"
#include "newparent.h"
#include "utilities.h"

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

    for (auto it = generatedClass.parents.begin();it != generatedClass.parents.end();++it)
        ui->classParents->addItem(&*it);

    connect(ui->finalClass, &QCheckBox::clicked, this, &NewClass::finalChanged);
    connect(ui->classType, &QComboBox::currentIndexChanged, this, &NewClass::typeChanged);
    connect(ui->className, &QLineEdit::textChanged, this, &NewClass::nameChanged);
    connect(ui->validate, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->addTemplate, &QPushButton::clicked, this, &NewClass::addTemplatePressed);
    connect(ui->classTemplates, &QListWidget::itemSelectionChanged, this, &NewClass::classTemplatesChanged);
    connect(ui->upTemplate, &QPushButton::clicked, this, &NewClass::upTemplatePressed);
    connect(ui->downTemplate, &QPushButton::clicked, this, &NewClass::downTemplatePressed);
    connect(ui->editTemplate, &QPushButton::clicked, this, &NewClass::editTemplatePressed);
    connect(ui->deleteTemplate, &QPushButton::clicked, this, &NewClass::deleteTemplatePressed);
    connect(ui->addParent, &QPushButton::clicked, this, &NewClass::addParentPressed);
    connect(ui->classParents, &QListWidget::itemSelectionChanged, this, &NewClass::classParentChanged);
    connect(ui->upParent, &QPushButton::clicked, this, &NewClass::upParentPressed);
    connect(ui->downParent, &QPushButton::clicked, this, &NewClass::downParentPressed);
    connect(ui->editParent, &QPushButton::clicked, this, &NewClass::editParentPressed);
    connect(ui->deleteParent, &QPushButton::clicked, this, &NewClass::deleteParentPressed);

    ui->className->selectAll();
}

NewClass::~NewClass()
{
    delete ui;
}

void NewClass::finalChanged()
{
    generatedClass.finalClass = ui->finalClass->isChecked();
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

void NewClass::editTemplatePressed()
{
    auto currentTemplate = dynamic_cast<TemplateName*>(ui->classTemplates->currentItem());
    NewTemplateType dialog(this, currentTemplate);
    if (dialog.exec())
    {
        *currentTemplate = dialog.getResult();
    }
}

void NewClass::deleteTemplatePressed()
{
    generatedClass.templateTypes.remove(
                generatedClass.templateTypes.find(
                    dynamic_cast<TemplateName*>(ui->classTemplates->takeItem(ui->classTemplates->currentRow()))));
    ui->classTemplates->setCurrentRow(-1);
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

void NewClass::addParentPressed()
{
    NewParent dialog(this);
    if (dialog.exec())
        ui->classParents->addItem(&*generatedClass.parents.push_back(dialog.getResult()));
}

void NewClass::editParentPressed()
{
    auto currentParent = dynamic_cast<Parent*>(ui->classParents->currentItem());
    NewParent dialog(this, currentParent);
    if (dialog.exec())
    {
        *currentParent = dialog.getResult();
    }
}

void NewClass::deleteParentPressed()
{
    generatedClass.parents.remove(
                generatedClass.parents.find(
                    dynamic_cast<Parent*>(ui->classParents->takeItem(ui->classParents->currentRow()))));
    ui->classParents->setCurrentRow(-1);
}

void NewClass::classParentChanged()
{
    if (ui->classParents->currentRow() != -1)
    {
        ui->editParent->setEnabled(true);
        ui->deleteParent->setEnabled(true);
        ui->upParent->setEnabled(true);
        ui->downParent->setEnabled(true);
    }
    else
    {
        ui->editParent->setEnabled(false);
        ui->deleteParent->setEnabled(false);
        ui->upParent->setEnabled(false);
        ui->downParent->setEnabled(false);
    }
}

void NewClass::upParentPressed()
{
    if (ui->classParents->currentRow() > 0)
    {
        auto it = generatedClass.parents.find(dynamic_cast<Parent*>(ui->classParents->currentItem()));
        Parent moved = *it;
        generatedClass.parents.remove(it);
        --it;
        it = generatedClass.parents.insert(it, moved);
        while (ui->classParents->count())
            ui->classParents->takeItem(0);
        for (auto it2 = generatedClass.parents.begin();it2 != generatedClass.parents.end();++it2)
            ui->classParents->addItem(&*it2);
        ui->classParents->setCurrentItem(&*it);
    }
}

void NewClass::downParentPressed()
{
    if (ui->classParents->currentRow() < ui->classParents->count()-1)
    {
        auto it = generatedClass.parents.find(dynamic_cast<Parent*>(ui->classParents->currentItem()));
        ++it;
        Parent moved = *it;
        generatedClass.parents.remove(it);
        --it;
        it = generatedClass.parents.insert(it, moved);
        ++it;
        while (ui->classParents->count())
            ui->classParents->takeItem(0);
        for (auto it2 = generatedClass.parents.begin();it2 != generatedClass.parents.end();++it2)
            ui->classParents->addItem(&*it2);
        ui->classParents->setCurrentItem(&*it);
    }
}

Class const &NewClass::getResult()
{
    return generatedClass;
}
