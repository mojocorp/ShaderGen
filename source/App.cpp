#include <QApplication>

#include "SGFrame.h"

int main (int argc, char* argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("GLSL ShaderGen");
    QApplication::setOrganizationName("mojocorp");
    QApplication::setQuitOnLastWindowClosed (true);

    SGFrame mainWin(QApplication::applicationName());
    mainWin.resize(800, 800);
    mainWin.readSettings();
    mainWin.show();

    return app.exec();;
}
