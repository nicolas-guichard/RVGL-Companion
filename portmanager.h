#ifndef PORTMANAGER_H
#define PORTMANAGER_H

#include <QObject>
#include <miniupnpc/upnpcommands.h>
#include <miniupnpc/miniupnpc.h>
#ifdef Q_OS_WIN
#include <winsock2.h>
#endif

class PortManager : public QObject
{
    Q_OBJECT
public:
    explicit PortManager(QObject *parent = nullptr);
    virtual ~PortManager();

public slots:
    void discover();
    void openPort();
    void closePort();

signals:
    void discoveryFinished(int);
    void openingFinished(int);
    void closingFinished(int);

private:
    char lanaddr[64] = "unset";
    const char * multicastif = 0;
    const char * minissdpdpath = 0;
    const char * description = "RVGL Companion";
#ifdef UPNP_LOCAL_PORT_ANY
    int localport = UPNP_LOCAL_PORT_ANY;
#else
    int localport = 0;
#endif
    int error = 0;
    int ipv6 = 0;
    const char * port = "2310";
    const char * protocol = "UDP";
    int timeout = 2000;
    unsigned char ttl = 2;
    struct UPNPDev * devlist = 0;
    struct UPNPUrls urls;
    struct IGDdatas data;
#ifdef Q_OS_WIN
    WSADATA wsaData;
    int nResult;
#endif
};

#endif // PORTMANAGER_H
