#include "Login.h"
#include "CStore.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CStore w;
    w.show();
    return a.exec();
}