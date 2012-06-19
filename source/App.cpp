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

#if TODO
App::App()
{
    frame = new SGFrame("3Dlabs GLSL ShaderGen");
    frame->resize(800,800);
    frame->show();

    return GLEW_OK == glewInit();
}

void App::Errorf(const char* format, ...)
{
    QString message;
    va_list marker;
    va_start(marker, format);
    message.vsprintf(format, marker);
    frame->SetStatusText(message, 0);
}

void App::Printf(const char* format, ...)
{
    QString message;
    va_list marker;
    va_start(marker, format);
    message.vsprintf(format, marker);
}

void App::Statusf(const char* format, ...)
{
    QString message;
    va_list marker;
    va_start(marker, format);
    message.vsprintf(format, marker);
    frame->SetStatusText(message, 0);
    frame->GetStatusBar()->Update();
}
#endif
