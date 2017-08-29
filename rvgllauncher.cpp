#include "rvgllauncher.h"
#include <QDebug>

int RVGLLauncher::openPort() {
    return UPNP_AddPortMapping(urls.controlURL, data.first.servicetype,
                        "2310", "2310", lanaddr, description,
                        "UDP", 0, 0);
}

int RVGLLauncher::closePort() {
    return UPNP_DeletePortMapping(urls.controlURL, data.first.servicetype, "2310", "UDP", 0);
}


RVGLLauncher::RVGLLauncher(QObject *parent) : QObject(parent)
{
#ifdef _WIN32
    nResult = WSAStartup(MAKEWORD(2,2), &wsaData);
#endif
    devlist = upnpDiscover(2000, multicastif, minissdpdpath, localport, ipv6, ttl, &error);
    UPNP_GetValidIGD(devlist, &urls, &data, lanaddr, sizeof(lanaddr));
}

RVGLLauncher::~RVGLLauncher()
{
    closePort();
#ifdef _WIN32
    nResult = WSACleanup();
#endif /* _WIN32 */
}

void RVGLLauncher::launch(QString dir, QStringList launchOptions) {
    launchRaw(dir, launchOptions);
}

QProcess* RVGLLauncher::launchRaw(QString dir, QStringList launchOptions) {
    int server = launchOptions.indexOf("-lobby");
    if (server != -1 && launchOptions[server+1][0] == "-") {
        openPort();
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
        if (server != -1 && launchOptions[server+1][0] == "-") {
            closePort();
        }
    });
    return rvgl;
}
