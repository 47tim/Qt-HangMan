#include "mainwindow.h"

#include <QApplication>
#include <QString>
#include <QList>
#include <QFile>
#include <random>
#include <QDebug>
#include <QPixmap>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    w.StartGame();

    return a.exec();
}
