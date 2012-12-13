#include <QApplication>

#include "SGFrame.h"

int main (int argc, char* argv[])
{
    QApplication app(argc, argv);
    QApplication::setQuitOnLastWindowClosed (true);
    SGFrame mainWin("3Dlabs GLSL ShaderGen");
    mainWin.resize(800, 800);
    mainWin.show();
    return app.exec();;
}
