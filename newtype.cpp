#include "newtype.h"
#include "ui_newtype.h"

NewType::NewType(QWidget *parent, Type const *ref) :
    QDialog(parent),
    ui(new Ui::NewType)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    setFixedSize(size());

    if (ref != nullptr)
    {
        generatedType = *ref;
        setWindowTitle("Éditer type");
    }

    ui->typeName->setText(generatedType.name);
    switch(generatedType.mode)
    {
    case VariableMode::COPY:
        ui->typeMode->setCurrentIndex(0);
        break;
    case VariableMode::REF:
        ui->typeMode->setCurrentIndex(1);
        break;
    case VariableMode::PTR:
        ui->typeMode->setCurrentIndex(2);
        break;
    }
    ui->typeConst->setChecked(generatedType.constant);
    ui->typeList->setChecked(generatedType.array);

    for (auto it = generatedType.templateValues.begin();it != generatedType.templateValues.end();++it)
        ui->typeTemplates->addItem(&*it);

    ui->typeName->selectAll();

    connect(ui->validate, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->typeName, &QLineEdit::textChanged, this, &NewType::typeNameChanged);
    connect(ui->typeMode, &QComboBox::currentIndexChanged, this, &NewType::typeModeChanged);
    connect(ui->typeConst, &QCheckBox::stateChanged, this, &NewType::typeConstChanged);
    connect(ui->typeList, &QCheckBox::stateChanged, this, &NewType::typeListChanged);
    connect(ui->addTemplate, &QPushButton::clicked, this, &NewType::addTemplatePressed);
    connect(ui->typeTemplates, &QListWidget::itemSelectionChanged, this, &NewType::typeTemplatesChanged);
    connect(ui->upTemplate, &QPushButton::clicked, this, &NewType::upTemplatePressed);
    connect(ui->downTemplate, &QPushButton::clicked, this, &NewType::downTemplatePressed);
    connect(ui->editTemplate, &QPushButton::clicked, this, &NewType::editTemplatePressed);
    connect(ui->deleteTemplate, &QPushButton::clicked, this, &NewType::deleteTemplatePressed);

}

NewType::~NewType()
{
    delete ui;
}

void NewType::typeNameChanged()
{
    generatedType.name = ui->typeName->text();
}

void NewType::typeModeChanged()
{
    switch(ui->typeMode->currentIndex())
    {
    case 0:
        generatedType.mode = VariableMode::COPY;
        break;
    case 1:
        generatedType.mode = VariableMode::REF;
        break;
    case 2:
        generatedType.mode = VariableMode::PTR;
        break;
    }
}

void NewType::typeConstChanged()
{
    generatedType.constant = ui->typeConst->isChecked();
}

void NewType::typeListChanged()
{
    generatedType.array = ui->typeList->isChecked();
}

void NewType::addTemplatePressed()
{
    NewType dialog(this);
    if (dialog.exec())
        ui->typeTemplates->addItem(&*generatedType.templateValues.push_back(dialog.getResult()));
}

void NewType::editTemplatePressed()
{
    auto currentTemplate = dynamic_cast<Type*>(ui->typeTemplates->currentItem());
    NewType dialog(this, currentTemplate);
    if (dialog.exec())
    {
        *currentTemplate = dialog.getResult();
    }
}

void NewType::deleteTemplatePressed()
{
    generatedType.templateValues.remove(
                generatedType.templateValues.find(
                    dynamic_cast<Type*>(ui->typeTemplates->takeItem(ui->typeTemplates->currentRow()))));
    ui->typeTemplates->setCurrentRow(-1);
}

void NewType::typeTemplatesChanged()
{
    if (ui->typeTemplates->currentRow() != -1)
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

void NewType::upTemplatePressed()
{
    if (ui->typeTemplates->currentRow() > 0)
    {
        auto it = generatedType.templateValues.find(dynamic_cast<Type*>(ui->typeTemplates->currentItem()));
        Type moved = *it;
        generatedType.templateValues.remove(it);
        --it;
        it = generatedType.templateValues.insert(it, moved);
        while (ui->typeTemplates->count())
            ui->typeTemplates->takeItem(0);
        for (auto it2 = generatedType.templateValues.begin();it2 != generatedType.templateValues.end();++it2)
            ui->typeTemplates->addItem(&*it2);
        ui->typeTemplates->setCurrentItem(&*it);
    }
}

void NewType::downTemplatePressed()
{
    if (ui->typeTemplates->currentRow() < ui->typeTemplates->count()-1)
    {
        auto it = generatedType.templateValues.find(dynamic_cast<Type*>(ui->typeTemplates->currentItem()));
        ++it;
        Type moved = *it;
        generatedType.templateValues.remove(it);
        --it;
        it = generatedType.templateValues.insert(it, moved);
        ++it;
        while (ui->typeTemplates->count())
            ui->typeTemplates->takeItem(0);
        for (auto it2 = generatedType.templateValues.begin();it2 != generatedType.templateValues.end();++it2)
            ui->typeTemplates->addItem(&*it2);
        ui->typeTemplates->setCurrentItem(&*it);
    }
}

Type const &NewType::getResult()
{
    return generatedType;
}
