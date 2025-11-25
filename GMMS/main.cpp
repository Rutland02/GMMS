#include <QApplication>
#include <QTextCodec>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Qt 5 中文显示适配（解决中文乱码）
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    // 创建并显示主窗口
    MainWindow w;
    w.setWindowTitle("健身房会员管理系统（GMMS）");
    w.resize(800, 600); // 窗口大小：宽800，高600
    w.show();

    return a.exec();
}
