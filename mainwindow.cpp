#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "newclass.h"
#include <iostream>


using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    setFixedSize(size());


    connect(ui->newClass, &QPushButton::released, this, &MainWindow::newClassSlot);
    connect(ui->classesView, &QListView::indexesMoved, this, &MainWindow::classesViewSelectionChanged);

    classesViewModel = new QStringListModel(this);
    ui->classesView->setModel(classesViewModel);


}

void MainWindow::newClassSlot()
{
    Class t;
    t.name = "test";
    t.abstract = true;
    int result = NewClass(this, &t).exec();
}

void MainWindow::classesViewSelectionChanged()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}

