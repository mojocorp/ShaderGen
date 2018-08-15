#include <QApplication>
#include <QSurfaceFormat>

#include "SGFrame.h"

int
main(int argc, char* argv[])
{
    // Force 'desktop' opengl
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    QApplication app(argc, argv);
    QApplication::setApplicationName("GLSL ShaderGen");
    QApplication::setOrganizationName("mojocorp");
    QApplication::setQuitOnLastWindowClosed(true);

    QSurfaceFormat format;
    format.setVersion(2, 0);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    QSurfaceFormat::setDefaultFormat(format);

    SGFrame mainWin(QApplication::applicationName());
    mainWin.resize(800, 800);
    mainWin.readSettings();
    mainWin.show();

    return app.exec();
}
