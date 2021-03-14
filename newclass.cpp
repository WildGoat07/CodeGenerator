#include "newclass.h"
#include "ui_newclass.h"
#include "newtemplatename.h"
#include "newparent.h"
#include "newattribute.h"
#include "newmethod.h"

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
    {
        ui->classType->setCurrentIndex(2);
        ui->finalClass->setEnabled(false);
    }
    else if (generatedClass.interface)
    {
        ui->classType->setCurrentIndex(1);
        ui->finalClass->setEnabled(false);
    }
    else
    {
        ui->classType->setCurrentIndex(0);
        ui->finalClass->setEnabled(true);
    }
    ui->finalClass->setChecked(generatedClass.finalClass);

    for (auto it = generatedClass.templateTypes.begin();it != generatedClass.templateTypes.end();++it)
        ui->classTemplates->addItem(&*it);

    for (auto it = generatedClass.parents.begin();it != generatedClass.parents.end();++it)
        ui->classParents->addItem(&*it);

    for (auto it = generatedClass.attributes.begin();it != generatedClass.attributes.end();++it)
        ui->classAttributes->addItem(&*it);

    for (auto it = generatedClass.methods.begin();it != generatedClass.methods.end();++it)
        ui->classMethods->addItem(&*it);

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
    connect(ui->classTemplates, &QListWidget::doubleClicked, this, &NewClass::editTemplatePressed);
    connect(ui->deleteTemplate, &QPushButton::clicked, this, &NewClass::deleteTemplatePressed);

    connect(ui->addParent, &QPushButton::clicked, this, &NewClass::addParentPressed);
    connect(ui->classParents, &QListWidget::itemSelectionChanged, this, &NewClass::classParentsChanged);
    connect(ui->upParent, &QPushButton::clicked, this, &NewClass::upParentPressed);
    connect(ui->downParent, &QPushButton::clicked, this, &NewClass::downParentPressed);
    connect(ui->editParent, &QPushButton::clicked, this, &NewClass::editParentPressed);
    connect(ui->classParents, &QListWidget::doubleClicked, this, &NewClass::editParentPressed);
    connect(ui->deleteParent, &QPushButton::clicked, this, &NewClass::deleteParentPressed);

    connect(ui->addAttribute, &QPushButton::clicked, this, &NewClass::addAttributePressed);
    connect(ui->classAttributes, &QListWidget::itemSelectionChanged, this, &NewClass::classAttributeChanged);
    connect(ui->upAttribute, &QPushButton::clicked, this, &NewClass::upAttributePressed);
    connect(ui->downAttribute, &QPushButton::clicked, this, &NewClass::downAttributePressed);
    connect(ui->editAttribute, &QPushButton::clicked, this, &NewClass::editAttributePressed);
    connect(ui->classAttributes, &QListWidget::doubleClicked, this, &NewClass::editAttributePressed);
    connect(ui->deleteAttribute, &QPushButton::clicked, this, &NewClass::deleteAttributePressed);

    connect(ui->addMethod, &QPushButton::clicked, this, &NewClass::addMethodPressed);
    connect(ui->classMethods, &QListWidget::itemSelectionChanged, this, &NewClass::classMethodsChanged);
    connect(ui->upMethod, &QPushButton::clicked, this, &NewClass::upMethodPressed);
    connect(ui->downMethod, &QPushButton::clicked, this, &NewClass::downMethodPressed);
    connect(ui->editMethod, &QPushButton::clicked, this, &NewClass::editMethodPressed);
    connect(ui->classMethods, &QListWidget::doubleClicked, this, &NewClass::editMethodPressed);
    connect(ui->deleteMethod, &QPushButton::clicked, this, &NewClass::deleteMethodPressed);

    ui->className->setFocus(Qt::PopupFocusReason);
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
        ui->finalClass->setEnabled(true);
        break;
    case 1:
        generatedClass.abstract = false;
        generatedClass.interface = true;
        generatedClass.finalClass = false;
        ui->finalClass->setEnabled(false);
        break;
    case 2:
        generatedClass.abstract = true;
        generatedClass.interface = false;
        generatedClass.finalClass = false;
        ui->finalClass->setEnabled(false);
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

void NewClass::classParentsChanged()
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

void NewClass::addAttributePressed()
{
    NewAttribute dialog(this);
    if (dialog.exec())
        ui->classAttributes->addItem(&*generatedClass.attributes.push_back(dialog.getResult()));
}

void NewClass::editAttributePressed()
{
    auto currentAttribute = dynamic_cast<Attribute*>(ui->classAttributes->currentItem());
    NewAttribute dialog(this, currentAttribute);
    if (dialog.exec())
    {
        *currentAttribute = dialog.getResult();
    }
}

void NewClass::deleteAttributePressed()
{
    generatedClass.attributes.remove(
                generatedClass.attributes.find(
                    dynamic_cast<Attribute*>(ui->classAttributes->takeItem(ui->classAttributes->currentRow()))));
    ui->classAttributes->setCurrentRow(-1);
}

void NewClass::classAttributeChanged()
{
    if (ui->classAttributes->currentRow() != -1)
    {
        ui->editAttribute->setEnabled(true);
        ui->deleteAttribute->setEnabled(true);
        ui->upAttribute->setEnabled(true);
        ui->downAttribute->setEnabled(true);
    }
    else
    {
        ui->editAttribute->setEnabled(false);
        ui->deleteAttribute->setEnabled(false);
        ui->upAttribute->setEnabled(false);
        ui->downAttribute->setEnabled(false);
    }
}

void NewClass::upAttributePressed()
{
    if (ui->classAttributes->currentRow() > 0)
    {
        auto it = generatedClass.attributes.find(dynamic_cast<Attribute*>(ui->classAttributes->currentItem()));
        Attribute moved = *it;
        generatedClass.attributes.remove(it);
        --it;
        it = generatedClass.attributes.insert(it, moved);
        while (ui->classAttributes->count())
            ui->classAttributes->takeItem(0);
        for (auto it2 = generatedClass.attributes.begin();it2 != generatedClass.attributes.end();++it2)
            ui->classAttributes->addItem(&*it2);
        ui->classAttributes->setCurrentItem(&*it);
    }
}

void NewClass::downAttributePressed()
{
    if (ui->classAttributes->currentRow() < ui->classAttributes->count()-1)
    {
        auto it = generatedClass.attributes.find(dynamic_cast<Attribute*>(ui->classAttributes->currentItem()));
        ++it;
        Attribute moved = *it;
        generatedClass.attributes.remove(it);
        --it;
        it = generatedClass.attributes.insert(it, moved);
        ++it;
        while (ui->classAttributes->count())
            ui->classAttributes->takeItem(0);
        for (auto it2 = generatedClass.attributes.begin();it2 != generatedClass.attributes.end();++it2)
            ui->classAttributes->addItem(&*it2);
        ui->classAttributes->setCurrentItem(&*it);
    }
}

void NewClass::addMethodPressed()
{
    NewMethod dialog(this);
    if (dialog.exec())
        ui->classMethods->addItem(&*generatedClass.methods.push_back(dialog.getResult()));
}

void NewClass::editMethodPressed()
{
    auto currentMethod = dynamic_cast<Method*>(ui->classMethods->currentItem());
    NewMethod dialog(this, currentMethod);
    if (dialog.exec())
    {
        *currentMethod = dialog.getResult();
    }
}

void NewClass::deleteMethodPressed()
{
    generatedClass.methods.remove(
                generatedClass.methods.find(
                    dynamic_cast<Method*>(ui->classMethods->takeItem(ui->classMethods->currentRow()))));
    ui->classMethods->setCurrentRow(-1);
}

void NewClass::classMethodsChanged()
{
    if (ui->classMethods->currentRow() != -1)
    {
        ui->editMethod->setEnabled(true);
        ui->deleteMethod->setEnabled(true);
        ui->upMethod->setEnabled(true);
        ui->downMethod->setEnabled(true);
    }
    else
    {
        ui->editMethod->setEnabled(false);
        ui->deleteMethod->setEnabled(false);
        ui->upMethod->setEnabled(false);
        ui->downMethod->setEnabled(false);
    }
}

void NewClass::upMethodPressed()
{
    if (ui->classMethods->currentRow() > 0)
    {
        auto it = generatedClass.methods.find(dynamic_cast<Method*>(ui->classMethods->currentItem()));
        Method moved = *it;
        generatedClass.methods.remove(it);
        --it;
        it = generatedClass.methods.insert(it, moved);
        while (ui->classMethods->count())
            ui->classMethods->takeItem(0);
        for (auto it2 = generatedClass.methods.begin();it2 != generatedClass.methods.end();++it2)
            ui->classMethods->addItem(&*it2);
        ui->classMethods->setCurrentItem(&*it);
    }
}

void NewClass::downMethodPressed()
{
    if (ui->classMethods->currentRow() < ui->classMethods->count()-1)
    {
        auto it = generatedClass.methods.find(dynamic_cast<Method*>(ui->classMethods->currentItem()));
        ++it;
        Method moved = *it;
        generatedClass.methods.remove(it);
        --it;
        it = generatedClass.methods.insert(it, moved);
        ++it;
        while (ui->classMethods->count())
            ui->classMethods->takeItem(0);
        for (auto it2 = generatedClass.methods.begin();it2 != generatedClass.methods.end();++it2)
            ui->classMethods->addItem(&*it2);
        ui->classMethods->setCurrentItem(&*it);
    }
}

Class const &NewClass::getResult()
{
    return generatedClass;
}
