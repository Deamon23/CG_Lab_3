#include <QApplication>
#include <QMainWindow>
#include "PyramidWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow window;
    PyramidWidget *pyramidWidget = new PyramidWidget;
    window.setCentralWidget(pyramidWidget);
    window.resize(800, 600); // Установка размера окна
    window.show();
    return a.exec();
}
