#include "mainwindow.h"

#include <QApplication>

#include <QString>
#include <QTextStream>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setTitle("Prueba 8");
    w.resize(640, 480);

    w.show();


    return a.exec();
}
