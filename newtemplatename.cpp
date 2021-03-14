#include "newtemplatename.h"
#include "ui_newtemplatename.h"
#include "utilities.h"

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

    ui->templateName->setText(generatedTemplate.name);
    ui->templateName->selectAll();

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

TemplateName const &NewTemplateType::getResult()
{
    return generatedTemplate;
}
