#include "newmethod.h"
#include "ui_newmethod.h"
#include "newtype.h"
#include "newtemplatename.h"
#include "newparameter.h"
#include <sstream>


NewMethod::NewMethod(QWidget *parent, Method const *ref) :
    QDialog(parent),
    ui(new Ui::NewMethod)
{
    /*****************************/
    // non resizable window
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    setFixedSize(size());

    if (ref != nullptr)
    {
        // if we are editing an existing method
        generatedMethod = *ref;
        setWindowTitle("Éditer une méthode");
    }

    /*****************************/
    // default values
    ui->cppModifier->setVisible(false);

    switch (generatedMethod.specialMethod) {
    case Method::Special::BASIC:
        ui->methodSpecial->setCurrentIndex(0);
        ui->methodName->setEnabled(true);
        ui->editReturnType->setEnabled(true);
        ui->methodReturnType->setEnabled(true);
        ui->methodRange->setEnabled(true);
        ui->methodModifier->setEnabled(true);
        ui->methodConst->setEnabled(true);
        ui->methodFinal->setEnabled(true);
        ui->templatePanel->setEnabled(true);
        break;
    case Method::Special::CONSTRUCTOR:
        ui->methodSpecial->setCurrentIndex(1);
        ui->methodName->setEnabled(false);
        ui->editReturnType->setEnabled(false);
        ui->methodReturnType->setEnabled(false);
        ui->methodRange->setEnabled(true);
        ui->methodModifier->setEnabled(false);
        ui->methodConst->setEnabled(false);
        ui->methodFinal->setEnabled(false);
        ui->templatePanel->setEnabled(false);
        break;
    case Method::Special::DESTRUCTOR:
        ui->methodSpecial->setCurrentIndex(2);
        ui->methodName->setEnabled(false);
        ui->editReturnType->setEnabled(false);
        ui->methodReturnType->setEnabled(false);
        ui->methodRange->setEnabled(false);
        ui->methodModifier->setEnabled(false);
        ui->methodConst->setEnabled(false);
        ui->methodFinal->setEnabled(false);
        ui->templatePanel->setEnabled(false);
        break;
    }
    ui->methodName->setText(generatedMethod.name);

    switch (generatedMethod.range) {
    case Range::PUBLIC:
        ui->methodRange->setCurrentIndex(0);
        break;
    case Range::PROTECTED:
        ui->methodRange->setCurrentIndex(1);
        break;
    case Range::PRIVATE:
        ui->methodRange->setCurrentIndex(2);
        break;
    }

    switch (generatedMethod.modifier) {
    case Method::Modifier::NONE:
        ui->methodModifier->setCurrentIndex(0);
        ui->cppModifier->setVisible(false);
        ui->methodFinal->setEnabled(false);
        ui->methodConst->setEnabled(true);
        ui->constCpp->setEnabled(true);
        break;
    case Method::Modifier::VIRTUAL:
        ui->methodModifier->setCurrentIndex(1);
        ui->cppModifier->setVisible(true);
        ui->methodFinal->setEnabled(false);
        ui->methodConst->setEnabled(true);
        ui->constCpp->setEnabled(true);
        break;
    case Method::Modifier::ABSTRACT:
        ui->methodModifier->setCurrentIndex(2);
        ui->cppModifier->setVisible(false);
        ui->methodFinal->setEnabled(false);
        ui->methodConst->setEnabled(true);
        ui->constCpp->setEnabled(true);
        break;
    case Method::Modifier::OVERRIDE:
        ui->methodModifier->setCurrentIndex(3);
        ui->cppModifier->setVisible(false);
        ui->methodFinal->setEnabled(true);
        ui->methodConst->setEnabled(true);
        ui->constCpp->setEnabled(true);
        break;
    case Method::Modifier::STATIC:
        ui->methodModifier->setCurrentIndex(4);
        ui->cppModifier->setVisible(false);
        ui->methodFinal->setEnabled(false);
        ui->methodConst->setEnabled(false);
        ui->constCpp->setEnabled(false);
        break;
    }
    {
        std::stringstream ss;
        ss << "` " << generatedMethod.returnType.name.toStdString() << '`';
        ui->methodReturnType->setText(QString(ss.str().c_str()));
    }

    ui->methodConst->setChecked(generatedMethod.constantMethod);
    ui->methodFinal->setChecked(generatedMethod.finalMethod);

    for (auto it = generatedMethod.templateTypes.begin();it != generatedMethod.templateTypes.end();++it)
        ui->methodTemplates->addItem(&*it);

    for (auto it = generatedMethod.parameters.begin();it != generatedMethod.parameters.end();++it)
        ui->methodParameters->addItem(&*it);

    /*****************************/
    // events

    // base buttons
    connect(ui->validate, &QPushButton::clicked, this, &NewMethod::validatePressed);
    connect(ui->cancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->methodSpecial, &QComboBox::currentIndexChanged, this, &NewMethod::methodSpecialChanged);
    connect(ui->methodName, &QLineEdit::textChanged, this, &NewMethod::methodNameChanged);
    connect(ui->methodRange, &QComboBox::currentIndexChanged, this, &NewMethod::methodRangeChanged);
    connect(ui->methodModifier, &QComboBox::currentIndexChanged, this, &NewMethod::methodModifierChanged);
    connect(ui->editReturnType, &QPushButton::clicked, this, &NewMethod::editReturnTypePressed);
    connect(ui->methodConst, &QCheckBox::stateChanged, this, &NewMethod::methodConstChanged);
    connect(ui->methodFinal, &QCheckBox::stateChanged, this, &NewMethod::methodFinalChanged);

    // templates
    connect(ui->addTemplate, &QPushButton::clicked, this, &NewMethod::addTemplatePressed);
    connect(ui->methodTemplates, &QListWidget::itemSelectionChanged, this, &NewMethod::methodTemplatesChanged);
    connect(ui->upTemplate, &QPushButton::clicked, this, &NewMethod::upTemplatePressed);
    connect(ui->downTemplate, &QPushButton::clicked, this, &NewMethod::downTemplatePressed);
    connect(ui->editTemplate, &QPushButton::clicked, this, &NewMethod::editTemplatePressed);
    connect(ui->methodTemplates, &QListWidget::doubleClicked, this, &NewMethod::editTemplatePressed);
    connect(ui->deleteTemplate, &QPushButton::clicked, this, &NewMethod::deleteTemplatePressed);

    // parameters
    connect(ui->addParameter, &QPushButton::clicked, this, &NewMethod::addParameterPressed);
    connect(ui->methodParameters, &QListWidget::itemSelectionChanged, this, &NewMethod::methodParametersChanged);
    connect(ui->upParameter, &QPushButton::clicked, this, &NewMethod::upParameterPressed);
    connect(ui->downParameter, &QPushButton::clicked, this, &NewMethod::downParameterPressed);
    connect(ui->editParameter, &QPushButton::clicked, this, &NewMethod::editParameterPressed);
    connect(ui->methodParameters, &QListWidget::doubleClicked, this, &NewMethod::editParameterPressed);
    connect(ui->deleteParameter, &QPushButton::clicked, this, &NewMethod::deleteParameterPressed);

    /*****************************/
    // base focus on method name
    ui->methodName->setFocus(Qt::PopupFocusReason);
    ui->methodName->selectAll();
}

NewMethod::~NewMethod()
{
    delete ui;
}

void NewMethod::addParameterPressed()
{
    NewParameter dialog(this);
    if (dialog.exec())
        ui->methodParameters->addItem(&*generatedMethod.parameters.push_back(dialog.getResult()));
}

void NewMethod::editParameterPressed()
{
    auto currentParameter = dynamic_cast<Variable*>(ui->methodParameters->currentItem());
    NewParameter dialog(this, currentParameter);
    if (dialog.exec())
    {
        *currentParameter = dialog.getResult();
    }
}

void NewMethod::deleteParameterPressed()
{
    generatedMethod.parameters.remove(
                generatedMethod.parameters.find(
                    dynamic_cast<Variable*>(ui->methodParameters->takeItem(ui->methodParameters->currentRow()))));
    ui->methodParameters->setCurrentRow(-1);
}

void NewMethod::methodParametersChanged()
{
    if (ui->methodParameters->currentRow() != -1)
    {
        ui->editParameter->setEnabled(true);
        ui->deleteParameter->setEnabled(true);
        ui->upParameter->setEnabled(true);
        ui->downParameter->setEnabled(true);
    }
    else
    {
        ui->editParameter->setEnabled(false);
        ui->deleteParameter->setEnabled(false);
        ui->upParameter->setEnabled(false);
        ui->downParameter->setEnabled(false);
    }
}

void NewMethod::upParameterPressed()
{
    if (ui->methodParameters->currentRow() > 0)
    {
        auto it = generatedMethod.parameters.find(dynamic_cast<Variable*>(ui->methodParameters->currentItem()));
        Variable moved = *it;
        generatedMethod.parameters.remove(it);
        --it;
        it = generatedMethod.parameters.insert(it, moved);
        while (ui->methodParameters->count())
            ui->methodParameters->takeItem(0);
        for (auto it2 = generatedMethod.parameters.begin();it2 != generatedMethod.parameters.end();++it2)
            ui->methodParameters->addItem(&*it2);
        ui->methodParameters->setCurrentItem(&*it);
    }
}

void NewMethod::downParameterPressed()
{
    if (ui->methodParameters->currentRow() < ui->methodParameters->count()-1)
    {
        auto it = generatedMethod.parameters.find(dynamic_cast<Variable*>(ui->methodParameters->currentItem()));
        ++it;
        Variable moved = *it;
        generatedMethod.parameters.remove(it);
        --it;
        it = generatedMethod.parameters.insert(it, moved);
        ++it;
        while (ui->methodParameters->count())
            ui->methodParameters->takeItem(0);
        for (auto it2 = generatedMethod.parameters.begin();it2 != generatedMethod.parameters.end();++it2)
            ui->methodParameters->addItem(&*it2);
        ui->methodParameters->setCurrentItem(&*it);
    }
}


void NewMethod::addTemplatePressed()
{
    NewTemplateType dialog(this);
    if (dialog.exec())
        ui->methodTemplates->addItem(&*generatedMethod.templateTypes.push_back(dialog.getResult()));
}

void NewMethod::editTemplatePressed()
{
    auto currentTemplate = dynamic_cast<TemplateName*>(ui->methodTemplates->currentItem());
    NewTemplateType dialog(this, currentTemplate);
    if (dialog.exec())
    {
        *currentTemplate = dialog.getResult();
    }
}

void NewMethod::deleteTemplatePressed()
{
    generatedMethod.templateTypes.remove(
                generatedMethod.templateTypes.find(
                    dynamic_cast<TemplateName*>(ui->methodTemplates->takeItem(ui->methodTemplates->currentRow()))));
    ui->methodTemplates->setCurrentRow(-1);
}

void NewMethod::methodTemplatesChanged()
{
    if (ui->methodTemplates->currentRow() != -1)
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

void NewMethod::upTemplatePressed()
{
    if (ui->methodTemplates->currentRow() > 0)
    {
        auto it = generatedMethod.templateTypes.find(dynamic_cast<TemplateName*>(ui->methodTemplates->currentItem()));
        TemplateName moved = *it;
        generatedMethod.templateTypes.remove(it);
        --it;
        it = generatedMethod.templateTypes.insert(it, moved);
        while (ui->methodTemplates->count())
            ui->methodTemplates->takeItem(0);
        for (auto it2 = generatedMethod.templateTypes.begin();it2 != generatedMethod.templateTypes.end();++it2)
            ui->methodTemplates->addItem(&*it2);
        ui->methodTemplates->setCurrentItem(&*it);
    }
}

void NewMethod::downTemplatePressed()
{
    if (ui->methodTemplates->currentRow() < ui->methodTemplates->count()-1)
    {
        auto it = generatedMethod.templateTypes.find(dynamic_cast<TemplateName*>(ui->methodTemplates->currentItem()));
        ++it;
        TemplateName moved = *it;
        generatedMethod.templateTypes.remove(it);
        --it;
        it = generatedMethod.templateTypes.insert(it, moved);
        ++it;
        while (ui->methodTemplates->count())
            ui->methodTemplates->takeItem(0);
        for (auto it2 = generatedMethod.templateTypes.begin();it2 != generatedMethod.templateTypes.end();++it2)
            ui->methodTemplates->addItem(&*it2);
        ui->methodTemplates->setCurrentItem(&*it);
    }
}

void NewMethod::methodFinalChanged()
{
    generatedMethod.finalMethod = ui->methodFinal->isChecked();
}

void NewMethod::methodConstChanged()
{
    generatedMethod.constantMethod = ui->methodConst->isChecked();
}

void NewMethod::editReturnTypePressed()
{
    NewType dialog(this, &generatedMethod.returnType);
    if (dialog.exec())
    {
        generatedMethod.returnType = dialog.getResult();
        std::stringstream ss;
        ss << '`' << generatedMethod.returnType.name.toStdString() << '`';
        ui->methodReturnType->setText(QString(ss.str().c_str()));
    }
}

void NewMethod::methodModifierChanged()
{
    switch (ui->methodModifier->currentIndex()) {
    case 0:
        generatedMethod.modifier = Method::Modifier::NONE;
        ui->cppModifier->setVisible(false);
        ui->methodFinal->setEnabled(false);
        ui->methodConst->setEnabled(true);
        ui->constCpp->setEnabled(true);
        break;
    case 1:
        generatedMethod.modifier = Method::Modifier::VIRTUAL;
        ui->cppModifier->setVisible(true);
        ui->methodFinal->setEnabled(false);
        ui->methodConst->setEnabled(true);
        ui->constCpp->setEnabled(true);
        break;
    case 2:
        generatedMethod.modifier = Method::Modifier::ABSTRACT;
        ui->cppModifier->setVisible(false);
        ui->methodFinal->setEnabled(false);
        ui->methodConst->setEnabled(true);
        ui->constCpp->setEnabled(true);
        break;
    case 3:
        generatedMethod.modifier = Method::Modifier::OVERRIDE;
        ui->cppModifier->setVisible(false);
        ui->methodFinal->setEnabled(true);
        ui->methodConst->setEnabled(true);
        ui->constCpp->setEnabled(true);
        break;
    case 4:
        generatedMethod.modifier = Method::Modifier::STATIC;
        ui->cppModifier->setVisible(false);
        ui->methodFinal->setEnabled(false);
        ui->methodConst->setEnabled(false);
        ui->constCpp->setEnabled(false);
        break;
    }
}

void NewMethod::methodRangeChanged()
{
    switch (ui->methodRange->currentIndex()) {
    case 0:
        generatedMethod.range = Range::PUBLIC;
        break;
    case 1:
        generatedMethod.range = Range::PROTECTED;
        break;
    case 2:
        generatedMethod.range = Range::PRIVATE;
        break;
    }
}

void NewMethod::methodNameChanged()
{
    generatedMethod.name = ui->methodName->text();
}

void NewMethod::methodSpecialChanged()
{
    switch(ui->methodSpecial->currentIndex())
    {
    case 0:
        generatedMethod.specialMethod = Method::Special::BASIC;
        ui->methodName->setEnabled(true);
        ui->editReturnType->setEnabled(true);
        ui->methodReturnType->setEnabled(true);
        ui->methodRange->setEnabled(true);
        ui->methodModifier->setEnabled(true);
        ui->methodConst->setEnabled(true);
        ui->methodFinal->setEnabled(true);
        ui->templatePanel->setEnabled(true);
        if (generatedMethod.returnType.name.size() == 0)
        {
            generatedMethod.returnType = Type("void");
            {
                std::stringstream ss;
                ss << '`' << generatedMethod.returnType.name.toStdString() << '`';
                ui->methodReturnType->setText(QString(ss.str().c_str()));
            }
        }
        break;
    case 1:
        generatedMethod.specialMethod = Method::Special::CONSTRUCTOR;
        ui->methodName->setEnabled(false);
        ui->editReturnType->setEnabled(false);
        ui->methodReturnType->setEnabled(false);
        ui->methodRange->setEnabled(true);
        ui->methodModifier->setEnabled(false);
        ui->methodConst->setEnabled(false);
        ui->methodFinal->setEnabled(false);
        ui->templatePanel->setEnabled(false);
        break;
    case 2:
        generatedMethod.specialMethod = Method::Special::DESTRUCTOR;
        ui->methodName->setEnabled(false);
        ui->editReturnType->setEnabled(false);
        ui->methodReturnType->setEnabled(false);
        ui->methodRange->setEnabled(false);
        ui->methodModifier->setEnabled(false);
        ui->methodConst->setEnabled(false);
        ui->methodFinal->setEnabled(false);
        ui->templatePanel->setEnabled(false);
        break;
    }
}

void NewMethod::validatePressed()
{
    if (generatedMethod.specialMethod != Method::Special::BASIC)
    {
        generatedMethod.name = "";
        generatedMethod.returnType = Type();
        generatedMethod.modifier = Method::Modifier::NONE;
        generatedMethod.constantMethod = false;
        generatedMethod.finalMethod = false;
        generatedMethod.templateTypes.clear();
        if (generatedMethod.specialMethod == Method::Special::DESTRUCTOR)
            generatedMethod.range = Range::PUBLIC;
    }
    if (generatedMethod.modifier == Method::Modifier::NONE ||
            generatedMethod.modifier == Method::Modifier::STATIC)
        generatedMethod.finalMethod = false;
    if (generatedMethod.modifier == Method::Modifier::STATIC)
        generatedMethod.constantMethod = false;
    accept();
}

Method const &NewMethod::getResult()
{
    return generatedMethod;
}
