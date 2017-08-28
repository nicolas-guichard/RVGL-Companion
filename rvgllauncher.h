#ifndef RVGLLAUNCHER_H
#define RVGLLAUNCHER_H

#include <QObject>
#include <QSettings>
#include <QProcess>
#include <miniupnpc/upnpcommands.h>
#include <miniupnpc/miniupnpc.h>
#ifdef _WIN32
#include <winsock2.h>
#endif

class RVGLLauncher : public QObject
{
    Q_OBJECT
public:
    explicit RVGLLauncher(QObject *parent = nullptr);
    virtual ~RVGLLauncher();
    Q_INVOKABLE void launch(QString, QStringList);
    QProcess* launchRaw(QString, QStringList);
    Q_INVOKABLE int openPort();

signals:

public slots:
    Q_INVOKABLE int closePort();

private:
    char lanaddr[64] = "unset";
    const char * multicastif = 0;
    const char * minissdpdpath = 0;
    int localport = UPNP_LOCAL_PORT_ANY;
    int error = 0;
    int ipv6 = 0;
    unsigned char ttl = 2;
    const char * description = "RVGL Companion";
    struct UPNPDev * devlist = 0;
    struct UPNPUrls urls;
    struct IGDdatas data;
<<<<<<< HEAD

#ifdef _WIN32
    WSADATA wsaData;
    int nResult;
#endif
=======
>>>>>>> First support for auto-opening rvgl port with UPnP
};

#endif // RVGLLAUNCHER_H
