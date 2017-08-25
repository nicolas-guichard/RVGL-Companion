#include "rvgllauncher.h"

RVGLLauncher::RVGLLauncher(QObject *parent) : QObject(parent)
{
}

void RVGLLauncher::launch(QString dir, QStringList launchOptions) {
    QString exe;
    #ifdef Q_OS_WIN
        exe = "\\rvgl.exe";
    #else
        exe = "/rvgl";
    #endif
    QString path = dir+exe;
    QProcess* rvgl = new QProcess();
    rvgl->setWorkingDirectory(dir);
    rvgl->start(path, launchOptions);
    connect(rvgl, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [=](){ rvgl->deleteLater(); });
}
