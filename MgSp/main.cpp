#include "login.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LogIn w;
    w.show();
    // MainWindow w;
    // w.show();
    return a.exec();
}
