#include "newparent.h"
#include "ui_newparent.h"
#include "newtype.h"

NewParent::NewParent(QWidget *parent, Parent const* ref) :
    QDialog(parent),
    ui(new Ui::NewParent)
{
    /*****************************/
    // non resizable window
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    setFixedSize(size());

    if (ref != nullptr)
    {
        // if we are editing a parent
        generatedParent = *ref;
        setWindowTitle("Éditer un parent");
    }

    /*****************************/
    // default values
    ui->parentName->setText(QString(generatedParent.name));
    ui->parentInterface->setChecked(generatedParent.interface);

    switch(generatedParent.range)
    {
    case Range::PUBLIC:
        ui->parentRange->setCurrentIndex(0);
        break;
    case Range::PROTECTED:
        ui->parentRange->setCurrentIndex(1);
        break;
    case Range::PRIVATE:
        ui->parentRange->setCurrentIndex(2);
        break;
    }

    for (auto it = generatedParent.templates.begin();it != generatedParent.templates.end();++it)
        ui->parentTemplates->addItem(&*it);

    /*****************************/
    // parent name focused by default
    ui->parentName->setFocus(Qt::PopupFocusReason);
    ui->parentName->selectAll();

    /*****************************/
    // events

    // base buttons
    connect(ui->validate, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->parentName, &QLineEdit::textChanged, this, &NewParent::nameChanged);
    connect(ui->parentInterface, &QCheckBox::stateChanged, this, &NewParent::interfaceChanged);
    connect(ui->parentRange, &QComboBox::currentIndexChanged, this, &NewParent::rangeChanged);

    // templates
    connect(ui->addTemplate, &QPushButton::clicked, this, &NewParent::addTemplatePressed);
    connect(ui->parentTemplates, &QListWidget::itemSelectionChanged, this, &NewParent::parentTemplatesChanged);
    connect(ui->upTemplate, &QPushButton::clicked, this, &NewParent::upTemplatePressed);
    connect(ui->downTemplate, &QPushButton::clicked, this, &NewParent::downTemplatePressed);
    connect(ui->editTemplate, &QPushButton::clicked, this, &NewParent::editTemplatePressed);
    connect(ui->parentTemplates, &QListWidget::doubleClicked, this, &NewParent::editTemplatePressed);
    connect(ui->deleteTemplate, &QPushButton::clicked, this, &NewParent::deleteTemplatePressed);

}

NewParent::~NewParent()
{
    delete ui;
}

void NewParent::nameChanged()
{
    generatedParent.name = ui->parentName->text();
}

void NewParent::interfaceChanged()
{
    generatedParent.interface = ui->parentInterface->isChecked();
}

void NewParent::rangeChanged()
{
    switch(ui->parentRange->currentIndex())
    {
    case 0:
        generatedParent.range = Range::PUBLIC;
        break;
    case 1:
        generatedParent.range = Range::PROTECTED;
        break;
    case 2:
        generatedParent.range = Range::PRIVATE;
        break;
    }
}

void NewParent::addTemplatePressed()
{
    NewType dialog(this);
    if (dialog.exec())
        ui->parentTemplates->addItem(&*generatedParent.templates.push_back(dialog.getResult()));
}

void NewParent::editTemplatePressed()
{
    auto currentTemplate = dynamic_cast<Type*>(ui->parentTemplates->currentItem());
    NewType dialog(this, currentTemplate);
    if (dialog.exec())
    {
        *currentTemplate = dialog.getResult();
    }
}

void NewParent::deleteTemplatePressed()
{
    generatedParent.templates.remove(
                generatedParent.templates.find(
                    dynamic_cast<Type*>(ui->parentTemplates->takeItem(ui->parentTemplates->currentRow()))));
    ui->parentTemplates->setCurrentRow(-1);
}

void NewParent::parentTemplatesChanged()
{
    if (ui->parentTemplates->currentRow() != -1)
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

void NewParent::upTemplatePressed()
{
    if (ui->parentTemplates->currentRow() > 0)
    {
        auto it = generatedParent.templates.find(dynamic_cast<Type*>(ui->parentTemplates->currentItem()));
        Type moved = *it;
        generatedParent.templates.remove(it);
        --it;
        it = generatedParent.templates.insert(it, moved);
        while (ui->parentTemplates->count())
            ui->parentTemplates->takeItem(0);
        for (auto it2 = generatedParent.templates.begin();it2 != generatedParent.templates.end();++it2)
            ui->parentTemplates->addItem(&*it2);
        ui->parentTemplates->setCurrentItem(&*it);
    }
}

void NewParent::downTemplatePressed()
{
    if (ui->parentTemplates->currentRow() < ui->parentTemplates->count()-1)
    {
        auto it = generatedParent.templates.find(dynamic_cast<Type*>(ui->parentTemplates->currentItem()));
        ++it;
        Type moved = *it;
        generatedParent.templates.remove(it);
        --it;
        it = generatedParent.templates.insert(it, moved);
        ++it;
        while (ui->parentTemplates->count())
            ui->parentTemplates->takeItem(0);
        for (auto it2 = generatedParent.templates.begin();it2 != generatedParent.templates.end();++it2)
            ui->parentTemplates->addItem(&*it2);
        ui->parentTemplates->setCurrentItem(&*it);
    }
}

Parent const &NewParent::getResult()
{
    return generatedParent;
}
