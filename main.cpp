#include <QtGui/QApplication>
#include "minirgui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    minirGui w;
    w.show();
    
    return a.exec();
}
