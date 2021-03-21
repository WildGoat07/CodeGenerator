#include "newtemplatename.h"
#include "ui_newtemplatename.h"
#include "utilities.h"

NewTemplateType::NewTemplateType(QWidget *parent, TemplateName const* ref) :
    QDialog(parent),
    ui(new Ui::NewTemplateType),
    generatedTemplate()
{
    /*****************************/
    // non resizable window
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    setFixedSize(size());


    if (ref != nullptr)
    {
        // if we are editing a template
        generatedTemplate = *ref;
        setWindowTitle("Éditer un nom générique");
    }

    /*****************************/
    // default value
    ui->templateName->setText(generatedTemplate.name);

    /*****************************/
    // name focused by default
    ui->templateName->setFocus(Qt::PopupFocusReason);
    ui->templateName->selectAll();

    /*****************************/
    // events
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
