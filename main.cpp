#include "widget.h"

#include <QApplication>
#include <QFile>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    QFont font;
    font.setFamily("微软雅黑");
    a.setFont(font);
    w.setWindowIcon(QIcon(":/images/icon.png"));
    w.setWindowTitle("MCZS To Do | Version:Alpha 1.0");
    w.show();
    return a.exec();
}
