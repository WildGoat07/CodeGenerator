#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "newclass.h"
#include <sstream>
#include "textdisplayer.h"
#include "generator.h"


using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    setFixedSize(size());


    connect(ui->newClass, &QPushButton::clicked, this, &MainWindow::newClassPressed);
    connect(ui->classesView, &QListWidget::itemSelectionChanged, this, &MainWindow::classesViewSelectionChanged);
    connect(ui->deleteClass, &QPushButton::clicked, this, &MainWindow::deleteClassPressed);
    connect(ui->classesView, &QListWidget::doubleClicked, this, &MainWindow::editClassPressed);
    connect(ui->editClass, &QPushButton::released, this, &MainWindow::editClassPressed);
    connect(ui->generateCpp, &QPushButton::released, this, &MainWindow::generateCppPressed);
    connect(ui->generateJava, &QPushButton::released, this, &MainWindow::generateJavaPressed);

}

void MainWindow::newClassPressed()
{
    NewClass dialog(this);
    if (dialog.exec())
    {
        ui->classesView->addItem(new Class(dialog.getResult()));
    }
}

void MainWindow::generateCppPressed()
{
    std::stringstream ss;
    Generator generator;
    for (auto i = 0;i<ui->classesView->count();++i)
        generator.classes.push_back(*dynamic_cast<Class*>(ui->classesView->item(i)));
    generator.GenerateCpp(ss);
    TextDisplayer(QString(ss.str().c_str()), this).exec();
}
void MainWindow::generateJavaPressed()
{
    std::stringstream ss;
    Generator generator;
    for (auto i = 0;i<ui->classesView->count();++i)
        generator.classes.push_back(*dynamic_cast<Class*>(ui->classesView->item(i)));
    generator.GenerateJava(ss);
    TextDisplayer(QString(ss.str().c_str()), this).exec();
}

void MainWindow::editClassPressed()
{
    auto currentClass = dynamic_cast<Class*>(ui->classesView->currentItem());
    NewClass dialog(this, currentClass);
    if (dialog.exec())
    {
        *currentClass = dialog.getResult();
    }
}

void MainWindow::deleteClassPressed()
{
    delete ui->classesView->takeItem(ui->classesView->currentRow());
    ui->classesView->setCurrentRow(-1);
}

void MainWindow::classesViewSelectionChanged()
{
    if (ui->classesView->currentItem())
    {
        ui->editClass->setEnabled(true);
        ui->deleteClass->setEnabled(true);
    }
    else
    {
        ui->editClass->setEnabled(false);
        ui->deleteClass->setEnabled(false);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

