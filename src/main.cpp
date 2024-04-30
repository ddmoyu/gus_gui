#include <QApplication>
#include "view/window.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    window m;
    m.show();

    return app.exec();
}