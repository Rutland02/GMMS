#include "gymmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GymMainWindow w;
    w.show();
    return a.exec();
}
