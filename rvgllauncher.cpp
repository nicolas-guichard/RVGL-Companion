#include "rvgllauncher.h"

RVGLLauncher::RVGLLauncher(QObject *parent) : QObject(parent)
{
}

int RVGLLauncher::launch(QStringList launchOptions) {
    QString exe;
    #ifdef Q_OS_WIN
        exe = "\\rvgl.exe";
    #else
        exe = "/rvgl";
    #endif
    QString path = settings.value("rvglDir").toString()+exe;
    QProcess rvgl;
    rvgl.setWorkingDirectory(settings.value("rvglDir").toString());
    rvgl.start(path, launchOptions);
    if (rvgl.waitForFinished())
        return 0;
    return -1;
}
