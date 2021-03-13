#include "newtemplatename.h"
#include "ui_newtemplatename.h"

NewTemplateType::NewTemplateType(QWidget *parent, TemplateName const* ref) :
    QDialog(parent),
    ui(new Ui::NewTemplateType),
    generatedTemplate()
{
    ui->setupUi(this);


    if (ref != nullptr)
    {
        generatedTemplate = *ref;
        setWindowTitle("Éditer un nom générique");
    }

    connect(ui->validate, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->templateName, &QLineEdit::textChanged, this, &NewTemplateType::nameChanged);
}

NewTemplateType::~NewTemplateType()
{
    delete ui;
}

void NewTemplateType::nameChanged()
{
    generatedTemplate.name = ui->templateName->text();
}

TemplateName NewTemplateType::getResult()
{
    return generatedTemplate;
}
