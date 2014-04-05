#include <QApplication>
#include "makeconfig.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MakeConfig w;
    QCoreApplication::setOrganizationName("MYTH Inc");
    QCoreApplication::setApplicationName("waterer");
    w.show();
    
    return a.exec();
}
