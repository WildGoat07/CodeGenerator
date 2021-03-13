#include <sstream>
#include <iostream>

#include "mainwindow.h"
#include "generator.h"
#include "type.h"
#include "method.h"
#include "class.h"
#include "List.hpp"

#include <QApplication>

using namespace std;
using namespace cmpt_info;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
