#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QListView>
#include <QStringListModel>
#include "class.h"
#include "List.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void newClassPressed();
    void editClassPressed();
    void deleteClassPressed();
    void classesViewSelectionChanged();
};
#endif // MAINWINDOW_H
