#include "rvgllauncher.h"

RVGLLauncher::RVGLLauncher(QObject *parent) : QObject(parent)
{
    PortManager* portManager = new PortManager;
    portManager->moveToThread(&portManagerThread);
    connect(this, &RVGLLauncher::discover, portManager, &PortManager::discover);
    connect(this, &RVGLLauncher::openPort, portManager, &PortManager::openPort);
    connect(this, &RVGLLauncher::closePort, portManager, &PortManager::closePort);
    portManagerThread.start();
    emit discover();
}

RVGLLauncher::~RVGLLauncher(){
    portManagerThread.quit();
    portManagerThread.wait();
}

void RVGLLauncher::launch(QString dir, QStringList launchOptions) {
    launchRaw(dir, launchOptions);
}

QProcess* RVGLLauncher::launchRaw(QString dir, QStringList launchOptions) {
    int lobbyConf = launchOptions.indexOf("-lobby");
    bool isServer = lobbyConf != -1 && (launchOptions[lobbyConf+1] == "" || launchOptions[lobbyConf+1][0] == "-");
    if (isServer) {
        emit openPort();
    }
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
            [=](){
        rvgl->deleteLater();
        if (isServer) {
            emit closePort();
        }
    });
    return rvgl;
}
