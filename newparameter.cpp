#include "newparameter.h"
#include "ui_newparameter.h"
#include "newtype.h"
#include <sstream>

NewParameter::NewParameter(QWidget *parent, Variable const *ref) :
    QDialog(parent),
    ui(new Ui::NewParameter)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    setFixedSize(size());

    if (ref != nullptr)
    {
        generatedVariable = *ref;
        setWindowTitle("Éditer un paramètre");
    }

    ui->parameterName->setText(generatedVariable.name);
    {
        std::stringstream ss;
        ss << "` " << generatedVariable.varType.name.toStdString() << '`';
        ui->parameterType->setText(QString(ss.str().c_str()));
    }


    connect(ui->validate, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->parameterName, &QLineEdit::textChanged, this, &NewParameter::parameterNameChanged);
    connect(ui->editType, &QPushButton::clicked, this, &NewParameter::editTypePressed);


    ui->parameterName->setFocus(Qt::PopupFocusReason);
    ui->parameterName->selectAll();
}

NewParameter::~NewParameter()
{
    delete ui;
}

void NewParameter::parameterNameChanged()
{
    generatedVariable.name = ui->parameterName->text();
}

void NewParameter::editTypePressed()
{
    NewType dialog(this, &generatedVariable.varType);
    if (dialog.exec())
    {
        generatedVariable.varType = dialog.getResult();
        std::stringstream ss;
        ss << '`' << generatedVariable.varType.name.toStdString() << '`';
        ui->parameterType->setText(QString(ss.str().c_str()));
    }
}

Variable const &NewParameter::getResult()
{
    return generatedVariable;
}
