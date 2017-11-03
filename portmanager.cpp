#include "portmanager.h"

PortManager::PortManager(QObject *parent) : QObject(parent)
{
#ifdef Q_OS_WIN
    nResult = WSAStartup(MAKEWORD(2,2), &wsaData);
#endif
}

void PortManager::discover() {
    int errorCode;
    devlist = upnpDiscover(timeout, multicastif, minissdpdpath,
                           localport, ipv6, ttl, &error);
    errorCode = UPNP_GetValidIGD(devlist, &urls, &data,
                                 lanaddr, sizeof(lanaddr));
    emit discoveryFinished(errorCode);
}

void PortManager::openPort() {
    int errorCode;
    errorCode = UPNP_AddPortMapping(urls.controlURL, data.first.servicetype,
                                   port, port, lanaddr, description,
                                   protocol, 0, 0);
    emit openingFinished(errorCode);
}

void PortManager::closePort() {
    int errorCode;
    errorCode = UPNP_DeletePortMapping(urls.controlURL, data.first.servicetype,
                                       port, protocol, 0);
    emit closingFinished(errorCode);
}

PortManager::~PortManager(){
    closePort();
    FreeUPNPUrls(&urls);
#ifdef Q_OS_WIN
    nResult = WSACleanup();
#endif
}
