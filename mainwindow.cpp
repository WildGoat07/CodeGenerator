#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "newclass.h"
#include <iostream>

#include <QMessageBox>


using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    setFixedSize(size());


    connect(ui->newClass, &QPushButton::released, this, &MainWindow::newClassPressed);
    connect(ui->classesView, &QListWidget::itemSelectionChanged, this, &MainWindow::classesViewSelectionChanged);
    connect(ui->deleteClass, &QPushButton::released, this, &MainWindow::deleteClassPressed);
    connect(ui->editClass, &QPushButton::released, this, &MainWindow::editClassPressed);

}

void MainWindow::newClassPressed()
{
    NewClass dialog(this);
    if (dialog.exec())
    {
        ui->classesView->addItem(new Class(dialog.getResult()));
    }
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
    auto ptr = ui->classesView->currentItem();
    ui->classesView->removeItemWidget(ptr);
    delete ptr;
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

