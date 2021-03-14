#include "newattribute.h"
#include "ui_newattribute.h"
#include "newtype.h"

#include <sstream>

NewAttribute::NewAttribute(QWidget *parent, Attribute const *ref) :
    QDialog(parent),
    ui(new Ui::NewAttribute)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    setFixedSize(size());

    if (ref != nullptr)
    {
        generatedAttribute = *ref;
        setWindowTitle("Éditer un attribut");
    }

    ui->attributeName->setText(generatedAttribute.variable.name);
    switch (generatedAttribute.range) {
    case Range::PUBLIC:
        ui->attributeRange->setCurrentIndex(0);
        break;
    case Range::PROTECTED:
        ui->attributeRange->setCurrentIndex(1);
        break;
    case Range::PRIVATE:
        ui->attributeRange->setCurrentIndex(2);
        break;
    }
    {
        std::stringstream ss;
        ss << '`' << generatedAttribute.variable.varType.name.toStdString() << '`';
        ui->attributeType->setText(QString(ss.str().c_str()));
    }
    ui->attributeStatic->setChecked(generatedAttribute.staticAttribute);
    ui->attributeTransient->setChecked(generatedAttribute.transientAttribute);

    connect(ui->validate, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->attributeName, &QLineEdit::textChanged, this, &NewAttribute::attributeNameChanged);
    connect(ui->attributeRange, &QComboBox::currentIndexChanged, this, &NewAttribute::attributeRangeChanged);
    connect(ui->attributeStatic, &QCheckBox::stateChanged, this, &NewAttribute::attributeStaticChanged);
    connect(ui->attributeTransient, &QCheckBox::stateChanged, this, &NewAttribute::attributeTransientChanged);
    connect(ui->editType, &QPushButton::clicked, this, &NewAttribute::editTypePressed);

    ui->attributeName->setFocus(Qt::PopupFocusReason);
    ui->attributeName->selectAll();
}

NewAttribute::~NewAttribute()
{
    delete ui;
}

void NewAttribute::attributeNameChanged()
{
    generatedAttribute.variable.name = ui->attributeName->text();
}
void NewAttribute::editTypePressed()
{
    NewType dialog(this, &generatedAttribute.variable.varType);
    if (dialog.exec())
    {
        generatedAttribute.variable.varType = dialog.getResult();
        {
            std::stringstream ss;
            ss << '`' << generatedAttribute.variable.varType.name.toStdString() << '`';
            ui->attributeType->setText(QString(ss.str().c_str()));
        }
    }
}
void NewAttribute::attributeRangeChanged()
{
    switch (ui->attributeRange->currentIndex()) {
    case 0:
        generatedAttribute.range = Range::PUBLIC;
        break;
    case 1:
        generatedAttribute.range = Range::PROTECTED;
        break;
    case 2:
        generatedAttribute.range = Range::PRIVATE;
        break;
    }
}
void NewAttribute::attributeStaticChanged()
{
    generatedAttribute.staticAttribute = ui->attributeStatic->isChecked();
}
void NewAttribute::attributeTransientChanged()
{
    generatedAttribute.transientAttribute = ui->attributeTransient->isChecked();
}
Attribute const &NewAttribute::getResult()
{
    return generatedAttribute;
}
