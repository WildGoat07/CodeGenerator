#include "textdisplayer.h"
#include "ui_textdisplayer.h"
#include <sstream>

TextDisplayer::TextDisplayer(QString const &text, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextDisplayer)
{
    /*****************************/
    // non resizable window
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    setFixedSize(size());

    /*****************************/
    // custom monospace font
    ui->content->setFontFamily("Consolas");
    ui->content->setText(text);

    connect(ui->cancel, &QPushButton::clicked, this, &QDialog::reject);
}

TextDisplayer::~TextDisplayer()
{
    delete ui;
}
