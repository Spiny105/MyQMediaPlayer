#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << argv[0];
    qDebug() << argv[1];
    MainWindow w;
    w.show();

    return a.exec();
}
