#include <QApplication>
#include "view/window.h"
//#include "components/addDialog.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    window m;
    /*CustomDialog c;
    c.exec();*/

    return app.exec();
}